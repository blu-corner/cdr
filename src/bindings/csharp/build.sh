#!/usr/bin/env bash
set -e

mcs -out:test.exe -reference:Cdr.dll example.cs
