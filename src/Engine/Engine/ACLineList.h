//***************************
// Estrutura que mantem a lista de linhas
// Creator: Daniel Gadens
// date: 16/06/2011
//***************************


#ifndef __ACLINELIST_H
#define __ACLINELIST_H

#include "EngineMath.h"
#include "ACLineStruct.h"
#include "ACAMTStructs.h"
#include <vector>

struct ACLineList
{
private:
	void AddCube(Vector3* min, Vector3* max, Vector4* color);
	void AddLineLocal(Vector3* startPoint, Vector4* startColor, Vector3* endPoint, Vector4* endColor);

public:
	UINT Instance;  //guarda o numero da instancia, quando for deletar ele so deleta se o numero de instancias for zero
	UINT GlobalID;

	std::vector<ACLineStruct> LineList;

	ACLineList() {};
	~ACLineList() {};

	void AddLine(Vector3* startPoint, Vector4* startColor, Vector3* endPoint, Vector4* endColor);
	void AddLine(Vector3* startPoint, Vector3* endPoint, Vector4* color);
	void AddNormals(AMT_MODEL* model, Vector4* color);
	void AddBoundingBox(BoundingBox* boundingBox, Vector4* color);
	void AddSphere(Vector3* center, float radius, Vector4* color);

	void Release();

};

#endif