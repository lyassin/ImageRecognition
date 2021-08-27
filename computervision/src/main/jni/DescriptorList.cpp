#include "DescriptorList.h"

#include "BufferAccess.h"
#include "ProcessUtils.h"

DescriptorList::DescriptorList(void)
{
}


DescriptorList::~DescriptorList(void)
{
	DeleteData();
}

int DescriptorList::IncrementalLoadData(ProcessUtils* proc, const void* data, unsigned int dataSize )
{
	descriptorData desc;
	proc->ReadDescriptorFromBuff(data, dataSize, desc.descriptor,desc.Key_Points);
	desc.index = m_list.size();
	m_list.push_back(desc);

	return 0;
}

int DescriptorList::DeleteData()
{
	m_list.clear();
	return 0;
}

unsigned int DescriptorList::count() const
{
	return m_list.size();
}

const cv::Mat& DescriptorList::Descriptor( unsigned int index ) const
{
	return m_list[index].descriptor;
}
const std::vector<cv::KeyPoint>& DescriptorList::KeyPoints( unsigned int index ) const
{
	return m_list[index].Key_Points;
}
