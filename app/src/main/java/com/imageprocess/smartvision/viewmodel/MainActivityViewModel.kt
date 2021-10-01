package com.imageprocess.smartvision.viewmodel

import android.app.Application
import android.net.Uri
import android.util.Log
import androidx.documentfile.provider.DocumentFile
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.viewModelScope
import com.imageprocess.smartvision.data.CachingDocumentFile
import com.imageprocess.smartvision.data.toCachingList
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class MainActivityViewModel (application: Application) : AndroidViewModel(application) {

    private val _documents = MutableLiveData<List<CachingDocumentFile>>()
    val documents = _documents

    fun loadDescriptorsFromDirectory(directoryUri: Uri) {
        val documentsTree = DocumentFile.fromTreeUri(getApplication(), directoryUri) ?: return
        val childDocuments = documentsTree.listFiles().toCachingList()
        viewModelScope.launch {
            val sortedDocuments = withContext(Dispatchers.IO) {
                childDocuments.toMutableList().apply {
                    sortBy { it.name }
                }
            }
            Log.d("TAG", sortedDocuments.size.toString())
            _documents.postValue(sortedDocuments)
        }

    }

}