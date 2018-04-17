#
#   Copyright 2014-2018 Neueda Ltd.
#
#   Generated 17/04/18
#
#!/usr/bin/env bash
set -e

mcs -out:test.exe -reference:Cdr.dll example.cs
