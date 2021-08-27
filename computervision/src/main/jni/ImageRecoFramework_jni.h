//
// Created by root on 17/12/20.
//

#ifndef SMARTVISIONAPP_IMAGERECOFRAMEWORK_JNI_H
#define SMARTVISIONAPP_IMAGERECOFRAMEWORK_JNI_H
#include <jni.h>
#include <jni.h>
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     ImageRecognitionFramework
 * Method:    CreateIntance
 * Signature: ()I
 */

JNIEXPORT jlong JNICALL Java_com_imageprocess_smartvision_LowLevelProcess_CreateIntance
  (JNIEnv *, jobject);

/*
 * Class:     ImageRecognitionFramework
 * Method:    DestroyIntance
 * Signature: (I)I
 */
JNIEXPORT jlong JNICALL Java_com_imageprocess_smartvision_LowLevelProcess_DestroyIntance
  (JNIEnv *, jobject, jlong);

/*
 * Class:     ImageRecognitionFramework
 * Method:    IncrementalLoadData
 * Signature: (I[B)I
 */
JNIEXPORT jint JNICALL Java_com_imageprocess_smartvision_LowLevelProcess_IncrementalLoadData
  (JNIEnv *, jobject, jlong, jbyteArray);

/*
 * Class:     ImageRecognitionFramework
 * Method:    DeleteData
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_imageprocess_smartvision_LowLevelProcess_DeleteData
  (JNIEnv *, jobject, jlong);

/*
 * Class:     ImageRecognitionFramework
 * Method:    DecodeFromImage
 * Signature: (I[BIII)I
 */
JNIEXPORT jint JNICALL Java_com_imageprocess_smartvision_LowLevelProcess_DecodeFromImage
  (JNIEnv *, jobject, jlong, jbyteArray, jint, jint, jint);

/*
 * Class:     ImageRecognitionFramework
 * Method:    DecodeFromSignature
 * Signature: (I[B)I
 */
JNIEXPORT jint JNICALL Java_com_imageprocess_smartvision_LowLevelProcess_DecodeFromSignature
  (JNIEnv *, jobject, jlong, jbyteArray);

/*
 * Class:     ImageRecognitionFramework
 * Method:    ExtractSignatureFromFile
 * Signature: (ILjava/lang/String;)[B
 */
JNIEXPORT jbyteArray Java_com_imageprocess_smartvision_LowLevelProcess_ExtractSignatureFromFile
  (JNIEnv *, jobject, jlong, jstring);

#ifdef __cplusplus
}
#endif
#endif //SMARTVISIONAPP_IMAGERECOFRAMEWORK_JNI_H
