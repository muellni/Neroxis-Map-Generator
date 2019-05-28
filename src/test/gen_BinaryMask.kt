package test

import map.BinaryMask
import java.io.PrintStream
import java.util.*

val binaryMaskSize = 16

val binaryMaskDensity = 0.5f

fun writeBinaryMask(out: PrintStream, map : BinaryMask) {
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

fun randomBinaryMask(): BinaryMask {
    return BinaryMask(binaryMaskSize, seed).randomize(binaryMaskDensity)
}

val gen_BinaryMask_rand: (PrintStream) -> Unit = { out ->
    writeBinaryMask(out, randomBinaryMask())
}

val gen_BinaryMask_invert: (PrintStream) -> Unit = { out ->
    writeBinaryMask(out, randomBinaryMask().invert())
}

val gen_BinaryMask_enlarge: (PrintStream) -> Unit = { out ->
    writeBinaryMask(out, randomBinaryMask().enlarge(binaryMaskSize + 16))
}

val gen_BinaryMask_shrink: (PrintStream) -> Unit = { out ->
    writeBinaryMask(out, randomBinaryMask().shrink(binaryMaskSize - 5))
}

val gen_BinaryMask_inflate: (PrintStream) -> Unit = { out ->
    writeBinaryMask(out, randomBinaryMask().inflate(0.5f))
}

val gen_BinaryMask_deflate: (PrintStream) -> Unit = { out ->
    writeBinaryMask(out, randomBinaryMask().deflate(0.5f))
}

val gen_BinaryMask_cutCorners: (PrintStream) -> Unit = { out ->
    writeBinaryMask(out, randomBinaryMask().cutCorners())
}

val gen_BinaryMask_acid: (PrintStream) -> Unit = { out ->
    writeBinaryMask(out, randomBinaryMask().acid(0.4f).acid(0.6f))
}

val gen_BinaryMask_outline: (PrintStream) -> Unit = { out ->
    writeBinaryMask(out, randomBinaryMask().outline())
}

val gen_BinaryMask_smooth: (PrintStream) -> Unit = { out ->
    writeBinaryMask(out, randomBinaryMask().smooth(0.4f))
}

val gen_BinaryMask_combine: (PrintStream) -> Unit = { out ->
    val other = randomBinaryMask().acid(0.4f)
    writeBinaryMask(out, randomBinaryMask().combine(other))
}

val gen_BinaryMask_intersect: (PrintStream) -> Unit = { out ->
    val other = randomBinaryMask().acid(0.4f)
    writeBinaryMask(out, randomBinaryMask().intersect(other))
}

val gen_BinaryMask_minus: (PrintStream) -> Unit = { out ->
    val other = randomBinaryMask().acid(0.4f)
    writeBinaryMask(out, randomBinaryMask().minus(other))
}

val gen_BinaryMask_fillCircle: (PrintStream) -> Unit = { out ->
    writeBinaryMask(out, randomBinaryMask().fillCircle(8.3f, 12.7f, 3.2f, true))
}

val gen_BinaryMask_trimEdge: (PrintStream) -> Unit = { out ->
    writeBinaryMask(out, randomBinaryMask().trimEdge(5))
}

val gen_BinaryMask_getRandomPosition: (PrintStream) -> Unit = { out ->
    write(out, randomBinaryMask().getRandomPosition())
}
