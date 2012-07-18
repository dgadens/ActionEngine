//***************************
// Classe estatica gerenciadora das linhas
// Creator: Daniel Gadens
// date: 17/06/2011
//***************************


#ifndef __ACLINEMANAGER_H
#define __ACLINEMANAGER_H

#include "ACLineList.h"
#include <map>
#include <string>

class ACLineManager
{
private:
	//mantem todas as linhas ja carregadas
	static std::map<std::string, ACLineList*> mapLines;

public:
	static BOOL LineExists(std::string name);
	static void AddLine(std::string name, Vector3* startPoint, Vector4* startColor, Vector3* endPoint, Vector4* endColor);
	static void AddLine(std::string name, Vector3* startPoint, Vector3* endPoint, Vector4* color);
	static void AddNormals(std::string name, AMT_MODEL* model, Vector4* color);
	static void AddBoundingBox(std::string name, BoundingBox* boundingBox, Vector4* color);
	static void AddSphere(std::string name, Vector3* center, float radius, Vector4* color);
	static ACLineList* GetLine(std::string name);
	static void RemoveLine(std::string name);

};

#endif