//**********************
// Classe q controla os scripts versao 1.0
// Creator: Daniel Gadens
// Date: 23/08/2012
//*********************

#ifndef __SCRIPTMANAGER_H
#define __SCRIPTMANAGER_H

#include <lua.hpp>
#include <string.h>

#include "GUtils.h"
#include "EngineCommon.h"

class ScriptMachine
{
	lua_State* lua;

	//exemplo q mostra como carregar o arquivo e as variaveis globais
	void FirstExample();
	//exemplo q mostra como executar um metodo c++ no lua
	void SecondExample();
	//exemplo de como chamar uma funcao lua
	void ThirdExample();

public:
	ScriptMachine();
	~ScriptMachine();

	void ExecuteScript(const std::string& name);

	void Initialize();
	void Release();


	//metodo invocado pelo lua para o exemplo 2
	static int SetVariables(lua_State* L);
	static int PrintNumber(lua_State* L);
};

#endif