package com.imageprocess.smartvision.viewmodel

import android.util.Size
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import com.imageprocess.computervision.ILowLeverProcess
import dagger.hilt.android.lifecycle.HiltViewModel
import timber.log.Timber
import javax.inject.Inject

class FrameViewModelProvider : ViewModelProvider.NewInstanceFactory() {
    override fun <T : ViewModel?> create(modelClass: Class<T>): T {
        return FrameViewModel() as T
    }

}
@HiltViewModel
class FrameViewModel @Inject constructor() : ViewModel() {
    @Inject
    lateinit var mLowLevelProcess: ILowLeverProcess
    //val mLowLevelProcess= com.imageprocess.computervision.LowLevelProcess();

    fun currentFrameData(data: ByteArray, previewSize: Size?) {
        mLowLevelProcess.DecodeFromImage(data, previewSize!!.width, previewSize.height, 1 )
    }

    init {
        Timber.i("New FrameViewModel instance")
    }

}