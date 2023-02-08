/***
scalaVersion := "2.12.10"

// https://mvnrepository.com/artifact/io.github.pityka/nspl-awt
libraryDependencies += "io.github.pityka" %% "nspl-awt" % "0.5.0"
*/

import java.util.regex.Pattern
import java.util.regex.Matcher
import java.lang.ProcessBuilder
import java.nio.file.Path
import java.io.File
import java.io.BufferedOutputStream
import java.io.FileOutputStream
import java.io.InputStream
import scala.util.{Try, Using}

import org.nspl._
import org.nspl.data.HistogramData
import org.nspl.awtrenderer._

val readLine: InputStream => Option[String] = (stream: InputStream) => {
    var c: Int = stream.read()
    var ret: List[Int] = Nil
    if (c != -1) {
        while (c != '\n' && c != -1) {
            ret = ret :+ c;
            c = stream.read()
        }
        val arr: Array[Byte] = ret.map(_.toByte).toArray
        Some(new String(arr))
    } else {
        None
    }
}
val outputLinePattern = Pattern.compile("Core\\-(?<myId>\\d):\\s+(?<theirIdPlusOne>\\d)0+(?<cycleCount>\\d+)")
val analyzeOutputLine: String => Option[(Int, Int, Int)] = (outputLine: String) => {
    val matcher = outputLinePattern.matcher(outputLine)
    if (!matcher.matches()) {
        println(s"Output line '$outputLine' does not match")
        None
    }
    Some((
        Integer.parseInt(matcher.group("myId")),
        Integer.parseInt(matcher.group("theirIdPlusOne")) - 1,
        Integer.parseInt(matcher.group("cycleCount"))
    ))
}
val writeBytesToFile = (data: Array[Byte], file: File) => {
    val target = new BufferedOutputStream( new FileOutputStream(file) )
    try data.foreach( target.write(_) ) finally target.close
}
val nocBenchmarksPath = Path.of(sys.env("FP_ROOT"), "programs", "benchmarks", "noc")
val lrssPath = nocBenchmarksPath.resolve("latency_random_sparse_send")

val doMake = (p: Path) => {
    val builder = new ProcessBuilder("make").directory(lrssPath.toFile())
    val process = builder.start()
    println(new String(process.getInputStream().readAllBytes()))
}

class CommunicationParameters(val senderCore: Int, val receiverCore: Int) {
    override def equals(x: Any): Boolean = {
        if (!x.isInstanceOf[CommunicationParameters]) false else (
            x.asInstanceOf[CommunicationParameters].senderCore == senderCore
            && x.asInstanceOf[CommunicationParameters].receiverCore == receiverCore
        )
    }
    override def hashCode(): Int = senderCore * 31 + receiverCore
    override def toString(): String = s"(sender=$senderCore, receiver=$receiverCore)"
}

type Results = Map[CommunicationParameters, Seq[Int]]

val doSimulate: Path => Results = (p: Path) => {
    var latencies: Results = Map()
    val process = new ProcessBuilder("fp-emu").directory(lrssPath.toFile()).start()
    Using(process.getErrorStream()) { stream =>
        while (readLine(stream) match {
            case Some(s) => {
                println(s)
                analyzeOutputLine(s) match {
                    case Some((myId, theirId, cycleCount)) => {
                        // Assume reporter is receiver
                        val parameters = new CommunicationParameters(theirId, myId)
                        latencies = if (latencies.contains(parameters)) latencies.updated(
                            parameters, latencies(parameters) :+ cycleCount
                        ) else (latencies + (parameters -> (cycleCount :: Nil)))
                    }
                    case _ => None
                }
                true
            }
            case _ => false
        }) { }
    }
    latencies
}


val makePlot = (latencies: Results) => {
    println(latencies)
    val someData = HistogramData(latencies.get(new CommunicationParameters(0, 1)).getOrElse(Nil).map(_.toDouble), 10) -> bar()

    val plot = xyplot(someData)(
                par(
                main="Distribution of latencies",
                xlab="latency",
                ylab="frequency"
                )
            )
    println("Writing to " + Path.of(sys.env("FP_ROOT"), "temp.png"))
    writeBytesToFile(renderToByteArray(plot.build, width=2000), Path.of(sys.env("FP_ROOT"), "temp.png").toFile())
}

doMake(lrssPath)
val latencies = doSimulate(lrssPath)
makePlot(latencies)


