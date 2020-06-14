// ==============================
// Fichier:         TAEventHandlerProxy.h
// Projets:         ATA Support
//                  Newton Bowels - Events
// Ecrit par:       Paul Guyot (pguyot@kallisys.net)
//
// Créé le:         27/10/2003
// Tabulation:      4 espaces
//
// Copyright (c) 2003, 2020, Paul Guyot.
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

#ifndef _TAEVENTHANDLERPROXY_H
#define _TAEVENTHANDLERPROXY_H

#include "NSandDDKIncludes.h"
#include <AEventHandler.h>

///
/// Proxy for an AEvent handler that will work on both NewtonOS 2.0 and
/// NewtonOS 2.1.
/// Ported for gcc runtime, i.e. does not assume the compiler uses the
/// runtime of NewtonOS 2.1.
/// It's a kind of dirty hack, but it's the cleanest I came with.
///
/// \author Paul Guyot <pguyot@kallisys.net>
/// \version 1.0
///
class TAEventHandlerProxy
{
public:
    ///
    /// Default constructor.
    ///
    TAEventHandlerProxy( void );

    ///
    /// Destructor.
    /// Frees up memory.
    ///
    virtual ~TAEventHandlerProxy( void );

    ///
    /// Accessor to the actual handler. This can be useful for some system calls.
    /// Actually, I use this when building a TUASyncMessage myself, putting the
    /// handler in the refcon of the event so I get called when the event has
    /// been delivered (through AECompletionProc). I don't recall where I learnt
    /// about this magic but it happens to work.
    ///
    /// \return the actual handler.
    ///
    TAEventHandler*     GetHandler( void )
        {
            return &mEventHandler->fEH.fHandler;
        }

    /// \name Interface TAEventHandler.

    ///
    /// Determine if an event should be passed to this handler.
    /// By default, returns \c true.
    /// Please note that you only get called for the events of the class & ID
    /// you were registered for (by calling Init) and for which this method
    /// didn't return \c false.
    ///
    /// \param inEvent  event to consider
    /// \return \c true if this event should be passed to the handler.
    ///
    virtual Boolean AETestEvent( TAEvent* inEvent );

    ///
    /// Method called when an event this handler is registered for
    /// is sent to the NewtWorld task.
    ///
    /// By default doesn't do anything.
    ///
    /// \param inToken  token for the event.
    /// \param inSize   size of the event (?) or of the reply (??)
    /// \param inEvent  event sent to the NewtWorld task.
    ///
    virtual void    AEHandlerProc(
                            TUMsgToken* inToken,
                            ULong* inSize,
                            TAEvent* inEvent);

    ///
    /// Method sent when an event was sent and processed. I guess.
    ///
    /// By default doesn't do anything.
    ///
    /// \param inToken  token for the event.
    /// \param inSize   size of the event (?) or of the reply (??)
    /// \param inEvent  event sent to the NewtWorld task.
    ///
    virtual void    AECompletionProc(
                            TUMsgToken* inToken,
                            ULong* inSize,
                            TAEvent* inEvent );

    ///
    /// Method sent when an idler event was received. This method,
    /// combined with InitIdler, makes EventHandlers perfect idlers
    /// within the NewtWorld task.
    ///
    /// By default doesn't do anything.
    ///
    /// \param inToken  token for the event.
    /// \param inSize   size of the event (?) or of the reply (??)
    /// \param inEvent  event sent to the NewtWorld task.
    ///
    virtual void    IdleProc(
                            TUMsgToken* token,
                            ULong* size,
                            TAEvent* event);

    /// \name Proxy to TAEventHandler methods.

    ///
    /// Register this handler for a given ID and class of events.
    /// All lower-case events are probably reserved for Apple.
    /// I don't think you can call this method several times, although
    /// there are some hints in that direction in comments related
    /// to the SystemEventHandler.
    ///
    /// \param inEventID    ID for the events you want to receive
    /// \param inEventClass Class for the events you want to receive
    /// \return an error code if a problem occurred.
    ///
    NewtonErr   Init(
                    AEEventID inEventID,
                    AEEventClass inEventClass = kNewtEventClass )
        {
            return mEventHandler->fEH.fHandler.Init( inEventID, inEventClass );
        }

    ///
    /// Accessor to the event class.
    ///
    /// \return the event class for this handler.
    ///
    AEEventClass    GetEventClass( void )
        {
            return *mEventClassPtr;
        }

    ///
    /// Accessor to the event ID.
    ///
    /// \return the event ID for this handler.
    ///
    AEEventID   GetEventID( void )
        {
            return *mEventIDPtr;
        }

    void        DeferReply( void )
        { mEventHandler->fEH.fHandler.DeferReply(); }
    NewtonErr   ReplyImmed( void )
        { return mEventHandler->fEH.fHandler.ReplyImmed(); }
    void        SetReply( ULong size, TAEvent* event )
        { mEventHandler->fEH.fHandler.SetReply( size, event ); }
    void        SetReply( TUMsgToken* token )
        { mEventHandler->fEH.fHandler.SetReply( token ); }
    void        SetReply( TUMsgToken* token, ULong size, TAEvent* event )
        { mEventHandler->fEH.fHandler.SetReply( token, size, event ); }

    ///
    /// Initialize the idler.
    ///
    /// \param inIdle   timer for the idling
    /// \param inRefCon cookie (no idea where this comes up)
    /// \param inStart  if the idler should be started after the method is called.
    ///
    /// \return an error code if a problem occurred.
    ///
    NewtonErr   InitIdler(
                    TTimeout inIdle,
                    ULong inRefCon = 0,
                    Boolean inStart = true)
        {
            return mEventHandler->fEH.fHandler.InitIdler( inIdle, inRefCon, inStart );
        }

    ///
    /// Initialize the idler (with time units)
    ///
    /// \param inIdleAmount amount of units for the idle period.
    /// \param inIdleUnits  units in which the idle period is expressed.
    /// \param inRefCon     cookie (no idea where this comes up)
    /// \param inStart      if the idler should be started after the method is called.
    ///
    /// \return an error code if a problem occurred.
    ///
    NewtonErr   InitIdler(
                    ULong inIdleAmount,
                    TimeUnits inIdleUnits,
                    ULong inRefCon = 0,
                    Boolean inStart = true)
        {
            return mEventHandler->fEH.fHandler.InitIdler( inIdleAmount, inIdleUnits, inRefCon, inStart );
        }

    ///
    /// Starts the idler
    ///
    /// \return an error code if a problem occurred.
    ///
    NewtonErr   StartIdle( void )
        {
            return mEventHandler->fEH.fHandler.StartIdle();
        }

    ///
    /// Stops the idler
    ///
    /// \return an error code if a problem occurred.
    ///
    NewtonErr   StopIdle( void )
        {
            return mEventHandler->fEH.fHandler.StopIdle();
        }

    ///
    /// Resets the idler, i.e. stops it if required and restart it.
    ///
    /// \return an error code if a problem occurred.
    ///
    NewtonErr   ResetIdle( void )
        {
            return mEventHandler->fEH.fHandler.ResetIdle();
        }

    ///
    /// Resets the idler with a new value, i.e. stops it if required
    /// change the idle period and restart it (with time units)
    ///
    /// \param inIdleAmount amount of units for the idle period.
    /// \param inIdleUnits  units in which the idle period is expressed.
    /// \return an error code if a problem occurred.
    ///
    NewtonErr   ResetIdle( ULong inIdleAmount, TimeUnits inIdleUnits )
        {
            return mEventHandler->fEH.fHandler.ResetIdle( inIdleAmount, inIdleUnits );
        }

    ///
    /// Resets the idler with a new value, i.e. stops it if required
    /// change the idle period and restart it.
    ///
    /// \param inIdle       idle period.
    /// \return an error code if a problem occurred.
    ///
    NewtonErr   ResetIdle( TTimeout inIdle )
        {
            return mEventHandler->fEH.fHandler.ResetIdle( inIdle );
        }

private:

    /// Pre-declaration.
    struct SAEHandlerWPtrToProxy;

    /// Type for the destructor (D1, i.e. we don't call delete)
    typedef void (*AEDestructorPtr)( SAEHandlerWPtrToProxy* );

    /// Type for the TestEvent proxy callback method
    typedef Boolean (*AETestEventProcPtr)( SAEHandlerWPtrToProxy*, TAEvent* );

    /// Type for the other proxy callback methods
    typedef void (*AEUserRoutineProcPtr)( SAEHandlerWPtrToProxy*, TUMsgToken*, ULong*, TAEvent* );

    /// Structure (on NewtonOS 2.0) of the TAEventHandler virtual table.
    struct SAEHandlerOS20VTable
    {
        ULong                   fLink;              ///< I guess this is some link with
                                                    ///< derived class.
        AEDestructorPtr         fDestructor;        ///< Pointer to the destructor code.
        AETestEventProcPtr      fAETestEvent;       ///< Pointer to AETestEvent
        AEUserRoutineProcPtr    fAEHandlerProc;     ///< Pointer to AEHandlerProc
        AEUserRoutineProcPtr    fAECompletionProc;  ///< Pointer to AECompletionEvent
        AEUserRoutineProcPtr    fIdleProc;          ///< Pointer to AEIdleProc
    };

    /// Structure (on NewtonOS 2.1) of the TAEventHandler virtual table.
    struct SAEHandlerOS21VTable
    {
        ULong           fJumpToDestructor;          ///< Branch instruction to the destructor
        ULong           fJumpToAETestEvent;         ///< Branch instruction to AETestEvent
        ULong           fJumpToAEHandlerProc;       ///< Branch instruction to AEHandlerProc
        ULong           fJumpToAECompletionProc;    ///< Branch instruction to AECompletionProc
        ULong           fJumpToIdleProc;            ///< Branch instruction to AEIdleProc
    };

    /// Structure (on NewtonOS 2.0) of TAEventHandler with the pointer to
    /// the virtual table.
    struct SAEventHandlerOnOS20
    {
        TAEventHandler*             fNext;          ///< Pointer to the next handler in the chain
        AEEventClass                fEventClass;    ///< Class for the events
        AEEventID                   fEventID;       ///< ID for the events
        TAEIdleTimer*               fIdler;         ///< Pointer to the idler object
        const SAEHandlerOS20VTable* fVirtualTable;  ///< Pointer to the virtual table.
    };

    /// Structure (on NewtonOS 2.1) of TAEventHandler with the pointer to
    /// the virtual table.
    struct SAEventHandlerOnOS21
    {
        const SAEHandlerOS21VTable* fVirtualTable;  ///< Pointer to the virtual table.
        TAEventHandler*             fNext;          ///< Pointer to the next handler in the chain
        AEEventClass                fEventClass;    ///< Class for the events.
        AEEventID                   fEventID;       ///< ID for the events.
        TAEIdleTimer*               fIdler;         ///< Pointer to the idler object.
    };

    /// Structure of the pointer to the handler.
    struct SAEHandlerWPtrToProxy
    {
        union {
            SAEventHandlerOnOS20    fHandler20;     ///< The handler on NewtonOS 2.0
            SAEventHandlerOnOS21    fHandler21;     ///< The handler on NewtonOS 2.1
            TAEventHandler          fHandler;       ///< What is passed to the ROM functions
        } fEH;
        TAEventHandlerProxy*    fProxyPtr;  ///< Pointer to the proxy to call proxy callbacks.
    };

    /// \name Interface for NewtonOS 2.0 & NewtonOS 2.1 runtimes

    /// Dtor callback (NewtonOS 2.0 only)
    static void     __AEDestructor( SAEHandlerWPtrToProxy* inThis );

    /// AETestEvent callback
    static Boolean  __AETestEvent( SAEHandlerWPtrToProxy* inThis, TAEvent* inEvent );

    /// AEHandlerProc callback
    static void     __AEHandlerProc( SAEHandlerWPtrToProxy* inThis, TUMsgToken* inToken, ULong* inSize, TAEvent* inEvent );

    /// AECompletionProc callback
    static void     __AECompletionProc( SAEHandlerWPtrToProxy* inThis, TUMsgToken* inToken, ULong* size, TAEvent* inEvent );

    /// AEIdleProc callback
    static void     __IdleProc( SAEHandlerWPtrToProxy* inThis, TUMsgToken* inToken, ULong* inSize, TAEvent* inEvent );

    /// Virtual table in TAEventHandlerProxy.s
    static const SAEHandlerOS21VTable kVirtualTable21;

    /// \name Variables
    SAEHandlerWPtrToProxy*  mEventHandler;      ///< Reference on the actual handler.
    SAEHandlerOS20VTable    mVirtualTable20;    ///< Virtual table (NewtonOS 2.0)
    AEEventClass*           mEventClassPtr;     ///< Pointer to the actual value.
    AEEventID*              mEventIDPtr;        ///< Idem.
};

#endif
        // _TAEVENTHANDLERPROXY_H

// ========================================== //
// Do not use the blue keys on this terminal. //
// ========================================== //
