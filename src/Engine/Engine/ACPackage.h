//************************************
// Interface para o dispositivo de network
// Creator: Daniel Gadens
// Date: 04/08/2011
//************************************

#ifndef __ACPACKAGE_H
#define __ACPACKAGE_H

#include "EngineCommon.h"

/**
 * nType reserved:
 *  0 - server message to new client giving its ID, pData = UINT nID
 *  1 - new client connected, pData = UINT nID
 *  2 - client disconnected,  pData = UINT nID
 */
typedef struct ACPACKAGE_TYPE 
{
   UINT   nLength;    // tamanho do pdata em bytes
   UCHAR  nType;      // usado para especificar o tipo (0 é o ID de notificacao)
   UINT   nSender;    // colocar os ids dos senders aqui (0 é o server)
   void  *pData;      // dados
} ACPACKAGE;

#endif