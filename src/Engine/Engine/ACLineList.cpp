#include "ACLineList.h"

void ACLineList::Release()
{
	LineList.clear();
};

void ACLineList::AddCube(Vector3* min, Vector3* max, Vector4* color)
{
	LineList.clear();
	AddLineLocal(&Vector3(max->X, min->Y, max->Z), color, &Vector3(max->X, min->Y, min->Z), color);
    AddLineLocal(&Vector3(max->X, min->Y, max->Z), color, &Vector3(min->X, min->Y, max->Z), color);
    AddLineLocal(&Vector3(min->X, min->Y, max->Z), color, &Vector3(min->X, min->Y, min->Z), color);
    AddLineLocal(&Vector3(min->X, min->Y, min->Z), color, &Vector3(max->X, min->Y, min->Z), color);
    AddLineLocal(&Vector3(max->X, min->Y, max->Z), color, &Vector3(max->X, max->Y, max->Z), color);
    AddLineLocal(&Vector3(min->X, min->Y, max->Z), color, &Vector3(min->X, max->Y, max->Z), color);
    AddLineLocal(&Vector3(max->X, min->Y, min->Z), color, &Vector3(max->X, max->Y, min->Z), color);
    AddLineLocal(&Vector3(min->X, min->Y, min->Z), color, &Vector3(min->X, max->Y, min->Z), color);
    AddLineLocal(&Vector3(max->X, max->Y, max->Z), color, &Vector3(max->X, max->Y, min->Z), color);
    AddLineLocal(&Vector3(max->X, max->Y, max->Z), color, &Vector3(min->X, max->Y, max->Z), color);
    AddLineLocal(&Vector3(min->X, max->Y, max->Z), color, &Vector3(min->X, max->Y, min->Z), color);
    AddLineLocal(&Vector3(min->X, max->Y, min->Z), color, &Vector3(max->X, max->Y, min->Z), color);
};

void ACLineList::AddLineLocal(Vector3* startPoint, Vector4* startColor, Vector3* endPoint, Vector4* endColor)
{
	ACLineStruct line(*startPoint, *startColor, *endPoint, *endColor);
	LineList.push_back(line);
};

void ACLineList::AddLine(Vector3* startPoint, Vector3* endPoint, Vector4* color)
{
	LineList.clear();
	AddLineLocal(startPoint, color, endPoint, color);
};

void ACLineList::AddLine(Vector3* startPoint, Vector4* startColor, Vector3* endPoint, Vector4* endColor)
{
	LineList.clear();
	AddLineLocal(startPoint, startColor, endPoint, endColor);
};

void ACLineList::AddNormals(AMT_MODEL* model, Vector4* color)
{
	LineList.clear();
	UINT vertexCount = model->Head.NumVertices;
	for (int i = 0; i<vertexCount; i++)
	{
		Vector3 start = model->pVertices[i]->Position;
		Vector3 end = start + (model->pVertices[i]->Normal / 20);

		AddLineLocal(&start, color, &end, color);
	}
};

void ACLineList::AddBoundingBox(BoundingBox* boundingBox, Vector4* color)
{
	AddCube(&boundingBox->Min, &boundingBox->Max, color);
};

void ACLineList::AddSphere(Vector3* center, float radius, Vector4* color)
{
	LineList.clear();

	int index=0;
    Vector3** vY = new Vector3*[180];
    Vector3** vX = new Vector3*[180];
    Vector3** vZ = new Vector3*[180];
    float s = 0, c = 0;
    for (int i = 0, j = 0; i < 360; i+=2, j+=2)
    {
        s = sin(MathHelper::ToRadians(i)) * radius;
        c = -cos(MathHelper::ToRadians(i)) * radius;

        vY[index] = new Vector3(center->X + c,
                            center->Y,
                            center->Z + s);

        vX[index] = new Vector3(center->X,
                            center->Y + c,
                            center->Z + s);

        vZ[index] = new Vector3(center->X + s,
                            center->Y + c,
                            center->Z);
        index++;
    }

    for (int i = 0; i < 180 - 1; i++)
    {
        AddLineLocal(vY[i], color, vY[i + 1], color);
        AddLineLocal(vX[i], color, vX[i + 1], color);
        AddLineLocal(vZ[i], color, vZ[i + 1], color);
    }
    AddLineLocal(vY[180 - 1], color, vY[0], color);
    AddLineLocal(vX[180 - 1], color, vX[0], color);
    AddLineLocal(vZ[180 - 1], color, vZ[0], color);

	for (int i = 0; i < 180 - 1; i++)
    {
		delete vY[i];
		delete vX[i];
    	delete vZ[i];
	}

	delete vY;
	delete vX;
    delete vZ;
};