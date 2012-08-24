#ifndef __GUTILS_H
#define __GUTILS_H

#include <windows.h>
#include <conio.h>
#include <iostream>

//esse static faz a variavel ser vista apenas dentro desse arquivo :)
static bool consoleVisible = false;

class GUtils
{

public:
	
	static void ShowConsole()
	{
		if (!consoleVisible)
		{
			AllocConsole();
			freopen("conin$","r",stdin);
			freopen("conout$","w",stdout);
			freopen("conout$","w",stderr);
			printf("Debugging Window:\n");

			consoleVisible = true;
		}
	};

	static void HideConsole()
	{
		consoleVisible = false;
		FreeConsole();
	};
};

#endif