package com.imageprocess.smartvisionapp.view.camera

import android.Manifest
import android.annotation.SuppressLint
import android.content.pm.PackageManager
import android.hardware.Camera
import android.os.Bundle
import android.util.Log
import android.util.Size
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.app.ActivityCompat
import androidx.fragment.app.Fragment
import androidx.fragment.app.viewModels
import com.google.android.gms.common.ConnectionResult
import com.google.android.gms.common.GoogleApiAvailability
import com.imageprocess.camerastream.camera.PreviewInterface
import com.imageprocess.smartvision.R
import com.imageprocess.smartvision.viewmodel.FrameViewModel
import dagger.hilt.android.AndroidEntryPoint
import java.io.IOException

@AndroidEntryPoint
class CameraFragment : Fragment(), PreviewInterface {

    private var mCameraSource: CameraSource? = null
    private var mPreview: CameraSourcePreview? = null
    private val mFrameViewModel: FrameViewModel by viewModels()

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        // Inflate the layout for this fragment
        val v: View = inflater.inflate(R.layout.camera_preview, container, false)
        return v;
    }

    public override fun onStart() {
        super.onStart()

        mPreview = getView()?.findViewById<CameraSourcePreview>(R.id.preview)
        // Set good defaults for capturing text.
        val autoFocus = true
        val useFlash = false

        // Check for the camera permission before accessing the camera.  If the
        // permission is not granted yet, request permission.
        val rc = ActivityCompat.checkSelfPermission(requireActivity(), Manifest.permission.CAMERA)
        if (rc == PackageManager.PERMISSION_GRANTED) {
            createCameraSource(autoFocus, useFlash)
        } else {
            requestCameraPermission()
        }

    }

    /**
     * Handles the requesting of the camera permission.  This includes
     * showing a "Snackbar" message of why the permission is needed then
     * sending the request.
     */
    private fun requestCameraPermission() {
        Log.w(TAG, "Camera permission is not granted. Requesting permission")

        val permissions = arrayOf(Manifest.permission.CAMERA)

        if (!ActivityCompat.shouldShowRequestPermissionRationale(requireActivity(),
                Manifest.permission.CAMERA)) {
            ActivityCompat.requestPermissions(requireActivity(), permissions, RC_HANDLE_CAMERA_PERM)
            return
        }
    }

    /**
     * Callback for the result from requesting permissions. This method
     * is invoked for every call on [.requestPermissions].
     *
     *
     * **Note:** It is possible that the permissions request interaction
     * with the user is interrupted. In this case you will receive empty permissions
     * and results arrays which should be treated as a cancellation.
     *
     * @param requestCode The request code passed in [.requestPermissions].
     * *
     * @param permissions The requested permissions. Never null.
     * *
     * @param grantResults The grant results for the corresponding permissions which is either [ ][PackageManager.PERMISSION_GRANTED] or [PackageManager.PERMISSION_DENIED]. Never null.
     * *
     * @see .requestPermissions
     */
    override fun onRequestPermissionsResult(requestCode: Int,
                                            permissions: Array<String>,
                                            grantResults: IntArray) {
        if (requestCode != RC_HANDLE_CAMERA_PERM) {
            Log.d(TAG, "Got unexpected permission result: " + requestCode)
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
            return
        }

        if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            Log.d(TAG, "Camera permission granted - initialize the camera source")
            createCameraSource(false, false)
            return
        }

        Log.e(
            TAG, "Permission not granted: results len = " + grantResults.size +
                    " Result code = " + if (grantResults.isNotEmpty()) grantResults[0] else "(empty)")

    }

    /**
     * Creates and starts the camera.  Note that this uses a higher resolution in comparison
     * to other detection examples to enable the ocr detector to detect small text samples
     * at long distances.
     * Suppressing InlinedApi since there is a check that the minimum version is met before using
     * the constant.
     */
    @SuppressLint("InlinedApi")
    private fun createCameraSource(autoFocus: Boolean, useFlash: Boolean) {
        // TODO: Create the mCameraSource using the TextRecognizer.
        mCameraSource = CameraSource.Builder(requireActivity().applicationContext, this)
            .setFacing(CameraSource.CAMERA_FACING_BACK)
            .setRequestedPreviewSize(1280, 1024)
            .setRequestedFps(15.0f)
            .setFlashMode(if (useFlash) Camera.Parameters.FLASH_MODE_TORCH else null)
            .setFocusMode(if (autoFocus) Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE else null)
            .build()
    }

    /**
     * Starts or restarts the camera source, if it exists.  If the camera source doesn't exist yet
     * (e.g., because onResume was called before the camera source was created), this will be called
     * again when the camera source is created.
     */
    @Throws(SecurityException::class)
    private fun startCameraSource() {
        // check that the device has play services available.
        val code = GoogleApiAvailability.getInstance().isGooglePlayServicesAvailable(
            requireActivity().applicationContext)
        if (code != ConnectionResult.SUCCESS) {
            val dlg = GoogleApiAvailability.getInstance().getErrorDialog(requireActivity(), code, RC_HANDLE_GMS)
            dlg.show()
        }

//        if (mCameraSource != null) {
        try {
            mPreview!!.start(mCameraSource!!)
        } catch (e: IOException) {
            Log.e(TAG, "Unable to start camera source.", e)
            mCameraSource!!.release()
            mCameraSource = null
        }

//        }
    }

    /**
     * Restarts the camera.
     */
    override fun onResume() {
        super.onResume()
        startCameraSource()
    }

    /**
     * Stops the camera.
     */
    override fun onPause() {
        super.onPause()
//        if (mPreview != null) {
        mPreview!!.stop()
//        }
    }

    /**
     * Releases the resources associated with the camera source, the associated detectors, and the
     * rest of the processing pipeline.
     */
    override fun onDestroy() {
        super.onDestroy()
        mPreview!!.release()

    }

    companion object {

        private val TAG = "CameraFragment"

        // Intent request code to handle updating play services if needed.
        private val RC_HANDLE_GMS = 9001

        // Permission request codes need to be < 256
        private val RC_HANDLE_CAMERA_PERM = 2

        // Constants used to pass extra data in the intent
        val AutoFocus = "AutoFocus"
        val UseFlash = "UseFlash"
        //val TextBlockObject = "String"
    }

    override fun onGetCameraPreview(data: ByteArray, previewSize: Size?) {
        mFrameViewModel.currentFrameData(data, previewSize)
    }
}