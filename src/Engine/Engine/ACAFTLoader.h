//*********************************
// Classe que importa os dados da fonte
// Creator: Daniel Gadens
// Date: 05/07/2012
// **********************************


#ifndef __ACAFTLOADER_H 
#define __ACAFTLOADER_H 

#include <stdio.h>
#include <string>
#include "EngineMath.h"
#include "ACAMTStructs.h"  
#include "ACFont.h"

class ACAFTLoader
{
private:
   static FILE*		mpFile;
public:
   static bool		LoadFromFile( const char* path, ACFont* outFont);

protected:
   static bool      CloseFile( bool rBool );								
};
#endif