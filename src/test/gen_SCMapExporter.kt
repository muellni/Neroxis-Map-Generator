package test

import java.io.PrintStream
import export.SCMapExporter
import map.SCMap
import map.Prop
import util.Vector3f


val gen_SCMapExporter_1: (PrintStream) -> Unit = { out ->
    val map = SCMap(32, 3, 3, 3)
    map.addProp(Prop("asdf", Vector3f(3f,4f,5f), 3.7f))
    SCMapExporter.exportSCMAP(out, map)
}