package test

import map.BinaryMask
import java.io.PrintStream
import java.util.*

val binaryMaskSize = 16

val binaryMaskDensity = 0.5f

fun randomBinaryMask(): BinaryMask {
    return BinaryMask(binaryMaskSize, seed).randomize(binaryMaskDensity)
}

val gen_BinaryMask_rand: (PrintStream) -> Unit = { out ->
    writeMap(out, randomBinaryMask())
}

val gen_BinaryMask_invert: (PrintStream) -> Unit = { out ->
    writeMap(out, randomBinaryMask().invert())
}

val gen_BinaryMask_enlarge: (PrintStream) -> Unit = { out ->
    writeMap(out, randomBinaryMask().enlarge(binaryMaskSize + 16))
}

val gen_BinaryMask_shrink: (PrintStream) -> Unit = { out ->
    writeMap(out, randomBinaryMask().shrink(binaryMaskSize - 5))
}

val gen_BinaryMask_inflate: (PrintStream) -> Unit = { out ->
    writeMap(out, randomBinaryMask().inflate(0.5f))
}

val gen_BinaryMask_deflate: (PrintStream) -> Unit = { out ->
    writeMap(out, randomBinaryMask().deflate(0.5f))
}

val gen_BinaryMask_cutCorners: (PrintStream) -> Unit = { out ->
    writeMap(out, randomBinaryMask().cutCorners())
}

val gen_BinaryMask_acid: (PrintStream) -> Unit = { out ->
    writeMap(out, randomBinaryMask().acid(0.4f))
}

val gen_BinaryMask_outline: (PrintStream) -> Unit = { out ->
    writeMap(out, randomBinaryMask().outline())
}

val gen_BinaryMask_smooth: (PrintStream) -> Unit = { out ->
    writeMap(out, randomBinaryMask().smooth(0.4f))
}

val gen_BinaryMask_combine: (PrintStream) -> Unit = { out ->
    val other = randomBinaryMask().acid(0.4f)
    writeMap(out, randomBinaryMask().combine(other))
}

val gen_BinaryMask_intersect: (PrintStream) -> Unit = { out ->
    val other = randomBinaryMask().acid(0.4f)
    writeMap(out, randomBinaryMask().intersect(other))
}

val gen_BinaryMask_minus: (PrintStream) -> Unit = { out ->
    val other = randomBinaryMask().acid(0.4f)
    writeMap(out, randomBinaryMask().minus(other))
}

val gen_BinaryMask_fillCircle: (PrintStream) -> Unit = { out ->
    writeMap(out, randomBinaryMask().fillCircle(8.3f, 12.7f, 3.2f, true))
}

val gen_BinaryMask_trimEdge: (PrintStream) -> Unit = { out ->
    writeMap(out, randomBinaryMask().trimEdge(5))
}

val gen_BinaryMask_getRandomPosition: (PrintStream) -> Unit = { out ->
    write(out, randomBinaryMask().getRandomPosition())
}
