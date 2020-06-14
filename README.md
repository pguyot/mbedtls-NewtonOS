NewtonOS port of mbed tls.

Original mbed TLS home page: https://tls.mbed.org/

mbedtls is subject to the terms of Apache License 2.0.
This port is subject to the same license (cf the file "LICENSE").

This port also includes printf.c from Marco Paland which is licensed under MIT
license.
https://github.com/mpaland/printf

# For end-users

Install `mbedTLS.pkg` package. This allow third-party applications to use SSL.
Make sure Newton's date is set properly as certificate issue and expiration
dates are checked. You will need [Eckhart Köppen's system patch](https://40hz.org/Pages/newton/hacking/newton-year-2010-problem/).

## Compatible software

### Mail V SSL Patch

Included with this port is `MailVSSLPatch.pkg`. This package enables IMAPS
access with [Mail V](https://www.newtonresearch.org/mailv/).
In your e-mail access setting, postfix the IMAP server address with :993.
The patch automatically configures endpoints connecting to port 993 to use SSL.

This patch may be incompatible with future versions of Mail V and was tested
with Mail V 5.2.1.

# For power users

## Root certificates

Mozilla root certificates are included with mbedTLS.
You can add additionally certificates by adding entries to the
`"CACertificates:Kallisys"` union soup.

Each entry requires the following slots:

* tag: the certificate name (in plain text)
* pem: the certificate in PEM format (an ASCII 8-bits binary)
* der: the certificate in DER format (a binary)
* trust: whether the certificate is trusted or not (should be TRUE)

The entry should include either `pem` or `der` slots, not both.

Additionally, you can **disable** a built-in certificate by adding an entry to
the union soup with the tag of the certificate you want to disable and set
trust to NIL.

# For developers

The package `mbedTLSTest.pkg` exhibits several usages and should be considered
as a first approach to sample code, especially file `NTK Projects/mbedTLSTest.f`.

Two interfaces are provided: bindings to many mbedTLS functions for NewtonScript
(low-level interface) and a patch that allows higher-level usage at the endpoint
level.

In both cases, include `mbedTLS.unit` file to your project at it contains all
necessary definitions.

If you have any trouble, don't hesitate to contact me.

## Endpoint interface

Usage with NIE consists in passing additional options to the endpoint to
transparently perform SSL connection.

Endpoint interface is currently limited to TCP clients (support for DTLS and
TCP servers have not been developed).

The minimum required options are `kCMOSSLEnable` and `kCMOSSLHostname`.

### kCMOSSLEnable (required)

    {
        label: kCMOSSLEnable,
        type: 'option,
        opCode: opSetRequired,
        result: nil,
        form: 'template,
        data: {arglist: [true], typelist: [kStruct, kBoolean,]}
    }

This option can be passed to `:Connect(options, callback)` endpoint method as
well as to `:Option(options, callback)` endpoint method.

If passed to `Connect`, the option will enable SSL and mbedTLS will perform
SSL handshake on connection.

If passed to `Option` while endpoint is already connected, mbedTLS will perform
SSL handshake. This is especially useful for SSL upgrade of a connection
(see IMAPStartTLS test code).

### kCMOSSLHostname (required)

    {
        label: kCMOSSLHostname,
        type: 'option,
        opCode: opSetRequired,
        result: nil,
        form: 'template,
        data: {arglist: [fRemoteHost], typelist: [ kStruct, [kArray, kChar, 0]]}
    }

This option can be passed to `:Connect(options, callback)` and
`:Option(options, callback)` endpoint methods. When passed to `Option`, you
should pass this option before `kCMOSSLEnable`. When passed to `Connect`, it can
be passed with `kCMOSSLEnable` in the same array.

This option defines the peer (server) hostname. If the hostname is not defined,
SSL authentication will not be performed.

### kCMOSSLVerifyMode (optional)

    {
        label: kCMOSSLVerifyMode,
        type: 'option,
        opCode: opSetRequired,
        result: nil,
        form: 'template,
        data: { arglist: [MBEDTLS_SSL_VERIFY_REQUIRED], typelist: [kStruct, kULong] }
    }

This option can be passed to `:Connect(options, callback)` and
`:Option(options, callback)` endpoint methods. When passed to `Option`, you
should pass this option before `kCMOSSLEnable`. When passed to `Connect`, it can
be passed with `kCMOSSLEnable` in the same array.

This option defines the verify mode. Only supported values are
`MBEDTLS_SSL_VERIFY_NONE` and `MBEDTLS_SSL_VERIFY_REQUIRED`.

### kCMOSSLCertificateAuthority (optional)

For PEM certificate(s):

    {
        label: kCMOSSLCertificateAuthority,
        type: 'option,
        opCode: opSetRequired,
        result: nil,
        form: 'template,
        data: {
            arglist: [StrLen(pemStr) + 1, 0, pemStr],
            typelist: [kStruct, kULong, kULong, [kArray, kChar, 0]]
        }
    }

For DER certificate:

    {
        label: kCMOSSLCertificateAuthority,
        type: 'option,
        opCode: opSetRequired,
        result: nil,
        form: 'template,
        data: {
            arglist: [Length(derArray), 0, derArray],
            typelist: [kStruct, kULong, kULong, [kArray, kChar, Length(derArray)]]
        }
    }

This option can be passed to `:Connect(options, callback)` and
`:Option(options, callback)` endpoint methods. When passed to `Option`, you
should pass this option before `kCMOSSLEnable`. When passed to `Connect`, it can
be passed with `kCMOSSLEnable` in the same array.

This option allow you to pass specific certificates to be used as the CA chain.
When used, the certificate(s) you pass will replace the default CA roots.

For PEM, the certificate is passed as a Newton string. Several certificates
can be passed at once by concatenating them.

For DER, the certificate is passed as an array of integers (bytes).

See associated tests for usage.

### kCMOSSLGetPeerCertificate (optional)

To fetch size:

    {
        label: kCMOSSLGetPeerCertificate,
        type: 'option,
        opCode: opSetRequired,
        result: nil,
        form: 'template,
        data: {
            arglist: [0, certIx], typelist: [kStruct, kULong, kULong]
        }
    }

To fetch certificate:

    local cert := Array(size, 0);

    {
        label: kCMOSSLGetPeerCertificate,
        type: 'option,
        opCode: opSetRequired,
        result: nil,
        form: 'template,
        data: {
            arglist: [size, certIx, cert],
            typelist: [kStruct, kULong, kULong, [kArray, kByte, size]]
        };
    }

This option can be passed to `:Option(options, callback)` endpoint method once
the endpoint is connected.

This option can be used to fetch the certificates from the peer. certIx
represent the index of the certificate in the chain.

The certificates can then be passed to low-level interface to extract data such
as subject name or issuer name.

See associated test for usage.

## Low-level interface

Bindings use NewtonOS unit mechanism and are listed in
`NTK\ Projects/Public/mbedTLS.unit`.
