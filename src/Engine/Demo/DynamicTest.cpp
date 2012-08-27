#include "DynamicTest.h"

DynamicTest::DynamicTest(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

	vertices1 = new ACVertexPositionColored[4];
	//linha 1
	vertices1[0].position	= Vector3(-30,-50, 0);
	vertices1[0].color		= Vector4(1, 1, 1, 1);
	vertices1[1].position	= Vector3( 30,-50, 0);
	vertices1[1].color		= Vector4(1, 1, 1, 1);
	//linha 2
	vertices1[2].position	= Vector3(-30,-40, 0);
	vertices1[2].color		= Vector4(1, 1, 1, 1);
	vertices1[3].position	= Vector3( 30,-40, 0);
	vertices1[3].color		= Vector4(1, 1, 1, 1);

	vertices2 = new ACVertexPositionColored[4];
	//linha 3
	vertices2[0].position	= Vector3(-30, 0, 0);
	vertices2[0].color		= Vector4(1, 1, 0, 1);
	vertices2[1].position	= Vector3( 30, 0, 0);
	vertices2[1].color		= Vector4(1, 1, 0, 1);
	//linha 4
	vertices2[2].position	= Vector3(-30, 10, 0);
	vertices2[2].color		= Vector4(1, 1, 0, 1);
	vertices2[3].position	= Vector3( 30, 10, 0);
	vertices2[3].color		= Vector4(1, 1, 0, 1);

	vertices3 = new ACVertexPositionColored[4];
	//linha 5
	vertices3[0].position	= Vector3(-30, 50, 0);
	vertices3[0].color		= Vector4(0, 1, 0, 1);
	vertices3[1].position	= Vector3( 30, 50, 0);
	vertices3[1].color		= Vector4(0, 1, 0, 1);
	//linha 6
	vertices3[2].position	= Vector3(-30, 60, 0);
	vertices3[2].color		= Vector4(0, 1, 0, 1);
	vertices3[3].position	= Vector3( 30, 60, 0);
	vertices3[3].color		= Vector4(0, 1, 0, 1);

	indices = nullptr;
	//indices = new UINT[3];
	//indices[0] = 0;
	//indices[1] = 1;
	//indices[2] = 2;

	//carrega os shaders
	mpVS = mpCManager->LoadVertexShader("Colored.VShlsl4", VertexFormat::VF_VertexPositionColored);
	mpPS = mpCManager->LoadPixelShader("Colored.PShlsl4");

	skin1 = mpCManager->CreateSkin();
	skin1->Material.DiffuseColor = Vector4(1,1,1,1);
	skin1->Material.EmissiveColor = Vector3(1,0,0);

	skin2 = mpCManager->CreateSkin();
	skin2->Material.DiffuseColor = Vector4(1,1,1,1);
	skin2->Material.EmissiveColor = Vector3(0,1,0);

	skin3 = mpCManager->CreateSkin();
	skin3->Material.DiffuseColor = Vector4(1,1,1,1);
	skin3->Material.EmissiveColor = Vector3(0,0,1);

};

DynamicTest::~DynamicTest()
{
	Release();
};

void DynamicTest::Update(FLOAT elapsedTime)
{
	mRotation.Y += elapsedTime;
	Matrix::CreateRotationY(mRotation.Y, &World1);

	Matrix::CreateRotationY(mRotation.Y, &World2);
	World2.M41 -= 100;

	Matrix::CreateRotationY(mRotation.Y, &World3);
	World3.M41 += 100;
};

void DynamicTest::Draw(ACCamera* camera)
{
	mpGDevice->SetShadeMode(ACSHADEMODE::ACSM_LineList);
	mpGDevice->ActiveVS(mpVS);
	mpGDevice->ActivePS(mpPS);
	
	mpGDevice->SetWorldMatrix(World1);
	mpGDevice->Render(VertexFormat::VF_VertexPositionColored, 
					  4, 
					  0, 
					  vertices1, 
					  nullptr, 
					  skin1);

	mpGDevice->Render(VertexFormat::VF_VertexPositionColored, 
					  4, 
					  0, 
					  vertices2, 
					  nullptr, 
					  skin1);

	mpGDevice->Render(VertexFormat::VF_VertexPositionColored, 
					  4, 
					  0, 
					  vertices3, 
					  nullptr, 
					  skin1);
};

void DynamicTest::Release()
{
	mpCManager->RemoveVertexShader(mpVS);
	mpCManager->RemovePixelShader(mpPS);

	SAFE_DELETE_A(vertices1);
	SAFE_DELETE_A(vertices2);
	SAFE_DELETE_A(vertices3);
	SAFE_DELETE_A(indices);
};