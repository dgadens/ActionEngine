#pragma once

#include "MathConverter.h"
#include "ACRenderDeviceW.h"
#include "ACContentManagerW.h"
#include "ACCameraW.h"
#include "ACSkinW.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EngineWrapper
{
public ref class ACModelW
{
	public:
		ACModel* Value;

		ACModelW(ACRenderDeviceW^ gDevice, ACContentManagerW^ cManager);
		~ACModelW();

		void SetSkin(ACSkinW^ skin);
		ACSkinW^ GetSkin();

		//mesh
		void Load(String path);

		//configurations
		//void SetRenderBB(bool value);
		void SetRenderNormals(BOOL value);
		void SetRenderBones(BOOL value);

		//Animations
		void ActiveAnimation(String^ name);

		//update
		void Update(FLOAT elapsedTime);
		void Render(ACCameraW^ camera);

		//position
		void SetAbsolutePosition(float x, float y, float z);
		void SetAbsolutePosition(EngineMathCLI::Vector3^ value);
		void AddToPosition(float x, float y, float z);
		void AddToPosition(EngineMathCLI::Vector3^ value);

		//scale
		void SetAbsoluteScale(float value);
		void SetAbsoluteScale(float x, float y, float z);
		void SetAbsoluteScale(EngineMathCLI::Vector3^ value);
		void AddToScale(float value);
		void AddToScale(float x, float y, float z);
		void AddToScale(EngineMathCLI::Vector3^ value);

		//rotation
		void SetAbsoluteRotation(float x, float y, float z);
		void SetAbsoluteRotation(EngineMathCLI::Vector3^ value);
		void AddToRotation(float x, float y, float z);
		void AddToRotation(EngineMathCLI::Vector3^ value);

		void Release();

	};
}
