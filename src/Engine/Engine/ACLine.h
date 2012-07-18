//*****************************************
// classe da camera
// creator: Daniel Gadens
// date: 17/06/2011
//****************************************


#ifndef __ACLINE_H
#define __ACLINE_H

#include "ActionTypeDef.h"
#include "EngineMath.h"
#include "ACLineStruct.h"
#include "ACLineList.h"
#include "ACLineManager.h"
#include "ACAMTStructs.h"
#include "ACCamera.h"
#include <vector>

class ACRenderDevice;

class ACLine
{
private:
	ACRenderDevice* mpGDevice;

public:
	std::string Name;
	Matrix World;
	ACLineList* pLines;

	void* pAPIBuffer;

	ACLine(ACRenderDevice* gDevice);
	~ACLine();

	void AddLine(std::string name, Vector3* startPoint, Vector4* startColor, Vector3* endPoint, Vector4* endColor, BufferOperation bo);
	void AddLine(std::string name, Vector3* startPoint, Vector3* endPoint, Vector4* color, BufferOperation bo);
	void AddNormals(std::string name, AMT_MODEL* model, Vector4* color, BufferOperation bo);
	void AddBoundingBox(std::string name, BoundingBox* boundingBox, Vector4* color, BufferOperation bo);
	void AddSphere(std::string name, Vector3* center, float radius, Vector4* color, BufferOperation bo);

	void Update(Matrix* world);
	void Render(ACCamera* camera, BOOL isDynamicBuffer);

};

#endif