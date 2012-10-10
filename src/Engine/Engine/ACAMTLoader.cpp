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
	AMT_VERTEX* pVertices = nullptr;                     

	// aloca memoria
	pVertices = new AMT_VERTEX[ numVertices ];
	if( !pVertices )
	{
		return E_FAIL;                                    
	}

	// le todos os vertices de  uma vez so
	ZeroMemory( pVertices, sizeof( AMT_VERTEX ) * numVertices );
	fread( pVertices, sizeof( AMT_VERTEX ), numVertices, mpFile );
	
	AMT_VERTEX* pVertex;
	for (int i = 0; i < numVertices; i++)
	{
		outModel->pVertices.push_back( new AMT_VERTEX );
		pVertex = outModel->pVertices[ i ];
		ZeroMemory( pVertex, sizeof( AMT_VERTEX ) );
		memcpy( pVertex, &pVertices[i], sizeof( AMT_VERTEX ) );
	}

	// libera memoria
	SAFE_DELETE_A(pVertices);

	// bye
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
			fread(&kf.bindMatrix, sizeof ( FLOAT ), 16, mpFile);
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


/*


// -----------------------------------------------------------------
// Name: ReadJoints( void )
// Info: Reads the Joints from to the opened File
//
// Return       = (HRESULT)    Status
// -----------------------------------------------------------------
HRESULT ACCBFLoader::ReadJoints( ACModelStruct* outModel )
{
	// Variablen init
	bool    bLoop  = true;                            // LoopFlag
	UINT    uiLoop = 0;                               // Counter
	UINT    uiNumJoints = mHeader.uiNumJoints;      // temp Var
	LPJOINT pJoint = nullptr;                            // Temp Joint

	// reserver some memory for the joints
	//LOG( 20, false, "Reading Joints [%d]...", uiNumJoints );

	// allocate memory
	JOINT_S* mpJoints = new JOINT_S[ uiNumJoints ];
	if( !mpJoints )
	{
		//LOG( 1, true, " FAILED [JOINTS]" );
		return E_FAIL;                                // bye
	}

	// loop until dawn...
	do{
		// find the next chunk
		switch( GetNextChunk( mChunk ) )
		{
			case V1_JOINT_MAIN:
					pJoint = &mpJoints[ uiLoop ];           // set Joint
					ReadJoint_Main( pJoint );                // read Joint
					uiLoop++;                                // next Joint
					break;

			case V1_JOINT_KEYFRAME_ROT:
					ReadJoint_KeyFrame_Rot( pJoint ); 
					break;

			case V1_JOINT_KEYFRAME_POS:
					ReadJoint_KeyFrame_Pos( pJoint ); 
					break;

			case V1_END:
					bLoop = false;                    
					break;
		}

	}while( bLoop );

	delete [] mpJoints;

	// seek for endchunk
	if( !bLoop ) {
		//LOG( 20, true, " OK" );                        // logit
		return S_OK;                                   // bye
	}

	// log failure
	//LOG( 1, true, " FAILED [JOINTS]" );

	// bye
	return E_FAIL;
}
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: ReadJoint_Main( LPJOINT pJoint )
// Info: Reads the Joints Mainpart from to the opened File
//
// Return       = (HRESULT)    Status
//
// pJoint       = (LPJOINT)    Parentjoint
// -----------------------------------------------------------------
HRESULT ACCBFLoader::ReadJoint_Main( LPJOINT pJoint )
{
	// Start
	//LOG( 20, false, "Reading Joint " );

	// read the joint
	fread( pJoint, sizeof( JOINT_S ), 1, mpFile );

	// seek for endchunk
	if( GetNextChunk( mChunk ) == V1_END ) {
		//LOG( 20, true, " OK" );                        // logit
		return S_OK;                                   // bye
	}

	// log failure
	//LOG( 1, true, " FAILED [JOINT_MAIN]" );

	// bye
	return E_FAIL;
}
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: ReadJoint_KeyFrame_Rot( LPJOINT pJoint )
// Info: Reads the Keyframe_Rotation from to the opened File
//
// Return       = (HRESULT)    Status
//
// pJoint       = (LPJOINT)    Parentjoint
// -----------------------------------------------------------------
HRESULT ACCBFLoader::ReadJoint_KeyFrame_Rot( LPJOINT pJoint )
{
	// variablen init
	UINT   uiNumKeys = pJoint->wNumKF_Rotation;

	// start reading materials
	//LOG( 20, false, "Reading KeyFrame Rotation [%d]...", uiNumKeys );

	// allocate memory
	pJoint->pKF_Rotation = new KF_ROT_S[ uiNumKeys ];
	if( !pJoint->pKF_Rotation )
	{
		//LOG( 1, true, " FAILED [JOINT_KEYFRAME_ROTATIONS]" );
		return E_FAIL;                                    // bye
	}

	// clear memory
	ZeroMemory( pJoint->pKF_Rotation, sizeof( KF_ROT_S ) * uiNumKeys );

	// read all of the Rotations at once
	fread( pJoint->pKF_Rotation, sizeof( KF_ROT_S ), uiNumKeys, mpFile );

	// seek for endchunk
	if( GetNextChunk( mChunk ) == V1_END ) 
	{
		//LOG( 20, true, " OK" );                        // logit
		return S_OK;                                   // bye
	}

	// log failure
	//LOG( 1, true, " FAILED [JOINT_KEYFRAME_ROTATIONS]" );

	// bye
	return E_FAIL;
}
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: ReadJoint_KeyFrame_Pos( LPJOINT pJoint )
// Info: Reads the Keyframe_Position from to the opened File
//
// Return       = (HRESULT)    Status
//
// pJoint       = (LPJOINT)    Parentjoint
// -----------------------------------------------------------------
HRESULT ACCBFLoader::ReadJoint_KeyFrame_Pos( LPJOINT pJoint )
{
	// variablen init
	UINT   uiNumKeys = pJoint->wNumKF_Position;

	// start reading materials
	//LOG( 20, false, "Reading KeyFrame Positions [%d]...", uiNumKeys );

	// allocate memory
	pJoint->pKF_Position = new KF_POS_S[ uiNumKeys ];
	if( !pJoint->pKF_Position )
	{
		//LOG( 1, true, " FAILED [JOINT_KEYFRAME_POSITIONS]" );
		return E_FAIL;                                    // bye
	}

	// clear memory
	ZeroMemory( pJoint->pKF_Position, sizeof( KF_POS_S ) * uiNumKeys );

	// read all of the Positions at once
	fread( pJoint->pKF_Position, sizeof( KF_POS_S ), uiNumKeys, mpFile );

	// seek for endchunk
	if( GetNextChunk( mChunk ) == V1_END ) 
	{
		//LOG( 20, true, " OK" );                        // logit
		return S_OK;                                   // bye
	}

	// log failure
	//LOG( 1, true, " FAILED [JOINT_KEYFRAME_POSITIONS]" );

	// bye
	return E_FAIL;
}
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: ReadAnimations( void )
// Info: Reads the Animations
//
// Return        = (HRESULT)  Status
// -----------------------------------------------------------------
HRESULT ACCBFLoader::ReadAnimations( ACModelStruct* outModel )
{
	// variablen init
	UINT    uiNumAnim = mHeader.uiNumAnimations;     // temp Var

	// start reading materials
	//LOG( 20, false, "Reading Animations [%d]...", uiNumAnim );

	// allocate memory
	ANIMATION_S* mpAnimations = new ANIMATION_S[ uiNumAnim ];
	if( !mpAnimations )
	{
		//LOG( 1, true, " FAILED [ANIMATIONS]" );        // logit
		return E_FAIL;                                 // bye
	}

	// clear memory
	ZeroMemory( mpAnimations, sizeof( ANIMATION_S ) * uiNumAnim );

	// read all of the faces at once
	fread( mpAnimations, sizeof( ANIMATION_S ), uiNumAnim, mpFile );


	delete [] mpAnimations;
	// seek for endchunk
	if( GetNextChunk( mChunk ) == V1_END ) 
	{
		//LOG( 20, true, " OK" );                        // logit
		return S_OK;                                   // bye
	}

	// log failure
	//LOG( 1, true, " FAILED [ANIMATIONS]" );

	// bye
	return E_FAIL;
}
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: Prepare( void )
// Info: Prepare the model
//
// Return       = (HRESULT)    Status
// -----------------------------------------------------------------
/*HRESULT ACCBFLoader::Prepare( void )
{
   // Variablen init
   ULONG      ulNumIndices  = 0;                      // counter
   ULONG      ulNumVertices = 0;                      // counter
   UINT       uiCurrentMat  = 0;                      // Material
   PWORD      pIndex        = nullptr;                   // Index
   ULONG      ulCounter     = 0;                      // counter
   LPMATERIAL pMaterial     = nullptr;                   // Material
   char       cTexture[256] = { 0 };                  // Texturepath
   PCHAR      pcSeperator   = nullptr;                   // Seperator
   ULONG      ulIndexCount  = 0;                      // Counter

   // 1. we have to setup the Bones
   SetupBones();

   // log it
   LOG( 20, false, "Sort Indices by Material [%d]", m_sHeader.uiNumMaterials );

   // get max memory for the indices and vertices
   m_sHeader.ulNumIndices = m_sHeader.ulNumFaces * 3;
   pIndex          = new WORD[ m_sHeader.ulNumIndices ];

   // prepare pointers
   m_ppIndices     = new PVOID[ m_sHeader.uiNumMaterials ];
   ZeroMemory( m_ppIndices, sizeof( PVOID ) * m_sHeader.uiNumMaterials );

   m_pIndices      = new WORD[ m_sHeader.ulNumIndices ];
   ZeroMemory( m_pIndices, sizeof( WORD ) * m_sHeader.ulNumIndices );

   m_puiNumIndices = new UINT[ m_sHeader.uiNumMaterials ];
   ZeroMemory( m_puiNumIndices, sizeof( UINT ) * m_sHeader.uiNumMaterials );
   
   m_puiSkinBuffer = new UINT[ m_sHeader.uiNumMaterials ];
   ZeroMemory( m_puiSkinBuffer, sizeof( UINT ) * m_sHeader.uiNumMaterials );

   // Sort all the Faces into the Indexarrays by MaterialID
   do{
    
	// clear indexarray
	ZeroMemory( pIndex, sizeof(WORD) * m_sHeader.ulNumIndices );
	
	// reset index counter
	ulNumIndices = 0;

    // loop through all the faces
	for( ulCounter = 0; ulCounter < m_sHeader.ulNumFaces; ulCounter++ )
	{
	   // same material as the current one?
	   if( m_pFaces[ ulCounter ].uiMaterialID == uiCurrentMat )
	   {
		   // copy index
		   m_pIndices[ ulIndexCount++ ] = pIndex[ ulNumIndices++ ] = (WORD)m_pFaces[ ulCounter ].ulIndices[ 0 ];
		   m_pIndices[ ulIndexCount++ ] = pIndex[ ulNumIndices++ ] = (WORD)m_pFaces[ ulCounter ].ulIndices[ 1 ];
		   m_pIndices[ ulIndexCount++ ] = pIndex[ ulNumIndices++ ] = (WORD)m_pFaces[ ulCounter ].ulIndices[ 2 ];
	   }
	}

	// enough Indices
	if( !ulNumIndices )
	{
		// set new Material
	    uiCurrentMat++;
		
		// Stop Error
		LOG( 1, true, "STOP Error: Not enough Indices..." );

		// goon
		continue;
	}

	// copy the indexarray
	m_puiNumIndices[ uiCurrentMat ] = ulNumIndices;
	m_ppIndices[ uiCurrentMat ] = new WORD[ ulNumIndices ];
	memcpy( m_ppIndices[ uiCurrentMat ], pIndex, sizeof(WORD) * ulNumIndices );

	// set current material
	pMaterial = &m_pMaterials[ uiCurrentMat ];

	// Readin the Materials
    if( FAILED( m_pRenderDevice->GetSkinManager()->AddSkin( 
 	                             (ZFXCOLOR*)&pMaterial->fAmbient,
                                 (ZFXCOLOR*)&pMaterial->fDiffuse,
                                 (ZFXCOLOR*)&pMaterial->fEmissive,
                                 (ZFXCOLOR*)&pMaterial->fSpecular,
                                 pMaterial->fSpecularPower,
                                 &m_puiSkinBuffer[ uiCurrentMat ])))
	{
	   // logit
	   LOG( 1, true, " FAILED [LOAD SKIN %d]", uiCurrentMat );
	}

	// prepare texture
	ZeroMemory( cTexture, sizeof( char ) * 256 );
	pcSeperator = strchr( strrev( strdup( m_pcFileName ) ), '/' );
	if( !pcSeperator ) pcSeperator = strchr( strrev( strdup( m_pcFileName ) ), 92 );
	if( pcSeperator )	strcpy( cTexture, strrev( pcSeperator ) );
	strcat( cTexture, pMaterial->cTexture_1 );

	// load texture
	if( FAILED( m_pRenderDevice->GetSkinManager()->AddTexture(	
                                 m_puiSkinBuffer[ uiCurrentMat ],
                                 cTexture, false,
								 0, nullptr, 0 ) ) )
	{
	   // logit
	   LOG( 1, true, " FAILED [LOAD TEXTURE %s]", pMaterial->cTexture_1 );
	}

	// set new Material
	uiCurrentMat++;    

   }while( uiCurrentMat != m_sHeader.uiNumMaterials );

   // now release old memory
   delete [] pIndex;

   // log it
   LOG( 20, true, " done" );

   // bye
   return S_OK;
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: SetupBones( void )
// Info: Prepare the bones from the Model
//
// Return       = (HRESULT)    Status
// -----------------------------------------------------------------
/*HRESULT ACCBFLoader::SetupBones( void )
{
	// Variablen init
    LPJOINT     pJoint       = nullptr;                    // Joint
	ULONG	    ulCounter    = 0;                       // Counter
	UINT	    uiLoop       = 0;                       // Counter
	UINT        uiParentID   = 0;                       // Parent ID
	Vector3		sVector_A;                              // Vector
	Vector3		sVector_B;                              // Vector
	ACVERTEXFORMAT     *pVertex     = nullptr;             // TempVertex
	MATRIX		matTemp; 
	
	Matrix::ToIdentity(&matTemp);

	// If we have no Bones we can leave at this point
	if( mHeader.uiNumJoints == 0 ) return S_OK;

	// empower the matrices
	for( ulCounter = 0; ulCounter < mHeader.uiNumJoints; ulCounter++ )
	{
		// Get the current joint
		pJoint = &mpJoints[ ulCounter ];

		// cria a matrix relativa
		pJoint->sMatrix_relative = CreateRotationMatrix( &pJoint->vRotation );

		// seta a posicao na matrix relativa
		pJoint->sMatrix_relative.M14 = pJoint->vPosition.X;
		pJoint->sMatrix_relative.M24 = pJoint->vPosition.Y;
		pJoint->sMatrix_relative.M34 = pJoint->vPosition.Z;

		// Find our Parent...
		for( uiLoop = 0; uiLoop < mHeader.uiNumJoints; uiLoop++ )
		{
			// set the parent id, for later checks
			uiParentID = 255;

			if( strcmp( mpJoints[ uiLoop ].cName, pJoint->cParentName ) == 0 )
			{
				// We have found our parent, so lets stop
				uiParentID = uiLoop;
				break;
			}
		}

		// Store the known parentid
		pJoint->wParentID = uiParentID;

		// Did we found our parent?
		if( uiParentID != 255 )
		{
			// We found our parents, so we have to multiply the parent absolut-
			// matrix with our relativmatrix. So we get a new and polished
			// absolutmatrix. Straight forward, eh?
			pJoint->sMatrix_absolute = mpJoints[ uiParentID ].sMatrix_absolute *
				              pJoint->sMatrix_relative;
		} else 
		{
			// We stay on our own.... what a pity...
			// So our relativmatrix is our absolutmatrix :)
			pJoint->sMatrix_absolute = pJoint->sMatrix_relative;

		}
		// get the final matrix
		Matrix::Transpose(&pJoint->sMatrix_absolute, &pJoint->sMatrix);

		// Transponieren der Matrizen
		matTemp = pJoint->sMatrix_relative;
		Matrix::Transpose(&matTemp, &pJoint->sMatrix_relative);
	}
	
	// Setup all Vertices
	for( ulCounter = 0; ulCounter < mHeader.ulNumVertices; ulCounter++ )
	{
		// get the current vertex
		pVertex = &mpVerticesOrig[ ulCounter ];

		// process only, when there is a bone
		if( pVertex->Bone1 != 255.0f )
		{
			// Get current matrix
			MATRIX.ToIdentity(&matTemp);
			matTemp = mpJoints[ (UINT)pVertex->Bone1 ].sMatrix;
			
			// 1. rotate the vertices
			sVector_A.X = pVertex->Position.X;
			sVector_A.Y = pVertex->Position.Y;
			sVector_A.Z = pVertex->Position.Z;
			sVector_A.X -= matTemp.M41;
			sVector_A.Y -= matTemp.M42;
			sVector_A.Z -= matTemp.M43;

			//usa a matriz transposta para calcular o vetor
			MATRIX transpose;
			Matrix::Transpose(&matTemp, &transpose);
			Vector3::TransformNormal(&sVector_A, &transpose, &sVector_A);
			pVertex->Position.X = sVector_A.X;
			pVertex->Position.Y = sVector_A.Y;
			pVertex->Position.Z = sVector_A.Z;

			// 2. rotate the normals
			sVector_A.X = pVertex->Normal.X;
			sVector_A.Y = pVertex->Normal.Y;
			sVector_A.Z = pVertex->Normal.Z;
			Vector3::TransformNormal(&sVector_A, &transpose, &sVector_A);
			pVertex->Normal.X = sVector_A.X;
			pVertex->Normal.Y = sVector_A.Y;
			pVertex->Normal.Z = sVector_A.Z;
		}
	}
		
	// bye
	return S_OK;
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: SetAnimation( UINT uiAnim )
// Info: Sets the Animationssequenze
//
// uiAnim       = (UINT) Animationssquenze
//
// Return       = (HRESULT)    Status
// -----------------------------------------------------------------
/*void ACCBFLoader::SetAnimation( UINT uiAnim )
{
	// In Range?
	if( uiAnim > mHeader.uiNumAnimations ) uiAnim = 0;
	if( uiAnim < 0 ) uiAnim = mHeader.uiNumAnimations;

	// Setz Animation
	mCurrentAnimation  = uiAnim;

	// Multiply Animations
	mAnimationRunOnce   = false;
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: SetAnimation( bool bSingle, UINT uiAnim )
// Info: Sets the Animationssequenze
//
// bSingle      = (bool) Singleanimation
// uiAnim       = (UINT) Animationssquenze
//
// Return       = (HRESULT)    Status
// -----------------------------------------------------------------
/*void ACCBFLoader::SetAnimation( bool bSingle, UINT uiAnim )
{
	// Set the wanted Animation
	SetAnimation( uiAnim );

	// Multiply Animations
	mAnimationChanged   = true;
	mAnimationRunOnce   = bSingle;
	mAnimationComplete  = false;
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: SetRenderBones( void )
// Info: Render the bones?
// -----------------------------------------------------------------
/*void ACCBFLoader::SetRenderBones( void )
{ 
	mRenderBones = !mRenderBones; 
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: SetRenderNormals( void )
// Info: Render the Normals?
// -----------------------------------------------------------------
/*void ACCBFLoader::SetRenderNormals( void )
{ 
	mRenderNormals = !mRenderNormals; 
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: SetScaling( float fScale /* = 0.0f  */ )
// Info: Sets the scaling of the model. 0.0f for no scaling.
//
// -----------------------------------------------------------------
/*void ACCBFLoader::SetScaling( float fScale )
{
	// Variablen init
	ULONG				ulCounter     = 0;                      // Counter
	ULONG				ulInner       = 0;                      // Counter
	PACVERTEXFORMAT     pVertex       = nullptr;                   // TempVertex
	float				fScaling      = 0.0f;                   // Scaling
	LPJOINT				pJoint        = nullptr;                   // Joint
	
	// shall we scale the model?
	if( fScale == 0.0f ) return;

	// get the bounding box
	// reset the min and max vertex of the BBox
	m_sBBoxMin.x = 999999.0f; m_sBBoxMax.x = -999999.0f;
	m_sBBoxMin.y = 999999.0f; m_sBBoxMax.y = -999999.0f;
	m_sBBoxMin.z = 999999.0f; m_sBBoxMax.z = -999999.0f;

	// Now setup all known vertices
	for( ulCounter = 0; ulCounter < mHeader.ulNumVertices; ulCounter++ )
	{
		// get the current Vertex
		pVertex    = &mpVertices[ ulCounter ];

		// 2. Calc BBox
		m_sBBoxMax.x = MAX( m_sBBoxMax.x, pVertex->x );
		m_sBBoxMax.y = MAX( m_sBBoxMax.y, pVertex->y );
		m_sBBoxMax.z = MAX( m_sBBoxMax.z, pVertex->z );
		m_sBBoxMin.x = MIN( m_sBBoxMin.x, pVertex->x );
		m_sBBoxMin.y = MIN( m_sBBoxMin.y, pVertex->y );
		m_sBBoxMin.z = MIN( m_sBBoxMin.z, pVertex->z );
	}

	// Calc scaling
	fScaling = ( m_sBBoxMax.y - m_sBBoxMin.y ) / fScale;
	
	// Now scale all known vertices
	for( ulCounter = 0; ulCounter < mHeader.ulNumVertices; ulCounter++ )
	{
		// get the current vertex
		pVertex      = &mpVertices[ ulCounter ];

		// scale the vertex
		pVertex->Position.X   /= fScaling;
		pVertex->Position.Y   /= fScaling;
		pVertex->Position.Z   /= fScaling;	
	}

	// copy the vertices if we have animations...
	if( mHeader.uiNumJoints > 0 )
		memcpy( mpVerticesOrig, mpVertices, sizeof( ACVERTEXFORMAT ) * mHeader.ulNumVertices );

	// Now scale all known bones
	for( ulCounter = 0; ulCounter < mHeader.uiNumJoints; ulCounter++ )
	{
		// get the current vertex
		pJoint       = &mpJoints[ ulCounter ];

		// scale the vertex
		pJoint->vPosition.X   /= fScaling;
		pJoint->vPosition.Y   /= fScaling;
		pJoint->vPosition.Z   /= fScaling;

		// scale the keyframe position
		for( ulInner = 0; ulInner < pJoint->wNumKF_Position; ulInner++ )
		{
			pJoint->pKF_Position[ ulInner ].vPosition.X /= fScaling;
			pJoint->pKF_Position[ ulInner ].vPosition.Y /= fScaling;
			pJoint->pKF_Position[ ulInner ].vPosition.Z /= fScaling;
		}
	}

	// create Aabb
	mBB.pMin->X = m_sBBoxMin.x;
	mBB.pMin->Y = m_sBBoxMin.y;
	mBB.pMin->Z = m_sBBoxMin.z;
	mBB.pMax->X = m_sBBoxMax.x;
	mBB.pMax->Y = m_sBBoxMax.y;
	mBB.pMax->Z = m_sBBoxMax.z;
	m_sAabb.vcCenter.x = ( m_sBBoxMax.x - m_sBBoxMin.x ) / 2;
	m_sAabb.vcCenter.y = ( m_sBBoxMax.y - m_sBBoxMin.y ) / 2;
	m_sAabb.vcCenter.z = ( m_sBBoxMax.z - m_sBBoxMin.z ) / 2;
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: GetAnimation( void )
// Info: Gets the Animationssequenze
//
// Return       = (HRESULT)    Animationssquenze
// -----------------------------------------------------------------
//UINT CZFXModel::GetAnimation( void ){ return m_uiCurrentAnimation; }
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: GetVertices( CVERTEX* *pVertices )
// Info: Gets the whole Data out of the model
//
// Return       = (ULONG)      Number of Vertices
//
// pVertices    = (CVERTEX*)   Pointer for the Vertices
// -----------------------------------------------------------------
/*ULONG CZFXModel::GetVertices( CVERTEX* *pVertices )
{ 
	// return the Pointer
	if( pVertices ) *pVertices = m_pVertices;
	
	// return the number of Vertices
	return m_sHeader.ulNumVertices;
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: GetIndices( PWORD *pIndices )
// Info: Gets the whole Data out of the model
//
// Return       = (ULONG)      Number of Vertices
//
// pIndices     = (PWORD)      Pointer for the Indices
// -----------------------------------------------------------------
/*ULONG CZFXModel::GetIndices( PWORD *pIndices )
{ 
	// return the Pointer
	if( pIndices ) *pIndices = m_pIndices;

	// return the number of Vertices
	return m_sHeader.ulNumIndices;
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: GetBBoxMin( void )
// Info: Returns the BBox MinVertex
//
// Return       = (VERTEX)  MinVertex
// -----------------------------------------------------------------
//VERTEX CZFXModel::GetBBoxMin( void ){ return m_sBBoxMin; }
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: GetBBoxMax( void )
// Info: Returns the BBox MaxVertex
//
// Return       = (VERTEX)  MaxVertex
// -----------------------------------------------------------------
//VERTEX CZFXModel::GetBBoxMax( void ){ return m_sBBoxMax; }
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: GetBBoxMax( void )
// Info: Returns the BBox MaxVertex
//
// Return       = (ZFXAabb)  Aabb
// -----------------------------------------------------------------
//ZFXAabb CZFXModel::GetAabb( void ){ return m_sAabb; }
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: IsAnimationCompleted( void )
// Info: Gets the Animation completeflag
//
// Return       = (bool)    Animation Complete Flag
// -----------------------------------------------------------------
//bool CZFXModel::IsAnimationCompleted( void ){ return m_bAnimationComplete; }
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: Animation( void )
// Info: Animate the model
//
// Return       = (HRESULT)    Status
// -----------------------------------------------------------------
/*HRESULT CZFXModel::Animation( void )
{
	// Variablen init
	float		fElapsed	= -1.0f;						// elapsed time
	float		fStart		= -1.0f;						// Start
	float		fEnd		= -1.0f;						// End
	LPANIMATION pAnimation  = nullptr;                         // Animation

	// Do we have some animations?
	if( m_sHeader.uiNumJoints == 0 )                     return S_OK;

	// Shall we run the Animation only one time?
	if( m_bAnimationRunOnce  &&
		m_bAnimationComplete &&
		!m_bAnimationChanged )
		return S_OK;

	// Get Time
	m_fTime = (float)GetTickCount();
	
	// New, then set the time as Starttime
	if( m_fStartTime == -1.0f )	m_fStartTime = m_fTime;

	// calc elapsed Time
	fElapsed = m_fTime - m_fStartTime;

	// get the current animation ----------------------------------------------
	pAnimation = &m_pAnimations[ m_uiCurrentAnimation ];

	fStart	= pAnimation->fStartFrame;						// Startzeit
	fEnd	= pAnimation->fEndFrame;						// Endzeit

	// Calculate the current frameposition
	m_fFrame = fStart + (m_sHeader.fAnimationFPS / 2048) * fElapsed;
	
	// Set the startframe if we are new
	if( m_fFrame <= fStart )	m_fFrame = fStart;

	// reached the end? if so, then tweak the time
	if( m_fFrame >= fEnd )
	{
		m_fStartTime = m_fTime;   			                // Zeit setzen
		m_fFrame     = fStart;
		m_bAnimationComplete	= true;						// Flag setzen
	}
	else
	{
		// Prepare the animation
		AnimationPrepare();

		// Setup all Vertices
		AnimationVertices();
		m_bAnimationComplete	= false;					// Flag setzen
		m_bAnimationChanged     = false;                    // Animation changed

	}

	// bye
	return S_OK;
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: AnimationPrepare( void )
// Info: Prepares the animationsequence
//
// Return       = (HRESULT)    Status
// -----------------------------------------------------------------
/*HRESULT CZFXModel::AnimationPrepare( void )
{
	// Variablen init
	LPJOINT     pJoint       = nullptr;                    // Joint
	ULONG	    ulCounter    = 0;                       // Counter
	UINT	    uiLoop       = 0;                       // Counter
	ZFXVector   sPosition;                              // Vector
	ZFXVector   sRotation;                              // Vector
	UINT        uiKeyPos     = 0;                       // KeyPositions
    UINT        uiKeyRot     = 0;                       // KeyRotations
	LPKF_ROT    pLastRot     = nullptr;                    // Rotation
	LPKF_ROT    pThisRot     = nullptr;                    // Rotation
	LPKF_ROT    pKeyRot      = nullptr;                    // Rotation
	LPKF_POS    pLastPos     = nullptr;                    // Position
	LPKF_POS    pThisPos     = nullptr;                    // Position
	LPKF_POS    pKeyPos      = nullptr;                    // Position
	float       fScale       = 0.0f;                    // Scaling
	ZFXMatrix   matTemp; matTemp.Identity();            // Temp Matrix
	ZFXMatrix   matFinal; matFinal.Identity();          // Temp Matrix

	// clip the animation
	if( m_fFrame > m_sHeader.uiNumFrames ) m_fFrame = 0;

	// empower the matrices
	for( ulCounter = 0; ulCounter < m_sHeader.uiNumJoints; ulCounter++ )
	{
		// Get the current joint
		pJoint   = &m_pJoints[ ulCounter ];

		// Get some other data
		uiKeyPos =  pJoint->wNumKF_Position;            // Position
		uiKeyRot =  pJoint->wNumKF_Rotation;            // Rotation

		// do we have to calculate a new matrix?
		if( ( uiKeyRot + uiKeyPos )  != 0 )
		{
			// yes... there is a new Position or Rotation

			// process the positions ------------------------------------------
			pLastPos = nullptr; pThisPos = nullptr; pKeyPos = nullptr;

			for( uiLoop = 0; uiLoop < uiKeyPos; uiLoop++ )
			{
				// get current position
				pKeyPos = &pJoint->pKF_Position[ uiLoop ];

				// check with the time
				if( pKeyPos->fTime >= m_fFrame )
				{
					// we've found the current Time
					pThisPos = pKeyPos;

					// bye
					break;
				}

				// nothing found
				pLastPos = pKeyPos;
			} // all positions

			// now interpolate the to positions
			if( pLastPos && pThisPos )
			{
				// calc scaling
				fScale = ( m_fFrame - pLastPos->fTime ) / ( pThisPos->fTime - pLastPos->fTime );

				// Interpolate the positions
				sPosition = pLastPos->vPosition + ( pThisPos->vPosition - pLastPos->vPosition ) * fScale;
			} else if( !pLastPos )
			{
				// copy the position
				sPosition = pThisPos->vPosition;
			} else {
				// copy the position
				sPosition = pLastPos->vPosition;
			}

			// process the rotations ------------------------------------------
			pLastRot = nullptr; pThisRot = nullptr; pKeyRot = nullptr;

			for( uiLoop = 0; uiLoop < uiKeyRot; uiLoop++ )
			{
				// get current Rotition
				pKeyRot = &pJoint->pKF_Rotation[ uiLoop ];

				// check with the time
				if( pKeyRot->fTime >= m_fFrame )
				{
					// we've found the current Time
					pThisRot = pKeyRot;

					// bye
					break;
				}

				// nothing found
				pLastRot = pKeyRot;
			} // all Rotitions

			// now interpolate the to Rotations
			if( pLastRot && pThisRot )
			{
				// Interpolate the Rotitions
				sRotation = pLastRot->vRotation + ( pThisRot->vRotation - pLastRot->vRotation ) * fScale;
			} else if( !pLastRot )
			{
				// copy the Rotition
				sRotation = pThisRot->vRotation;
			} else {
				// copy the Rotition
				sRotation = pLastRot->vRotation;
			}

			// setup the joint matrix
			matTemp.SetTranslation( sPosition );        // Position
			matTemp.Rota( sRotation );                  // Rotation

			// calc the relative matrix
			matFinal = matTemp * pJoint->sMatrix_relative;

			// Did we found our parent?
			if( pJoint->wParentID != 255 )
			{
				// We found our parents, so we have to multiply the parent absolut-
				// matrix with our relativmatrix. So we get a new and polished
				// absolutmatrix. Straight forward, eh?
				pJoint->sMatrix = matFinal * m_pJoints[ pJoint->wParentID ].sMatrix;
			} else {
				// We stay on our own.... what a pity...
				// So our relativmatrix is our absolutmatrix :)
				pJoint->sMatrix = matFinal;
			}           

		} else {

			// We have no new Matrix, so copy the old one....
			pJoint->sMatrix = pJoint->sMatrix_relative;
		} // change Matrix
	} // all Joints

	// bye
	return S_OK;
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: AnimationVertices( void  )
// Info: Calculates the Vertices with the current animationsequence
//
// Return       = (HRESULT)    Status
// -----------------------------------------------------------------
/*HRESULT CZFXModel::AnimationVertices( void )
{
	// Variablen init
	ULONG	    ulCounter     = 0;                      // Counter
	CVERTEX     *pVertex      = nullptr;                   // TempVertex
	CVERTEX     *pVertex_Orig = nullptr;                   // TempVertex
	ZFXVector   sVector_A, sVector_B;                   // Vector
	
	// reset the min and max vertex of the BBox
	m_sBBoxMin.x =  999999.0f; m_sBBoxMin.y =  999999.0f;
	m_sBBoxMin.z =  999999.0f; m_sBBoxMax.x = -999999.0f;
	m_sBBoxMax.y = -999999.0f; m_sBBoxMax.z = -999999.0f;

	// Now setup all known vertices
	for( ulCounter = 0; ulCounter < m_sHeader.ulNumVertices; ulCounter++ )
	{
		// get the current vertex
		pVertex      = &m_pVertices[ ulCounter ];
		pVertex_Orig = &m_pVertices_Orig[ ulCounter ];

		// process only, when there is a bone
		if( pVertex->fBone1 != 255.0f )
		{
			// 1. get the current orig vertex
			sVector_A.x = pVertex_Orig->x;
			sVector_A.y = pVertex_Orig->y;
			sVector_A.z = pVertex_Orig->z;

			// 2. rotate the vertex
			sVector_A.RotateWith( m_pJoints[ (UINT)pVertex_Orig->fBone1 ].sMatrix );

			// 3. get the Position
			sVector_A  += m_pJoints[ (UINT)pVertex_Orig->fBone1 ].sMatrix.GetTranslation();

            // 4. save and calc the new position
			pVertex->x = sVector_A.x;
			pVertex->y = sVector_A.y;
			pVertex->z = sVector_A.z;

			// 5. skin the normals
			sVector_A.x = pVertex_Orig->vcN[ 0 ];
			sVector_A.y = pVertex_Orig->vcN[ 1 ];
			sVector_A.z = pVertex_Orig->vcN[ 2 ];
			sVector_A.RotateWith( m_pJoints[ (UINT)pVertex_Orig->fBone1 ].sMatrix );
			pVertex->vcN[ 0 ] = sVector_A.x;
			pVertex->vcN[ 1 ] = sVector_A.y;
			pVertex->vcN[ 2 ] = sVector_A.z;

			// 6. Calc BBox
			m_sBBoxMax.x = MAX( m_sBBoxMax.x, pVertex->x );
			m_sBBoxMax.y = MAX( m_sBBoxMax.y, pVertex->y );
			m_sBBoxMax.z = MAX( m_sBBoxMax.z, pVertex->z );
			m_sBBoxMin.x = MIN( m_sBBoxMin.x, pVertex->x );
			m_sBBoxMin.y = MIN( m_sBBoxMin.y, pVertex->y );
			m_sBBoxMin.z = MIN( m_sBBoxMin.z, pVertex->z );
		}
	}

	// create Aabb
	m_sAabb.vcMin.x = m_sBBoxMin.x;
	m_sAabb.vcMin.y = m_sBBoxMin.y;
	m_sAabb.vcMin.z = m_sBBoxMin.z;
	m_sAabb.vcMax.x = m_sBBoxMax.x;
	m_sAabb.vcMax.y = m_sBBoxMax.y;
	m_sAabb.vcMax.z = m_sBBoxMax.z;
	m_sAabb.vcCenter.x = ( m_sBBoxMax.x - m_sBBoxMin.x ) / 2;
	m_sAabb.vcCenter.y = ( m_sBBoxMax.y - m_sBBoxMin.y ) / 2;
	m_sAabb.vcCenter.z = ( m_sBBoxMax.z - m_sBBoxMin.z ) / 2;

	// bye
	return S_OK;
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: Update( float fTime  )
// Info: Updates the model
//
// fTime        = Deltatime
//
// Return       = (HRESULT)    Status
// -----------------------------------------------------------------
/*HRESULT CZFXModel::Update( float fTime )
{
	// Set time
	m_fTime = fTime;

	// Do the animation
	return Animation();
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: Render( void )
// Info: Render the model
//
// Return       = (HRESULT)    Status
// -----------------------------------------------------------------
/*HRESULT CZFXModel::Render( void )
{
   // Variablen init
   UINT     uiCounter = 0;                                // Counter
   HRESULT	hRet = S_OK;

   // Set Culling
   m_pRenderDevice->SetBackfaceCulling(RS_CULL_CCW);

   // render the VB
   for( uiCounter = 0; uiCounter < m_sHeader.uiNumMaterials; uiCounter++ )
         if( FAILED( hRet = m_pRenderDevice->GetVertexManager()->Render( VID_CA, m_sHeader.ulNumVertices, m_puiNumIndices[ uiCounter ],
			                                                      m_pVertices, (PWORD)m_ppIndices[ uiCounter ], m_puiSkinBuffer[ uiCounter ] ) ) )
		 {
			 LOG( 1, true, "ERROR Failed to Render VB: %d [%d]", m_puiSkinBuffer[ uiCounter ], uiCounter );
			 if ( hRet == ZFX_BUFFERSIZE ) LOG( 1, true, "Buffer" );
		 };

   // render other stuff?
   if( m_bRenderBones )   RenderBones();                 // Bones
   if( m_bRenderNormals ) RenderNormals();               // Normals
  
   // bye
   return S_OK;
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: RenderBones( void )
// Info: Render the bones of the model
//
// fTime        = Deltatime
//
// Return       = (HRESULT)    Status
// -----------------------------------------------------------------
/*HRESULT CZFXModel::RenderBones( void )
{
	// Variablen init
	UINT     uiCounter = 0;                                // Counter
	LVERTEX  pLine[3];                                     // Joints
	WORD     pIndis[3] = { 0, 1, 2 };                      // Indices
	DWORD    dwColor   = 0x00ffff;                         // Bonecolor
	
	// got bones?
	if( m_sHeader.uiNumJoints == 0 ) return S_OK;

	// render the bones
	m_pRenderDevice->SetBackfaceCulling(RS_CULL_NONE);
	m_pRenderDevice->SetDepthBufferMode(RS_DEPTH_NONE);

	// render the bones
	for( uiCounter = 0; uiCounter < m_sHeader.uiNumJoints; uiCounter++ )
	{
		// First Vertex
		pLine[0].x = m_pJoints[ uiCounter ].sMatrix._41;
		pLine[0].y = m_pJoints[ uiCounter ].sMatrix._42;
		pLine[0].z = m_pJoints[ uiCounter ].sMatrix._43;
		pLine[0].Color  = dwColor;

		if( m_pJoints[ uiCounter ].wParentID != 255 )
		{
			// Second Vertex
			pLine[1].x = m_pJoints[ m_pJoints[
			uiCounter ].wParentID ].sMatrix._41;
			pLine[1].y = m_pJoints[ m_pJoints[
			uiCounter ].wParentID ].sMatrix._42;
			pLine[1].z = m_pJoints[ m_pJoints[
			uiCounter ].wParentID ].sMatrix._43;
			pLine[1].Color  = dwColor;

			// Third Vertex
			pLine[2].x = pLine[1].x + 1.0f;
			pLine[2].y = pLine[1].y + 1.0f;
			pLine[2].z = pLine[1].z + 1.0f;
			pLine[2].Color  = dwColor;

			// Render
			m_pRenderDevice->GetVertexManager()->Render(VID_UL, 3, 3, pLine, pIndis, 0);
		}
	}
	    
	// Settings for the engine
	m_pRenderDevice->SetDepthBufferMode(RS_DEPTH_READWRITE);
	m_pRenderDevice->SetBackfaceCulling(RS_CULL_CCW);

	// bye
	return S_OK;
}*/
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Name: RenderNormals( void )
// Info: Render the normals of the model
//
// fTime        = Deltatime
//
// Return       = (HRESULT)    Status
// -----------------------------------------------------------------
/*HRESULT CZFXModel::RenderNormals( void )
{
	// Variablen init
	ULONG    ulCounter = 0;                                // Counter
	float    fStart[3] = { 0,0,0 };                        // Start
	float    fEnd[3]   = { 0,0,0 };                        // End
	ZFXCOLOR sColor    = { 1.0f, 0, 0, 0 };                // Colors
	CVERTEX* pVertex   = nullptr;                             // Vertex


	// render the bones
	for( ulCounter = 0; ulCounter < m_sHeader.ulNumVertices; ulCounter++ )
	{
		// get current Vertex
		pVertex = &m_pVertices[ ulCounter ];

		// set Startpoint
		memcpy( fStart, &pVertex->x, sizeof( float ) * 3 );

		// set the Endpoint
		fEnd[0] = fStart[0] + (pVertex->vcN[0] * 2.0f);
		fEnd[1] = fStart[1] + (pVertex->vcN[1] * 2.0f);
		fEnd[2] = fStart[2] + (pVertex->vcN[2] * 2.0f);

		// Render the normal
		m_pRenderDevice->GetVertexManager()->RenderLine( fStart, fEnd, &sColor );
	}

	// bye
	return S_OK;
}*/
// -----------------------------------------------------------------


// ----------------------------------------------------------------------------
// Name: CreateRotationMatrix( Vector_C *pVector )
// Info: Erstellt eine Rotationsmatrix aus dem Vector
//
// pVector			= Rotationsvector
// ----------------------------------------------------------------------------
/*ZFXMatrix CZFXModel::CreateRotationMatrix( ZFXVector *pVector )
{
	// Variablen init
	float	sr, sp, sy, cr, cp, cy;
	ZFXMatrix matRet;

	matRet.Identity();

	sy = (float)sin( pVector->z );
	cy = (float)cos( pVector->z );
	sp = (float)sin( pVector->y );
	cp = (float)cos( pVector->y );
	sr = (float)sin( pVector->x );
	cr = (float)cos( pVector->x );

	matRet._11	= cp*cy;
	matRet._21 = cp*sy;
	matRet._31 = -sp;
	matRet._12 = sr*sp*cy+cr*-sy;
	matRet._22 = sr*sp*sy+cr*cy;
	matRet._32 = sr*cp;
	matRet._13 = (cr*sp*cy+-sr*-sy);
	matRet._23 = (cr*sp*sy+-sr*cy);
	matRet._33 = cr*cp;
	matRet._14 = 0.0;
	matRet._24 = 0.0;
	matRet._34 = 0.0;
	
	return matRet;
}*/
// ----------------------------------------------------------------------------
