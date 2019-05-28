package test

import java.io.PrintStream
import java.util.Random

val gen_rand_int: (PrintStream) -> Unit = {out ->
    val random = Random(seed)
    for (i in 1..1000) {
        out.println(random.nextInt())
    }
}

val gen_rand_int_bound: (PrintStream) -> Unit = {out ->
    val random = Random(seed)
    for (i in 1..100) {
        out.println(random.nextInt(i))
    }
}

val gen_rand_long: (PrintStream) -> Unit = {out ->
    val random = Random(seed)
    for (i in 1..1000) {
        out.println(random.nextLong())
    }
}
val gen__rand_float: (PrintStream) -> Unit = {out ->
    val random = Random(seed)
    for (i in 1..1000) {
        out.printf("%.3f\n", random.nextFloat())
    }
}