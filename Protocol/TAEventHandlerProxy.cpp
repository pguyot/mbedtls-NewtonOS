// ==============================
// Fichier:         TAEventHandlerProxy.cp
// Projets:         ATA Support
//                  Newton Bowels - Events
// Ecrit par:       Paul Guyot (pguyot@kallisys.net)
//
// Créé le:         27/10/2003
// Tabulation:      4 espaces
//
// Copyright (c) 2003, 2020 by Paul Guyot.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// * Redistributions of source code must retain the above copyright
//   notice and the following disclaimer.
// * Redistributions in binary form must reproduce the above
//   copyright notice and the following disclaimer in the
//   documentation and/or other materials provided with the
//   distribution.
// * Neither the name of Kallisys nor the names of its contributors
//   may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
// HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ===========
// $Id$
// ===========

#include "TAEventHandlerProxy.h"
#include <NewtonGestalt.h>
#include <new.h>

// ------------------------------------------------------------------------ //
//  * TAEventHandlerProxy( void )
// ------------------------------------------------------------------------ //
TAEventHandlerProxy::TAEventHandlerProxy( void )
{
    // Let's determine the version of the OS here.
    TGestaltSystemInfo theInfos;

    TUGestalt theGestaltObject;
    // What would I do with the error anyway?
    (void) theGestaltObject.Gestalt(
                        kGestalt_SystemInfo, &theInfos, sizeof( theInfos ) );

    // Create the handler with, at the end, a pointer to ourself.
    // We don't want to subclass the event handler because runtime differ.
    mEventHandler = (SAEHandlerWPtrToProxy*)
                            ::malloc( sizeof (SAEHandlerWPtrToProxy) );

    // Call the TAEventHandler constructor in place.
    new (&mEventHandler->fEH.fHandler) TAEventHandler();

    // I also set the pointer to ourselves here.
    mEventHandler->fProxyPtr = this;

    if (theInfos.fROMVersion < 0x20002)
    {
        // NewtonOS 2.0

        // Fill in the virtual table for NewtonOS 2.0 runtime architecture.
        mVirtualTable20.fLink = 0;
        mVirtualTable20.fDestructor = __AEDestructor;
            // I keep the destructor, even if this is never called (it could
            // only be called by NewtonOS 2.0 code, and the system is not going
            // to delete the object in our back).
        mVirtualTable20.fAETestEvent = __AETestEvent;
        mVirtualTable20.fAEHandlerProc = __AEHandlerProc;
        mVirtualTable20.fAECompletionProc = __AECompletionProc;
        mVirtualTable20.fIdleProc = __IdleProc;

        // Finally, I save the table for the object.
        mEventHandler->fEH.fHandler20.fVirtualTable = &mVirtualTable20;

        // I also need to save pointers to the actual event class and ID
        // values. That way, if ever the system changes the values (like
        // Init method is called), I'll return the updated values from
        // the GetEventClass and GetEventID methods. I cannot call
        // the inline GetEventClass and GetEventID methods because these
        // are based on NewtonOS 2.0 runtime architecture (with the
        // virtual table at the end instead of being at the beginning).
        mEventClassPtr = &(mEventHandler->fEH.fHandler20.fEventClass);
        mEventIDPtr = &(mEventHandler->fEH.fHandler20.fEventID);
    } else {
        // NewtonOS 2.1

        // Set the virtual table
        mEventHandler->fEH.fHandler21.fVirtualTable = &kVirtualTable21;

        // And I save the pointers to the actual variables of the object.
        mEventClassPtr = &(mEventHandler->fEH.fHandler21.fEventClass);
        mEventIDPtr = &(mEventHandler->fEH.fHandler21.fEventID);
    }
}

// ------------------------------------------------------------------------ //
//  * ~TAEventHandlerProxy( void )
// ------------------------------------------------------------------------ //
TAEventHandlerProxy::~TAEventHandlerProxy( void )
{
    // I directly call the destructor.
    // This line won't go through the virtual table. Which is good because
    // the compiler's runtime doesn't match NewtonOS 2.0 and NewtonOS 2.1
    // runtimes.
    if (mEventHandler) {
        mEventHandler->fEH.fHandler.TAEventHandler::~TAEventHandler();

        // And I free the object I allocated with malloc.
        ::free( mEventHandler );
    }
}

// ------------------------------------------------------------------------ //
//  * AETestEvent( TAEvent* )
// ------------------------------------------------------------------------ //
Boolean
TAEventHandlerProxy::AETestEvent( TAEvent* /* inEvent */ )
{
    // Default behavior is to return true. I could actually
    // call the system method here but it just returns true as well.
    return true;
}

// ------------------------------------------------------------------------ //
//  * AEHandlerProc( TUMsgToken*, ULong*, TAEvent*)
// ------------------------------------------------------------------------ //
void
TAEventHandlerProxy::AEHandlerProc(
            TUMsgToken* /* inToken */,
            ULong* /* inSize */,
            TAEvent* /* inEvent */ )
{
    // This place for rent.
}

// ------------------------------------------------------------------------ //
//  * AECompletionProc( TUMsgToken*, ULong*, TAEvent*)
// ------------------------------------------------------------------------ //
void
TAEventHandlerProxy::AECompletionProc(
            TUMsgToken* /* inToken */,
            ULong* /* inSize */,
            TAEvent* /* inEvent */ )
{
    // Kilroy was here.
}

// ------------------------------------------------------------------------ //
//  * IdleProc( TUMsgToken*, ULong*, TAEvent*)
// ------------------------------------------------------------------------ //
void
TAEventHandlerProxy::IdleProc(
            TUMsgToken* /* inToken */,
            ULong* /* inSize */,
            TAEvent* /* inEvent */ )
{
    // Défense d'afficher.
}

// ------------------------------------------------------------------------ //
//  * __AEDestructor( SAEHandlerWPtrToProxy* )
// ------------------------------------------------------------------------ //
void
TAEventHandlerProxy::__AEDestructor( SAEHandlerWPtrToProxy* inThis )
{
    // Make sure this is linked with D1 code, i.e. non-delete destructor
    inThis->fProxyPtr->mEventHandler->fEH.fHandler.TAEventHandler::~TAEventHandler();
    ::free( inThis->fProxyPtr->mEventHandler );
    inThis->fProxyPtr->mEventHandler = NULL;
}

// ------------------------------------------------------------------------ //
//  * __AETestEvent( SAEHandlerWPtrToProxy*, TAEvent* )
// ------------------------------------------------------------------------ //
Boolean
TAEventHandlerProxy::__AETestEvent(
            SAEHandlerWPtrToProxy* inThis,
            TAEvent* inEvent )
{
    // Just call the proxy (virtual) method.
    return inThis->fProxyPtr->AETestEvent( inEvent );
}

// ------------------------------------------------------------------------ //
//  * __AEHandlerProc( TUMsgToken*, ULong*, TAEvent*)
// ------------------------------------------------------------------------ //
void
TAEventHandlerProxy::__AEHandlerProc(
            SAEHandlerWPtrToProxy* inThis,
            TUMsgToken* inToken,
            ULong* inSize,
            TAEvent* inEvent )
{
    // Just call the proxy (virtual) method.
    inThis->fProxyPtr->AEHandlerProc( inToken, inSize, inEvent );
}

// ------------------------------------------------------------------------ //
//  * __AECompletionProc( TUMsgToken*, ULong*, TAEvent*)
// ------------------------------------------------------------------------ //
void
TAEventHandlerProxy::__AECompletionProc(
            SAEHandlerWPtrToProxy* inThis,
            TUMsgToken* inToken,
            ULong* inSize,
            TAEvent* inEvent )
{
    // Just call the proxy (virtual) method (you guessed this time).
    inThis->fProxyPtr->AECompletionProc( inToken, inSize, inEvent );
}

// ------------------------------------------------------------------------ //
//  * __IdleProc( TUMsgToken*, ULong*, TAEvent*)
// ------------------------------------------------------------------------ //
void
TAEventHandlerProxy::__IdleProc(
            SAEHandlerWPtrToProxy* inThis,
            TUMsgToken* inToken,
            ULong* inSize,
            TAEvent* inEvent )
{
    // Just .... the ..... (virtual) method
    // [Fill the blanks]
    inThis->fProxyPtr->IdleProc( inToken, inSize, inEvent );
}

// ==================================================================== //
// MESSAGE ACKNOWLEDGED -- The Pershing II missiles have been launched. //
// ==================================================================== //
