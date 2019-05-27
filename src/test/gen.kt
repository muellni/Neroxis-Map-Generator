package test

import map.BinaryMask
import util.Vector2f
import java.io.PrintStream
import java.io.FileOutputStream
import java.nio.file.Paths
import java.io.File

val seed : Long = 23

fun writeMap(out: PrintStream, map : BinaryMask) {
    for (y in 0..(map.size-1)) {
        for (x in 0..(map.size-1)) {
            if (map.get(x, y)) {
                out.print("1")
            }
            else {
                out.print("0")
            }
            if (x < map.size-1)
                out.print(" ")
        }
        out.print("\n")
    }
}

fun write(out: PrintStream, v : Vector2f) {
    out.printf("%.0f\n", v.x)
    out.printf("%.0f\n", v.y)
}

fun main() {
    val gens = mapOf(
            "gen_rand_int" to gen_rand_int,
            "gen_rand_int_bound" to gen_rand_int_bound,
            "gen_rand_float" to gen__rand_float,
            "gen_BinaryMask_rand" to gen_BinaryMask_rand,
            "gen_BinaryMask_invert" to gen_BinaryMask_invert,
            "gen_BinaryMask_enlarge" to gen_BinaryMask_enlarge,
            "gen_BinaryMask_shrink" to gen_BinaryMask_shrink,
            "gen_BinaryMask_inflate" to gen_BinaryMask_inflate,
            "gen_BinaryMask_deflate" to gen_BinaryMask_deflate,
            "gen_BinaryMask_cutCorners" to gen_BinaryMask_cutCorners,
            "gen_BinaryMask_acid" to gen_BinaryMask_acid,
            "gen_BinaryMask_outline" to gen_BinaryMask_outline,
            "gen_BinaryMask_smooth" to gen_BinaryMask_smooth,
            "gen_BinaryMask_combine" to gen_BinaryMask_combine,
            "gen_BinaryMask_intersect" to gen_BinaryMask_intersect,
            "gen_BinaryMask_minus" to gen_BinaryMask_minus,
            "gen_BinaryMask_fillCircle" to gen_BinaryMask_fillCircle,
            "gen_BinaryMask_trimEdge" to gen_BinaryMask_trimEdge,
            "gen_BinaryMask_getRandomPosition" to gen_BinaryMask_getRandomPosition
    )

    val gendir = Paths.get("/tmp", "java")
    File(gendir.toString()).mkdirs()

    for ((name, gen) in gens) {
        val gendataPath : String = Paths.get(gendir.toString(), name + ".gendata").toString()
        val out = PrintStream(FileOutputStream(gendataPath))
        gen(out)
    }
}