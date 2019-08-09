#!/bin/bash

echo
echo "=== Generating tests ==="
echo
mkdir -p $NESTING/work
opp_test gen -v -w $NESTING/work $NESTING/tests/*.test || exit 1

echo
echo "=== Building tests ==="
echo
(cd $NESTING/work; opp_makemake -f --deep -lnesting -L$NESTING/src -lINET -L$INET/src -o nesting; make) || exit 1

echo
echo "=== Running tests ==="
echo
opp_test run -v -w $NESTING/work -p $NESTING/work/nesting $NESTING/tests/*.test \
    -a "-n $NESTING/work:$NESTING/src:$INET/src"

#cd $NESTING/work/forwarding
#ls -l $NESTING/work/nesting
#$NESTING/work/nesting -u Cmdenv -n $NESTING/work:$NESTING/src:$INET/src omnetpp.ini _defaults.ini