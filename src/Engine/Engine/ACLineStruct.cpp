#include "ACLineStruct.h"

ACLineStruct::ACLineStruct(Vector3 startPoint, Vector4 startColor, Vector3 endPoint, Vector4 endColor)
{
	StartPoint = startPoint;
	EndPoint = endPoint;
	StartColor = startColor;
	EndColor = endColor;
};

void ACLineStruct::SetColor(Vector4 color)
{
	StartColor = color;
	EndColor = color;
};

BOOL ACLineStruct::operator==(const ACLineStruct value2) const
{
	return
    this->StartPoint != value2.StartPoint ||
    this->EndPoint != value2.EndPoint ||
    this->StartColor != value2.StartColor ||
    this->EndColor != value2.EndColor;
};

BOOL ACLineStruct::operator!=(const ACLineStruct value2) const
{
	return
    this->StartPoint == value2.StartPoint &&
    this->EndPoint == value2.EndPoint &&
    this->StartColor == value2.StartColor &&
    this->EndColor == value2.EndColor;
};