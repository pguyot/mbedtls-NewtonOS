// =========== Header ===========
// File:				stdint.h
// Project:				mbed TLS for NewtonOS
// Written by:			Paul Guyot (pguyot@kallisys.net)
//
// Created on:			07/05/2020
// Internal version:	1
//
// Copyright:			© 2020 by Paul Guyot.
// 						All rights reserved worldwide.
// ===========

// =========== Change History ===========
// 07/05/2020	v1	[PG]	Creation of the file
// ===========

#ifndef __STDINT_H__
#define __STDINT_H__

#include <limits.h>
#define SIZE_MAX		    UINT_MAX

typedef unsigned char	    uint8_t;
typedef char			    int8_t;

typedef unsigned short	    uint16_t;
typedef short			    int16_t;

typedef unsigned long	    uint32_t;
typedef long			    int32_t;

typedef unsigned long long	uint64_t;
typedef long long			int64_t;

typedef unsigned long	    uintptr_t;
typedef long			    intptr_t;

typedef long long			intmax_t;

#endif
