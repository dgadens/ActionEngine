//*********************************
// Classe que importa os dados do arquivo cbf
// Creator: Daniel Gadens
// Date: 07/06/2011
// **********************************


#ifndef __ACAMTLOADER_H 
#define __ACAMTLOADER_H 

#include <stdio.h>
#include <string>
#include "EngineMath.h"
#include "ACAMTStructs.h"   

class ACAMTLoader
{
private:
   static FILE*		mpFile;
public:
   static bool		LoadFromFile( const char* path, AMT_MODEL* outModel);

protected:
   static bool      CloseFile( bool rBool );								
   static HRESULT   ReadHeader( AMT_MODEL* outModel );                  
   static HRESULT   ReadVertices( AMT_MODEL* outModel );                
   static HRESULT   ReadFaces( AMT_MODEL* outModel );                   
   static HRESULT   ReadMesh( AMT_MODEL* outModel );                    
   static HRESULT   ReadMaterials( AMT_MODEL* outModel );               
   static HRESULT   ReadJoints( AMT_MODEL* outModel );   
   static HRESULT   ReadAnimations( AMT_MODEL* outModel );   
};
#endif