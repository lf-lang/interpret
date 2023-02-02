package interpret
import chisel3._
import chisel3.util.experimental.loadMemoryFromFileInline // To load program into ISpm
import chisel3.experimental.{annotate, ChiselAnnotation}
import firrtl.annotations.MemorySynthInit
import flexpret.core.{Core, FlexpretConfiguration, GPIO, HostIO, ISpm}

import s4noc.{Config, S4NoCTop}


case class TopConfig(
  coreCfgs : Seq[FlexpretConfiguration],
  nCores : Int,
  freq : Int
){
  require(nCores <= 4)
}


class TopIO(topCfg: TopConfig) extends Bundle {
 val gpio = new GPIO()(topCfg.coreCfgs(0)) // FIXME: Do this properly
  val uart = new Bundle {
    val rx = Input(Bool())
    val tx = Output(Bool())
  }
}

class Top(topCfg: TopConfig) extends Module {
  val io = IO(new TopIO(topCfg))
  // Flexpret cores
  val cores = for (i <- 0 until topCfg.nCores) yield Module(new Core(topCfg.coreCfgs(i)))

  // Create WB master, UART and bus. One per FlexPRET core
  val wbMasters = for (i <- 0 until topCfg.nCores) yield Module(new WishboneMaster(topCfg.coreCfgs(i).busAddrBits)(topCfg.coreCfgs(i)))
  val wbUarts = for (i <- 0 until topCfg.nCores) yield Module(new WishboneUart()(topCfg))
  val wbBuses = for (i <- 0 until topCfg.nCores) yield {
    Module(new WishboneBus(
      masterWidth =  topCfg.coreCfgs(i).busAddrBits,
      deviceWidths = Seq(4) // Uart width = 4
    ))
  }
  // Create the core-bus onto which WBMaster and NoC is connected
  val interpretBuses = for (i <- 0 until topCfg.nCores) yield {
    Module(new InterpretBus(
      masterWidth = topCfg.coreCfgs(i).busAddrBits,
      deviceWidths = Seq(5,4), // WB_Master needs 5 bits (addresses 0,4,8,12,16). NoC starts at address 32.
      threadBits = topCfg.coreCfgs(i).threadBits
    ))
  }

  // Normally InterPRET requires 2x2, 3x3, 4x4 etc cores. But we also support the edge case of
  //  have a single core. We then make a NoC with 4 ports and just drive the remaining 3 to defaults
  val nNocPorts = if (topCfg.nCores == 1) 4 else topCfg.nCores
  val noc = Module(new S4NoCTop(Config(nNocPorts, 2, 2, 2, 32)))

  for (i <- 0 until nNocPorts) {
    noc.io.cpuPorts(i).wrData := 0.U
    noc.io.cpuPorts(i).wr := false.B
    noc.io.cpuPorts(i).rd := false.B
    noc.io.cpuPorts(i).address := 0.U
  }

  // Termination and printing logic (just for simulation)
  val regCoreDone = RegInit(VecInit(Seq.fill(topCfg.nCores)(false.B)))
  val regCorePrintNext = RegInit(VecInit(Seq.fill(topCfg.nCores)(false.B)))

  for (i <- 0 until topCfg.nCores) {
    // Drove core IO to defaults
    cores(i).io.dmem.driveDefaultsFlipped()
    cores(i).io.imem_bus.driveDefaultsFlipped()
    cores(i).io.int_exts.foreach(_ := false.B)

    // Connect core to interpretBus
    cores(i).io.bus <> interpretBuses(i).io.coreIf

    // Connect the WBm and the NoC to the interPret bus
    interpretBuses(i).io.deviceIfs(0) <> wbMasters(i).busIO
    interpretBuses(i).io.deviceIfs(1).connectS4NoC(noc.io.cpuPorts(i))

    // Connect WbMaster to WbBus
    wbMasters(i).wbIO <> wbBuses(i).io.wbMaster

    // Connect WbBus to Uart
    wbBuses(i).io.wbDevices(0) <> wbUarts(i).io.port

    // Connect all cores to uart input
    wbUarts(i).ioUart.rx := io.uart.rx


    // Catch termination from core
    when(cores(i).io.host.to_host === "hdeaddead".U) {
      when(!regCoreDone(i)) {
        printf(cf"Core-${i} is done\n")
      }
      regCoreDone(i) := true.B
    }

    // Catch abort from core
    when(cores(i).io.host.to_host === "hdeadbeef".U) {
        printf(cf"ERROR: Core-${i} aborted simulation\n")
        assert(false.B, "Program aborted!")
    }

    // Handle printfss
    when(cores(i).io.host.to_host === "hbaaabaaa".U) {
      regCorePrintNext(i) := true.B
    }.elsewhen(regCorePrintNext(i)) {
      printf(cf"Core-$i: ${cores(i).io.host.to_host}\n")
      regCorePrintNext(i) := false.B
    }
  }


  // Drive gpio input of each core to 0 by default
  cores.map(_.io.gpio.in.map(_ := 0.U))
  // Only core0 can read/write to top-level GPIOs
  // FIXME: We should have dedicated IO banks to each core
  io.gpio.in <> cores(0).io.gpio.in
  io.gpio.out <> cores(0).io.gpio.out

  // Only core0 can write on the uart
  io.uart.tx := wbUarts(0).ioUart.tx

  // Wait until all cores are done
  when(regCoreDone.asUInt.andR) {
    printf("All cores are done terminating\n")
    assert(false.B, "Program terminated sucessfully")
  }
}
