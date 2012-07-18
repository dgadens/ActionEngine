//************************************
// Classe q controla os conteudos
// creator: Daniel Gadens
// date: 24/04/2012
//***********************************

#ifndef __ACCONTENTMANAGER_H
#define __ACCONTENTMANAGER_H

#include <map>
#include <vector>

#include "ACAMTLoader.h"
#include "ACAMTStructs.h"
#include "ACModelDefinition.h"

#include "ACTexture.h"
#include "ACMaterial.h"
#include "ACSkin.h"

#include "ACSound.h"
#include "ACSoundSource.h"

#include "ACShaders.h"

#include "ACFont.h"
#include "ACAFTLoader.h"

class ACRenderDevice;
class ACAudioDevice;

class ACContentManager
{
	ACRenderDevice* mpGDevice;
	ACAudioDevice* mpADevice;

	//mantem todas os objetos ja carregados
	std::map<UINT, ACTexture*>  textures;
	std::map<UINT, ACVertexShader*> vertexShaders;
	std::map<UINT, ACPixelShader*> pixelShaders;
	std::map<UINT, ACGeometryShader*> geometryShaders;
	std::map<UINT, ACSound*> sounds;
	std::map<UINT, ACModelDefinition*> models;
	std::map<UINT, ACFont*> fonts;

public:

	ACContentManager(ACRenderDevice* gDevice, ACAudioDevice* aDevice);
	~ACContentManager();

	//models
	ACModelDefinition* LoadModel(const std::string& name);
	void RemoveModel(ACModelDefinition* model);
	void RemoveModel(const std::string& modelName);

	//texturas
	ACTexture* LoadTexture(std::string name);
	void RemoveTexture(ACTexture* texture);
	void RemoveTexture(std::string textureName);

	//shaders
	ACVertexShader* LoadVertexShader(std::string name, VertexFormat vertexFormat);
	void RemoveVertexShader(ACVertexShader* texture);
	void RemoveVertexShader(std::string vertexShaderName);

	ACPixelShader* LoadPixelShader(std::string name);
	void RemovePixelShader(ACPixelShader* pixelShader);
	void RemovePixelShader(std::string pixelShaderName);

	ACGeometryShader* LoadGeometryShader(std::string name);
	void RemoveGeometryShader(ACGeometryShader* geometryShader);
	void RemoveGeometryShader(std::string geometryShaderName);

	//sounds
	ACSound* LoadSound(std::string name);
	void RemoveSound(ACSound* sound);
	void RemoveSound(std::string soundName);

	//fonts
	ACFont* LoadFont(std::string name);
	void RemoveFont(ACFont* font);
	void RemoveFont(std::string fontName);

	//skin
	//cria um skin novo
	ACSkin* CreateSkin(BOOL useMaterial = true);
	//cria um clone do skin copiando todos os atributos do src pro dest
	//tb é feito o controle do numero de instancias de cada material e textura clonado
	void CloneSkin(ACSkin* destSkin, const ACSkin* srcSkin);

	//SoundSource
	//cria um soundsource novo
	ACSoundSource* CreateSoundSource();
};

#endif