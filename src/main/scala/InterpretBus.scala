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
  val deviceWidths: Seq[Int]
) extends Module {
  val io = IO(new InterpretBusIO(masterWidth, deviceWidths))
  io.coreIf.setDefaults()
  io.deviceIfs.map(_.setDefaultsFlipped())

  var devAddr = Seq(0)
  for ((dev,i) <- io.deviceIfs.zipWithIndex) {
    // Calculate address range of this device
    devAddr = devAddr ++ Seq(devAddr.last + (1 << deviceWidths(i)))
    dev.addr := io.coreIf.addr
    dev.data_in := io.coreIf.data_in

    // Only connect rest of wires when the device is being addressed
    when(io.coreIf.enable
      && io.coreIf.addr >= devAddr(devAddr.length-2).U
      && io.coreIf.addr < devAddr.last.U
    ) {
      dev.enable := io.coreIf.enable
      dev.write := io.coreIf.write
      io.coreIf.data_out := dev.data_out
    }
  }
}
