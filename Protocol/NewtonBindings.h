// =========== Header ===========
// File:				NewtonBindings.h
// Project:				mbed TLS for NewtonOS
// Written by:			Paul Guyot (pguyot@kallisys.net)
//
// Created on:			03/06/2020
// Internal version:	1
//
// Copyright:			© 2020 by Paul Guyot.
// 						All rights reserved worldwide.
// ===========

// =========== Change History ===========
// 03/06/2020	v1	[PG]	Creation of the file
// ===========

#ifndef __NEWTONBINDINGS__
#define __NEWTONBINDINGS__

#include <time.h>
#include <mbedtls/x509.h>

// Determine if we are currently in NewtApp task
bool IsInNewtTask();

// Bindings for mbedtls
extern "C" {
    int mbedtls_hardclock_poll(void *data, unsigned char *output, size_t len, size_t *olen);
	int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen);
	int mbedtls_platform_std_nv_seed_read(unsigned char *buf, size_t buf_len);
	int mbedtls_platform_std_nv_seed_write(unsigned char *buf, size_t buf_len);
	void mbedtls_platform_zeroize(void *buf, size_t len);
	struct tm *mbedtls_platform_gmtime_r(const time_t *tt, struct tm *tm_buf);
};

// Additional API for ConnectionEnd (inet task)
#define SERIAL_NUMBER_LEN 8
int GetSerialNumber(unsigned char outSerial[SERIAL_NUMBER_LEN]);
int ListRootCertificates(char* outBuf, size_t* ioSize);
int GetRootCertificate(const char* name, unsigned char* outBuf, size_t* ioSize);
long TimeToLocal1904Minutes(const mbedtls_x509_time& time);

#endif
    // __NEWTONBINDINGS__

// ================================================================= //
// Painting, n.:                                                     //
//         The art of protecting flat surfaces from the weather, and //
// exposing them to the critic.                                      //
//                 -- Ambrose Bierce                                 //
// ================================================================= //
