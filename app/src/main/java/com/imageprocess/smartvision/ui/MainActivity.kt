package com.imageprocess.smartvision.ui

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.ViewModelProviders
import com.google.android.material.floatingactionbutton.FloatingActionButton
import com.imageprocess.smartvision.R
import com.imageprocess.smartvision.viewmodel.MainActivityViewModel

class MainActivity : AppCompatActivity() {

    private lateinit var mainActivityViewModel: MainActivityViewModel
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        mainActivityViewModel = ViewModelProviders.of(this)
            .get(MainActivityViewModel::class.java)

        val openDirectoryButton = findViewById<FloatingActionButton>(R.id.fab_open_directory)
        openDirectoryButton?.setOnClickListener {
            openDirectory()
        }
    }
    private fun openDirectory() {
        val intent = Intent(Intent.ACTION_OPEN_DOCUMENT_TREE)
        startActivityForResult(intent, OPEN_DIRECTORY_REQUEST_CODE)
    }


    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == OPEN_DIRECTORY_REQUEST_CODE && resultCode == Activity.RESULT_OK) {
            val directoryUri = data?.data ?: return
            Log.d("TAG", directoryUri.path.toString())
            mainActivityViewModel.loadDescriptorsFromDirectory(directoryUri)
        }
    }
}
private const val OPEN_DIRECTORY_REQUEST_CODE = 0xf11e