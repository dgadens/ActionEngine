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

struct AMT_HEAD
{
	UINT Version;
    UINT NumVertices;
    UINT NumIndices;
    UINT NumFaces;
    UINT NumMeshes;
    UINT NumMaterials;
    UINT NumJoints;
    FLOAT AnimationFPS;
    FLOAT CurrentTime;
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
    UINT BoneIndices[4];
    FLOAT BoneWeights[4];
    UINT Flag; 
};

//faces, usada para pegar os indices, tb todos eles vao para o indexbuffer
struct AMT_FACE
{
    INT Indices [ 3 ];      
    Vector3 Normal;        //normal da face
    UINT MeshID;           //id do mesh no array principal
    UINT Flag;         
};

struct AMT_MATERIAL
{
    CHAR Name[ 64 ];                   
    Vector3 Ambient;                   
    Vector3 Diffuse;                   
    Vector3 Specular;                  
    Vector3 Emissive;                  
    FLOAT SpecularPower;               
    FLOAT Transparency;                
    CHAR DiffuseTexture[64];
    CHAR SpecularTexture[64];
    CHAR NormalTexture[64];
    CHAR AnimatedTexture[64];
    UINT Flag;    
};

//mesh faz a separacao da faces assim é possivel criar o IB apenas com os 
//indices (q estao nas faces de indice = ao pindices)
//indicadas pelos que estao no mesh atual
//tb tem o id do material entao é possivel
struct AMT_MESH
{
    CHAR Name [ 64 ];         
    INT NumFaces; 
    PINT pIndices;   //indices das faces, o numero de indices é o numfaces          
    UINT MaterialID;          
    UINT Flag;
};

struct AMT_KF
{
    FLOAT Time;                             // Time
    Matrix BindMatrix;                      // Position
};

struct AMT_JOINT
{
    UINT ID;                                //Id q ta na lista principal, armazeno para ser facil de achar
    CHAR Name [ 64 ];                         
    INT ParentID;                           // ParentID
	UINT NumChildren;                       // Numero de filhos
	std::vector<UINT> JointChildren;
	UINT NumKF;								// Number of KF
    std::vector<AMT_KF> KFData;				// KF Data
	UINT IsAnimated;                        // Animierd
    UINT Flag;                              // Flags
    Matrix BindMatrix;                      // Bind Matrix (joint)
    Matrix MatrixAbsolute;                  // Matrix absolute
    Matrix InverseBindMatrix;               // Inverse Bind Matrix (inverse joint)
};

struct AMT_ANIMATION
{
    CHAR Name [ 64 ];                         
    FLOAT StartFrame;                    
    FLOAT EndFrame;
    UINT Active;                         
};

struct AMT_MODEL
{
	AMT_HEAD				    Head;                 // Header
	std::vector<AMT_VERTEX*>    pVertices;            // Vertices
	std::vector<AMT_FACE*>	    pFaces;               // Faces
	std::vector<AMT_MESH*>	    pMeshes;              // Meshs
	std::vector<AMT_MATERIAL*>  pMaterials;           // Materials
	std::vector<AMT_JOINT*>     pJoints;				 // Bones	
	std::vector<AMT_JOINT*>     pOriginalJoints;		 // Original bones
	std::vector<AMT_ANIMATION*> pAnimations;		 // Animations 
};

// -----------------------------------------------------------------
#endif
