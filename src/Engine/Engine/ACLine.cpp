#include "ACLine.h"
#include "ACRenderDevice.h"

ACLine::ACLine(ACRenderDevice* gDevice)
{
	mpGDevice = gDevice;
	Matrix::ToIdentity(&World);
	pLines = nullptr;
	pAPIBuffer = nullptr;
};

ACLine::~ACLine()
{
	if (pLines!=nullptr)
	{
		ACLineManager::RemoveLine(Name);
	}
};

void ACLine::AddLine(std::string name, Vector3* startPoint, Vector4* startColor, Vector3* endPoint, Vector4* endColor, BufferOperation bo)
{
	//se ele forca a insercao entao ele cria um id novo com o id
	Name = name;
	if (bo == BO_ForceInsert)
		Name = Name + GenericTools::IntToString(ACGlobals::GlobalID);

	pLines = ACLineManager::GetLine(Name);
	if (pLines==nullptr)
	{
		ACLineManager::AddLine(Name, startPoint, startColor, endPoint, endColor);
		pLines = ACLineManager::GetLine(Name);
	}
	else
	{
		if (bo == BO_UpdateData)
			pLines->AddLine(startPoint, startColor, endPoint, endColor);

	}
};

void ACLine::AddLine(std::string name, Vector3* startPoint, Vector3* endPoint, Vector4* color, BufferOperation bo)
{
	//se ele forca a insercao entao ele cria um id novo com o id
	Name = name;
	if (bo == BO_ForceInsert)
		Name = Name + GenericTools::IntToString(ACGlobals::GlobalID);

	pLines = ACLineManager::GetLine(Name);
	if (pLines==nullptr)
	{
		ACLineManager::AddLine(Name, startPoint, endPoint, color);
		pLines = ACLineManager::GetLine(Name);
	}
	else
	{
		if (bo == BO_UpdateData)
			pLines->AddLine(startPoint, endPoint, color);

	}
};

void ACLine::AddNormals(std::string name, AMT_MODEL* model, Vector4* color, BufferOperation bo)
{
	//se ele forca a insercao entao ele cria um id novo com o id
	Name = name;
	if (bo == BO_ForceInsert)
		Name = Name + GenericTools::IntToString(ACGlobals::GlobalID);

	pLines = ACLineManager::GetLine(Name);
	if (pLines==nullptr)
	{
		ACLineManager::AddNormals(Name, model, color);
		pLines = ACLineManager::GetLine(Name);
	}
	else
	{
		if (bo == BO_UpdateData)
			pLines->AddNormals(model, color);

	}
};

void ACLine::AddBoundingBox(std::string name, BoundingBox* boundingBox, Vector4* color, BufferOperation bo)
{
	//se ele forca a insercao entao ele cria um id novo com o id
	Name = name;
	if (bo == BO_ForceInsert)
		Name = Name + GenericTools::IntToString(ACGlobals::GlobalID);

	pLines = ACLineManager::GetLine(Name);
	if (pLines==nullptr)
	{
		ACLineManager::AddBoundingBox(Name, boundingBox, color);
		pLines = ACLineManager::GetLine(Name);
	}
	else
	{
		if (bo == BO_UpdateData)
			pLines->AddBoundingBox(boundingBox, color);

	}
};

void ACLine::AddSphere(std::string name, Vector3* center, float radius, Vector4* color, BufferOperation bo)
{
	//se ele forca a insercao entao ele cria um id novo com o id
	Name = name;
	if (bo == BO_ForceInsert)
		Name = Name + GenericTools::IntToString(ACGlobals::GlobalID);

	pLines = ACLineManager::GetLine(Name);
	if (pLines==nullptr)
	{
		ACLineManager::AddSphere(name, center, radius, color);
		pLines = ACLineManager::GetLine(Name);
	}
	else
	{
		if (bo == BO_UpdateData)
			pLines->AddSphere(center, radius, color);

	}
};

void ACLine::Update(Matrix* world)
{
	memcpy(&World, world, sizeof ( Matrix ) );
};

void ACLine::Render(ACCamera* camera, BOOL isDynamicBuffer)
{
	//mpGDevice->RenderLines(camera, this, isDynamicBuffer);
};