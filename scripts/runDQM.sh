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

mkdir -p $output/$run/
mkdir -p $output/$run/$spill

$HOME/DAQ/H4DQM/bin/unpack -i $input  -o $output -r $run -s $spill > $output/$run/$spill/unpack.log

### PLOT MAKER -- make me configurable
### $HOME/DAQ/H4DQM/bin/plotterHodo -i $output -o $output  -r $run -s $spill -I integrated.root
### $HOME/DAQ/H4DQM/bin/plotterDAQStatus -i $output -o $output  -r $run -s $spill -I integrated.root
### $HOME/DAQ/H4DQM/bin/plotterTDC -i $output -o $output  -r $run -s $spill 
$HOME/DAQ/H4DQM/bin/plotterTotal -i $output -o $output  -r $run -s $spill -I integrated.root  >  $output/$run/$spill/plotter.log
#$HOME/DAQ/H4DQM/bin/plotterDigitizer -i $output -o $output  -r $run -s $spill 

DQMtypes="digitizer hodo DAQ TDC"
cd $output/$run/$spill
for dir in $DQMtypes
do
    mkdir $dir
    mv total/$dir* $dir
done

mv total/*.root .
rm -r total


## hodo , TDC , DAQ 
# copy skeleton php
rsync -aP $HOME/DAQ/H4DQM/skel_DQM/ $output/$run/ 
rsync -aP $HOME/DAQ/H4DQM/skel_DQM/ $output/$run/$spill/
for dir in $DQMtypes
do
	rsync -aP $HOME/DAQ/H4DQM/skel_DQM/ $output/$run/$spill/$dir/
done

# touch -R
find $output/$run/$spill -type f -exec touch {} \;

[ -h $output/$run/last ] && rm $output/$run/last
[ -h $output/last ] && rm $output/last

ln -s $output/$run/$spill $output/$run/last
ln -s $output/$run $output/last

#rsync -aP $output/$run/ pcethtb3.cern.ch:/data/public_DQM_plots/$run/
#rsync -aP $output/last pcethtb3.cern.ch:/data/public_DQM_plots/

