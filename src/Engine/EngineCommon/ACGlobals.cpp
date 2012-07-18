#include "ACGlobals.h"

std::string ACGlobals::AC_CURRENT_DIRECTORY;
std::string ACGlobals::AC_PATH_MODELS;
std::string ACGlobals::AC_PATH_SHADERS;
std::string ACGlobals::AC_PATH_SOUNDS;
std::string ACGlobals::AC_PATH_TEXTURES;
std::string ACGlobals::AC_PATH_FONTS;
UINT ACGlobals::GlobalID = 0;
bool ACGlobals::ENABLE_LOG = false;
bool ACGlobals::EDITOR_MODE = false;

std::string ACGlobals::ACGetCurrentDirectory()
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

std::string ACGlobals::GetCurrentDirectory()
{
	if (AC_CURRENT_DIRECTORY == "")
		AC_CURRENT_DIRECTORY = ACGetCurrentDirectory();

	return AC_CURRENT_DIRECTORY;
};

std::string ACGlobals::GetPathModels()
{
	if (AC_PATH_MODELS=="")
	{
		AC_PATH_MODELS = ACGetCurrentDirectory();
		AC_PATH_MODELS.append("\\Contents\\Models\\");
	}

	return AC_PATH_MODELS;
};

std::string ACGlobals::GetPathTextures()
{
	if (AC_PATH_TEXTURES=="")
	{
		AC_PATH_TEXTURES = ACGetCurrentDirectory();
		AC_PATH_TEXTURES.append("\\Contents\\Textures\\");
	}

	return AC_PATH_TEXTURES;
};

std::string ACGlobals::GetPathFonts()
{
	if (AC_PATH_FONTS=="")
	{
		AC_PATH_FONTS = ACGetCurrentDirectory();
		AC_PATH_FONTS.append("\\Contents\\Fonts\\");
	}

	return AC_PATH_FONTS;
};

std::string ACGlobals::GetPathShaders()
{
	if (AC_PATH_SHADERS=="")
	{
		AC_PATH_SHADERS = ACGetCurrentDirectory();
		AC_PATH_SHADERS.append("\\Contents\\Shaders\\");
	}

	return AC_PATH_SHADERS;
};

std::string ACGlobals::GetPathSounds()
{
	if (AC_PATH_SOUNDS=="")
	{
		AC_PATH_SOUNDS = ACGetCurrentDirectory();
		AC_PATH_SOUNDS.append("\\Contents\\Sounds\\");
	}

	return AC_PATH_SOUNDS;
};