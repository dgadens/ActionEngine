#include "GPowerCube.h"

bool foi;

GPowerCube::GPowerCube(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

	mCurrentRotation = 0;

	foi = false;

#pragma region EXEMPLO DE CARGA DE MODELO
	for (int i = 0; i < 10; i++)
	{
		ACModel* model = new ACModel(mpGDevice, mpCManager);
		model->Load("BoxTNT.amt");
		
		//faco isso para adicionar a textura de animacao no braco
		const ACSkin* modelDefaultSkin = model->GetSkin();
		ACSkin* skin = mpCManager->CreateSkin();
		skin->Material = modelDefaultSkin->Material;
		skin->Textures[0] = mpCManager->LoadTexture(modelDefaultSkin->Textures[0]->Name);
		skin->Textures[1] = modelDefaultSkin->Textures[1];
		skin->Textures[2] = modelDefaultSkin->Textures[2];
		skin->Textures[3] = mpCManager->LoadTexture("BoxTNTAnimated.png"); //seta essa textura para o canal de animacao 
		model->SetSkin(skin);
		delete skin;

		mpModels[i] = model;
	}
#pragma endregion

#pragma region EXEMPLO DE SOM
	//carrega o som
	mpSoundBuffer = mpCManager->LoadSound("bomb.ogg");
	mpSoundSource = mpCManager->CreateSoundSource();
	mpSoundSource->BindSound(mpSoundBuffer);
	mpSoundSource->SetLoop(false);
#pragma endregion

	//int iErr = 0;
	//lua_State *lua = lua_open();

	//luaL_openlibs(lua);
	//if ((iErr = luaL_loadfile (lua, "teste.lua")) == 0)
	//{
	//   // Call main...
	//   if ((iErr = lua_pcall (lua, 0, LUA_MULTRET, 0)) == 0)
	//   { 
	//	  // Push the function name onto the stack
	//	  lua_pushstring (lua, "helloWorld");
	//	  // Function is located in the Global Table
	//	  lua_gettable (lua, LUA_GLOBALSINDEX);  
	//	  lua_pcall (lua, 0, 0, 0);
	//   }
	//}
	//lua_close (lua);
};

GPowerCube::~GPowerCube()
{
	SAFE_DELETE(mpSoundSource);

	if (mpSoundBuffer!= nullptr)
	{
		mpCManager->RemoveSound(mpSoundBuffer);
		mpSoundBuffer = nullptr;
	}

	for (int i = 0; i < 10; i++)
		SAFE_RELEASE(mpModels[i]);
};

void GPowerCube::SetSkin(ACSkin* skin)
{
	mpModels[0]->SetSkin(skin);
};

ACModel* GPowerCube::GetModel()
{
	return mpModels[0];
};

void GPowerCube::Update(float elapsedTime)
{
	//if (!foi)
	//{
	mCurrentRotation += (elapsedTime * 2);

	mpModels[0]->SetAbsolutePosition(-100, 0, 0);
	mpModels[0]->SetAbsoluteScale(100);
	mpModels[0]->SetAbsoluteRotation(0, mCurrentRotation, 0);
	mpModels[0]->Update(elapsedTime);
	/*foi = true;
	}*/
};
	 
void GPowerCube::Render(ACCamera* camera)
{
	mpGDevice->SetShadeMode(ACSHADEMODE::ACSM_TriangleList);
	mpGDevice->SetBlendState(ACBLENDSTATE::ACBS_Opaque);
	mpGDevice->SetSamplerState(ACSAMPLERSTATE::ACSS_Bilinear_Wrap, 0);
	//mpGDevice->SetRasterizeState(ACRASTERIZESTATE::ACRS_SolidCullCCW);
	mpModels[0]->Render(camera);
};

void GPowerCube::Explode()
{
	//dispara o som da explosao
	mpSoundSource->Play();
};

void GPowerCube::Broke()
{
};