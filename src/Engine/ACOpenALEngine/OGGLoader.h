
#ifndef __OGGLOADER
#define __OGGLOADER

#include <vorbis\vorbisfile.h>
#include <vector>
#include "al.h"

#define BUFFER_SIZE   32768     // 32 KB buffers ele vai carregando o arquivo de 32 em 32 kb

class OGGLoader
{
public:
	static void DecodeOGG(const char* fileName, std::vector < char > &buffer, ALenum &format, ALsizei &freq);
};

#endif