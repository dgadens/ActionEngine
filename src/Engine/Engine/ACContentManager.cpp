#include "ACContentManager.h"
#include "ACRenderDevice.h"
#include "ACAudioDevice.h"

ACContentManager::ACContentManager(ACRenderDevice* gDevice, ACAudioDevice* aDevice)
{	
	mpGDevice = gDevice;
	mpADevice = aDevice;

	//seta o contentmanager para o graphics device
	mpGDevice->SetContentManager(this);
	mpADevice->SetContentManager(this);
};

ACContentManager::~ACContentManager()
{
	Release();
};

void ACContentManager::Release()
{
	if (textures.size() > 0 || 
		sounds.size() > 0   || 
		vertexShaders.size() > 0 || 
		pixelShaders.size() > 0 || 
		geometryShaders.size() > 0 ||
		models.size() > 0 ||
		fonts.size() > 0)
	{
		std::string abc = "fudeu";
	}
};

#pragma region Models

ACModelDefinition* ACContentManager::LoadModel(const std::string& name)
{
	//se a texture ja existe entao ele retorna apenas o indice
	for (auto it = models.begin(); it != models.end(); it++)
	{
		if (it->second->Name == name)
		{
			it->second->Instance++;
			return it->second;
		}
	}

	//pega o endereco completo do arquivo
	std::string currentPath = ACConfigurations::Instance()->GetPathModels();
	currentPath.append(name);

	//cria o model def
	ACModelDefinition* pModelDef = nullptr;
	//cria o amt q vai ser retornado e ja carrega do arquivo
	AMT_MODEL* model = new AMT_MODEL();
	ACAMTLoader::LoadFromFile(currentPath.c_str(), model);
	
	pModelDef = new ACModelDefinition(mpGDevice, this);
	pModelDef->Name = name;
	pModelDef->Instance = 1;
	pModelDef->ID = ACConfigurations::Instance()->GlobalID++;
	pModelDef->Prepare(model);

	//adiciona a textura
	models.insert(std::pair<UINT, ACModelDefinition*>(pModelDef->ID, pModelDef));

	return pModelDef;
};

void ACContentManager::RemoveModel(ACModelDefinition* model)
{
	if (model != nullptr)
	{
		if (model->Instance > 1)
			model->Instance--;
		else
		{
			UINT id = model->ID;
			//remove da memoria o dado
			SAFE_DELETE(model);
			//remove da lista
			models.erase(id);
		}
	}
};

void ACContentManager::RemoveModel(const std::string& modelName)
{
	ACModelDefinition* model = nullptr;

	for (auto it = models.begin(); it != models.end(); it++)
	{
		if (it->second->Name == modelName)
		{
			model = it->second;
			break;
		}
	}

	RemoveModel(model);
};

#pragma endregion

#pragma region Textures

ACTexture* ACContentManager::LoadTexture(std::string name)
{
	//se a texture ja existe entao ele retorna apenas o indice
	for (auto it = textures.begin(); it != textures.end(); it++)
	{
		if (it->second->Name == name)
		{
			it->second->Instance++;
			return it->second;
		}
	}

	std::string currentPath = ACConfigurations::Instance()->GetPathTextures();
	currentPath.append(name);

	ACTexture* pTexture = nullptr;
	mpGDevice->LoadTexture(currentPath, &pTexture);
	pTexture->Name = name;
	pTexture->Instance = 1;
	pTexture->ID = ACConfigurations::Instance()->GlobalID++;
	pTexture->IsRenderTarget = false;

	//adiciona a textura
	textures.insert(std::pair<UINT, ACTexture*>(pTexture->ID, pTexture));

	return pTexture;
};

void ACContentManager::RemoveTexture(ACTexture* texture)
{
	if (texture != nullptr)
	{
		if (texture->Instance > 1)
			texture->Instance--;
		else
		{
			UINT id = texture->ID;

			//remove a textura da api depois remove da lista
			mpGDevice->ReleaseTexture(texture);
			//remove da memoria o dado
			SAFE_DELETE(texture);
			//remove da lista
			textures.erase(id);
		}
	}
};

void ACContentManager::RemoveTexture(std::string textureName)
{
	ACTexture* texture = nullptr;

	for (auto it = textures.begin(); it != textures.end(); it++)
	{
		if (it->second->Name == textureName)
		{
			texture = it->second;
			break;
		}
	}

	RemoveTexture(texture);
};

#pragma endregion

#pragma region Shaders

//VERTEX SHADER
ACVertexShader* ACContentManager::LoadVertexShader(std::string name, VertexFormat vertexFormat)
{
	//se o shader ja existe entao ele retorna apenas o indice
	for (auto it = vertexShaders.begin(); it != vertexShaders.end(); it++)
	{
		if (it->second->Name == name)
		{
			it->second->Instance++;
			return it->second;
		}
	}

	std::string currentPath = ACConfigurations::Instance()->GetPathShaders();
	currentPath.append(name);

	ACVertexShader* pVS = nullptr;
	mpGDevice->CompileVS(currentPath, vertexFormat, &pVS);
	pVS->Name = name;
	pVS->Instance = 1;
	pVS->ID = ACConfigurations::Instance()->GlobalID++;

	//adiciona o shader
	vertexShaders.insert(std::pair<UINT, ACVertexShader*>(pVS->ID, pVS));

	return pVS;
};

void ACContentManager::RemoveVertexShader(ACVertexShader* vertexShader)
{
	if (vertexShader != nullptr)
	{
		if (vertexShader->Instance > 1)
			vertexShader->Instance--;
		else
		{
			UINT id = vertexShader->ID;

			//remove o shader da api depois remove da lista
			mpGDevice->ReleaseVS(vertexShader);
			//remove da memoria o dado
			SAFE_DELETE(vertexShader);
			//remove da lista
			vertexShaders.erase(id);
		}
	}
};

void ACContentManager::RemoveVertexShader(std::string vertexShaderName)
{
	ACVertexShader* vertexShader = nullptr;

	for (auto it = vertexShaders.begin(); it != vertexShaders.end(); it++)
	{
		if (it->second->Name == vertexShaderName)
		{
			vertexShader = it->second;
			break;
		}
	}

	RemoveVertexShader(vertexShader);
};

//PIXEL SHADER
ACPixelShader* ACContentManager::LoadPixelShader(std::string name)
{
	//se o shader ja existe entao ele retorna apenas o indice
	for (auto it = pixelShaders.begin(); it != pixelShaders.end(); it++)
	{
		if (it->second->Name == name)
		{
			it->second->Instance++;
			return it->second;
		}
	}

	std::string currentPath = ACConfigurations::Instance()->GetPathShaders();
	currentPath.append(name);

	ACPixelShader* pPS = nullptr;
	mpGDevice->CompilePS(currentPath, &pPS);
	pPS->Name = name;
	pPS->Instance = 1;
	pPS->ID = ACConfigurations::Instance()->GlobalID++;

	//adiciona o shader
	pixelShaders.insert(std::pair<UINT, ACPixelShader*>(pPS->ID, pPS));

	return pPS;
};

void ACContentManager::RemovePixelShader(ACPixelShader* pixelShader)
{
	if (pixelShader != nullptr)
	{
		if (pixelShader->Instance > 1)
			pixelShader->Instance--;
		else
		{
			UINT id = pixelShader->ID;

			//remove o shader da api depois remove da lista
			mpGDevice->ReleasePS(pixelShader);
			//remove da memoria o dado
			SAFE_DELETE(pixelShader);
			//remove da lista
			pixelShaders.erase(id);
		}
	}
};

void ACContentManager::RemovePixelShader(std::string pixelShaderName)
{
	ACPixelShader* pixelShader = nullptr;

	for (auto it = pixelShaders.begin(); it != pixelShaders.end(); it++)
	{
		if (it->second->Name == pixelShaderName)
		{
			pixelShader = it->second;
			break;
		}
	}

	RemovePixelShader(pixelShader);
};

//GEOMETRY SHADER
ACGeometryShader* ACContentManager::LoadGeometryShader(std::string name)
{
	//se o shader ja existe entao ele retorna apenas o indice
	for (auto it = geometryShaders.begin(); it != geometryShaders.end(); it++)
	{
		if (it->second->Name == name)
		{
			it->second->Instance++;
			return it->second;
		}
	}

	std::string currentPath = ACConfigurations::Instance()->GetPathShaders();
	currentPath.append(name);

	ACGeometryShader* pGS = nullptr;
	mpGDevice->CompileGS(currentPath, &pGS);
	pGS->Name = name;
	pGS->Instance = 1;
	pGS->ID = ACConfigurations::Instance()->GlobalID++;

	//adiciona o shader
	geometryShaders.insert(std::pair<UINT, ACGeometryShader*>(pGS->ID, pGS));

	return pGS;
};

void ACContentManager::RemoveGeometryShader(ACGeometryShader* geometryShader)
{
	if (geometryShader != nullptr)
	{
		if (geometryShader->Instance > 1)
			geometryShader->Instance--;
		else
		{
			UINT id = geometryShader->ID;

			//remove o shader da api depois remove da lista
			mpGDevice->ReleaseGS(geometryShader);
			//remove da memoria o dado
			SAFE_DELETE(geometryShader);
			//remove da lista
			geometryShaders.erase(id);
		}
	}
};

void ACContentManager::RemoveGeometryShader(std::string geometryShaderName)
{
	ACGeometryShader* geometryShader = nullptr;

	for (auto it = geometryShaders.begin(); it != geometryShaders.end(); it++)
	{
		if (it->second->Name == geometryShaderName)
		{
			geometryShader = it->second;
			break;
		}
	}

	RemoveGeometryShader(geometryShader);
};
#pragma endregion

#pragma region Sounds

ACSound* ACContentManager::LoadSound(std::string name)
{
	//se a texture ja existe entao ele retorna apenas o indice
	for (auto it = sounds.begin(); it != sounds.end(); it++)
	{
		if (it->second->Name == name)
		{
			it->second->Instance++;
			return it->second;
		}
	}

	std::string currentPath = ACConfigurations::Instance()->GetPathSounds();
	currentPath.append(name);

	ACSound* pSound = nullptr;
	mpADevice->LoadSound(currentPath, &pSound);
	pSound->Name = name;
	pSound->Instance = 1;

	//adiciona o son (USA O SOUNDID PQ É CRIADO DENTRO DO DEVICE)
	sounds.insert(std::pair<UINT, ACSound*>(pSound->SoundID, pSound));

	return pSound;
};

void ACContentManager::RemoveSound(ACSound* sound)
{
	if (sound != nullptr)
	{
		if (sound->Instance > 1)
			sound->Instance--;
		else
		{
			UINT id = sound->SoundID;
			//remove a textura da api depois remove da lista
			mpADevice->ReleaseSound(sound);
			//remove da memoria o dado
			SAFE_DELETE(sound);
			//remove da lista
			sounds.erase(id);
		}
	}
};

void ACContentManager::RemoveSound(std::string soundName)
{
	ACSound* sound = nullptr;

	for (auto it = sounds.begin(); it != sounds.end(); it++)
	{
		if (it->second->Name == soundName)
		{
			sound = it->second;
			break;
		}
	}

	RemoveSound(sound);
};

#pragma endregion

#pragma region Fonts

ACFont* ACContentManager::LoadFont(std::string name)
{
	for (auto it = fonts.begin(); it != fonts.end(); it++)
	{
		if (it->second->Name == name)
		{
			it->second->Instance++;
			return it->second;
		}
	}

	//pega o endereco completo do arquivo
	std::string currentPath = ACConfigurations::Instance()->GetPathFonts();
	currentPath.append(name);

	ACFont* pFont = new ACFont();
	ACAFTLoader::LoadFromFile(currentPath.c_str(), pFont);
	pFont->Instance = 1;
	pFont->ID = ACConfigurations::Instance()->GlobalID++;
	//substitui as extensao por png
	pFont->Texture = LoadTexture(name.replace(name.end() - 3, name.end(), + "png"));

	//adiciona a textura
	fonts.insert(std::pair<UINT, ACFont*>(pFont->ID, pFont));

	return pFont;
};

void ACContentManager::RemoveFont(ACFont* font)
{
	if (font != nullptr)
	{
		if (font->Instance > 1)
			font->Instance--;
		else
		{
			UINT id = font->ID;
			//primeiro remove a textura
			RemoveTexture(font->Texture);
			//remove da memoria o dado
			SAFE_DELETE(font);
			//remove da lista
			fonts.erase(id);
		}
	}
};

void ACContentManager::RemoveFont(std::string fontName)
{
	ACFont* font = nullptr;

	for (auto it = fonts.begin(); it != fonts.end(); it++)
	{
		if (it->second->Name == fontName)
		{
			font = it->second;
			break;
		}
	}

	RemoveFont(font);
};

#pragma endregion

#pragma region Skin

ACSkin* ACContentManager::CreateSkin(BOOL useMaterial)
{
	ACSkin* skin = new ACSkin(mpGDevice, this, useMaterial);
	return skin;
};

#pragma endregion

#pragma region SoundSource

//cria um novo source e ja faz o bind com o buffer passado como parametro
ACSoundSource* ACContentManager::CreateSoundSource()
{
	//carrega um novo source e ja faz o bind com o som 
	ACSoundSource* ss = new ACSoundSource(mpADevice, this);
	mpADevice->AddSource(&ss->SoundSourceID);
	return ss;
};

#pragma endregion
