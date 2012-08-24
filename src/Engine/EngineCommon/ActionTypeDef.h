//**************************************************************
// Definicao dos tipos utilizado na action engine
//**************************************************************

#ifndef __ACTIONDEF_H
#define __ACTIONDEF_H

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef char				CHAR;
typedef unsigned char       UCHAR;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef int                 INT;
typedef unsigned int        UINT;


// everything went smooth
#define AC_OK                        S_OK

// just reports no errors
#define AC_CANCELED                  0x82000000

// generell error message
#define AC_FAIL                      0x82000001

// specific error messages
#define AC_CREATEAPI                 0x82000002
#define AC_CREATEDEVICE              0x82000003
#define AC_CREATEBUFFER              0x82000004
#define AC_INVALIDPARAM              0x82000005
#define AC_INVALIDFORMAT			 0x82000006
#define AC_BUFFERSIZE                0x82000007
#define AC_BUFFERLOCK                0x82000008
#define AC_NOTCOMPATIBLE             0x82000009
#define AC_OUTOFMEMORY               0x8200000a
#define AC_FILENOTFOUND              0x8200000b
#define AC_INVALIDFILE               0x8200000c
#define AC_NOSHADERSUPPORT           0x8200000d


#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef NULL
#define NULL                0
#endif

#ifndef FLOAT_EPSILON
#define FLOAT_EPSILON       1.4013e-045f
#endif

#ifndef FLOAT_MAXVALUE
#define FLOAT_MAXVALUE      3.40282e+038f
#endif

#ifndef FLOAT_MINVALUE
#define FLOAT_MINVALUE     -3.40282e+038f
#endif

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

#ifndef DLLIMPORT
#define DLLIMPORT extern "C" __declspec(dllimport)
#endif

#endif