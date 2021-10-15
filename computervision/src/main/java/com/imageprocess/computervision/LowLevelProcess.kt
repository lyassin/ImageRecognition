package com.imageprocess.computervision

import javax.inject.Inject
import javax.inject.Singleton

class LowLevelProcess @Inject constructor(): ILowLeverProcess{

	init {
		Initialize()
	}
    companion object {
        // This is used to load the 'ImageRecognitionFramework' library
        init {
            System.loadLibrary("ImageRecognitionFramework")
        }
    }

	private var instancePtr: Long = 0

    private external fun CreateIntance(): Long
    private external fun DestroyIntance(instance : Long): Long
    private external fun IncrementalLoadData(instance: Long, data: ByteArray): Int
    private external fun DeleteData(instance: Long): Int
    private external fun DecodeFromImage(instance: Long, data: ByteArray, width: Int, height: Int,
                                         format: Int): Int
    private external fun DecodeFromSignature(instance: Long, data: ByteArray): Int
	private external fun ExtractSignatureFromFile (instance: Long, data: String) : ByteArray

	public fun finalize(){
		if ( instancePtr != null )
			DestroyIntance(instancePtr)
	}

	fun Initialize(): Long{
		instancePtr = CreateIntance()
		return 0
	}

	override fun IncrementalLoadData(data: ByteArray): Int {
		if ( instancePtr != null )
			return IncrementalLoadData(instancePtr, data)
		return -1
	}

	override fun DeleteData(): Int{
		if ( instancePtr != null )
			return DeleteData(instancePtr)
		return -1
	}

	override fun DecodeFromImage(data: ByteArray, width: Int, height: Int, format: Int): Int {
		if ( instancePtr != null )
			return DecodeFromImage(instancePtr, data, width, height, format)
		return -1
	}

	override fun DecodeFromSignature(data: ByteArray): Int {
		if ( instancePtr != null )
			return DecodeFromSignature(instancePtr, data)
		return -1
	}

	override fun ExtractSignatureFromFile(path: String):ByteArray? {
		if ( instancePtr != null )
			return ExtractSignatureFromFile(instancePtr, path)
		return null
	}

}