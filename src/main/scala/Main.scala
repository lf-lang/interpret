package interpret

import flexpret.core.FlexpretConfiguration

object Main {
  def main(args: Array[String]): Unit = {
    println("Hello World")

    assert(args.length >= 3)

    val targetVerilator = (args(0) == "verilator")

    // Parse second parameter which is the number of cores
    val nCores = args(2).toInt

    // Parse the coreCfg string passed as a command line argument
    val coreCfgs = for (i <- 0 until nCores) yield {
      FlexpretConfiguration.parseString(confString=args(1), coreId=i)
    }

    println(s"Building $nCores-core SoC")

    // Extract arguments to pass further to the chisel scala compiler
    val chiselArgs = if (args.length > 3) {
      args.slice(3,args.length)
    } else {
      Array("")
    }

    val topConfig = TopConfig(
      coreCfgs = coreCfgs,
      nCores = nCores,
      freq = 50000000, // Xilinx board runs with 50 MHz
      emulation = targetVerilator
    )

    // Pass configuration to FlexPRET processor.
    (new chisel3.stage.ChiselStage).emitVerilog(new Top(topConfig), chiselArgs)
  }
}
