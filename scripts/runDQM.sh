#!/bin/bash

input="/tmp/"
output="/tmp"
run="0"
spill="0"
prescale=1
keepUnpack=1
clean=0

TEMP=`getopt -o cki:o:r:s:p:u:w:a: --long clean,keepUnpack,unpackFolder,input:,output:,run:,spill:,unprescaledSpills:,prescale:,webDQM: -n 'runDQM.sh' -- "$@"`
if [ $? != 0 ] ; then echo "Options are wrong..." >&2 ; exit 1 ; fi

eval set -- "$TEMP"

while true; do
case "$1" in
-i | --input ) input="$2"; shift 2 ;;
-o | --output ) output="$2"; shift 2 ;;
-u | --unpackFolder ) unpackFolder="$2"; shift 2 ;;
-a | --unprescaledSpills ) unprescaledSpills="$2"; shift 2 ;;
-r | --run ) run="$2"; shift 2;;
-s | --spill ) spill="$2"; shift 2;;
-p | --prescale ) prescale=$2; shift 2;;
-k | --keepUnpack ) keepUnpack=1; shift 1;;
-c | --clean ) clean=1; shift 1;;
-w | --webDQM ) webDQM=$2; shift 2;;
-- ) shift; break ;;
* ) break ;;
esac
done


### PLOT MAKER -- make me configurable
### /home/cmsdaq/DAQ/H4DQM/bin/plotterHodo -i $output -o $output  -r $run -s $spill -I integrated.root
### /home/cmsdaq/DAQ/H4DQM/bin/plotterDAQStatus -i $output -o $output  -r $run -s $spill -I integrated.root
### /home/cmsdaq/DAQ/H4DQM/bin/plotterTDC -i $output -o $output  -r $run -s $spill 
#for runtype in beam ped led;do


mkdir -p $unpackFolder
echo "UNPACK => /home/cmsdaq/DAQ/H4DQM/bin/unpack -i $input  -o $unpackFolder -r $run -s $spill"
/home/cmsdaq/DAQ/H4DQM/bin/unpack -i $input  -o $unpackFolder -r $run -s $spill 	    

if [ $((spill%$prescale)) -eq 0 ] || [ $((spill)) -lt $unprescaledSpills ] ; then
#    if [ $((spill)) -ne 6 ]; then #skip spill 3 so that it's faster to see plots of first spill in the run
	for runtype in led ped beam;do
	    /home/cmsdaq/DAQ/H4DQM/bin/plotterTotal -i $unpackFolder -o $output  -r $run -s $spill -t$runtype -I integrated.root 
#/home/cmsdaq/DAQ/H4DQM/bin/plotterDigitizer -i $output -o $output  -r $run -s $spill 
	    
	    cd $output/$run/$spill/$dir/$runtype/
	    mkdir hodo
	    mkdir DAQ
	    mkdir ADC
	    mkdir TDC 
	    mkdir digitizer 
	    
	    mv total/hodo_* hodo/
	    mv total/DAQStatus_* DAQ/
	    mv total/ADC_* ADC/
	    mv total/TDC_* TDC/
	    mv total/digitizer_* digitizer/
	    mv total/*.root .
	    
	    rm -r total
	    cd -
	    
## hodo , TDC , DAQ 
# copy skeleton php
	    rsync -aP /home/cmsdaq/skel_DQM/ $output/$run/ 
	    rsync -aP /home/cmsdaq/skel_DQM/ $output/$run/$spill/

	    
	    rsync -aP $output/$run/ /data/public_DQM_plots/$run/


	done

	#clean unpack file
	[ "${keepUnpack}" == "1" ] || rm -rfv $unpackFolder/${run}/${spill.root}

	for runtype in led ped beam;do
	    rsync -aP /home/cmsdaq/skel_DQM/ $output/$run/$spill/$runtype/
	    for dir in digitizer hodo DAQ TDC ADC
#for dir in hodo DAQ ADC TDC
	    do
 		rsync -aP /home/cmsdaq/skel_DQM/ $output/$run/$spill/$runtype/$dir/
	    done
	done


    
# touch -R
	find $output/$run/$spill -type f -exec touch {} \;
	
	[ -h $output/$run/last ] && rm -v $output/$run/last
	[ -h $output/last ] && rm -v $output/last
	
	ln -s $output/$run/$spill $output/$run/last
	ln -s $output/$run $output/last
	
	chmod -R a+rx $output/$run/
	chmod -R g+rx $output/$run/

	rsync -aP $output/$run/ $webDQM:/data/public_DQM_plots/$run/
	rsync -aP $output/last $webDQM:/data/public_DQM_plots/

	#clean unpack file
	[ "${clean}" == "1" ] && rm -rfv ${output}/${run}/${spill}
	
#    fi

fi