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
	AllocConsole();
	freopen("conin$","r",stdin);
	freopen("conout$","w",stdout);
	freopen("conout$","w",stderr);
	printf("Debugging Window:\n");

	std::cout << "teste" << std::endl;
	luaL_dostring(lua, "print(\"Teste lua\")");

	FreeConsole();
};
