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
	luaL_dostring(lua, "print(\"Teste lua\")");
	GUtils::HideConsole();
};
