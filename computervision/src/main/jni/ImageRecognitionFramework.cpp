#include "ImageRecognitionFramework.h"

#include <stdlib.h>
#include <stdio.h>

#include <opencv2/opencv.hpp>

#include "ProcessUtils.h"
#include "DescriptorList.h"


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

#define DEF_PROCESSING ProcessUtils* proc = (ProcessUtils*)processing
#define DEF_DESCRIPTOR_LIST DescriptorList* desc = (DescriptorList*)descriptors


ImageRecognitionFramework::ImageRecognitionFramework(void)
{
	descriptors = new DescriptorList();
	processing = new ProcessUtils();
}

ImageRecognitionFramework::~ImageRecognitionFramework(void)
{
	delete (DescriptorList*)descriptors;
	delete (ProcessUtils*)processing;
}

int ImageRecognitionFramework::IncrementalLoadData( const void* data, unsigned int dataSize )
{
	DEF_PROCESSING;
	DEF_DESCRIPTOR_LIST;
	desc->IncrementalLoadData(proc, data, dataSize);
	return 1;
}

int ImageRecognitionFramework::DeleteData()
{
	DEF_DESCRIPTOR_LIST;
	desc->DeleteData();
	return 1;
}

int ImageRecognitionFramework::DecodeFromImage( const void* data, int width, int height, int format ) const
{
	DEF_PROCESSING;
	cv::Mat* pImgOrig = NULL;
	cv::Mat pImgGray;

	int window_width = width / 3;
	int window_height = height / 2;
	Rect roi;
	roi.x = width / 2 - window_width / 2;
	roi.y = height / 2 - window_height / 2;
	roi.width = window_width;
	roi.height = window_height;

	if ( format == 17){
		LOGD("format 17");
		//LOGE("Convert 17\n");
		pImgOrig = new cv::Mat(height + height / 2, width, CV_8U, (void*)data, cv::Mat::AUTO_STEP);
		cv::Mat img;
		cv::cvtColor(*pImgOrig, img, cv::COLOR_YUV2GRAY_NV21 );
		Mat pImgGrayAux = img(roi);
		pImgGray=pImgGrayAux;
		
	}
	else if ( format == 4 ){
		LOGD("format 4");
		//LOGE("Convert\n");
		pImgOrig = new cv::Mat(height, width, CV_8UC4, (void*)data, cv::Mat::AUTO_STEP);
		cv::Mat smallImg=(*pImgOrig)( roi);
		//pImgGray=pImgGrayAux;
		cv::cvtColor(smallImg, pImgGray, cv::COLOR_RGBA2GRAY );
	} else {
		LOGD("format default");
		//LOGE("Convert\n");
		pImgOrig = new cv::Mat(height, width, CV_8UC3, (void*)data, cv::Mat::AUTO_STEP);
		cv::Mat smallImg=(*pImgOrig)( roi);
		
		cv::cvtColor(smallImg, pImgGray, cv::COLOR_RGB2GRAY );
	}
	
	delete pImgOrig;

#ifndef __ANDROID__
	//cv::imshow("det", pImgGray);
	//cv::waitKey();
#endif
	cv::Mat d;
	vector<cv::KeyPoint> Key_Points;
	//LOGE("Calc descriptor\n");
	int res = proc->CalculateDescriptor(pImgGray,Key_Points, d);
	
	if ( res )
		return res;

	DEF_DESCRIPTOR_LIST;
	//LOGE("FindImage descriptor\n");
	return proc->FindImage(desc, d,Key_Points);
}

int ImageRecognitionFramework::DecodeFromSignature( const void* data, const unsigned int dataSize ) const
{
	DEF_PROCESSING;
	DEF_DESCRIPTOR_LIST;
	cv::Mat d;
	vector<cv::KeyPoint> Key_Points;
	int res = proc->ReadDescriptorFromBuff(data, dataSize, d,Key_Points);
	if ( res )
		return res;

	return proc->FindImage(desc, d,Key_Points);
}

int ImageRecognitionFramework::ExtractSignatureFromFile( const char* filename, void* &outData, unsigned int &outDataSize ) const
{
	DEF_PROCESSING;
	cv::Mat imgGray;
	imgGray = cv::imread(filename, IMREAD_GRAYSCALE);

	if ( imgGray.rows == 0 )
		return 1;

	return proc->ExtractSignatureFromMat(imgGray, outData, outDataSize);
}

int ImageRecognitionFramework::FreeData( void* &outData ) const
{
	if ( outData ){
		free(outData);
		outData = NULL;
	}
	return 0;
}
