#pragma once

#include "MathConverter.h"
#include "ACCamera.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EngineWrapper
{
public ref class ACCameraW
{
	public:
		ACCamera* Value;

		ACCameraW();
		~ACCameraW();

		//sets
		void SetCameraProjection(ACCameraProjection value);
		void SetFOV(FLOAT value);
		void SetWidth(FLOAT value);
		void SetHeight(FLOAT value);
		void SetNear(FLOAT value);
		void SetFar(FLOAT value);
		void SetPosition(EngineMathCLI::Vector3^ value);
		void SetTarget(EngineMathCLI::Vector3^ value);
		void SetUp(EngineMathCLI::Vector3^ value);

		//gets
		ACCameraProjection GetCameraProjection();
		FLOAT GetFOV();
		FLOAT GetWidth();
		FLOAT GetHeight();
		FLOAT GetNear();
		FLOAT GetFar();

		EngineMathCLI::Vector3 GetPosition();
		EngineMathCLI::Vector3 GetTarget();
		EngineMathCLI::Vector3 GetUp();

		EngineMathCLI::Matrix GetView();
		EngineMathCLI::Matrix GetProjection();
		EngineMathCLI::Matrix GetViewProjection();

		void Update();

		void Release();

	};
}
