//**********************************************
// Classe com os ids do som provenientes do device
// creator: Daniel Gadens
// date: 25/04/2012
//*********************************************

#ifndef __ACSOUND_H
#define __ACSOUND_H

#include "ActionTypeDef.h"
#include <string>

struct ACSound
{
	UINT            SoundID;
	UINT			Instance; //so vai deletar quando o numero de instancias for zero
	UINT            SourcesCoupled; //numero de sources acoplados (so pode remover quando o numero de sources acoplados for zero)
	std::string		Name;
};

#endif