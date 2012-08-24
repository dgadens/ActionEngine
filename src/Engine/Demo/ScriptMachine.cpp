#include "ScriptMachine.h"

ScriptMachine::ScriptMachine()
{
	lua = nullptr;
};

ScriptMachine::~ScriptMachine()
{
	Release();
};

void ScriptMachine::Initialize()
{
	//cria o estado da lua
	lua = lua_open();
	luaL_openlibs(lua);
};

void ScriptMachine::Release()
{
	lua_close(lua);
};

void ScriptMachine::ExecuteScript(const std::string& name)
{
	GUtils::ShowConsole();
	
	//FirstExample();

	//SecondExample();

	ThirdExample();

	GUtils::HideConsole();
};

//exemplo q mostra como carregar o arquivo e as variaveis globais
void ScriptMachine::FirstExample()
{
	std::string path = ACGlobals::GetPathScripts();
	path.append("FirstExample.lua");

	//executo o arquivo
	luaL_dofile(lua, path.c_str() );

	//pego a variavel e coloca ela na pilha, 
	//-1 é o ultimo, -2 o penultimo ...
	// 1 é o primeiro 2 o segundo ..
	lua_getglobal(lua, "name"); //empilha o nome
	lua_getglobal(lua, "x"); //empilha o x
	lua_getglobal(lua, "y"); //empilha o y

	int x = 0;
	float y = 0;
	std::string str = "";

	bool error =false;

	//pego o ultimo da pilha
	if (lua_isstring(lua, -3))
		str = lua_tostring(lua, -3);
	else
		error = true;

	if (lua_isnumber(lua, -2))
		x = lua_tonumber(lua, -2);
	else
		error = true;

	if (lua_isnumber(lua, -1))
		y = lua_tonumber(lua, -1);
	else
		error = true;

	if (error)
		std::cout << "Alguma variavel nao é do tipo correto ou nao existe.";
	else
		std::cout <<  "X: " << x << std::endl << "Y: " << y << std::endl << "Str: " << str;

};	

//exemplo q mostra como executar um metodo c++ no lua e vice-versa
void ScriptMachine::SecondExample()
{
	std::string path = ACGlobals::GetPathScripts();
	path.append("SecondExample.lua");

	//registro o metodo no lua e no c++, tem q ser feito antes do dofile pq o dofile executa tudo q tem no arquivo
	//lua_register(lua, "SetVariables", ScriptMachine::SetVariables);
	lua_register(lua, "PrintNumber", ScriptMachine::PrintNumber);

	//executo o arquivo
	luaL_dofile(lua, path.c_str() );
};

void ScriptMachine::ThirdExample()
{
	std::string path = ACGlobals::GetPathScripts();
	path.append("ThirdExample.lua");

	//executo o arquivo
	luaL_dofile(lua, path.c_str() );

	//empilho o nome do metodo 
	lua_getglobal(lua, "Update");

	if (lua_isfunction(lua, -1))
	{
		//chama o metodo passando o numero de parametros e o numero de retornos
		lua_call(lua, 0, 0);
	}
};

int ScriptMachine::SetVariables(lua_State* L)
{
	//vou alterar esses valores passando como parametro no lua
	int x = 200;
	float y = 300;
	std::string Name = "ABC";

	//pega o primeiro parametro
	if (lua_isnumber(L, -3))
		x = lua_tonumber(L, -3);

	if (lua_isnumber(L, -2))
		y = lua_tonumber(L, -2);

	if (lua_isstring(L, -1))
		Name = lua_tostring(L, -1);

	std::cout <<  "X: " << x << std::endl << "Y: " << y << std::endl << "Str: " << Name;

	//nao retorna nada do metodo lua entao retorna zero
	return 0;
};

int ScriptMachine::PrintNumber(lua_State* L)
{
	lua_pushnumber(L, 10);

	//retorna o numero de valores retornados pelo lua no caso 1
	return 1;
};