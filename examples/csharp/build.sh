#
# Copyright 2014-2018 Neueda Ltd.
#
#!/usr/bin/env bash
set -e

cp `pwd`/../../../lib/csharp/CdrBindings.dll .
LD_LIBRARY_PATH=`pwd`/../../../lib:`pwd`/../../../lib/csharp:$LD_LIBRARY_PATH mcs -out:test.exe -reference:CdrBindings.dll example.cs
