//********************************
// Classe com varios metodos interessantes
// Creator: Daniel Gadens
// Date: 07/07/2012
//********************************

#ifndef __ACUTILS_H
#define __ACUTILS_H

#include "ACVertexFormat.h"
#include <math.h>

class ACUtils
{
public:
	static ACVertexPositionColored* CreateLine(const Vector3& init, const Vector4& initColor, const Vector3& final, const Vector4& finalColor);
	static ACVertexPositionColored* CreateBBLines(const BoundingBox& bb, const Vector4& color);
	static ACVertexPositionColored* CreateSphereLines(const BoundingSphere& bs, const Vector4& color1, const Vector4& color2, const Vector4& color3);

};

//Return 2 vertices
inline ACVertexPositionColored* ACUtils::CreateLine(const Vector3& init, const Vector4& initColor, const Vector3& final, const Vector4& finalColor)
{
	ACVertexPositionColored* vertices = new ACVertexPositionColored[2];
	vertices[0].position = init;
	vertices[0].color = initColor;
	vertices[1].position = final;
	vertices[1].color = finalColor;

	return vertices;
};

//Return 24 vertices
inline ACVertexPositionColored* ACUtils::CreateBBLines(const BoundingBox& bb, const Vector4& color)
{
	ACVertexPositionColored* vertices = new ACVertexPositionColored[24];

	Vector3 max = bb.Max;
	Vector3 min = bb.Min;

	ACVertexPositionColored* L1 = CreateLine(Vector3(max.X, min.Y, max.Z), color, Vector3(max.X, min.Y, min.Z), color);
    ACVertexPositionColored* L2 = CreateLine(Vector3(max.X, min.Y, max.Z), color, Vector3(min.X, min.Y, max.Z), color);
    ACVertexPositionColored* L3 = CreateLine(Vector3(min.X, min.Y, max.Z), color, Vector3(min.X, min.Y, min.Z), color);
    ACVertexPositionColored* L4 = CreateLine(Vector3(min.X, min.Y, min.Z), color, Vector3(max.X, min.Y, min.Z), color);
    ACVertexPositionColored* L5 = CreateLine(Vector3(max.X, min.Y, max.Z), color, Vector3(max.X, max.Y, max.Z), color);
    ACVertexPositionColored* L6 = CreateLine(Vector3(min.X, min.Y, max.Z), color, Vector3(min.X, max.Y, max.Z), color);
    ACVertexPositionColored* L7 = CreateLine(Vector3(max.X, min.Y, min.Z), color, Vector3(max.X, max.Y, min.Z), color);
    ACVertexPositionColored* L8 = CreateLine(Vector3(min.X, min.Y, min.Z), color, Vector3(min.X, max.Y, min.Z), color);
    ACVertexPositionColored* L9 = CreateLine(Vector3(max.X, max.Y, max.Z), color, Vector3(max.X, max.Y, min.Z), color);
    ACVertexPositionColored* L10 = CreateLine(Vector3(max.X, max.Y, max.Z), color, Vector3(min.X, max.Y, max.Z), color);
    ACVertexPositionColored* L11 = CreateLine(Vector3(min.X, max.Y, max.Z), color, Vector3(min.X, max.Y, min.Z), color);
    ACVertexPositionColored* L12 = CreateLine(Vector3(min.X, max.Y, min.Z), color, Vector3(max.X, max.Y, min.Z), color);

	vertices[0] = L1[0];
	vertices[1] = L1[1];
	vertices[2] = L2[0];
	vertices[3] = L2[1];
	vertices[4] = L3[0];
	vertices[5] = L3[1];
	vertices[6] = L4[0];
	vertices[7] = L4[1];
	vertices[8] = L5[0];
	vertices[9] = L5[1];
	vertices[10] = L6[0];
	vertices[11] = L6[1];
	vertices[12] = L7[0];
	vertices[13] = L7[1];
	vertices[14] = L8[0];
	vertices[15] = L8[1];
	vertices[16] = L9[0];
	vertices[17] = L9[1];
	vertices[18] = L10[0];
	vertices[19] = L10[1];
	vertices[20] = L11[0];
	vertices[21] = L11[1];
	vertices[22] = L12[0];
	vertices[23] = L12[1];

	delete[] L1;
	delete[] L2;
	delete[] L3;
	delete[] L4;
	delete[] L5;
	delete[] L6;
	delete[] L7;
	delete[] L8;
	delete[] L9;
	delete[] L10;
	delete[] L11;
	delete[] L12;

	return vertices;
};

//Return 360 vertices, USE Strip
inline ACVertexPositionColored* ACUtils::CreateSphereLines(const BoundingSphere& bs, const Vector4& color1, const Vector4& color2, const Vector4& color3)
{
	int i = 0;
	int angle = 0;
	ACVertexPositionColored* circle = new ACVertexPositionColored[360];

	Vector3 p;

	//circulo 1
	for (i = 0; i < 119; i++)
	{
		p.X = bs.Center.X + cos(MathHelper::ToRadians((float)angle * 3)) * bs.Radius;
		p.Y = bs.Center.Y + sin(MathHelper::ToRadians((float)angle * 3)) * bs.Radius;
		p.Z = bs.Center.Z;

		circle[i].position = p;
		circle[i].color = color1;

		angle++;

		//o ultimo vertice junta com o primeiro
		if (i < 118)
		{
			p.X = bs.Center.X + cos(MathHelper::ToRadians((float)(angle+1) * 3)) * bs.Radius;
			p.Y = bs.Center.Y + sin(MathHelper::ToRadians((float)(angle+1) * 3)) * bs.Radius;
			p.Z = bs.Center.Z;

			i++;
			circle[i].position = p;
			circle[i].color = color1;
		}
		else
			circle[119] = circle[0];

		angle++;
	}
	
	//circulo2
	angle = 0;
	for (i = 120; i < 239; i++)
	{
		p.X = bs.Center.X + cos(MathHelper::ToRadians((float)angle * 3)) * bs.Radius;
		p.Y = bs.Center.Y;
		p.Z = bs.Center.Z + sin(MathHelper::ToRadians((float)angle * 3)) * bs.Radius;

		circle[i].position = p;
		circle[i].color = color2;

		angle++;

		//o ultimo vertice junta com o primeiro
		if (i < 238)
		{
			p.X = bs.Center.X + cos(MathHelper::ToRadians((float)(angle+1) * 3)) * bs.Radius;
			p.Y = bs.Center.Y;
			p.Z = bs.Center.Z + sin(MathHelper::ToRadians((float)(angle+1) * 3)) * bs.Radius;

			i++;
			circle[i].position = p;
			circle[i].color = color2;
		}
		else
			circle[239] = circle[120];

		angle++;
	}

	//circulo3
	angle = 0;
	for (i = 240; i < 359; i++)
	{
		p.X = bs.Center.X;
		p.Y = bs.Center.Y + cos(MathHelper::ToRadians((float)angle * 3)) * bs.Radius;
		p.Z = bs.Center.Z + sin(MathHelper::ToRadians((float)angle * 3)) * bs.Radius;

		circle[i].position = p;
		circle[i].color = color3;

		angle++;

		//o ultimo vertice junta com o primeiro
		if (i < 358)
		{
			p.X = bs.Center.X;
			p.Y = bs.Center.Y + cos(MathHelper::ToRadians((float)(angle+1) * 3)) * bs.Radius;
			p.Z = bs.Center.Z + sin(MathHelper::ToRadians((float)(angle+1) * 3)) * bs.Radius;

			i++;
			circle[i].position = p;
			circle[i].color = color3;
		}
		else
			circle[359] = circle[240];

		angle++;
	}
	
	return circle;
};

#endif