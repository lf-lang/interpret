package interpret

import flexpret.core.FlexpretConfiguration

object Main {
  def main(args: Array[String]): Unit = {
    println("Hello World")

    assert(args.length >= 2)

    // Parse second parameter which is the number of cores
    val nCores = args(1).toInt

    // Parse the coreCfg string passed as a command line argument
    val coreCfgs = for (i <- 0 until nCores) yield {
      FlexpretConfiguration.parseString(confString=args(0), coreId=i)
    }

    println(s"Building $nCores-core SoC")

    // Extract arguments to pass further to the chisel scala compiler
    val chiselArgs = if (args.length > 2) {
      args.slice(2,args.length)
    } else {
      Array("")
    }

    val topConfig = TopConfig(
      coreCfgs = coreCfgs,
      nCores = nCores,
      freq = 50000000 // Xilinx board runs with 50 MHz
    )

    // Pass configuration to FlexPRET processor.
    (new chisel3.stage.ChiselStage).emitVerilog(new Top(topConfig), chiselArgs)
  }
}
