package test

import map.FloatMask
import java.io.PrintStream

val floatMaskSize = 16

fun writeFloatMask(out: PrintStream, map : FloatMask) {
    out.println(map.size)
    out.println(map.size)
    for (y in 0..(map.size-1)) {
        for (x in 0..(map.size-1)) {
            out.printf("%.4f\n", map.get(x, y))
        }
    }
}

fun randomFloatMask(): FloatMask {
    return FloatMask(floatMaskSize, seed).init(randomBinaryMask(), 0.2f, 0.8f)
}

val gen_FloatMask_rand: (PrintStream) -> Unit = { out ->
    writeFloatMask(out, randomFloatMask())
}

val gen_FloatMask_maskToMoutains: (PrintStream) -> Unit = { out ->
    writeFloatMask(out, randomFloatMask().maskToMoutains(1.3f, 0.1f, randomBinaryMask()))
}

val gen_FloatMask_maskToHeightmap: (PrintStream) -> Unit = { out ->
    writeFloatMask(out, randomFloatMask().maskToHeightmap(1.3f, 0.1f, 5, randomBinaryMask()))
}

val gen_FloatMask_smooth: (PrintStream) -> Unit = { out ->
    writeFloatMask(out, randomFloatMask().smooth(1.7f))
}

val gen_FloatMask_smooth2: (PrintStream) -> Unit = { out ->
    writeFloatMask(out, randomFloatMask().smooth(1.7f, randomBinaryMask()))
}
