#include "Floor.h"

Floor::Floor(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

	//cria os vertices
	mpVertices = new ACVertexPositionNormalTextured[4];
	mpVertices[0].position = Vector3(-0.5f, -0.0f, 0.5f);
	mpVertices[0].normal = Vector3( 0.0f, 1.0f, 0.0f);
	mpVertices[0].texcoord = Vector2( 0.0f, 1.0f);

	mpVertices[1].position = Vector3( 0.5f, -0.0f, 0.5f);
	mpVertices[1].normal = Vector3( 0.0f, 1.0f, 0.0f);
	mpVertices[1].texcoord = Vector2( 1.0f, 1.0f);

	mpVertices[2].position = Vector3(-0.5f, -0.0f,-0.5f);
	mpVertices[2].normal = Vector3( 0.0f, 1.0f, 0.0f);
	mpVertices[2].texcoord = Vector2( 0.0f, 0.0f);

	mpVertices[3].position = Vector3( 0.5f, -0.0f,-0.5f);
	mpVertices[3].normal = Vector3( 0.0f, 1.0f, 0.0f);
	mpVertices[3].texcoord = Vector2( 1.0f, 0.0f);

	//carrega os shaders
	mpVS = mpCManager->LoadVertexShader("LightingTextured.VShlsl4", VertexFormat::VF_VertexPositionNormalTextured);
	mpPS = mpCManager->LoadPixelShader("LightingTextured.PShlsl4");

	//cria um skin
	mpSkin = mpCManager->CreateSkin();
	mpSkin->Material.AmbientColor = Vector3(1,1,1);
	mpSkin->Material.DiffuseColor = Vector4(1,1,1,1);
	mpSkin->Textures[0] = mpCManager->LoadTexture("Floor.dds");


	//cria o VB estatico e passa o skin
	mpVB = nullptr;
	mpGDevice->CreateStaticBuffer(VertexFormat::VF_VertexPositionNormalTextured, 
								  4, 0, mpVertices, nullptr, mpSkin ,&mpVB);
};

Floor::~Floor()
{
	mpCManager->RemoveVertexShader(mpVS);
	mpCManager->RemovePixelShader(mpPS);
	mpGDevice->ReleaseBuffer(mpVB);
	mpSkin->Release();
};

void Floor::Update(float elapsedTime)
{
	Matrix::CreateScale(300,1,300, &mWorld);
};

void Floor::Draw(ACCamera* camera)
{
	mpGDevice->SetShadeMode(ACSHADEMODE::ACSM_TriangleStrip);

	mpGDevice->ActiveVS(mpVS);
	mpGDevice->ActivePS(mpPS);

	mpGDevice->SetWorldMatrix(mWorld);

	mpGDevice->ApplyConstants();

	//renderiza
	mpGDevice->Render(mpVB);
};
