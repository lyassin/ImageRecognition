package com.imageprocess.camerastream.camera

import android.util.Size

interface PreviewInterface {
    fun onGetCameraPreview(data: ByteArray, previewSize: Size?)
}