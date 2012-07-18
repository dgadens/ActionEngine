//****************************
// estrutura com os vertices do quadrado e tb com as coordenadas de textura
// creator: Daniel Gadens
// date: 02/07/2011
//**************************************

#ifndef __ACQUADSTRUCT_H
#define __ACQUADSTRUCT_H

#include "Vector3.h"
#include "Vector2.h"

struct ACQuadStruct
{
	Vector3 Vertices[4];
	Vector2 TexCoords[4];

	//inicializa com o centro no meio
	ACQuadStruct()
	{
		Vertices[0].X = -0.5f;
		Vertices[0].Y =  0.5f;
		Vertices[0].Z =  0.0f;

		TexCoords[0].X = 0.0f;
		TexCoords[0].Y = 0.0f;

		Vertices[1].X = -0.5f;
		Vertices[1].Y = -0.5f;
		Vertices[1].Z =  0.0f;

		TexCoords[1].X = 0.0f;
		TexCoords[1].Y = 1.0f;

		Vertices[2].X =  0.5f;
		Vertices[2].Y =  0.5f;
		Vertices[2].Z =  0.0f;

		TexCoords[2].X = 1.0f;
		TexCoords[2].Y = 0.0f;

		Vertices[3].X =  0.5f;
		Vertices[3].Y = -0.5f;
		Vertices[3].Z =  0.0f;

		TexCoords[3].X = 1.0f;
		TexCoords[3].Y = 1.0f;

	};
	  
};

#endif