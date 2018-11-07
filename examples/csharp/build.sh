#
# Copyright 2014-2018 Neueda Ltd.
#
#!/usr/bin/env bash
set -e

cp `pwd`/../../../lib/csharp/Cdr.dll .
LD_LIBRARY_PATH=`pwd`/../../../lib:`pwd`/../../../lib/csharp:$LD_LIBRARY_PATH mcs -out:test.exe -reference:Cdr.dll example.cs
