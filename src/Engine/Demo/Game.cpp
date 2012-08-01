#include "Game.h"

Game::Game(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;
};

Game::~Game()
{
	if (mpFont !=nullptr)
		mpCManager->RemoveFont(mpFont);

	SAFE_DELETE(mpPowerCube);
	SAFE_DELETE(mpDynamicTest);
	SAFE_DELETE(mpSpriteTest);
	SAFE_DELETE(mpLinePointTest);
	SAFE_DELETE(mpCamera);
	SAFE_DELETE(mpSpriteBatch);
	SAFE_DELETE(mpCManager);
};

//Ativa o game ou desativa, quando a janela perde o foco ele desativa quando ganyha ativa
void Game::Active(BOOL value)
{
	if (!value)
		ACTimeControl::Stop();
	else
		ACTimeControl::Start();
};

//configura toda a inicializacao do game
void Game::Initialize()
{
	//timecontrol
	ACTimeControl::FixedFPS = false;

	//font
	mpFont = mpCManager->LoadFont("CourierNew20Bold.aft");

	//spritebatch
	mpSpriteBatch = new ACSpriteBatch(mpGDevice, mpCManager);

	//inicializa o randon
	ACRandom::Randomize(ACTimeControl::GetMilliseconds());

	//cria a camera padrao
	mpCamera = new ACCamera();
	mpCamera->ResetCamera();
	mpCamera->SetWidth(mpGDevice->GetVPWidth());
	mpCamera->SetHeight(mpGDevice->GetVPHeight());

	//graphics device
	mpGDevice->GoFullscreen(false);
	mpGDevice->SetClearColor(Vector4(0,0,0,1));
	mpGDevice->SetWireColor(Vector3(1,1,0));
	mpGDevice->SetRasterizeState(ACRASTERIZESTATE::ACRS_SolidCullCCW);
	mpGDevice->SetShadeMode(ACSHADEMODE::ACSM_LineList);
};

//quando a tela for redimensionada eu preciso passa isso pra tudo q é lado
void Game::ReziseWindow(INT width, INT height)
{
	mpGDevice->Resize(width, height);
	mpCamera->SetWidth(width);
	mpCamera->SetHeight(height);
};

//carrega os conteudos necessarios
void Game::LoadContents()
{
	mpPowerCube = new GPowerCube(mpGDevice, mpCManager);
	mpDynamicTest = new DynamicTest(mpGDevice, mpCManager);
	mpSpriteTest = new SpriteTest(mpGDevice, mpCManager);
	mpLinePointTest = new LinePointTest(mpGDevice, mpCManager);
};

//upate macro dos objetos
void Game::Update()
{
	if (Globals::IsRunning)
	{
		////SS
		//ACKeyboardState keysState[256];
		//Globals::pKeyboard->GetState(keysState);
		//if (keysState->IsPressed(ACKEYS::ACKEY_S))
		//	mpGDevice->SaveScreenShot("c:\\acSS.jpg");


		ACTimeControl::Update();
		mpCamera->Update();

		//TODO: só programar :)
		mpPowerCube->Update(ACTimeControl::GetFElapsedTime());
		mpDynamicTest->Update(ACTimeControl::GetFElapsedTime());
		mpSpriteTest->Update(ACTimeControl::GetFElapsedTime());
		mpLinePointTest->Update(ACTimeControl::GetFElapsedTime());
	}
};

//renderiza oq tem pra renderizar
void Game::Draw()
{
	if (Globals::IsRunning)
	{
		mpGDevice->Clear(TRUE, TRUE, TRUE);
		mpGDevice->BeginRendering();
		SetPerframeData();

		//Mandar renderizar daqui 

		//mpDynamicTest->Render(mpCamera);
		//mpSpriteTest->Draw();
		//mpPowerCube->Render(mpCamera);
		mpLinePointTest->Render(mpCamera);
		//ate aqui

		DrawTexts();
		mpGDevice->EndRendering();
	}
};

void Game::SetPerframeData()
{
	//light and camera
	mpGDevice->SetViewProjectionMatrix(mpCamera->ViewProjection);
	mpGDevice->SetCameraPosition(mpCamera->Position);
	mpGDevice->SetLightPosition(Vector3(0, 0, 100));
	mpGDevice->SetLightDirection(Vector3(0, 0, -0.5f));
	mpGDevice->SetLightColor(Vector3(0.1f, 0.1f, 0.1f));
	mpGDevice->SetLightRange(600);
	mpGDevice->SetLightFalloff(10);
	mpGDevice->SetTime(ACTimeControl::GetTotalSeconds());
	
	mpGDevice->ApplyConstants();
};

void Game::DrawTexts()
{
	mpSpriteBatch->BeginRender(ACBLENDSTATE::ACBS_NonPremultiplied);
	mpSpriteBatch->RenderString(mpFont, "FPS: " + GenericTools::IntToString(ACTimeControl::GetFPS()), Vector2(0,0), Vector4(1,1,0,1));
	mpSpriteBatch->EndRender();
};
