# Build instructions

mbedTLS was built on MacOS X 10.15.5 but can probably be built with on any
POSIX-compliant system.

## Toolchain

* Install arm-none-eabi-binutils (used 2.34).
* Install arm-none-eabi-gcc compilers (user 8.3.0).

For example, with [MacPorts](https://www.macports.org), simply replace the
version line and the extract suffix:

```
diff --git a/cross/arm-none-eabi-gcc/Portfile b/cross/arm-none-eabi-gcc/Portfile
index fdb4cff9907..d529da1d3e6 100644
--- a/cross/arm-none-eabi-gcc/Portfile
+++ b/cross/arm-none-eabi-gcc/Portfile
@@ -3,9 +3,10 @@
 PortSystem          1.0
 PortGroup           crossgcc 1.0
 
-crossgcc.setup      arm-none-eabi 9.2.0
+crossgcc.setup      arm-none-eabi 8.3.0
 crossgcc.setup_libc newlib 3.1.0
-revision            1
+revision            0
+extract.suffix      .gz
 
 maintainers         nomaintainer
 
```

* Compile [DCL Toolchain utils](https://github.com/pguyot/DCL/tree/master/Sample_Code/ToolchainUtils)
and install them on your PATH

## Newton development environment

mbedTLS port requires C++ headers for NewtonOS development as well as a complete
development environment (to be run in SheepSaver or Basilisk II) for the GUI
and packaging part.

* Get Basilisk II from the [official repository](https://github.com/cebix/macemu)
or from [Matthias Melcher's fork](http://messagepad.org/Newton_Installs.html)
* Get [NewtonDev](http://www.unna.org/view.php?/development/NewtonDev) from UNNA
* Copy the `NCT_Projects` folder to your native environment (for gcc)
* Optionally convert all endlines, for example with this one-line:
`$ find . -name "*.h" | sed -E "s|(.+).h|perl -pi -e 's/\\r/\\n/g' \1.h|g" | sh -`

## Native compilation

```
make NCT_PROJECTS=PATH_TO_NEWTONDEV
```

should work as expected.
It generates the following three files:

* mbedTLSInterface.ntkc
* mbedTLSProtocol.pkg
* ca-certificates.txt

Copy them to the Basilisk II environment.

## Newton Toolkit compilation

Newton Toolkit files include resource forks which cannot easily be checked into
modern version controlling systems. For convenience, a Disk Copy disk image
is included.

* Copy the image `mbedTLS-NTK-SourceCode` to the Basilisk II environment and
extract it.
* Copy the three generated files from native complation inside the extracted
copy (next to `mbedTLS` file).
* Open and build project `mbedTLSCACertificates`.
* Open and build project `mbedTLS`.
* Optionally open and build projects `mbedTLSTest` and `MailVSSLPatch`.
