#ifndef ImageRecognitionFramework_h__
#define ImageRecognitionFramework_h__

class ImageRecognitionFramework
{
public:
	ImageRecognitionFramework();
	~ImageRecognitionFramework();

	int IncrementalLoadData(const void* data, unsigned int dataSize);

	int DeleteData();

	int DecodeFromImage(const void* data, int width, int height, int format) const;

	int DecodeFromSignature(const void* data, const unsigned int dataSize) const;

	int ExtractSignatureFromFile(const char* filename, void* &outData, unsigned int &outDataSize) const;

	int FreeData(void* &outData) const;

private:

	void* descriptors;
	void* processing;
};


#endif // ImageRecognitionFramework_h__

