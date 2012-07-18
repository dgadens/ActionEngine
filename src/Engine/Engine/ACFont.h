//**************************
// classe q carrega e monta os textos
// creator: Daniel Gadens
// date: 02/07/2011
//****************************

#ifndef __ACFONTSTRUCTS_H
#define __ACFONTSTRUCTS_H

#include "Vector2.h"
#include "ACTexture.h"

struct ACCharSettings
{
	INT Id;
	INT X;
	INT Y;
	INT Width;
	INT Height;
	INT XOffset;
	INT YOffset;
	INT XAdvance;
};

struct ACFont
{
	UINT ID;             //id dentro do contentmanager
	UINT Instance;
	std::string Name;
	INT Size;
	INT Width;
	INT Height;
	ACCharSettings Characteres[256];
	ACTexture* Texture;

	static Vector2 GetTextSize(const ACFont* font, const std::string& text); 
};

inline Vector2 ACFont::GetTextSize(const ACFont* font, const std::string& text)
{
	Vector2 size(0);
	size.Y = font->Size;

	char const *c = text.c_str();
	int texSize = text.size();
	float currentSize = 0;
	for (int i = 0; i < texSize; i++)
	{
		ACCharSettings ch = font->Characteres[(BYTE)c[i]];
		currentSize += ch.XAdvance + ch.XOffset;
		if (currentSize > size.X)
			size.X = currentSize;

		if (c[i]=='\n')
		{
			currentSize = 0;
			size.Y += font->Size;
		}
	}

	return size;
};

#endif