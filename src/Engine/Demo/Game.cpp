#include "Game.h"

Game::Game(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

	mpScriptMachine = nullptr;
};

Game::~Game()
{
	mpCManager->RemoveFont(mpFont);

	SAFE_DELETE(mpFloor);
	SAFE_DELETE(mpScriptMachine);
	SAFE_DELETE(mpPowerCube);
	SAFE_DELETE(mpDynamicTest);
	SAFE_DELETE(mpCamera);
	SAFE_DELETE(mpSpriteBatch);
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

	//ScriptMachine
	mpScriptMachine = new ScriptMachine();
	mpScriptMachine->Initialize();
	mpScriptMachine->ExecuteScript("");

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
	mpCamera->SetPosition(Vector3(0,200.0f,-200));
	mpCamera->SetTarget(Vector3(0,0,0));
	mpCamera->SetUp(Vector3(0,1,0));

	//graphics device
	mpGDevice->GoFullscreen(false);
	mpGDevice->SetClearColor(Vector4(0,0,0,1));
	mpGDevice->SetWireColor(Vector3(1,1,0));
	mpGDevice->SetRasterizeState(ACRASTERIZESTATE::ACRS_SolidCullCCW);
	mpGDevice->SetShadeMode(ACSHADEMODE::ACSM_LineList);

	mRenderTargetID = mpGDevice->CreateRenderTarget(mpGDevice->GetVPWidth(), mpGDevice->GetVPHeight());

};

//quando a tela for redimensionada eu preciso passa isso pra tudo q é lado
void Game::ReziseWindow(INT width, INT height)
{
	mpGDevice->Resize(width, height);
	mpCamera->SetWidth(width);
	mpCamera->SetHeight(height);
	mpGDevice->ResizeRenderTarget(mRenderTargetID, mpGDevice->GetVPWidth(), mpGDevice->GetVPHeight());
};

//carrega os conteudos necessarios
void Game::LoadContents()
{
	mpFloor = new Floor(mpGDevice, mpCManager);
	mpPowerCube = new GPowerCube(mpGDevice, mpCManager);
	mpDynamicTest = new DynamicTest(mpGDevice, mpCManager);
};

//upate macro dos objetos
void Game::Update()
{
	if (Globals::IsRunning)
	{
		ACTimeControl::Update();
		mpCamera->Update();

		//TODO: só programar :)
		mpFloor->Update(ACTimeControl::GetFElapsedTime());
		mpPowerCube->Update(ACTimeControl::GetFElapsedTime());
		mpDynamicTest->Update(ACTimeControl::GetFElapsedTime());
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

		//ativa o rendertarget pra textura
		mpGDevice->RenderTargetClear(mRenderTargetID, Vector4(0,0,0,1));
		mpGDevice->RenderTargetActivate(mRenderTargetID);

		mpFloor->Draw(mpCamera);
		mpPowerCube->Draw(mpCamera);
		mpDynamicTest->Draw(mpCamera);

		//seta o render target para screen
		mpGDevice->RenderTargetActivate(0);
		ACTexture* rtTexture = mpGDevice->RenderTargetGetTexture(mRenderTargetID);

		mpSpriteBatch->BeginRender(ACBLENDSTATE::ACBS_Opaque);
		mpSpriteBatch->Render(rtTexture, Vector2(0,0), Vector4(1,1,1,1));
		mpSpriteBatch->EndRender();

		DrawTexts();
		mpGDevice->EndRendering();
	}
};

void Game::SetPerframeData()
{
	//light and camera
	mpGDevice->SetViewProjectionMatrix(mpCamera->ViewProjection);
	mpGDevice->SetCameraPosition(mpCamera->GetPosition());
	mpGDevice->SetLightPosition(Vector3(0, 200, 0));
	mpGDevice->SetLightDirection(Vector3(0, -1, 0));
	mpGDevice->SetLightColor(Vector3(1.0f, 0.0f, 0.0f));
	mpGDevice->SetLightRange(600);
	mpGDevice->SetLightFalloff(10);
	mpGDevice->SetTime(ACTimeControl::GetTotalSeconds());
	
	mpGDevice->ApplyConstants();
};

void Game::DrawTexts()
{
	mpSpriteBatch->BeginRender(ACBLENDSTATE::ACBS_NonPremultiplied);
	std::string text = "FPS: " + GenericTools::IntToString(ACTimeControl::GetFPS()) +"\n";
	text += "GPU: " + std::string(mpGDevice->VideoCardDescription) +"\n";
	text += "Res: " + GenericTools::IntToString(mpGDevice->GetVPWidth()) + " x " +  GenericTools::IntToString(mpGDevice->GetVPHeight()) +"\n";
	mpSpriteBatch->RenderString(mpFont, text, Vector2(0,0), Vector4(1,1,0,1));
	mpSpriteBatch->EndRender();
};
