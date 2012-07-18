//****************************************
// Estruturas utilizadas para caregar o cbf
// Creator: Daniel Gadens
// Date: 07/06/2011
//***********************************************


#ifndef __ACCBFMODELSTRUCT_H                       
#define __ACCBFMODELSTRUCT_H                       

#include "EngineCommon.h"
#include "EngineMath.h"
#include <vector>
#include <string>

struct AMT_HEAD{
	UINT Version;
    UINT NumVertices;
    UINT NumIndices;
    UINT NumFaces;
    UINT NumMeshes;
    UINT NumMaterials;
    UINT NumJoints;
    float AnimationFPS;
    float CurrentTime;
    UINT NumFrames;
    UINT NumAnimations;
    UINT HasSkeleton;
};

//vertices : todos eles sao armazenados no vertex buffer
struct AMT_VERTEX
{
    Vector3 Position;      
    Vector2 TexCoord1;     
    Vector2 TexCoord2;     
    Vector3 Normal;        
    UINT BoneID_A;               
    float BoneWeight_A;    
	UINT BoneID_B;               
    float BoneWeight_B; 
	UINT BoneID_C;              
    float BoneWeight_C; 
	UINT BoneID_D;               
    float BoneWeight_D; 
    UINT Flag; 
};

//faces, usada para pegar os indices, tb todos eles vao para o indexbuffer
struct AMT_FACE
{
    int Indices [ 3 ];      
    Vector3 Normal;        //normal da face
    UINT MeshID;           //id do mesh no array principal
    UINT Flag;         
};

struct AMT_MATERIAL
{
    char Name[ 64 ];                   
    Vector3 Ambient;                   
    Vector3 Diffuse;                   
    Vector3 Specular;                  
    Vector3 Emissive;                  
    float SpecularPower;               
    float Transparency;                
    char DiffuseTexture[64];
    char SpecularTexture[64];
    char NormalTexture[64];
    char AnimatedTexture[64];
    UINT Flag;    
};

//mesh faz a separacao da faces assim é possivel criar o IB apenas com os 
//indices (q estao nas faces de indice = ao pindices)
//indicadas pelos que estao no mesh atual
//tb tem o id do material entao é possivel
struct AMT_MESH
{
    char Name [ 64 ];         
    int NumFaces; 
    PINT pIndices;   //indices das faces, o numero de indices é o numfaces          
    UINT MaterialID;          
    UINT Flag;
};

struct AMT_MODEL{
	AMT_HEAD      Head;                               // Header
	std::vector<AMT_VERTEX*>   pVertices;                           // Vertices
	std::vector<AMT_FACE*>	   pFaces;                              // Faces
	std::vector<AMT_MESH*>	   pMeshes;                              // Meshs
	std::vector<AMT_MATERIAL*> pMaterials;                          // Materials
/*   LPJOINT		pJoints;                             // Joints
   LPANIMATION  pAnimation;                          // Animations*/
};

// -----------------------------------------------------------------
#endif
