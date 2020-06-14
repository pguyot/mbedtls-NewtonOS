// =========== Header ===========
// File:                mbedTLSEvents.cpp
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

#include "mbedTLSEvents.h"

// NewtonOS
#include <Newton.h>
#include <NSandDDKIncludes.h>
#include <NewtonScript.h>
#include <NameServer.h>
#include <NewtonExceptions.h>
#include <OSErrors.h>
#include <NewtonErrors.h>
#include <UserTasks.h>

#include "NewtonBindings.h"
#include "PMbedTLS.impl.h"

// Faster access to Newt Task port than the Name Server, exported in Public JT
TUPort* GetNewtTaskPort();

// ------------------------------------------------------------------------- //
//  * SendRPCToNewtApp(ULong*)
// ------------------------------------------------------------------------- //
NewtonErr
TMbedTLSBaseEvent::SendRPCToNewtApp(ULong* outResult, ULong size)
{
    // Send the event synchronously
    ULong returnSize = sizeof(ULong);
    NewtonErr theErr = GetNewtTaskPort()->SendRPC(&returnSize, this, size, outResult, sizeof(ULong));
    return theErr;
}

// ------------------------------------------------------------------------- //
//  * AEHandlerProc(TUMsgToken*, ULong*, TAEvent*)
// ------------------------------------------------------------------------- //
void
TMbedTLSHardwarePollEventHandler::AEHandlerProc(TUMsgToken* token, ULong* size, TAEvent* event)
{
    TMbedTLSHardwarePollEvent* pollEvent = (TMbedTLSHardwarePollEvent*) event;
    size_t len = pollEvent->fBufLen;
    unsigned char* output = pollEvent->fBuf;
    size_t olen;
    PMbedTLS::HardwarePoll(output, len, &olen);
    ULong result = olen;
    token->ReplyRPC(&result, sizeof(result), noErr);
}

// ------------------------------------------------------------------------- //
//  * AEHandlerProc(TUMsgToken*, ULong*, TAEvent*)
// ------------------------------------------------------------------------- //
void
TMbedTLSNVSeedReadEventHandler::AEHandlerProc(TUMsgToken* token, ULong* size, TAEvent* event)
{
    TMbedTLSNVSeedReadEvent* readEvent = (TMbedTLSNVSeedReadEvent*) event;
    size_t buf_len = readEvent->fBufLen;
    unsigned char* buf = readEvent->fBuf;

    ULong result = PMbedTLS::NVSeedRead(buf, buf_len);
    token->ReplyRPC(&result, sizeof(result), noErr);
}

// ------------------------------------------------------------------------- //
//  * AEHandlerProc(TUMsgToken*, ULong*, TAEvent*)
// ------------------------------------------------------------------------- //
void
TMbedTLSNVSeedWriteEventHandler::AEHandlerProc(TUMsgToken* token, ULong* size, TAEvent* event)
{
    TMbedTLSNVSeedWriteEvent* writeEvent = (TMbedTLSNVSeedWriteEvent*) event;
    size_t buf_len = writeEvent->fBufLen;
    unsigned char* buf = writeEvent->fBuf;

    ULong result = PMbedTLS::NVSeedWrite(buf, buf_len);
    token->ReplyRPC(&result, sizeof(result), noErr);
}

// ------------------------------------------------------------------------- //
//  * AEHandlerProc(TUMsgToken*, ULong*, TAEvent*)
// ------------------------------------------------------------------------- //
void
TMbedTLSGetGMTDeltaEventHandler::AEHandlerProc(TUMsgToken* token, ULong* size, TAEvent* event)
{
    TMbedTLSGetGMTDeltaEvent* loadEvent = (TMbedTLSGetGMTDeltaEvent*) event;
    ULong result = (ULong) PMbedTLS::GetGMTDelta();
    token->ReplyRPC(&result, sizeof(result), noErr);
}

// ------------------------------------------------------------------------- //
//  * AEHandlerProc(TUMsgToken*, ULong*, TAEvent*)
// ------------------------------------------------------------------------- //
void
TMbedTLSGetSerialNumberEventHandler::AEHandlerProc(TUMsgToken* token, ULong* size, TAEvent* event)
{
    TMbedTLSGetSerialNumberEvent* getEvent = (TMbedTLSGetSerialNumberEvent*) event;
    ULong result = (ULong) PMbedTLS::GetSerialNumber(getEvent->fBuf);
    token->ReplyRPC(&result, sizeof(result), noErr);
}

// ------------------------------------------------------------------------- //
//  * AEHandlerProc(TUMsgToken*, ULong*, TAEvent*)
// ------------------------------------------------------------------------- //
void
TMbedTLSGetRootCertificateEventHandler::AEHandlerProc(TUMsgToken* token, ULong* size, TAEvent* event)
{
    TMbedTLSGetRootCertificateEvent* getEvent = (TMbedTLSGetRootCertificateEvent*) event;
    ULong result = (ULong) PMbedTLS::GetRootCertificate(getEvent->fName, getEvent->fBuf, getEvent->fIoLen);
    token->ReplyRPC(&result, sizeof(result), noErr);
}

// ------------------------------------------------------------------------- //
//  * AEHandlerProc(TUMsgToken*, ULong*, TAEvent*)
// ------------------------------------------------------------------------- //
void
TMbedTLSListRootCertificatesEventHandler::AEHandlerProc(TUMsgToken* token, ULong* size, TAEvent* event)
{
    TMbedTLSListRootCertificatesEvent* listEvent = (TMbedTLSListRootCertificatesEvent*) event;
    ULong result = (ULong) PMbedTLS::ListRootCertificates(listEvent->fBuf, listEvent->fIoLen);
    token->ReplyRPC(&result, sizeof(result), noErr);
}

// ================================= //
// In the war of wits, he's unarmed. //
// ================================= //
