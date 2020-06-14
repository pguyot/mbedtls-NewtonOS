// =========== Header ===========
// File:                mbedTLSEvents.h
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

#ifndef __MBEDTLSEVENTS__
#define __MBEDTLSEVENTS__

#include "TAEventHandlerProxy.h"

#include <NSandDDKIncludes.h>
#include <UserTasks.h>
#include <NewtonTime.h>
#include <AEvents.h>

#define kMbedTLSEventClass                  'mTLS'
#define kMbedTLSHardwarePollEventID         'hpol'
#define kMbedTLSNVSeedReadEventID           'nvsr'
#define kMbedTLSNVSeedWriteEventID          'nvsw'
#define kMbedTLSGetGMTDeltaEventID          'ggmt'
#define kMbedTLSGetSerialNumberEventID      'gsrl'
#define kMbedTLSListRootCertificatesEventID 'lcrt'
#define kMbedTLSGetRootCertificateEventID   'gcrt'

// Event handlers to make sure calls to NewtonScript happens in the NewtWorld
// task. Used for the communication tool.

class TMbedTLSBaseEvent : public TAEvent
{
public:
    TMbedTLSBaseEvent(ULong eventId) :
        TAEvent()
        {
            fAEventClass = kMbedTLSEventClass;
            fAEventID = eventId;
        };

    NewtonErr   SendRPCToNewtApp(ULong* outResult, ULong size = sizeof(TMbedTLSBaseEvent));
};

class TMbedTLSBaseBufferEvent : public TMbedTLSBaseEvent
{
public:
    TMbedTLSBaseBufferEvent(unsigned char* buf, size_t bufLen, ULong eventId) :
        TMbedTLSBaseEvent(eventId),
        fBuf(buf),
        fBufLen(bufLen)
        {
        };
    unsigned char*  fBuf;
    size_t          fBufLen;

    NewtonErr   SendRPCToNewtApp(ULong* outResult, ULong size = sizeof(TMbedTLSBaseBufferEvent)) {
        return TMbedTLSBaseEvent::SendRPCToNewtApp(outResult, size);
    }
};

class TMbedTLSHardwarePollEvent : public TMbedTLSBaseBufferEvent
{
public:
    TMbedTLSHardwarePollEvent(unsigned char* buf, size_t bufLen) :
        TMbedTLSBaseBufferEvent(buf, bufLen, kMbedTLSHardwarePollEventID) {};
};

class TMbedTLSNVSeedReadEvent : public TMbedTLSBaseBufferEvent
{
public:
    TMbedTLSNVSeedReadEvent(unsigned char* buf, size_t bufLen) :
        TMbedTLSBaseBufferEvent(buf, bufLen, kMbedTLSNVSeedReadEventID) {};
};

class TMbedTLSNVSeedWriteEvent : public TMbedTLSBaseBufferEvent
{
public:
    TMbedTLSNVSeedWriteEvent(unsigned char* buf, size_t bufLen) :
        TMbedTLSBaseBufferEvent(buf, bufLen, kMbedTLSNVSeedWriteEventID) {};
};

class TMbedTLSGetGMTDeltaEvent : public TMbedTLSBaseEvent
{
public:
    TMbedTLSGetGMTDeltaEvent() :
        TMbedTLSBaseEvent(kMbedTLSGetGMTDeltaEventID) {};
};

class TMbedTLSGetSerialNumberEvent : public TMbedTLSBaseBufferEvent
{
public:
    TMbedTLSGetSerialNumberEvent(unsigned char* buf, size_t bufLen) :
        TMbedTLSBaseBufferEvent(buf, bufLen, kMbedTLSGetSerialNumberEventID) {};
};

class TMbedTLSListRootCertificatesEvent : public TMbedTLSBaseEvent
{
public:
    TMbedTLSListRootCertificatesEvent(char* buf, size_t* ioLen) :
        TMbedTLSBaseEvent(kMbedTLSListRootCertificatesEventID),
        fBuf(buf),
        fIoLen(ioLen)
        {
        };
    char*       fBuf;
    size_t*     fIoLen;

    NewtonErr   SendRPCToNewtApp(ULong* outResult, ULong size = sizeof(TMbedTLSListRootCertificatesEvent)) {
        return TMbedTLSBaseEvent::SendRPCToNewtApp(outResult, size);
    }
};

class TMbedTLSGetRootCertificateEvent : public TMbedTLSBaseEvent
{
public:
    TMbedTLSGetRootCertificateEvent(const char* name, unsigned char* buf, size_t* ioLen) :
        TMbedTLSBaseEvent(kMbedTLSGetRootCertificateEventID),
        fName(name),
        fBuf(buf),
        fIoLen(ioLen)
        {
        };
    const char*     fName;
    unsigned char*  fBuf;
    size_t*         fIoLen;

    NewtonErr   SendRPCToNewtApp(ULong* outResult, ULong size = sizeof(TMbedTLSGetRootCertificateEvent)) {
        return TMbedTLSBaseEvent::SendRPCToNewtApp(outResult, size);
    }
};

/// Event handler for entropy hardware poll
class TMbedTLSHardwarePollEventHandler : public TAEventHandlerProxy
{
public:
    TMbedTLSHardwarePollEventHandler() { Init(kMbedTLSHardwarePollEventID, kMbedTLSEventClass); };
    virtual ~TMbedTLSHardwarePollEventHandler() {};

    ///
    /// Method called when an event is received.
    ///
    void AEHandlerProc(TUMsgToken* token, ULong* size, TAEvent* event) override;
};

/// Event handler for NV seed reader
class TMbedTLSNVSeedReadEventHandler : public TAEventHandlerProxy
{
public:
    TMbedTLSNVSeedReadEventHandler() { Init(kMbedTLSNVSeedReadEventID, kMbedTLSEventClass); };
    virtual ~TMbedTLSNVSeedReadEventHandler() {};

    ///
    /// Method called when an event is received.
    ///
    void AEHandlerProc(TUMsgToken* token, ULong* size, TAEvent* event) override;
};

/// Event handler for NV seed writer
class TMbedTLSNVSeedWriteEventHandler : public TAEventHandlerProxy
{
public:
    TMbedTLSNVSeedWriteEventHandler() { Init(kMbedTLSNVSeedWriteEventID, kMbedTLSEventClass); };
    virtual ~TMbedTLSNVSeedWriteEventHandler() {};

    ///
    /// Method called when an event is received.
    ///
    void AEHandlerProc(TUMsgToken* token, ULong* size, TAEvent* event) override;
};

/// Event handler for getting GMT delta
class TMbedTLSGetGMTDeltaEventHandler : public TAEventHandlerProxy
{
public:
    TMbedTLSGetGMTDeltaEventHandler() { Init(kMbedTLSGetGMTDeltaEventID, kMbedTLSEventClass); };
    virtual ~TMbedTLSGetGMTDeltaEventHandler() {};

    ///
    /// Method called when an event is received.
    ///
    void AEHandlerProc(TUMsgToken* token, ULong* size, TAEvent* event) override;
};

/// Event handler for getting serial number
class TMbedTLSGetSerialNumberEventHandler : public TAEventHandlerProxy
{
public:
    TMbedTLSGetSerialNumberEventHandler() { Init(kMbedTLSGetSerialNumberEventID, kMbedTLSEventClass); };
    virtual ~TMbedTLSGetSerialNumberEventHandler() {};

    ///
    /// Method called when an event is received.
    ///
    void AEHandlerProc(TUMsgToken* token, ULong* size, TAEvent* event) override;
};

/// Event handler for listing root certificates
class TMbedTLSListRootCertificatesEventHandler : public TAEventHandlerProxy
{
public:
    TMbedTLSListRootCertificatesEventHandler() { Init(kMbedTLSListRootCertificatesEventID, kMbedTLSEventClass); };
    virtual ~TMbedTLSListRootCertificatesEventHandler() {};

    ///
    /// Method called when an event is received.
    ///
    void AEHandlerProc(TUMsgToken* token, ULong* size, TAEvent* event) override;
};

/// Event handler for getting a root certificate
class TMbedTLSGetRootCertificateEventHandler : public TAEventHandlerProxy
{
public:
    TMbedTLSGetRootCertificateEventHandler() { Init(kMbedTLSGetRootCertificateEventID, kMbedTLSEventClass); };
    virtual ~TMbedTLSGetRootCertificateEventHandler() {};

    ///
    /// Method called when an event is received.
    ///
    void AEHandlerProc(TUMsgToken* token, ULong* size, TAEvent* event) override;
};

#endif
    // __TMBEDTLSEVENTHANDLER__

// ======================================================================= //
// It was pleasant to me to get a letter from you the other day.  Perhaps  //
// I should have found it pleasanter if I had been able to decipher it.  I //
// don't think that I mastered anything beyond the date (which I knew) and //
// the signature (which I guessed at).  There's a singular and a perpetual //
// charm in a letter of yours; it never grows old, it never loses its      //
// novelty.  Other letters are read and thrown away and forgotten, but     //
// yours are kept forever -- unread.  One of them will last a reasonable   //
// man a lifetime.                                                         //
//                 -- Thomas Aldrich                                       //
// ======================================================================= //
