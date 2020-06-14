// =========== Header ===========
// File:                PMbedTLSInterface.cpp
// Project:             mbed TLS for NewtonOS
// Written by:          Paul Guyot (pguyot@kallisys.net)
//
// Created on:          03/06/2020
// Internal version:    1
//
// Copyright:           © 2020 by Paul Guyot.
//                      All rights reserved worldwide.
// ===========

// =========== Change History ===========
// 03/06/2020   v1  [PG]    Creation of the file
// ===========

#include "PMbedTLSInterface.h"

// NewtonOS
#include <NSandDDKIncludes.h>
#include <NewtonScript.h>
#include <NameServer.h>
#include <NewtonExceptions.h>
#include <OSErrors.h>
#include <NewtonErrors.h>

// Constantes pour l'interface.
#define kNameRegisterFirstString "mbedTLS"
#define kNameRegisterSecondString "Kallisys"
#define kVersion    0

NewDefineException(exMbedTLSException,exRootException,evt.ex.mbedTLSException);

// ------------------------------------------------------------------------ //
// * GetInterfaceSingleton(bool)
// ------------------------------------------------------------------------ //
PMbedTLSInterface*
PMbedTLSInterface::GetInterfaceSingleton(bool create)
{
    TUNameServer NameServer;
    ULong theVersion;
    PMbedTLSInterface* theResult = nil;
    NewtonErr theErr = NameServer.Lookup( kNameRegisterFirstString, kNameRegisterSecondString, (ULong*) &theResult, &theVersion );
    if (theErr == kError_Not_Registered) {
        if (create) {
            theResult = New("PMbedTLS");
            if (theResult == nil)
            {
                OutOfMemory();
            }
            theErr = NameServer.RegisterName( kNameRegisterFirstString, kNameRegisterSecondString, (ULong) theResult, kVersion);
        } else {
            theErr = noErr;
        }
    }
    if (theErr != noErr)
    {
        throw2( exMbedTLSException, theErr );
    }
    return theResult;
}

// ------------------------------------------------------------------------ //
// * DeleteInterfaceSingleton()
// ------------------------------------------------------------------------ //
void
PMbedTLSInterface::DeleteInterfaceSingleton()
{
    PMbedTLSInterface* interface = GetInterfaceSingleton(false);
    if (interface) {
        interface->Delete();

        TUNameServer NameServer;
        (void) NameServer.UnRegisterName( kNameRegisterFirstString, kNameRegisterSecondString );
    }
}

// =============================================================== //
// You give me space to belong to myself yet without separating me //
// from your own life.  May it all turn out to your happiness.     //
//         -- Goethe                                               //
// =============================================================== //
