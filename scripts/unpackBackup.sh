#!/bin/bash

input="/tmp/"
output="/tmp"
run="0"
spill="0"
log="/tmp/${USER}"
prescale=1

TEMP=`getopt -o i:o:r:s:l:p: --long input:,output:,prescale:,run:,spill:,log: -n 'unpackBackup.sh' -- "$@"`
if [ $? != 0 ] ; then echo "Options are wrong..." >&2 ; exit 1 ; fi

eval set -- "$TEMP"

while true; do
case "$1" in
-i | --input ) input="$2"; shift 2 ;;
-o | --output ) output="$2"; shift 2 ;;
-l | --log ) log="$2"; shift 2 ;;
-r | --run ) run="$2"; shift 2;;
-s | --spill ) spill="$2"; shift 2;;
-p | --prescale ) prescale=$2; shift 2;;
-- ) shift; break ;;
* ) break ;;
esac
done

mkdir -p $log
source /opt/root/bin/thisroot.sh
/home/cmsdaq/DAQ/H4DQM/bin/unpack -i $input  -o $output -r $run -s $spill  -p $prescale > $log/${run}_${spill}_unpack.log 2>&1
echo "$input/$run/$spill.raw" > /var/spool/tbb/${run}_${spill}_raw
echo "$output/$run/$spill.root" > /var/spool/tbb/${run}_${spill}_dataTree
