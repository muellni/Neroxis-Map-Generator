package test
import map.BinaryMask

fun main() {
    val size = 4096
    val start = System.nanoTime()
    val mask = BinaryMask(size, 123)
            .randomize(0.4f)
            .acid(0.4f)
            .deflate(0.2f)
            .inflate(0.2f)
            .cutCorners()
            .enlarge(2*size)
            .invert()
            .shrink(size)
            .outline()
            .smooth(2f)
    //val mask = BinaryMask(size, 123).randomize(0.4f)
    val finish = System.nanoTime()
    println((finish - start)/1e6)
}