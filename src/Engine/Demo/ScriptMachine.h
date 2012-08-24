//**********************
// Classe q controla os scripts versao 1.0
// Creator: Daniel Gadens
// Date: 23/08/2012
//*********************

#ifndef __SCRIPTMANAGER_H
#define __SCRIPTMANAGER_H

#include <lua.hpp>
#include <string>

#include "GUtils.h"

class ScriptMachine
{
	lua_State* lua;

public:
	ScriptMachine();
	~ScriptMachine();

	void ExecuteScript(const std::string& name);

	void Initialize();
	void Release();
};

#endif