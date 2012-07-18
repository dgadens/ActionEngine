//****************************
// Contem todas as estruturas de vertices possiveis na engine
// Creator: Daniel Gadens
// Date: 12/05/2012
//****************************

#ifndef __ACVERTEXFORMATS_H
#define __ACVERTEXFORMATS_H

#include "EngineMath.h"

struct ACVertexPosition
{
	Vector3 position;
};

struct ACVertexPositionColored
{
	Vector3 position;
	Vector4 color;
};

struct ACVertexPositionTextured
{
	Vector3 position;
	Vector2 texcoord;
};

struct ACVertexPositionNormalTextured
{
	Vector3 position;
	Vector3 normal;
	Vector2 texcoord;
};

struct ACVertexPositionTexturedExtraInfo
{
	Vector3 position;
	Vector4 texcoord;
	Vector4 extraInfo;
};

struct ACVertexSkinnedMesh
{
	Vector3 position;
    Vector3 normal;
    Vector2 texcoord;
    Vector4 blendWeights;
    Vector4 blendIndices;
};

struct ACVertexSprite
{
	Vector2	position;
    Vector2	dimension;
	Vector4	rectangle;
	float   rotation;
	float   aspect;
	Vector2 origin;
    Vector4	color;
};

#endif