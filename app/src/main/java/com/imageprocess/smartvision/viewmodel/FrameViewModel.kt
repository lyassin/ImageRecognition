package com.imageprocess.smartvision.viewmodel

import android.util.Size
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import com.imageprocess.computervision.LowLevelProcess
import timber.log.Timber

class FrameViewModelProvider : ViewModelProvider.NewInstanceFactory() {
    override fun <T : ViewModel?> create(modelClass: Class<T>): T {
        return FrameViewModel() as T
    }

}
class FrameViewModel() : ViewModel() {
    val mLowLevelProcess= com.imageprocess.computervision.LowLevelProcess();

    fun currentFrameData(data: ByteArray, previewSize: Size?) {
        mLowLevelProcess.DecodeFromImage(data, previewSize!!.width, previewSize.height, 1 )
    }

    init {
        Timber.i("New FrameViewModel instance")
    }

}