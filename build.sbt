

val chiselVersion = "3.5.5"

lazy val interpret = (project in file("."))
  .settings(
    name := "interpret",
    libraryDependencies ++= Seq(
      "edu.berkeley.cs" %% "chisel3" % chiselVersion,
      "edu.berkeley.cs" %% "chiseltest" % "0.5.5",
    ),
scalaVersion := "2.12.10", // Issue on scalamacros:paradise
scalacOptions ++= Seq(
  "-deprecation",
  "-feature",
  "-unchecked",
  "-language:reflectiveCalls",
),
addCompilerPlugin("edu.berkeley.cs" %% "chisel3-plugin" % chiselVersion cross CrossVersion.full),
addCompilerPlugin("org.scalamacros" %% "paradise" % "2.1.0" cross CrossVersion.full)
  ).dependsOn(flexpret,soc_comm)

// Import flexpret
lazy val flexpret = (project in file("./flexpret"))

// Import soc-comm
lazy val soc_comm = (project in file("./soc-comm"))
