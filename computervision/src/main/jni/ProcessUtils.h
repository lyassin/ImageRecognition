#ifndef ProcessUtils_h__
#define ProcessUtils_h__
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string.h>
#include <string>
#include <memory.h>
#include <opencv2/opencv.hpp>
#include <stdio.h>

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <malloc.h>

#include "DescriptorList.h"
#define SINGLE_MATCH_THRESH 15 // minimum number of matches to start cross check matching. ratio_single_match = 0.7
#define MAX_NUMBER_OF_SAMPLE_OBJECTS 500 
#define N_INLIERS 4		// 8 default number of inliers using cross match without L-distance or 4 with L-distance
using namespace std;
using namespace cv;
class ProcessUtils {

public:
	ProcessUtils();
	~ProcessUtils();

	int CalculateDescriptor(const cv::Mat& mat,vector<cv::KeyPoint> &Key_Points, cv::Mat& descriptor);
	//void* MatchingDescriptors(int rand_inx);
	//static void* doMatchingDescriptors( void* context );
	int WriteDescriptrorToBuff(const cv::Mat& descriptor,vector<cv::KeyPoint> &Key_Points, void* &outData, unsigned int &outDataSize);
	int ReadDescriptorFromBuff(const void* data, unsigned int dataSize, cv::Mat& descriptor,vector<cv::KeyPoint> &Key_Points);
	
	int WriteKeypointToBuff(vector<KeyPoint> &keypoints, void* &outData, unsigned int &outDataSize);
	int ReadKeypointFromBuff(const void* data, unsigned int dataSize, vector<KeyPoint> &keypoints);

	int ExtractSignatureFromMat(const cv::Mat& mat, void* &outData, unsigned int &outDataSize );

	int MatchDescriptors(const cv::Mat& query, const cv::Mat& train,vector<KeyPoint> q_KeyPoints,vector<KeyPoint> t_Key_Points);

	int FindImage(DescriptorList* list, const cv::Mat& descriptor,vector<cv::KeyPoint> &Key_Points);
	
	//int crossCheckMatching( vector<DMatch> &matches, int i, int knn );
	private:
	cv::Mat query;
	cv::Mat train;
	//vector<int> num_matches ; 
	int minHessianVal;
	int pointCount;
	int octaves;
	int octaveLayers;
	
	const float ratio_single_match = 0.7; // -- accurate
	
	//vector<DMatch> matchesThread[MAX_NUMBER_OF_SAMPLE_OBJECTS]; 
	
	
	DescriptorList* descriptor_list;
	//vector<cv::KeyPoint> Key_Pointsquery;
	//int SingleMatches[MAX_NUMBER_OF_SAMPLE_OBJECTS];
	float det_threshold;
	int match_treshold;
	int last_index ;
	
	struct ParamThread
{
  void  *myClass;
  int   rand_inx;
  /* other variables */
};
	vector<ParamThread> param_thread;
};

#endif // ProcessUtils_h__

