#include "OGGLoader.h"

size_t read_istream(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	FILE *hfile=(FILE *)datasource;
	return fread(ptr, 1, size*nmemb, hfile);
};

int seek_istream(void *datasource, ogg_int64_t offset, int whence)
{
	FILE *hfile=(FILE *)datasource;
	int integer_offert=(int)offset;
	int returnVal;
	switch(whence)
	{
	case SEEK_SET:
	case SEEK_CUR:
	case SEEK_END:
		returnVal=fseek(hfile, integer_offert, whence);
		break;
	default:		//Bad value
		return -1;
	}

	if(returnVal==0)
		return 0;
	else
		return -1;		//Could not do a seek. Device not capable of seeking. (Should never encounter this case)
};

int close_istream(void *datasource)
{
	FILE *hfile=(FILE *)datasource;
	fclose(hfile);
	return 0;
};

long tell_istream(void *datasource)
{
	FILE *hfile=(FILE *)datasource;
	return ftell(hfile);
};

void OGGLoader::DecodeOGG(const char* fileName, std::vector < char > &buffer, ALenum &format, ALsizei &freq)
{
	int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
	int bitStream;
	long bytes;
	char data[BUFFER_SIZE];    // Local fixed size array
	FILE *f;

	// Open for binary reading
	f = fopen(fileName, "rb");

	vorbis_info *pInfo;
	OggVorbis_File oggFile;

	//cria o callback
	ov_callbacks cb;
	cb.close_func = close_istream;
	cb.read_func = read_istream;
	cb.seek_func = seek_istream;
	cb.tell_func = tell_istream;

	ov_open_callbacks(f,  &oggFile, nullptr, 0, cb);

	// Get some information about the OGG file
	pInfo = ov_info(&oggFile, -1);

	// Check the number of channels... always use 16-bit samples
	if (pInfo->channels == 1)
		format = AL_FORMAT_MONO16;
	else
		format = AL_FORMAT_STEREO16;
	// end if

	// The frequency of the sampling rate
	freq = pInfo->rate;

	do {
		// Read up to a buffer's worth of decoded sound data
		bytes = ov_read(&oggFile, data, BUFFER_SIZE, endian, 2, 1, &bitStream);
		// Append to end of buffer
		buffer.insert(buffer.end(), data, data + bytes);
	} while (bytes > 0);

	ov_clear(&oggFile);   
};