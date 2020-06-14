// =========== Header ===========
// File:                PMbedTLSCEOptions.h
// Project:             mbed TLS for NewtonOS
// Written by:          Paul Guyot (pguyot@kallisys.net)
//
// Created on:          02/06/2020
// Internal version:    1
//
// Copyright:           © 2020 by Paul Guyot.
//                      All rights reserved worldwide.
// ===========

// =========== Change History ===========
// 02/06/2020   v1  [PG]    Creation of the file
// ===========

#ifndef __PMBEDTLSCEOPTIONS__
#define __PMBEDTLSCEOPTIONS__

#include <mbedtls/ssl.h>

#define kSSLEnableLabel 'SSL '
#define kSSLHostnameLabel 'SSLh'
#define kSSLVerifyModeLabel 'SSLv'
#define kSSLCertificateAuthorityLabel 'SSLA'
#define kSSLPeerCertificateLabel 'SSLp'

class TCMOSSLEnable : public TOption
{
    public:
                        TCMOSSLEnable() : TOption(), enabled(false) {};

        Boolean         enabled;
};

class TCMOSSLHostname : public TOption
{
    public:
                        TCMOSSLHostname() : TOption() { hostname[0] = 0; };
        UChar           hostname[256];
};

class TCMOSSLVerifyMode : public TOption
{
    public:
                        TCMOSSLVerifyMode() : TOption() { verifyMode = MBEDTLS_SSL_VERIFY_REQUIRED; };
        ULong           verifyMode;
};

class TCMOSSLCertificate : public TOption
{
    public:
                        TCMOSSLCertificate() : TOption() {};
        ULong           length;
        ULong           indexInChain;
        UChar           certificate[];
};


#endif
    // __PMBEDTLSCEOPTIONS__
