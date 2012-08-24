#ifndef MAKE4CC
#define MAKE4CC(ch0, ch1, ch2, ch3) \
	((unsigned long)(unsigned char)(ch0) | \
	((unsigned long)(unsigned char)(ch1) << 8) | \
	((unsigned long)(unsigned char)(ch2) << 16) | \
	((unsigned long)(unsigned char)(ch3) << 24 ))
#endif

#define DS_FOURCC_DXT1 (MAKE4CC('D','X','T','1'))
#define DS_FOURCC_DXT2 (MAKE4CC('D','X','T','2'))
#define DS_FOURCC_DXT3 (MAKE4CC('D','X','T','3'))
#define DS_FOURCC_DXT4 (MAKE4CC('D','X','T','4'))
#define DS_FOURCC_DXT5 (MAKE4CC('D','X','T','5'))

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ActionTypeDef.h"
#include "Enums.h"

struct ImageInfoDDS
{
	UINT m_numMipMaps;
	UINT m_width;
	UINT m_height;
	UINT m_components;
	DDSType m_type;
};

class TextureTools
{
private:
	static EndianType GetEndian(); //pega o endian dos bytes para verificar a rodem de armazenagem, dependo o processador ele armazena em ordem diferente
	static void SwapBytes(char *data, int size); //se for endian_big entao ele inverte a ordem dos bytes

public:
	UCHAR* LoadDDS(char *file, ImageInfoDDS &info); //carrega a textura DDS, so funciona com as compressoes dxt1, 3 e 5 se passar outro tipo ele retorna null
};