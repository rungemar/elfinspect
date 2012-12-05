/*****************************************************************************/
/**
 @file         libmain.cpp
 @copyright    (c) Rohde & Schwarz GmbH & Co. KG, Munich
 @version      $Header:  $
 *
 @author       $Author: $(Author) $
 @responsible
 *
 @language     C++
 *
 @description
 @see
 *
 @history
 * $Log: libmain.cpp $
 *
 *
 *******************************************************************************
 ******************************************************************************/

#include <iostream>

#ifdef WIN32
#  include <windows.h>
#endif

using namespace std;

void libelfinspect_ctor(void);
void libelfinspect_dtor(void);

#ifdef WIN32

BOOL WINAPI DllMain( HINSTANCE hinstDll,
        DWORD fwdReason,
        LPVOID lpvReserved
)
{
    switch ( fwdReason )
    {
        case DLL_PROCESS_ATTACH:
        libelfinspect_ctor();
        break;
        case DLL_PROCESS_DETACH:
        libelfinspect_dtor();
        break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        default:
        break;
    }

    return TRUE;
} //lint !e715 !e818 : hinstDall, fwdReason and lpvReserved necessary to satisfy interface

#else

void __attribute__ ((constructor)) libelfinspect_ctor(void);
void __attribute__ ((destructor)) libelfinspect_dtor(void);

#endif

void libelfinspect_ctor(void)
{
    cout << "libelfinspect_ctor: libelfinspect loaded." << endl;
}

void libelfinspect_dtor(void)
{
    cout << "libelfinspect_dtor: libelfinspect unloaded." << endl;
}

