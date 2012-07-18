//*********************************
// Classe q armazena ponteiros de funcoes pra executar em sequencia
// Creator: Daniel Gadens
// Date: 10/07/2012
//*******************************

#ifndef __EVENT_H
#define __EVENT_H

#include <list>

struct EventArgs
{
	int Value;
};

//declaracao do formato do delegate
typedef void (*fPtr)(void* sender, EventArgs& e);

class Event
{
protected:

	std::list<fPtr> methods;

public:
	Event();
	~Event();

	void AddMethod(fPtr);
	void RemoveMethod(fPtr);
	void Execute();
};

inline void Event::AddMethod(fPtr func)
{
	methods.push_back(func);
};

inline void Event::RemoveMethod(fPtr func)
{
	methods.remove(func);
};

inline void Event::Execute()
{
	//invoca os methodos
	for (auto it = methods.begin(); it != methods.end(); it++)
		(*it)("this", EventArgs());
};

#endif