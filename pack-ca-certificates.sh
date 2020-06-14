#!/usr/bin/env bash

(
for cert in `find ca-certificates -name "*.crt" -type f`; do
    certSubpath=${cert#*/}
    certSubpath=${certSubpath%%.crt*}
    echo -n 'call kAddPEM with (theSoup, "'
    echo -n "$certSubpath" | iconv -t ASCII --unicode-subst='\u%04X\u'
    echo '",'
    echo -n '"'
    cat $cert
    echo '");'
done
) | tr '\n' '\r' > ca-certificates.txt
