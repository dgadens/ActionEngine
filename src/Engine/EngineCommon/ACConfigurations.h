
#ifndef __ACCONFIGURATIONS_H
#define __ACCONFIGURATIONS_H

#include "ActionTypeDef.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef WIN32
	#include <direct.h>    
	#define GetCurrentDir _getcwd
#else    
	#include <unistd.h>    
	#define GetCurrentDir getcwd 
#endif 

class ACConfigurations
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

	static UINT MAX_VERTICES_IN_BUFFER;
	static UINT MAX_INDICES_IN_BUFFER;


public:

	//mantem o id globals dos objetos
	static void Initialize();
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

	static const UINT GetMaxVerticesInBuffer();
	static const UINT GetMaxIndicesInBuffer();
};

#endif