#include "ACConfigurations.h"

std::string ACConfigurations::AC_CURRENT_DIRECTORY;
std::string ACConfigurations::AC_PATH_MODELS;
std::string ACConfigurations::AC_PATH_SHADERS;
std::string ACConfigurations::AC_PATH_SOUNDS;
std::string ACConfigurations::AC_PATH_TEXTURES;
std::string ACConfigurations::AC_PATH_FONTS;
std::string ACConfigurations::AC_PATH_SCRIPTS;

UINT ACConfigurations::MAX_VERTICES_IN_BUFFER;
UINT ACConfigurations::MAX_INDICES_IN_BUFFER;
		 
UINT ACConfigurations::GlobalID = 0;
bool ACConfigurations::ENABLE_LOG = false;
bool ACConfigurations::EDITOR_MODE = false;

void ACConfigurations::Initialize() 
{
    std::ifstream fin("ConfigurationFile.ini");
    std::string line;
    while (getline(fin, line)) {
        std::istringstream sin(line.substr(line.find("=") + 1));
        if (line.find("ContentPath") != -1)
            sin >> AC_CURRENT_DIRECTORY;
        if (line.find("EnableLOG") != -1)
            sin >> ENABLE_LOG;
        if (line.find("MaxVerticesInBuffer") != -1)
            sin >> MAX_VERTICES_IN_BUFFER;
        if (line.find("MaxIndicesInBuffer") != -1)
            sin >> MAX_INDICES_IN_BUFFER;
    }

	fin.close();
}

const std::string ACConfigurations::ACGetCurrentDirectory()
{
	char currentdirectory[FILENAME_MAX];
	if (!GetCurrentDir(currentdirectory, sizeof(currentdirectory)))  
	{
		return nullptr;     
	}
	
	currentdirectory[sizeof(currentdirectory) - 1] = '/0';


	std::string ret = currentdirectory;  


	return ret;
};

const std::string& ACConfigurations::GetCurrentDirectory()
{
	if (AC_CURRENT_DIRECTORY == "")
		AC_CURRENT_DIRECTORY = ACGetCurrentDirectory();

	return AC_CURRENT_DIRECTORY;
};

const std::string& ACConfigurations::GetPathModels()
{
	if (AC_PATH_MODELS=="")
	{
		AC_PATH_MODELS = GetCurrentDirectory();
		AC_PATH_MODELS.append("\\Models\\");
	}

	return AC_PATH_MODELS;
};

const std::string& ACConfigurations::GetPathTextures()
{
	if (AC_PATH_TEXTURES=="")
	{
		AC_PATH_TEXTURES = GetCurrentDirectory();
		AC_PATH_TEXTURES.append("\\Textures\\");
	}

	return AC_PATH_TEXTURES;
};

const std::string& ACConfigurations::GetPathFonts()
{
	if (AC_PATH_FONTS=="")
	{
		AC_PATH_FONTS = GetCurrentDirectory();
		AC_PATH_FONTS.append("\\Fonts\\");
	}

	return AC_PATH_FONTS;
};

const std::string& ACConfigurations::GetPathShaders()
{
	if (AC_PATH_SHADERS=="")
	{
		AC_PATH_SHADERS = GetCurrentDirectory();
		AC_PATH_SHADERS.append("\\Shaders\\");
	}

	return AC_PATH_SHADERS;
};

const std::string& ACConfigurations::GetPathSounds()
{
	if (AC_PATH_SOUNDS=="")
	{
		AC_PATH_SOUNDS = GetCurrentDirectory();
		AC_PATH_SOUNDS.append("\\Sounds\\");
	}

	return AC_PATH_SOUNDS;
};

const std::string& ACConfigurations::GetPathScripts()
{
	if (AC_PATH_SCRIPTS=="")
	{
		AC_PATH_SCRIPTS = GetCurrentDirectory();
		AC_PATH_SCRIPTS.append("\\Scripts\\");
	}

	return AC_PATH_SCRIPTS;
};

const UINT ACConfigurations::GetMaxVerticesInBuffer()
{
	return MAX_VERTICES_IN_BUFFER;
};

const UINT ACConfigurations::GetMaxIndicesInBuffer()
{
	return MAX_INDICES_IN_BUFFER;
};