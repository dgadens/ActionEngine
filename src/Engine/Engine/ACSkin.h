//******************************************************
// Estrutura q junta o material com as texturas
// creator: Daniel Gadens
// date: 22/04/2012
//*****************************************************


#ifndef __ACSKIN_H
#define __ACSKIN_H

#include "EngineCommon.h"
#include "ACMaterial.h"
#include "ACTexture.h"

class ACContentManager;
class ACRenderDevice;

class ACSkin
{

private:
	ACRenderDevice* mpGDevice;
	ACContentManager* mpCManager;

public:
	//indica se usa o material ou apenas as texturas
	BOOL UseMaterial;
	ACMaterial Material;
	ACTexture* Textures[NUM_TEXTURES];

	ACSkin(ACRenderDevice* pGDevice, ACContentManager* pCManager, BOOL useMaterial);
	~ACSkin();

	void SetMaterial(Vector3& ambientColor, Vector4& diffuseColor, Vector3& specularColor, Vector3& emissiveColor, float specularPower);
	void SetTexture(std::string name, BYTE index);

	void Release();	 
};					 

#endif