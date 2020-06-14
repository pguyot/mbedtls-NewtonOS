// =========== Header ===========
// File:				NewtonBindings.cp
// Project:				mbed TLS for NewtonOS
// Written by:			Paul Guyot (pguyot@kallisys.net)
//
// Created on:			15/05/2020
// Internal version:	1
//
// Copyright:			© 2020 by Paul Guyot.
// 						All rights reserved worldwide.
// ===========

// =========== Change History ===========
// 15/05/2020	v1	[PG]	Creation of the file
// ===========

// NewtonOS
#include <Newton.h>
#include <NSandDDKIncludes.h>
#include <NewtonScript.h>
#include <NameServer.h>
#include <NewtonExceptions.h>
#include <OSErrors.h>
#include <NewtonErrors.h>
#include <NewtonTime.h>

#include "NewtonBindings.h"
#include "mbedTLSEvents.h"

#include "PMbedTLS.impl.h"

bool IsInNewtTask()
{
    STaskSwitchedGlobals * g = ((STaskSwitchedGlobals*) GetGlobals()) - 1;
	return g->fTaskName == 'newt';
}

// ------------------------------------------------------------------------	//
// * mbedtls_hardclock_poll( void*, unsigned char*, size_t, size_t* )
// ------------------------------------------------------------------------	//
// Get some entropy from the clock
extern "C" int mbedtls_hardclock_poll( void *data, unsigned char *output, size_t len, size_t *olen )
{
    unsigned long timer = (ULong) GetGlobalTime();
    *olen = 0;

    if( len < sizeof(unsigned long) )
        return( 0 );

    memcpy( output, &timer, sizeof(unsigned long) );
    *olen = sizeof(unsigned long);

    return( 0 );
}

// ------------------------------------------------------------------------	//
// * mbedtls_hardware_poll( void*, unsigned char*, size_t, size_t* )
// ------------------------------------------------------------------------	//
// Get some entropy from the NewtonScript entropy collector.
extern "C" int mbedtls_hardware_poll( void *data, unsigned char *output, size_t len, size_t *olen )
{
    int ret = 0;
    if (IsInNewtTask()) {
        PMbedTLS::HardwarePoll( output, len, olen );
    } else {
        ULong result = 0;
        TMbedTLSHardwarePollEvent event(output, len);
        NewtonErr theErr = event.SendRPCToNewtApp(&result);
        if (theErr) {
            ret = theErr;
        }
        *olen = result;
    }
    return ret;
}

// ------------------------------------------------------------------------	//
// * mbedtls_platform_std_nv_seed_read( unsigned char*, size_t )
// ------------------------------------------------------------------------	//
extern "C" int mbedtls_platform_std_nv_seed_read( unsigned char *buf, size_t buf_len )
{
    ULong result = 0;
    if (IsInNewtTask()) {
        result = PMbedTLS::NVSeedRead( buf, buf_len );
    } else {
        TMbedTLSNVSeedReadEvent event(buf, buf_len);
        NewtonErr theErr = event.SendRPCToNewtApp(&result);
        if (theErr) {
            result = theErr;
        }
    }
    return result;
}

// ------------------------------------------------------------------------	//
// * mbedtls_platform_std_nv_seed_write( unsigned char*, size_t )
// ------------------------------------------------------------------------	//
extern "C" int mbedtls_platform_std_nv_seed_write( unsigned char *buf, size_t buf_len )
{
    ULong result = 0;
    if (IsInNewtTask()) {
        result = PMbedTLS::NVSeedWrite( buf, buf_len );
    } else {
        TMbedTLSNVSeedWriteEvent event(buf, buf_len);
        NewtonErr theErr = event.SendRPCToNewtApp(&result);
        if (theErr) {
            result = theErr;
        }
    }
    return result;
}

// ------------------------------------------------------------------------	//
// * mbedtls_platform_zeroize( void*, size_t )
// ------------------------------------------------------------------------	//
extern "C" void mbedtls_platform_zeroize( void *buf, size_t len )
{
    if( len > 0 )
        memset( buf, 0, len );
}

// ------------------------------------------------------------------------	//
// * mbedtls_platform_gmtime_r(const mbedtls_time_t*, struct tm*)
// ------------------------------------------------------------------------	//
// NewtonOS gmtime always return NULL.
extern "C" struct tm*
mbedtls_platform_gmtime_r(const time_t* tt, struct tm* tm_buf)
{
    long gmtdelta = 0;
    if (IsInNewtTask()) {
        gmtdelta = PMbedTLS::GetGMTDelta();
    } else {
        TMbedTLSGetGMTDeltaEvent event;
        (void) event.SendRPCToNewtApp((ULong*)&gmtdelta);
        // In case of error, use 0
    }
    // Convert date
// This will overflow in 2040
#define NEWTON_EPOCH_DELTA 2082841761
    const unsigned gmt_sec = *tt - gmtdelta - NEWTON_EPOCH_DELTA;
    tm_buf->tm_sec = gmt_sec % 60;         // we don't handle leap seconds
    const unsigned gmt_min = gmt_sec / 60;
    tm_buf->tm_min = gmt_min % 60;
    const unsigned gmt_hour = gmt_min / 60;
    tm_buf->tm_hour = gmt_hour % 24;
    const int gmt_day = gmt_hour / 24;
    // http://howardhinnant.github.io/date_algorithms.html
    const int gmt_shift = gmt_day + 719468;
    const int era = (gmt_shift >= 0 ? gmt_shift : gmt_shift - 146096) / 146097;
    const unsigned doe = static_cast<unsigned>(gmt_shift - era * 146097);
    const unsigned yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;
    const int y = static_cast<int>(yoe) + era * 400;
    const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    const unsigned mp = (5*doy + 2)/153;
    const unsigned d = doy - (153*mp+2)/5 + 1;
    const unsigned m = mp + (mp < 10 ? 2 : -10);    // 0-11 instead of 1-12
    const unsigned civil_y = y + (m <= 1);
    const unsigned wday = static_cast<unsigned>(gmt_day >= -4 ? (gmt_day+4) % 7 : (gmt_day+5) % 7 + 6);
    tm_buf->tm_mday = d;
    tm_buf->tm_mon = m;
    tm_buf->tm_year = civil_y - 1900;
    tm_buf->tm_wday = wday;
    tm_buf->tm_yday = doy;
    tm_buf->tm_isdst = false;
    return tm_buf;
}

// ------------------------------------------------------------------------	//
// * TimeToLocal1904Minutes(const mbedtls_x509_time&)
// ------------------------------------------------------------------------	//
long
TimeToLocal1904Minutes(const mbedtls_x509_time& time)
{
    // Convert date
#define NEWTON_000301_DELTA_DAYS 695361
    // http://howardhinnant.github.io/date_algorithms.html
    const int m = time.mon;
    const int y = time.year - (m <= 2);
    const int d = time.day;
    const int era = (y >= 0 ? y : y-399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);
    const unsigned doy = (153*(m + (m > 2 ? -3 : 9)) + 2)/5 + d-1;
    const unsigned doe = yoe * 365 + yoe/4 - yoe/100 + doy;
    const long days000301 = era * 146097 + static_cast<int>(doe);
    const long days19040101 = days000301 - NEWTON_000301_DELTA_DAYS;
    const long localmins19040101 = days19040101 * 1440;
    return localmins19040101;
}

// ------------------------------------------------------------------------	//
// * Get the serial number.
// ------------------------------------------------------------------------	//
int GetSerialNumber(unsigned char outSerial[SERIAL_NUMBER_LEN])
{
    int result;
    if (IsInNewtTask()) {
        result = PMbedTLS::GetSerialNumber(outSerial);
    } else {
        TMbedTLSGetSerialNumberEvent event(outSerial, SERIAL_NUMBER_LEN);
        NewtonErr theErr = event.SendRPCToNewtApp((ULong*)&result);
        if (theErr) result = theErr;
    }
    return result;
}

// ------------------------------------------------------------------------	//
// * List root certificates
// ------------------------------------------------------------------------	//
int ListRootCertificates(char* outBuf, size_t* ioSize)
{
    int result;
    if (IsInNewtTask()) {
        result = PMbedTLS::ListRootCertificates(outBuf, ioSize);
    } else {
        TMbedTLSListRootCertificatesEvent event(outBuf, ioSize);
        NewtonErr theErr = event.SendRPCToNewtApp((ULong*)&result);
        if (theErr) result = theErr;
    }
    return result;
}

// ------------------------------------------------------------------------	//
// * Get a root certificate
// ------------------------------------------------------------------------	//
int GetRootCertificate(const char* name, unsigned char* outBuf, size_t* ioSize)
{
    int result;
    if (IsInNewtTask()) {
        result = PMbedTLS::GetRootCertificate(name, outBuf, ioSize);
    } else {
        TMbedTLSGetRootCertificateEvent event(name, outBuf, ioSize);
        NewtonErr theErr = event.SendRPCToNewtApp((ULong*)&result);
        if (theErr) result = theErr;
    }
    return result;
}
