#ifndef DescriptorList_h__
#define DescriptorList_h__

#include <opencv2/features2d/features2d.hpp>

class ProcessUtils;

class DescriptorList
{
public:
	DescriptorList(void);
	~DescriptorList(void);

	int IncrementalLoadData(ProcessUtils* proc, const void* data, unsigned int dataSize);
	int DeleteData();

	unsigned int count() const;
	const cv::Mat& Descriptor(unsigned int index) const;
	const std::vector<cv::KeyPoint>& KeyPoints(unsigned int index) const;
	
private:

	struct descriptorData 
	{
		int index;
		cv::Mat descriptor;
		std::vector<cv::KeyPoint> Key_Points;
	};

	std::vector<descriptorData> m_list;
};

#endif // DescriptorList_h__

