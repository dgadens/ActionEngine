
#ifndef __ACGLOBALS_H
#define __ACGLOBALS_H

#include "ActionTypeDef.h"
#include <string>

const UINT NUM_TEXTURES = 8;
const UINT NUM_CACHES = 10;
const UINT MAX_VERTICES_IN_BUFFER = 16384;
const UINT MAX_INDICES_IN_BUFFER = 16384;

#ifdef WIN32
	#include <direct.h>    
	#define GetCurrentDir _getcwd
#else    
	#include <unistd.h>    
	#define GetCurrentDir getcwd 
#endif 

class ACGlobals
{
private:

	static const std::string ACGetCurrentDirectory();
	static std::string AC_CURRENT_DIRECTORY;
	static std::string AC_PATH_MODELS;
	static std::string AC_PATH_TEXTURES;
	static std::string AC_PATH_SHADERS;
	static std::string AC_PATH_SOUNDS;
	static std::string AC_PATH_FONTS;
	static std::string AC_PATH_SCRIPTS;

public:

	//mantem o id globals dos objetos
	static bool ENABLE_LOG;
	static UINT GlobalID;
	static bool EDITOR_MODE;
	static const std::string& GetCurrentDirectory();
	static const std::string& GetPathModels();
	static const std::string& GetPathTextures();
	static const std::string& GetPathShaders();
	static const std::string& GetPathSounds();
	static const std::string& GetPathFonts();
	static const std::string& GetPathScripts();
};

#endif