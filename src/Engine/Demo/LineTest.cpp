#include "LineTest.h"

LineTest::LineTest(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

	//cria o vb
	lines = new ACVertexPositionColored[2];

	lines[0].position = Vector3(-200,-50,0);
	lines[0].color = Vector4(1,0,0,1);
	lines[1].position = Vector3(-100,-50,0);
	lines[1].color = Vector4(0,1,0,0);

	//carrega os shaders
	mpVS = mpCManager->LoadVertexShader("Colored.VShlsl4", VertexFormat::VF_VertexPositionColored);
	mpPS = mpCManager->LoadPixelShader("Colored.PShlsl4");
};

LineTest::~LineTest()
{
	Release();
};

void LineTest::Update(FLOAT elapsedTime)
{
	mRotation.Y += elapsedTime;

	lines[1].position.X = lines[0].position.X + cos(mRotation.Y) * 100;
	lines[1].position.Y = lines[0].position.Y + sin(mRotation.Y) * 100;
};

void LineTest::Render(ACCamera* camera)
{
	mpGDevice->SetRasterizeState(ACRASTERIZESTATE::ACRS_SolidCullNone);

	mpGDevice->ActiveVS(mpVS);
	mpGDevice->ActivePS(mpPS);

	mpGDevice->SetWorldMatrix(World);

//	mpGDevice->SetTexture(pModelDefinition->GetSkin()->Textures[0], 0);
	mpGDevice->SetSamplerState(ACSAMPLERSTATE::ACSS_Bilinear_Wrap, 0);

	mpGDevice->ApplyConstants();

	mpGDevice->RenderLines(2,0,lines,nullptr,false);
};

void LineTest::Release()
{
	if (mpVS!=nullptr)
	{
		mpCManager->RemoveVertexShader(mpVS);
		mpVS = nullptr;
	}

	if (mpPS!=nullptr)
	{
		mpCManager->RemovePixelShader(mpPS);
		mpPS = nullptr;
	}
};