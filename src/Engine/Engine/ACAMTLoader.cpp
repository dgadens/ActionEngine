#include "ACAMTLoader.h"

FILE* ACAMTLoader::mpFile;


bool ACAMTLoader::CloseFile( bool rBool )
{
	// close file
	if( mpFile )
	{
		fclose( mpFile );
		mpFile = nullptr;
	}

	// bye
	return rBool;
};

bool ACAMTLoader::LoadFromFile( const CHAR* path, AMT_MODEL* outModel)
{
	// open the file
	mpFile = fopen( path, "rb" );
	if( !mpFile ) return false;

	ReadHeader(outModel);    
	ReadVertices(outModel);  
	ReadFaces(outModel);     
	ReadMesh(outModel);      
	ReadMaterials(outModel);
	ReadJoints(outModel);
	ReadAnimations(outModel);

	return CloseFile( true );
};

HRESULT ACAMTLoader::ReadHeader( AMT_MODEL* outModel )
{
	AMT_HEAD head;

	ZeroMemory( &head, sizeof( AMT_HEAD ) );

	fread( &head, sizeof ( AMT_HEAD ), 1, mpFile );

	memcpy( &outModel->Head, &head,	sizeof( AMT_HEAD ) );
	
	return AC_OK;
};

HRESULT ACAMTLoader::ReadVertices( AMT_MODEL* outModel )
{
	// variablen init
	UINT numVertices = outModel->Head.NumVertices;   

	AMT_VERTEX* pVertex;
	for (int i = 0; i < numVertices; i++)
	{
		outModel->pVertices.push_back( new AMT_VERTEX );
		pVertex = outModel->pVertices[ i ];
		ZeroMemory( pVertex, sizeof( AMT_VERTEX ) );

		fread( &pVertex->Position, sizeof( FLOAT ), 3, mpFile );
		fread( &pVertex->TexCoord1, sizeof( FLOAT ), 2, mpFile );
		fread( &pVertex->TexCoord2, sizeof( FLOAT ), 2, mpFile );
		fread( &pVertex->Normal, sizeof( FLOAT ), 3, mpFile );

        for (int j = 0; j < 4; j++)
        {
			fread( &pVertex->BoneIndices[j], sizeof( UINT ), 1, mpFile );
			fread( &pVertex->BoneWeights[j], sizeof( FLOAT ), 1, mpFile );
        }

		fread( &pVertex->Flag, sizeof( UINT ), 1, mpFile );
	}

	return AC_OK;
};

HRESULT ACAMTLoader::ReadFaces( AMT_MODEL* outModel )
{
	UINT numFaces = outModel->Head.NumFaces; 

	//copia do arquivo
	AMT_FACE* pFace;
	for (int i = 0; i < numFaces; i++)
	{
		outModel->pFaces.push_back( new AMT_FACE );
		pFace = outModel->pFaces[ i ];
		ZeroMemory( pFace, sizeof( AMT_FACE ) );
		fread(pFace, sizeof( AMT_FACE ), 1, mpFile);
	}

	return S_OK;
};

HRESULT ACAMTLoader::ReadMesh( AMT_MODEL* outModel )
{
	UINT numMeshes = outModel->Head.NumMeshes;  

	AMT_MESH* pMesh;
	for (int i = 0; i < numMeshes; i++)
	{
		outModel->pMeshes.push_back( new AMT_MESH );
		pMesh = outModel->pMeshes[ i ];
		ZeroMemory( pMesh, sizeof( AMT_MESH ) );

		fread(pMesh->Name, sizeof( char ), 64, mpFile);
		fread(&pMesh->NumFaces, sizeof ( int ), 1, mpFile);
		
		pMesh->pIndices = new int[pMesh->NumFaces];
		fread(pMesh->pIndices, sizeof ( int ), pMesh->NumFaces, mpFile);

        fread(&pMesh->MaterialID, sizeof ( UINT ), 1, mpFile);
		fread(&pMesh->Flag, sizeof ( UINT ), 1, mpFile);
	}

	return S_OK;
};

HRESULT ACAMTLoader::ReadMaterials( AMT_MODEL* outModel )
{
	UINT numMaterials = outModel->Head.NumMaterials; 

	//copia do arquivo
	AMT_MATERIAL* pMaterial;
	for (int i = 0; i < numMaterials; i++)
	{
		outModel->pMaterials.push_back( new AMT_MATERIAL );
		pMaterial = outModel->pMaterials[ i ];
		ZeroMemory( pMaterial, sizeof( AMT_MATERIAL ) );
		fread(pMaterial->Name, sizeof( CHAR ), 64, mpFile);
		fread(&pMaterial->Ambient, sizeof( FLOAT ), 14, mpFile);
		fread(&pMaterial->DiffuseTexture, sizeof( CHAR ), 64, mpFile);
		fread(&pMaterial->SpecularTexture, sizeof( CHAR ), 64, mpFile);
		fread(&pMaterial->NormalTexture, sizeof( CHAR ), 64, mpFile);
		fread(&pMaterial->AnimatedTexture, sizeof( CHAR ), 64, mpFile);
		fread(&pMaterial->Flag, sizeof( FLOAT ), 1, mpFile);
	}

	return S_OK;
};

HRESULT ACAMTLoader::ReadJoints( AMT_MODEL* outModel )
{
	UINT numJoints = outModel->Head.NumJoints; 
	//copia do arquivo
	AMT_JOINT* pJoint;
	for (int i = 0; i < numJoints; i++)
	{
		outModel->pJoints.push_back( new AMT_JOINT );
		pJoint = outModel->pJoints[ i ];
		ZeroMemory( pJoint, sizeof( AMT_JOINT ) );
		fread(&pJoint->Name, sizeof ( CHAR ), 64, mpFile); 
		fread(&pJoint->ID, sizeof ( UINT ), 1, mpFile); 
		fread(&pJoint->ParentID, sizeof ( UINT ), 1, mpFile); 
		fread(&pJoint->NumChildren, sizeof ( UINT ), 1, mpFile);
		for (int i=0; i<pJoint->NumChildren; i++)
		{
			UINT childID;
			fread(&childID, sizeof ( UINT ), 1, mpFile); 
			pJoint->JointChildren.push_back(childID);
		}

		fread(&pJoint->NumKF, sizeof ( UINT ), 1, mpFile);
		for (int i = 0; i < pJoint->NumKF; i++)
		{
			AMT_KF kf;;
			fread(&kf.Time, sizeof ( FLOAT ), 1, mpFile);
			fread(&kf.BindMatrix, sizeof ( FLOAT ), 16, mpFile);
			pJoint->KFData.push_back(kf);
		}

		fread(&pJoint->IsAnimated, sizeof ( UINT ), 1, mpFile);
		fread(&pJoint->Flag, sizeof ( UINT ), 1, mpFile);
		fread(&pJoint->BindMatrix, sizeof ( FLOAT ), 16, mpFile);
		fread(&pJoint->MatrixAbsolute, sizeof ( FLOAT ), 16, mpFile);
		fread(&pJoint->InverseBindMatrix, sizeof ( FLOAT ), 16, mpFile);
	}

	for (int i = 0; i < numJoints; i++)
	{
		AMT_JOINT* joint = new AMT_JOINT();
		memcpy(joint, outModel->pJoints[i], sizeof (AMT_JOINT));
		outModel->pOriginalJoints.push_back(joint);
	}

	return AC_OK;
};

HRESULT ACAMTLoader::ReadAnimations( AMT_MODEL* outModel )
{
	UINT numAnimations = outModel->Head.NumAnimations; 
	//copia do arquivo
	AMT_ANIMATION* pAnimation;
	for (int i = 0; i < numAnimations; i++)
	{
		outModel->pAnimations.push_back( new AMT_ANIMATION );
		pAnimation = outModel->pAnimations[ i ];
		ZeroMemory( pAnimation, sizeof( AMT_ANIMATION ) );
		fread(&pAnimation->Name, sizeof ( CHAR ), 64, mpFile); 
		fread(&pAnimation->StartFrame, sizeof ( UINT ), 1, mpFile); 
		fread(&pAnimation->EndFrame, sizeof ( UINT ), 1, mpFile); 
		fread(&pAnimation->Active, sizeof ( UINT ), 1, mpFile); 
	}

	return AC_OK;
};