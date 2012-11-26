#include "StdAfx.h"
#include "ACCameraW.h"

using namespace EngineWrapper;

ACCameraW::ACCameraW()
{
	Value = new ACCamera();
}

ACCameraW::~ACCameraW()
{
	Release();
}

void ACCameraW::Release()
{
	Value->Release();
}

//sets
void ACCameraW::SetCameraProjection(ACCameraProjection value)
{
	Value->SetCameraProjection(value);
}

void ACCameraW::SetFOV(FLOAT value)
{
	Value->SetFOV(value);
}

void ACCameraW::SetWidth(FLOAT value)
{
	Value->SetWidth(value);
}

void ACCameraW::SetHeight(FLOAT value)
{
	Value->SetHeight(value);
}

void ACCameraW::SetNear(FLOAT value)
{
	Value->SetNear(value);
}

void ACCameraW::SetFar(FLOAT value)
{
	Value->SetFar(value);
}

void ACCameraW::SetPosition(EngineMathCLI::Vector3^ value)
{
	Value->SetPosition(MathConverter::ToUnmanaged(value));
}

void ACCameraW::SetTarget(EngineMathCLI::Vector3^ value)
{
	Value->SetTarget(MathConverter::ToUnmanaged(value));
}

void ACCameraW::SetUp(EngineMathCLI::Vector3^ value)
{
	Value->SetUp(MathConverter::ToUnmanaged(value));
}

//gets
ACCameraProjection ACCameraW::GetCameraProjection()
{
	return Value->GetCameraProjection();
}

FLOAT ACCameraW::GetFOV()
{
	return Value->GetFOV();
}

FLOAT ACCameraW::GetWidth()
{
	return Value->GetWidth();
}

FLOAT ACCameraW::GetHeight()
{
	return Value->GetHeight();
}

FLOAT ACCameraW::GetNear()
{
	return Value->GetNear();
}

FLOAT ACCameraW::GetFar()
{
	return Value->GetFar();
}

EngineMathCLI::Vector3 ACCameraW::GetPosition()
{
	return MathConverter::ToManaged(const_cast<Vector3*>(&Value->GetPosition()));
}

EngineMathCLI::Vector3 ACCameraW::GetTarget()
{
	return MathConverter::ToManaged(const_cast<Vector3*>(&Value->GetTarget()));
}

EngineMathCLI::Vector3 ACCameraW::GetUp()
{
	return MathConverter::ToManaged(const_cast<Vector3*>(&Value->GetUp()));
}

EngineMathCLI::Matrix ACCameraW::GetView()
{
	return MathConverter::ToManaged(const_cast<Matrix*>(&Value->GetView()));
}

EngineMathCLI::Matrix ACCameraW::GetProjection()
{
	return MathConverter::ToManaged(const_cast<Matrix*>(&Value->GetProjection()));
}

EngineMathCLI::Matrix ACCameraW::GetViewProjection()
{
	return MathConverter::ToManaged(const_cast<Matrix*>(&Value->GetViewProjection()));
}

void ACCameraW::Update()
{
	Value->Update();
}
