//
// Created by root on 17/12/20.
//
#include <jni.h>

#include <stdlib.h>
#include "ImageRecoFramework_jni.h"
#include "ImageRecognitionFramework.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __ANDROID__
#include <android/log.h>
#define TAG "ImageRecognitionFramework"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO   , TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN   , TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , TAG,__VA_ARGS__)
#else
#define LOGV(...) printf(__VA_ARGS__)
#define LOGD(...) printf(__VA_ARGS__)
#define LOGI(...) printf(__VA_ARGS__)
#define LOGW(...) printf(__VA_ARGS__)
#define LOGE(...) printf(__VA_ARGS__)
#endif // __ANDROID__

JNIEXPORT jlong JNICALL Java_com_imageprocess_smartvision_LowLevelProcess_CreateIntance
  (JNIEnv *env, jobject obj){
	return (jlong)(new ImageRecognitionFramework());
}

JNIEXPORT jlong JNICALL Java_com_imageprocess_smartvision_LowLevelProcess_DestroyIntance
  (JNIEnv *env, jobject obj, jlong instance){
	if (instance) {
		ImageRecognitionFramework* tmp = (ImageRecognitionFramework*)instance;
		delete tmp;
		return 0;
	}
	return -1;
}

JNIEXPORT jint JNICALL Java_com_imageprocess_smartvision_LowLevelProcess_IncrementalLoadData
  (JNIEnv *env, jobject obj, jlong instance, jbyteArray data){
	jbyte *jdata;
	jsize len;
	if (!instance)
		return -1;

	ImageRecognitionFramework* irf = (ImageRecognitionFramework*)instance;

	len = env->GetArrayLength(data);
	if ( len <= 0 ) return -2;

	jdata = env->GetByteArrayElements(data,0);

	const unsigned char* cdata = (const unsigned char*)jdata;
	unsigned int dataSize = (unsigned int)len;

	int res = irf->IncrementalLoadData(cdata, dataSize);

	env->ReleaseByteArrayElements(data, jdata, JNI_ABORT);

	return res;
}

JNIEXPORT jint JNICALL Java_com_imageprocess_smartvision_LowLevelProcess_DeleteData
  (JNIEnv *env, jobject obj, jlong instance){
  	if (!instance)
		return -1;

	ImageRecognitionFramework* irf = (ImageRecognitionFramework*)instance;
	return irf->DeleteData();
}

JNIEXPORT jint JNICALL Java_com_imageprocess_smartvision_LowLevelProcess_DecodeFromImage
  (JNIEnv *env, jobject obj, jlong instance, jbyteArray data, jint width, jint height, jint format){
	jbyte *jdata;
	jsize len;
	if (!instance)
		return -1;
	ImageRecognitionFramework* irf = (ImageRecognitionFramework*)instance;
	len = env->GetArrayLength(data);
	if ( len <= 0 ) return -2;
	jdata = env->GetByteArrayElements(data,NULL);
	void* cdata = (void*)jdata;
	int res = irf->DecodeFromImage(cdata, width, height, format);
	env->ReleaseByteArrayElements(data, jdata, JNI_ABORT);
	return res;
}

JNIEXPORT jint JNICALL Java_com_imageprocess_smartvision_LowLevelProcess_DecodeFromSignature
  (JNIEnv *env, jobject obj, jlong instance, jbyteArray data){
	jbyte *jdata;
	jsize len;
	if (!instance)
		return -1;

	ImageRecognitionFramework* irf = (ImageRecognitionFramework*)instance;

	len = env->GetArrayLength(data);
	if ( len <= 0 ) return -2;

	jdata = env->GetByteArrayElements(data,0);

	const unsigned char* cdata = (const unsigned char*)jdata;
	unsigned int dataSize = (unsigned int)len;

	int res = irf->DecodeFromSignature(cdata, dataSize);

	env->ReleaseByteArrayElements(data, jdata, JNI_ABORT);

	return res;
}

JNIEXPORT jbyteArray JNICALL Java_com_imageprocess_smartvision_LowLevelProcess_ExtractSignatureFromFile
  (JNIEnv *env, jobject obj, jlong instance, jstring filename){
  	jboolean isCopy;
	jbyteArray retData = NULL;
	if (!instance)
		return retData;

	ImageRecognitionFramework* irf = (ImageRecognitionFramework*)instance;

	const char *fname = env->GetStringUTFChars(filename, &isCopy);

	void* outData = NULL;
	unsigned int outDataSize = 0;

	int res = irf->ExtractSignatureFromFile(fname, outData, outDataSize);

	env->ReleaseStringUTFChars(filename, fname);

	retData = env->NewByteArray( outDataSize );
	env->SetByteArrayRegion(retData, 0, outDataSize, (jbyte*)outData);
	irf->FreeData(outData);

	return retData;
}

#ifdef __cplusplus
}
#endif
