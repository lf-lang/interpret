package interpret

import chisel3._
import chisel3.util._
import flexpret.core.BusIO
import soc.MemoryMappedIO

// Wishbone bus inspired by Martonis wbPlumbing repo: https://github.com/Martoni/WbPlumbing/blob/master/src/main/scala/wbplumbing/wbplumbing.scala


class InterpretBusPortIO(
  val addrBits: Int
) extends Bundle {

  val addr = Input(UInt(addrBits.W)) // assume word aligned
  val enable = Input(Bool())
  val data_out = Output(UInt(32.W))
  val write = Input(Bool())
  val data_in = Input(UInt(32.W))

  def setDefaults(): Unit = {
    data_out := 0.U
  }

  def setDefaultsFlipped() = {
    addr := 0.U
    enable := false.B
    write := false.B
    data_in := 0.U
  }

  def connectS4NoC(noc: MemoryMappedIO) = {
    noc.address := (addr >> 2) // The S4NoC expects addresses 0,1,2, not 0,4,8
    noc.wrData := data_in
    noc.rd:= enable && !write
    noc.wr := enable && write
    data_out := noc.rdData
  }
}

class InterpretBusIO(
  val masterWidth: Int,
  val deviceWidths: Seq[Int]
) extends Bundle {
  val coreIf = new InterpretBusPortIO(masterWidth)
  val deviceIfs = MixedVec(deviceWidths.map{i => Flipped(new InterpretBusPortIO(i))})

  // Master address must be big enough
  require(masterWidth > deviceWidths.sum)
}

class InterpretBus(
  val masterWidth: Int,
  val deviceWidths: Seq[Int],
  val threadBits: Int
) extends Module {
  val io = IO(new InterpretBusIO(masterWidth, deviceWidths))
  io.coreIf.setDefaults()
  io.deviceIfs.map(_.setDefaultsFlipped())

  // Remove thread ID from address.
  // FIXME: We might want to use it also? Not sure maybe part of this bus interface is the thread ID?
  def filterOutThreadBits(addr: UInt): UInt = {
    addr(masterWidth-threadBits-1,0)
  }

  // Register for storing which device was accessed. In the following cycle we
  // will use this register to connect the right `data_out` to the core
  val deviceRespPtr = RegInit(0.U(log2Ceil(deviceWidths.length).W))

  var devAddr = Seq(0)
  for ((dev,i) <- io.deviceIfs.zipWithIndex) {
    // Calculate address range of this device
    devAddr = devAddr ++ Seq(devAddr.last + (1 << deviceWidths(i)))
    dev.addr := io.coreIf.addr
    dev.data_in := io.coreIf.data_in

    // Connect the data_out from the device back to the core.
    //  This connects the data_out from the device which was accessed in
    //  the previous cycle. A 1CC access latency is assumed
    when(deviceRespPtr === i.U) {
      io.coreIf.data_out := dev.data_out
    }

    // The is the actual requested address. The thread bits are filtered out
    // FIXME: The threadID should be part of the interface
    val reqDevAddr = WireDefault(filterOutThreadBits(io.coreIf.addr))

    // Only connect rest of wires when the device is being addressed
    when(io.coreIf.enable
      && reqDevAddr >= devAddr(devAddr.length-2).U
      && reqDevAddr < devAddr.last.U
    ) {
      dev.enable := io.coreIf.enable
      dev.write := io.coreIf.write

      // Update the deviceRespPtr. Such that next cycle we will connect
      //  this devices' `data_out` to the core
      deviceRespPtr := i.U
    }
  }
}
