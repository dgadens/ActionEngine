#include "ACLineManager.h"

std::map<std::string, ACLineList*> ACLineManager::mapLines;

BOOL ACLineManager::LineExists(std::string name)
{
	std::map<std::string, ACLineList*>::size_type count = mapLines.count(name);

	return count > 0;
};

void ACLineManager::AddLine(std::string name, Vector3* startPoint, Vector4* startColor, Vector3* endPoint, Vector4* endColor)
{
	std::map<std::string, ACLineList*>::size_type count = mapLines.count(name);

	if (count <= 0)
	{
		ACLineList* pline = new ACLineList();
		pline->GlobalID = ACGlobals::GlobalID++;
		pline->Instance = 1;
		pline->AddLine(startPoint, startColor, endPoint, endColor);
		// inser o elemento
		mapLines.insert(std::pair<std::string, ACLineList*>(name, pline));
	}
	else
	{
		ACLineList* pLine = mapLines[name];
		pLine->Instance++;
	}
};

void ACLineManager::AddLine(std::string name, Vector3* startPoint, Vector3* endPoint, Vector4* color)
{
	std::map<std::string, ACLineList*>::size_type count = mapLines.count(name);

	if (count <= 0)
	{
		ACLineList* pline = new ACLineList();
		pline->GlobalID = ACGlobals::GlobalID++;
		pline->Instance = 1;
		pline->AddLine(startPoint, endPoint, color);
		// inser o elemento
		mapLines.insert(std::pair<std::string, ACLineList*>(name, pline));
	}
	else
	{
		ACLineList* pLine = mapLines[name];
		pLine->Instance++;
	}
};

void ACLineManager::AddNormals(std::string name, AMT_MODEL* model, Vector4* color)
{
	std::map<std::string, ACLineList*>::size_type count = mapLines.count(name);

	if (count <= 0)
	{
		ACLineList* pline = new ACLineList();
		pline->GlobalID = ACGlobals::GlobalID++;
		pline->Instance = 1;
		pline->AddNormals(model, color);
		// inser o elemento
		mapLines.insert(std::pair<std::string, ACLineList*>(name, pline));
	}
	else
	{
		ACLineList* pLine = mapLines[name];
		pLine->Instance++;
	}
};

void ACLineManager::AddBoundingBox(std::string name, BoundingBox* boundingBox, Vector4* color)
{
	std::map<std::string, ACLineList*>::size_type count = mapLines.count(name);

	if (count <= 0)
	{
		ACLineList* pline = new ACLineList();
		pline->GlobalID = ACGlobals::GlobalID++;
		pline->Instance = 1;
		pline->AddBoundingBox(boundingBox, color);
		// inser o elemento
		mapLines.insert(std::pair<std::string, ACLineList*>(name, pline));
	}
	else
	{
		ACLineList* pLine = mapLines[name];
		pLine->Instance++;
	}
};

void ACLineManager::AddSphere(std::string name, Vector3* center, float radius, Vector4* color)
{
	std::map<std::string, ACLineList*>::size_type count = mapLines.count(name);

	if (count <= 0)
	{
		ACLineList* pline = new ACLineList();
		pline->GlobalID = ACGlobals::GlobalID++;
		pline->Instance = 1;
		pline->AddSphere(center, radius, color);
		// inser o elemento
		mapLines.insert(std::pair<std::string, ACLineList*>(name, pline));
	}
	else
	{
		ACLineList* pLine = mapLines[name];
		pLine->Instance++;
	}
};

void ACLineManager::RemoveLine(std::string name)
{
	if (LineExists(name))
	{
		ACLineList* pLine = mapLines[name];
		pLine->Instance--;
		if (pLine->Instance == 0)
		{
			SAFE_DELETE(pLine);
			mapLines.erase(name);
		}
	}
};

ACLineList* ACLineManager::GetLine(std::string name)
{
	std::map<std::string, ACLineList*>::size_type count = mapLines.count(name);

	if (count > 0)
		return mapLines[name];
	else
		return nullptr;
};
