#include "LinePointTest.h"

LinePointTest::LinePointTest(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

	//cria uma linha
	lines = ACUtils::CreateLine(Vector3(-200,-50,0), Vector4(1,0,0,1), Vector3(-180, 50,0), Vector4(1,1,0,1));

	//cria um BB 24 vertices
	Vector3 min = Vector3(-300, -300, -300);
	Vector3 max = Vector3(-200, -200, -200);
	BoundingBox bb(&min, &max);
	bbLines = ACUtils::CreateBBLines(bb, Vector4(1,0,1,1));

	//cria um BS com 360 vertices nao strip
	Vector3 center(200,150,0);
	BoundingSphere bs(&center, 50);
	bsLines = ACUtils::CreateSphereLines(bs, Vector4(1,0,0,1), Vector4(0,1,0,1), Vector4(0,0,1,1));

	//cria trocentos pontos
	points = new ACVertexPositionColored[40];
	for (int i = 0; i < 40; i++)
	{
		points[i].position = Vector3(200 - i,-50,0);
		points[i].color = Vector4(i/40.0f,i/40.0f,0,1);
	}

	//carrega os shaders
	mpVS = mpCManager->LoadVertexShader("Colored.VShlsl4", VertexFormat::VF_VertexPositionColored);
	mpPS = mpCManager->LoadPixelShader("Colored.PShlsl4");
};

LinePointTest::~LinePointTest()
{
	Release();
};

void LinePointTest::Update(FLOAT elapsedTime)
{
	mRotation.Y += elapsedTime*3;

	lines[1].position.X = lines[0].position.X + cos(mRotation.Y) * 100;
	lines[1].position.Y = lines[0].position.Y + sin(mRotation.Y) * 100;

	//anima os pontos
	for ( int i = 1; i < 40; i++)
	{
		if (i%2==0)
		{
			points[i].position.X = points[0].position.X + cos(mRotation.Y) * (i + 10);
			points[i].position.Y = points[0].position.Y + sin(mRotation.Y) * (i + 10);
		}
		else
		{
			points[i].position.X = points[0].position.X - cos(mRotation.Y) * (i + 10);
			points[i].position.Y = points[0].position.Y - sin(mRotation.Y) * (i + 10);
		}
	}
};

void LinePointTest::Draw(ACCamera* camera)
{
	mpGDevice->ActiveVS(mpVS);
	mpGDevice->ActivePS(mpPS);

	mpGDevice->SetWorldMatrix(World);

	mpGDevice->ApplyConstants();

	mpGDevice->RenderLines(2,0,lines,nullptr,true);
	mpGDevice->RenderLines(24,0,bbLines,nullptr,false);
	mpGDevice->RenderLines(360,0,bsLines,nullptr,false);

	mpGDevice->RenderPoints(40,points);
};

void LinePointTest::Release()
{
	mpCManager->RemoveVertexShader(mpVS);
	mpCManager->RemovePixelShader(mpPS);

	SAFE_DELETE_A(lines);
	SAFE_DELETE_A(bbLines);
	SAFE_DELETE_A(bsLines);
};