ThisBuild / scalaVersion     := "2.12.10"
ThisBuild / version          := "0.1.0"
ThisBuild / organization     := "PRETIS"

val chiselVersion = "3.5.6"

lazy val interpret = (project in file("."))
  .settings(
    name := "interpret",
    libraryDependencies ++= Seq(
      "edu.berkeley.cs" %% "chisel3" % chiselVersion,
      "edu.berkeley.cs" %% "chiseltest" % "0.5.6" % "test"
    ),
    scalacOptions ++= Seq(
      "-language:reflectiveCalls",
      "-deprecation",
      "-feature",
      "-Xcheckinit",
      "-P:chiselplugin:genBundleElements",
    ),
    addCompilerPlugin("edu.berkeley.cs" % "chisel3-plugin" % chiselVersion cross CrossVersion.full),
  ).dependsOn(flexpret,soc_comm)

// Import flexpret
lazy val flexpret = (project in file("./flexpret"))

// Import soc-comm
lazy val soc_comm = (project in file("./soc-comm"))
