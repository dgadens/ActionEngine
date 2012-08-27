#include "BasicTest.h"

BasicTest::BasicTest(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

	//cria o vb
	mpVB = new ACVertexBuffer();

	ACVertexPosition* vertices = new ACVertexPosition[3];
	vertices[0].position = Vector3(-30, 0, 0);
	vertices[1].position = Vector3( 30, 0, 0);
	vertices[2].position = Vector3( 0, 60, 0);

	UINT* indices = new UINT[3];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	mpGDevice->CreateStaticBuffer(VertexFormat::VF_VertexPosition, 3, 3, vertices, indices, nullptr, &mpVB);

	delete[] vertices;
	vertices = nullptr;
	delete[] indices;
	indices = nullptr;

	//carrega os shaders
	mpVS = mpCManager->LoadVertexShader("Colored.VShlsl4", VertexFormat::VF_VertexPosition);
	mpPS = mpCManager->LoadPixelShader("Colored.PShlsl4");
};

BasicTest::~BasicTest()
{
	Release();
};

void BasicTest::Update(FLOAT elapsedTime)
{
	mRotation.Y += elapsedTime;
	//Matrix::CreateScale(30, &World);
	Matrix::CreateRotationY(mRotation.Y, &World);
};

void BasicTest::Draw(ACCamera* camera)
{
	mpGDevice->ActiveVS(mpVS);
	mpGDevice->ActivePS(mpPS);

	mpGDevice->SetWorldMatrix(World);

//	mpGDevice->SetTexture(pModelDefinition->GetSkin()->Textures[0], 0);
	mpGDevice->SetSamplerState(ACSAMPLERSTATE::ACSS_Bilinear_Wrap, 0);

	mpGDevice->ApplyConstants();

	mpGDevice->Render(mpVB);
};

void BasicTest::Release()
{
	mpCManager->RemoveVertexShader(mpVS);
	mpCManager->RemovePixelShader(mpPS);
};