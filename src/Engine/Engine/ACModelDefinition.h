//****************************
// Classe que mantem as definicoes dos modelos
// Creator: Daniel Gadens
// Date: 08/06/2011
//****************************


#ifndef __ACMODELDEFINITION_H
#define __ACMODELDEFINITION_H

#include <string>
#include "ACAMTStructs.h"
#include "EngineCommon.h"
#include "EngineMath.h"
#include "ACVertexBuffer.h"
#include "ACVertexFormat.h"
#include "ACShaders.h"
#include "ACSkin.h"
#include "ACCamera.h"
#include "ACMark.h"

class ACContentManager;
class ACRenderDevice;

class ACModelDefinition
{   
private:
	//recebe o contentmanager e o gdevice pq ele precisa criar o skin dentro do contentmanager
	//assim o controle fica todo la
	ACRenderDevice* mpGDevice;
	ACContentManager* mpCManager;

	AMT_MODEL* mpModel;

	//Renderizar o joints
	Matrix mRootJointMatrix;
	ACMark* mpJointMark;

	//linhas dos bones
	ACVertexPositionColored* mpLines;

	//Current Skin
	ACSkin* mpSkin;

	//indices q vao para o indexbuffer
	UINT							mNumIndices; //numero de indices do array
	UINT*							mpIndices;

	//lista de buffers possiveis
	UINT							mNumVertices; //numero de vertices que estao em algum dos buffers
    ACVertexPositionNormalTextured* mpVPNTCache;
	ACVertexSkinnedMesh*			mpVSMCache;

	//prepara os dados para o vpnt
	void PrepareVPNT(AMT_MODEL* model);
	//prepara os dados para o skinnedmesh
	void PrepareVSM(AMT_MODEL* model);

	//renderiza a estrura de bones
	void RenderBonesTree(ACCamera* camera, AMT_JOINT* joint);

public:
	UINT		   ID;								// id dentro do contentmanager
	UINT           Instance;						// numero de instancias do modelo
	std::string    Name;
	BoundingBox	   BBOriginal;                      // bb
	OrientedBoundingBox OBBOriginal;                // obb original
	VertexFormat   VFormat;							// formato do vertice (joints > 0 VPNTSkinned senao VPNT)

	ACVertexBuffer*    pVertexBuffer;

	BOOL HasAnimation;
	BOOL HasSkeleton;

	ACModelDefinition(ACRenderDevice* mpGDevice, ACContentManager* mpCManager);
	~ACModelDefinition();

	void SetSkin(ACSkin* skin);
	const ACSkin const * GetSkin();

	void Prepare(AMT_MODEL* pointer);

	void RenderBones(ACCamera* camera, Matrix& world);

	void Release();

};

#endif