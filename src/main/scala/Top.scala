package interpret
import chisel3._
import chisel3.util.experimental.loadMemoryFromFileInline // To load program into ISpm
import flexpret.core.{Core, FlexpretConfiguration, GPIO, HostIO, ISpm}

import wishbone.{S4NoCTopWB}
import s4noc.Config


case class TopConfig(
  coreCfgs : Seq[FlexpretConfiguration],
  nCores : Int,
  freq : Int
){
  require(nCores <= 4)
}


class TopIO(topCfg: TopConfig) extends Bundle {
//  val gpio = new GPIO()(topCfg.coreCfg)
//  val host = new HostIO()
  val uart = new Bundle {
    val rx = Input(Bool())
    val tx = Output(Bool())
  }
}

class Top(topCfg: TopConfig) extends Module {
  val io = IO(new TopIO(topCfg))
  // Flexpret cores and wb masters
  val cores = for (i <- 0 until topCfg.nCores) yield Module(new Core(topCfg.coreCfgs(i)))
  val wbMasters = for (i <- 0 until topCfg.nCores) yield Module(new WishboneMaster(topCfg.coreCfgs(i).busAddrBits)(topCfg.coreCfgs(i)))
  val wbUarts = for (i <- 0 until topCfg.nCores) yield Module(new WishboneUart()(topCfg))
  val wbBuses = for (i <- 0 until topCfg.nCores) yield {
    Module(new WishboneBus(
      masterWidth =  topCfg.coreCfgs(i).busAddrBits,
      deviceWidths = Seq(4,4) // NOC width=4 and Uart width = 4
    ))
  }

  // NoC with n ports
  val noc = Module(new S4NoCTopWB(Config(4, 2, 2, 2, 32)))
  noc.io.wbPorts.map(_.setDefaults)


  // Termination and printing logic (just for simulation)
  val regCoreDone = RegInit(VecInit(Seq.fill(topCfg.nCores)(false.B)))
  val regCorePrintNext = RegInit(VecInit(Seq.fill(topCfg.nCores)(false.B)))

  for (i <- 0 until topCfg.nCores) {
    // Drove core IO to defaults
    cores(i).io.dmem.driveDefaultsFlipped()
    cores(i).io.imem_bus.driveDefaultsFlipped()
    cores(i).io.int_exts.foreach(_ := false.B)
    // Connect to wbM master
    cores(i).io.bus <> wbMasters(i).busIO

    // Connect WbMaster to WbBus
    wbMasters(i).wbIO <> wbBuses(i).io.wbMaster

    // Connect WbBus to NOC
    wbBuses(i).io.wbDevices(0) <> noc.io.wbPorts(i)

    // Connect WbBus to Uart
    wbBuses(i).io.wbDevices(1) <> wbUarts(i).io.port

    // Connect all cores to uart input
    wbUarts(i).ioUart.rx := io.uart.rx

    // Tie off GPIO inputs
    cores(i).io.gpio.in.map(_ := false.B)

    // Initialize instruction scratchpad memory
    loadMemoryFromFileInline(cores(i).imem.get.ispm, "ispm.mem")

    // Catch termination from core
    when(cores(i).io.host.to_host === "hdeaddead".U) {
      when(!regCoreDone(i)) {
        printf(cf"Core-${i} is done\n")
      }
      regCoreDone(i) := true.B
    }
    
    // Handle printfs
    when(cores(i).io.host.to_host === "hbaaabaaa".U) {
      regCorePrintNext(i) := true.B
    }.elsewhen(regCorePrintNext(i)) {
      printf(cf"Core-$i: ${cores(i).io.host.to_host}\n")
      regCorePrintNext(i) := false.B
    }
  }

  // Only core0 can write on the uart
  io.uart.tx := wbUarts(0).ioUart.tx

  // Wait until all cores are done
  when(regCoreDone.asUInt.andR) {
    printf("All cores are done terminating\n")
    assert(false.B, "Program terminated sucessfully")
  }
}
