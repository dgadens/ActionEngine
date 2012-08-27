#include "ACSkin.h"
#include "ACRenderDevice.h"
#include "ACContentManager.h"

ACSkin::ACSkin(ACRenderDevice* pGDevice, ACContentManager* pCManager, BOOL useMaterial)
{
	memset(&Material,0, sizeof (ACMaterial));

	for(int i = 0; i < NUM_TEXTURES; i++)
		Textures[i] = nullptr;

	UseMaterial = useMaterial;

	mpGDevice = pGDevice;
	mpCManager = pCManager;
};

void ACSkin::SetMaterial(Vector3& ambientColor, Vector4& diffuseColor, Vector3& specularColor, Vector3& emissiveColor, float specularPower)
{
	Material.AmbientColor = ambientColor;
	Material.DiffuseColor = diffuseColor;
	Material.SpecularColor = specularColor;
	Material.EmissiveColor = emissiveColor;
	Material.SpecularPower = specularPower;
};

void ACSkin::SetTexture(std::string name, BYTE index)
{
	//se ja existe uma textura no indice passado entao ele remove primeiro pra atualizar as instancias do cmanager
	if (Textures[index] != nullptr)
		mpCManager->RemoveTexture(Textures[index]);

	Textures[index] = mpCManager->LoadTexture(name);
};
	
ACSkin::~ACSkin()
{
	Release();
};

void ACSkin::Release()
{
	for(char i = 0; i < NUM_TEXTURES; i++)
	{
		if (Textures[i] != nullptr)
		{
			if (!Textures[i]->IsRenderTarget)
				mpCManager->RemoveTexture(Textures[i]);
		}
	}
};