#include "TextureTools.h"

EndianType TextureTools::GetEndian()
{
	DWORD data = 0x12345678;
	PUCHAR ptr = (PUCHAR)&data;

	if(*ptr == 0x12 && *(ptr+1) == 0x34 &&
	   *(ptr+2) == 0x56 && *(ptr+3) == 0x78)
	{
		return ENDIAN_Big;
	}
	else if(*ptr == 0x78 && *(ptr + 1) == 0x56 &&
			*(ptr + 2) == 0x34 && *(ptr + 3) == 0x12)
	{
		return ENDIAN_Little;
	}
		return ENDIAN_Unknown;
};

void TextureTools::SwapBytes(PCHAR data, INT size)
{
	assert((size & 1) == 0);
	PCHAR ptr = data;
	CHAR temp = 0;
	for(INT i = 0, j = size - 1; i < size / 2; i++, j--)
	{
		temp = ptr[i];
		ptr[i] = ptr[j];
		ptr[j] = temp;
	}
};

PUCHAR TextureTools::LoadDDS(PCHAR file, ImageInfoDDS &info)
{
	const INT ddsHeightOffset = 12;
	const INT ddsWidthOffset = 16;
	const INT ddsLinearSizeOffset = 20;
	const INT ddsMipMapNumOffset = 28;
	const INT ddsFourCCOffset = 84;
	const INT ddsImageDataOffset = 128;
	EndianType e = GetEndian();
	BOOL byteSwap = FALSE;

	if (e == ENDIAN_Big)
		byteSwap = TRUE;

	FILE *fp = fopen(file, "rb");

	if (fp == nullptr)
		return nullptr;

	CHAR imageID[4];
	fread(imageID, 1, 4, fp);
	if(strncmp(imageID, "DDS ", 4) != 0)
	{
		fclose(fp);

		return FALSE;
	}

	UINT dwHeight = 0, dwWidth = 0,
	dwLinearSize, dwMipMaps = 0,
	dwFourCC = 0;
	fseek(fp, ddsHeightOffset, SEEK_SET);
	fread(&dwHeight, sizeof(UINT), 1, fp);

	if(byteSwap == TRUE)
		SwapBytes((PCHAR)&dwHeight, sizeof(UINT));
	fseek(fp, ddsWidthOffset, SEEK_SET);
	fread(&dwWidth, sizeof(UINT), 1, fp);

	if(byteSwap == TRUE)
		SwapBytes((PCHAR)&dwWidth, sizeof(UINT));
	fseek(fp, ddsLinearSizeOffset, SEEK_SET);
	fread(&dwLinearSize, sizeof(UINT), 1, fp);

	if(byteSwap == TRUE)
		SwapBytes((PCHAR)&dwLinearSize, sizeof(UINT));
	fseek(fp, ddsMipMapNumOffset, SEEK_SET);
	fread(&dwMipMaps, sizeof(UINT), 1, fp);

	if(byteSwap == TRUE)
		SwapBytes((PCHAR)&dwMipMaps, sizeof(UINT));
	fseek(fp, ddsFourCCOffset, SEEK_SET);
	fread(&dwFourCC, sizeof(UINT), 1, fp);

	if(byteSwap == TRUE)
		SwapBytes((PCHAR)&dwFourCC, sizeof(UINT));
	if(dwLinearSize == 0)
		dwLinearSize = dwHeight * dwWidth;

	if(dwLinearSize <= 0)
	{
		fclose(fp);

		return nullptr;
	}

	info.m_numMipMaps = dwMipMaps;
	info.m_width = dwWidth;
	info.m_height = dwHeight;

	INT mipFactor = 0;
	switch(dwFourCC)
	{
		case DS_FOURCC_DXT1:
			mipFactor = 2;
			info.m_components = 3;
			info.m_type = DDS_DXT1;
		break;
		case DS_FOURCC_DXT3:
			mipFactor = 4;
			info.m_components = 4;
			info.m_type = DDS_DXT3;
		break;
		case DS_FOURCC_DXT5:
			mipFactor = 4;
			info.m_components = 4;
			info.m_type = DDS_DXT5;
		break;
		default:
			fclose(fp);
			return nullptr;
		break;
	}

	INT totalSize = 0;
	// Take into account multiple mipmaps.
	if(dwMipMaps > 1)
		totalSize = dwLinearSize * mipFactor;
	else
		totalSize = dwLinearSize;

	PUCHAR image = nullptr;
	image = new UCHAR[totalSize * sizeof(UCHAR)];
	if(image != nullptr)
	{
		fseek(fp, ddsImageDataOffset, SEEK_SET);
		fread(image, 1, totalSize, fp);
	}
	fclose(fp);

	return image;
};