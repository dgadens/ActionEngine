
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

	static ACConfigurations* mpInstance;

	ACConfigurations();
	~ACConfigurations();

	const std::string ACGetCurrentDirectory();
	std::string AC_CURRENT_DIRECTORY;
	std::string AC_PATH_MODELS;
	std::string AC_PATH_TEXTURES;
	std::string AC_PATH_SHADERS;
	std::string AC_PATH_SOUNDS;
	std::string AC_PATH_FONTS;
	std::string AC_PATH_SCRIPTS;

	UINT MAX_VERTICES_IN_BUFFER;
	UINT MAX_INDICES_IN_BUFFER;


public:

	//mantem o id globals dos objetos
	static ACConfigurations* Instance();

	void Initialize();

	bool ENABLE_LOG;
	UINT GlobalID;
	bool EDITOR_MODE;
	const std::string& GetCurrentDirectory();
	const std::string& GetPathModels();
	const std::string& GetPathTextures();
	const std::string& GetPathShaders();
	const std::string& GetPathSounds();
	const std::string& GetPathFonts();
	const std::string& GetPathScripts();

	const UINT GetMaxVerticesInBuffer();
	const UINT GetMaxIndicesInBuffer();
};

#endif