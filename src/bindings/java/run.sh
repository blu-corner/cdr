#
#   Copyright 2014-2018 Neueda Ltd.
#
#   Generated 17/04/18
#
#!/usr/bin/env bash
set -e

LD_LIBRARY_PATH=`pwd`/../:$LD_LIBRARY_PATH java -cp `pwd`/CdrJNI.jar:`pwd` example
