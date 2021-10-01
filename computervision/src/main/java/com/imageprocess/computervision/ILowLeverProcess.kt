package com.imageprocess.computervision

interface ILowLeverProcess {

    public fun IncrementalLoadData(data: ByteArray): Int
    public fun DeleteData(): Int
    public fun DecodeFromImage(data: ByteArray, width: Int, height: Int, format: Int): Int
    public fun DecodeFromSignature(data: ByteArray): Int

}