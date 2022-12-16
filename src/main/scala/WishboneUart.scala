package interpret

import chisel3._
import chisel3.util._

import chisel.lib.uart.{Rx, Tx}
import chisel.lib.fifo
import wishbone.WishboneDevice

object Constants {
  val TX_ADDR=0
  val RX_ADDR=4
  val CSR_ADDR=8

  val DATA_RDY_BIT=0
  val TX_FIFO_FULL_BIT=1
  val FAULT_BIT=2
}
import Constants._

class WishboneUartIO extends Bundle {
  val rx = Input(Bool())
  val tx = Output(Bool())
}

class WishboneUart(implicit cfg: TopConfig) extends WishboneDevice(4) {
  val ioUart = IO(new WishboneUartIO())

  val rx = Module(new Rx(cfg.freq, 115000)).io
  val tx = Module(new Tx(cfg.freq, 115000)).io

  ioUart.tx := tx.txd
  rx.rxd := ioUart.rx


  // FIXME: Use Martins FIFO
  val rxFifo = Module(new Queue(UInt(8.W), 8)).io
  val txFifo = Module(new Queue(UInt(8.W), 8)).io
  rx.channel <> rxFifo.enq
  tx.channel <> txFifo.deq
  txFifo.enq.valid :=false.B
  txFifo.enq.bits := 0.U
  rxFifo.deq.ready := false.B

  // Status register 1. DataRdy, 2. TxFifoFull 3. Fault
  val regCSR = RegInit(0.U(3.W))
  val wCSR = WireInit(VecInit(Seq.fill(3)(false.B)))
  wCSR(DATA_RDY_BIT) := rxFifo.deq.valid
  wCSR(TX_FIFO_FULL_BIT) := !txFifo.enq.ready
  regCSR := wCSR.asUInt

  // Read register
  val regReadData = RegInit(0.U(8.W))

  // Wire it up
  val port = io.port
  port.setDefaultsFlipped()
  port.rdData := regReadData

  val sIdle :: sRead :: sWrite :: Nil = Enum(3)
  val regState = RegInit(sIdle)
  val start = port.cyc & port.stb

  switch (regState) {
    is(sIdle) {
      when(start) {
        when(port.we) {
          when(port.addr === TX_ADDR.U) {
            txFifo.enq.valid := true.B
            txFifo.enq.bits := port.wrData
            assert(txFifo.enq.fire)
          }.otherwise {
            assert(false.B)
          }
          regState := sWrite
        }.otherwise {
          when(port.addr === RX_ADDR.U) {
            regReadData := rxFifo.deq.bits
            rxFifo.deq.ready := true.B
            assert(rxFifo.deq.fire)
          }.elsewhen(port.addr === CSR_ADDR.U) {
            regReadData := regCSR
          }.otherwise {
            assert(false.B)
          }
          regState := sRead
        }
      }
    }

    is(sWrite) {
      port.ack := true.B
      regState := sIdle
    }

    is(sRead) {
      port.ack := true.B
      regState := sIdle
      regReadData := 0.U
    }
  }
}
