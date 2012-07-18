#include "OrientedBoundingBox.h"

OrientedBoundingBox::OrientedBoundingBox()
{
	ZeroMemory( &Edges, sizeof ( Vector3) * 8);
};

OrientedBoundingBox::OrientedBoundingBox(const BoundingBox* baseBB)
{
	ZeroMemory( &Edges, sizeof ( Vector3) * 8);
	CreateFromBB(baseBB, this);
};

OrientedBoundingBox::~OrientedBoundingBox()
{
};

void OrientedBoundingBox::CreateFromBB(const BoundingBox *bb, OrientedBoundingBox* outOBB)
{
	outOBB->Edges[0] = Vector3( bb->Min.X, bb->Min.Y, bb->Min.Z ); 
	outOBB->Edges[1] = Vector3( bb->Max.X, bb->Min.Y, bb->Min.Z ); 
	outOBB->Edges[2] = Vector3( bb->Min.X, bb->Max.Y, bb->Min.Z ); 
	outOBB->Edges[3] = Vector3( bb->Max.X, bb->Max.Y, bb->Min.Z ); 
	outOBB->Edges[4] = Vector3( bb->Min.X, bb->Min.Y, bb->Max.Z ); 
	outOBB->Edges[5] = Vector3( bb->Max.X, bb->Min.Y, bb->Max.Z ); 
	outOBB->Edges[6] = Vector3( bb->Min.X, bb->Max.Y, bb->Max.Z ); 
	outOBB->Edges[7] = Vector3( bb->Max.X, bb->Max.Y, bb->Max.Z ); 
};

void OrientedBoundingBox::TransformOBB(OrientedBoundingBox* OBB, const Matrix* world, OrientedBoundingBox* outOBB)
{
	Vector3::Transform(OBB->Edges, 0, world, outOBB->Edges, 0, CORNER_COUNT);
};