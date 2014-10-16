#!/bin/bash

input="/tmp/"
output="/tmp"
run="0"
spill="0"

TEMP=`getopt -o i:o:r:s: --long input:,output:,run:,spill: -n 'runDQM.sh' -- "$@"`
if [ $? != 0 ] ; then echo "Options are wrong..." >&2 ; exit 1 ; fi

eval set -- "$TEMP"


while true; do
case "$1" in
-i | --input ) input="$2"; shift 2 ;;
-o | --output ) output="$2"; shift 2 ;;
-r | --run ) run="$2"; shift 2;;
-s | --spill ) spill="$2"; shift 2;;
-- ) shift; break ;;
* ) break ;;
esac
done

/home/cmsdaq/DAQ/H4DQM/bin/unpack -i $input  -o $output -r $run -s $spill

### PLOT MAKER -- make me configurable
/home/cmsdaq/DAQ/H4DQM/bin/plotterHodo -i $output -o $output  -r $run -s $spill -I integrated.root
/home/cmsdaq/DAQ/H4DQM/bin/plotterDAQStatus -i $output -o $output  -r $run -s $spill -I integrated.root
/home/cmsdaq/DAQ/H4DQM/bin/plotterTDC -i $output -o $output  -r $run -s $spill 
#/home/cmsdaq/DAQ/H4DQM/bin/plotterDigitizer -i $output -o $output  -r $run -s $spill 

## hodo , TDC , DAQ 
# copy skeleton php
rsync -aP /home/cmsdaq/skel_DQM/ $output/$run/ 
rsync -aP /home/cmsdaq/skel_DQM/ $output/$run/$spill/
for dir in hodo TDC DAQ digitizer
do
	rsync -aP /home/cmsdaq/skel_DQM/ $output/$run/$spill/$dir/
done

rsync -aP $output/$run/ pcethtb3.cern.ch:/data/public_DQM_plots/$run/
