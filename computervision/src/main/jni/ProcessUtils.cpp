#include "ProcessUtils.h"
#include "BufferAccess.h"

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
//#include <google/protobuf/io/coded_stream.h>
//#include <google/protobuf/io/zero_copy_stream_impl.h>
//#include <google/protobuf/wire_format_lite.h>
//
//using namespace google::protobuf::io;
//using namespace google::protobuf::internal;

#define EXTRACTOR_ALG "ORB"
#define NUM_KP 100
//#define EXTRACTOR_ALG "FREAK"
//#define EXTRACTOR_ALG "SURF"
//#define EXTRACTOR_ALG "BRISK"
//#define MATCHER_ALG "FlannBased"
//#define MATCHER_ALG "BruteForce"
#define MATCHER_ALG "BruteForce-Hamming"
bool TAG2 = false; // likely pair is found or not

ProcessUtils::ProcessUtils() {
	minHessianVal = 4000;
	pointCount = 50;
	octaves = 3;
	octaveLayers = 4;

	det_threshold = 0.7f;
	match_treshold = 4;
	last_index=-1;
	
#ifdef __ANDROID__
	LOGE("SURFX");
#endif // __ANDROID__
}

ProcessUtils::~ProcessUtils() {

}

int ProcessUtils::CalculateDescriptor(const cv::Mat& mat,
		vector<cv::KeyPoint> &Key_Points, cv::Mat& descriptor) {
	//vector<KeyPoint> keypoints;
	Ptr<FeatureDetector> detector = ORB::create();
	detector->detect(mat, Key_Points);

	/*SURFX customSurf(minHessianVal, true, false, octaves, octaveLayers,
	 pointCount);

	 customSurf(mat, Mat(), keypoints);

	 */
	Ptr<DescriptorExtractor> extr = ORB::create();
	//cv::Ptr < DescriptorExtractor > extr = cv::DescriptorExtractor::create(
	//		EXTRACTOR_ALG);

	if (extr->empty()) {
		//LOGE(
			//	"CalculateDescriptor: Cannot create descriptor extractor: %s\n", EXTRACTOR_ALG);
		return -2;
	}

	extr->compute(mat, Key_Points, descriptor);

	if (!descriptor.rows) {
	//	LOGE("CalculateDescriptor: descriptor.rows is 0\n");
		return -3;
	}

#ifndef __ANDROID__
	//Mat keys(mat);
	//for ( unsigned int i = 0; i < keypoints.size(); ++i )
	//	cv::circle(keys, keypoints[i].pt, 10, cv::Scalar(0, 0, 255));
	//cv::imshow("desc", keys);
#endif

	return 0;
}

int ProcessUtils::WriteDescriptrorToBuff(const cv::Mat& descriptor,
		vector<cv::KeyPoint> &Key_Points, void* &outData,
		unsigned int &outDataSize) {
	//ArrayOutputStream *outStrm = new ArrayOutputStream(cdata, outDataSize);
	//CodedOutputStream *out = new CodedOutputStream(outStrm);
	//out->WriteLittleEndian32(WireFormatLite::EncodeFloat(1.f));

	outDataSize = 2 + 2 + 2 + 4
			+ descriptor.cols * descriptor.rows * descriptor.elemSize()
			+ 1
			+ (/*size Point2f*/4+4+4) * Key_Points.size();
	;
	outData = malloc(outDataSize);

	BufferWriter bfr(outData, outDataSize);

	bfr.WriteUShort(descriptor.rows);
	bfr.WriteUShort(descriptor.cols);
	bfr.WriteUShort(descriptor.elemSize());
	bfr.WriteUInt(descriptor.type());

	for (int i = 0; i < descriptor.rows; ++i)
		for (int j = 0; j < descriptor.cols; ++j)
			switch (descriptor.elemSize()) {
			case 1:
				bfr.WriteUByte(descriptor.at < uint8_t > (i, j));
				break;
			case 2:
				bfr.WriteUShort(descriptor.at < uint16_t > (i, j));
				break;
			case 4:
				bfr.WriteUInt(descriptor.at < uint32_t > (i, j));
				break;
			case 8:
				bfr.WriteUInt64(descriptor.at < uint64_t > (i, j));
				break;
			}
			
			LOGE("WriteDescriptrorToBuff: write key points");
//for (int i=0;i<Key_Points.size();i++)
	//LOGE("KeyPoints[%d]=(%f, %f)",i,Key_Points[i].pt.x,Key_Points[i].pt.y);
	//LOGE("WriteDescriptrorToBuff: end writing key points");
	bfr.WriteByte(Key_Points.size());

	for (unsigned int i = 0; i < Key_Points.size(); i++) {
		bfr.WriteFloat(Key_Points[i].pt.x);
		bfr.WriteFloat(Key_Points[i].pt.y);
		bfr.WriteFloat(Key_Points[i].size);
	}

	// Serialize keypoints
	/*	outDataSize = keypoints.size() * 24 + 1;
	 outData = malloc(outDataSize);

	 BufferWriter bfr(outData, outDataSize);

	 bfr.WriteByte(keypoints.size());
	 for (unsigned int i = 0; i < keypoints.size(); ++i) {
	 bfr.WriteFloat(keypoints[i].pt.x);
	 bfr.WriteFloat(keypoints[i].pt.y);
	 bfr.WriteFloat(keypoints[i].size);
	 }
	 */
	return 0;
}
int ProcessUtils::WriteKeypointToBuff(vector<KeyPoint> &keypoints,
		void* &outData, unsigned int &outDataSize) {
//ArrayOutputStream *outStrm = new ArrayOutputStream(cdata, outDataSize);
	//CodedOutputStream *out = new CodedOutputStream(outStrm);
	//out->WriteLittleEndian32(WireFormatLite::EncodeFloat(1.f));

	outDataSize = 1 + (/*size Point2f*/3 * 4) * keypoints.size();
	outData = malloc(outDataSize);

	BufferWriter bfr(outData, outDataSize);

	bfr.WriteByte(keypoints.size());

	for (int i = 0; i < keypoints.size(); i++) {
		bfr.WriteFloat(keypoints[i].pt.x);
		bfr.WriteFloat(keypoints[i].pt.y);
		bfr.WriteFloat(keypoints[i].size);
	}

	// Serialize keypoints
	/*	outDataSize = keypoints.size() * 24 + 1;
	 outData = malloc(outDataSize);

	 BufferWriter bfr(outData, outDataSize);

	 bfr.WriteByte(keypoints.size());
	 for (unsigned int i = 0; i < keypoints.size(); ++i) {
	 bfr.WriteFloat(keypoints[i].pt.x);
	 bfr.WriteFloat(keypoints[i].pt.y);
	 bfr.WriteFloat(keypoints[i].size);
	 }
	 */
	return 0;
}
int ProcessUtils::ReadKeypointFromBuff(const void* data, unsigned int dataSize,
		vector<KeyPoint> &keypoints) {
	if (dataSize <= 2) {
		//LOGE("ReadKeyPointFromBuff: DataSize is less than 2 bytes\n");
		return 1;
	}
	BufferReader bfr(data, dataSize);
	unsigned int size = bfr.ReadByte();
	for (int i = 0; i < size; ++i)
		keypoints.push_back(
				KeyPoint(bfr.ReadFloat(), bfr.ReadFloat(), bfr.ReadFloat()));
	return 0;
}
int ProcessUtils::ReadDescriptorFromBuff(const void* data,
		unsigned int dataSize, cv::Mat& descriptor,
		vector<cv::KeyPoint> &Key_Points) {
	if (dataSize <= 10) {
		//LOGE("ReadDescriptorFromBuff: DataSize is less than 10 bytes\n");
		return 1;
	}

	BufferReader bfr(data, dataSize);

	unsigned int rows = bfr.ReadUShort();
	unsigned int cols = bfr.ReadUShort();
	unsigned int elemSize = bfr.ReadUShort();
	unsigned int type = bfr.ReadUInt();

	descriptor.create(rows, cols, type);

	for (int i = 0; i < descriptor.rows; ++i)
		for (int j = 0; j < descriptor.cols; ++j)
			switch (elemSize) {
			case 1:
				descriptor.at < uint8_t > (i, j) = bfr.ReadUByte();
				break;
			case 2:
				descriptor.at < uint16_t > (i, j) = bfr.ReadUShort();
				break;
			case 4:
				descriptor.at < uint32_t > (i, j) = bfr.ReadUInt();
				break;
			case 8:
				descriptor.at < uint64_t > (i, j) = bfr.ReadUInt64();
				break;
			}
	unsigned int size = bfr.ReadByte();
	//LOGE("reading keypoint fromBuff: read key points");

	
	
	float x, y, size_keypoint;
	for (int i = 0; i < size; i++)
	{
	x=bfr.ReadFloat();
	y=bfr.ReadFloat();
	size_keypoint=bfr.ReadFloat();
		Key_Points.push_back(KeyPoint(x,y,size_keypoint));
		//LOGE("KeyPoints[%d]=(%f, %f)",i,x,y);
				}
				//LOGE("end reading keypoint fromBuff: key points loaded");
	return 0;
}

int ProcessUtils::ExtractSignatureFromMat(const cv::Mat& mat, void* &outData,
		unsigned int &outDataSize) {
	cv::Mat descriptor;
	vector < cv::KeyPoint > Key_Points;
	CalculateDescriptor(mat, Key_Points, descriptor);
	WriteDescriptrorToBuff(descriptor, Key_Points, outData, outDataSize);
	//LOGE("descriptor extracted and writed to buff\n");
	return 0;
}


int ProcessUtils::MatchDescriptors(const cv::Mat& query, const cv::Mat& train,vector<KeyPoint> q_KeyPoints,vector<KeyPoint> t_Key_Points) {
vector<DMatch> matches; //recent
matches.clear();
unsigned int num_matches=0;
    vector<vector<DMatch> > matches12, matches21;
	   cv::Ptr < DescriptorMatcher > descriptorMatcher =
			cv::DescriptorMatcher::create(MATCHER_ALG); 
	    descriptorMatcher->knnMatch( query, train, matches12, 1 );
	    descriptorMatcher->knnMatch( train, query, matches21, 1 );
		
		
		for( int q = 0; q < matches12.size()-1; q++ )
    {			
			if ( matches12[q][0].distance < ratio_single_match * matches12[q+1][0].distance )
					num_matches++;
			
    }
	
if ( num_matches < SINGLE_MATCH_THRESH ) return 0; 
	num_matches=0;
	    for( size_t m = 0; m < matches12.size(); m++ )
	    {
	        bool findCrossCheck = false;
	        for( size_t fk = 0; fk < matches12[m].size(); fk++ )
	        {
	            DMatch forward = matches12[m][fk];
	
	            for( size_t bk = 0; bk < matches21[forward.trainIdx].size(); bk++ )
	            {
                DMatch backward = matches21[forward.trainIdx][bk];
	                if( backward.trainIdx == forward.queryIdx )
	                {
	                   num_matches++;
					   matches.push_back(forward);
	                    findCrossCheck = true;
	                    break;
	                }
	            }
	            if( findCrossCheck ) break;
	        }
	    }
		 if ( num_matches < N_INLIERS ) return 0; 
		
		vector<int> queryIdxs(matches.size()), trainIdxs(matches.size());

	for (size_t i = 0; i < matches.size(); i++) {
		queryIdxs[i] = matches[i].queryIdx;
		trainIdxs[i] = matches[i].trainIdx;
	}
	vector < Point2f > points1;
	KeyPoint::convert(q_KeyPoints, points1, queryIdxs);
	vector < Point2f > points2;
	KeyPoint::convert(t_Key_Points, points2, trainIdxs);

	Mat H12;
	H12 = findHomography(Mat(points1), Mat(points2), /*CV_RANSAC*/FM_LMEDS, 0);
	num_matches=0;
	if (!H12.empty()) {
		Mat points1t;
		perspectiveTransform(Mat(points1), points1t, H12);

		double maxInlierDist =1;
		for (size_t i1 = 0; i1 < points1.size(); i1++) {
			if (norm(points2[i1] - points1t.at < Point2f > ((int) i1, 0))
					< maxInlierDist) // inlier
					{
				//matchesMask[i1] = 1;
				num_matches++; //= num_matches[atoi((char*)inx_thread)] + 1;
				//LOGE("num_matches ++ %d\n",inx_thread);
			}
		}

		//matchesMaskThread[i] = matchesMask;				
	} //end if ( !H12.empty() )

	return num_matches;
}
void fill_with_unique_rand(int* array, int N, int min_value, int max_value) {
	int available = max_value - min_value + 1;
	int required = N;

	for (int i = 0; i < available; ++i)
		if ((rand() % (available - i)) < required) // we have to choose required
				// out of the remaining (available-i)
				{
			--required;
			array[required] = min_value + i;
		}
	random_shuffle(array, array + N); // if needed
}
/**
void* ProcessUtils::doMatchingDescriptors( void* context ){

	return ((ProcessUtils*)((ParamThread *) context)->myClass)->MatchingDescriptors(((ParamThread *) context)->rand_inx);
}
**/
int ProcessUtils::FindImage(DescriptorList* list, const cv::Mat& descriptor,
		vector<cv::KeyPoint> &Key_Points) {
		
		
	query = descriptor;
	if (!query.rows) {
		LOGE("MatchDescriptors: query.rows is 0\n");
		return -1;}
	match_treshold=6;
	int maxMatch = 0;
	int maxIndex = -1;
	int iMarker = -1;
	descriptor_list = list;
	if (last_index!=-1)
	{
	LOGE("last_index= %d\n",last_index);
	int match = MatchDescriptors(descriptor, list->Descriptor(last_index),Key_Points,list->KeyPoints(last_index));
		if(match>=4){return last_index;}
		else last_index=-1;
	}
	int objectsCollected = list->count();
	int *rand_index_tab = ( int* )malloc( objectsCollected*sizeof(int) );
	fill_with_unique_rand( rand_index_tab, objectsCollected, 0, objectsCollected-1 ) ;
	int rand_inx = 0;

	for (unsigned int i = 0; i < list->count(); i++)
	{
	rand_inx = rand_index_tab[i];
		int match = MatchDescriptors(descriptor, list->Descriptor(rand_inx),Key_Points,list->KeyPoints(rand_inx));
		if(match>=5){last_index=rand_inx;return rand_inx;}
		if (maxMatch < match) {
			maxMatch = match;
			maxIndex = rand_inx;
		}
	}

	if ( maxMatch >= 4)
	{last_index=maxIndex;
		return maxIndex;}
	return -1;
}

