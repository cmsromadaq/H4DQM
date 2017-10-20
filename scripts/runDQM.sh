#!/bin/bash

input="/tmp/"
output="/tmp"
run="0"
spill="0"
prescale=1
keepUnpack=0
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

if [ $((spill%$prescale)) -eq 0 ] || [ $((spill)) -lt $unprescaledSpills ] ; then
    mkdir -p $unpackFolder
    echo "UNPACK => /home/cmsdaq/DAQ/H4DQM/bin/unpack -i $input  -o $unpackFolder -r $run -s $spill"
    /home/cmsdaq/DAQ/H4DQM/bin/unpack -i $input  -o $unpackFolder -r $run -s $spill > /tmp/${run}_${spill}_unpack.log 2>&1 	    

    ###---Run H4Analysis reco
    /home/cmsdaq/DAQ/H4Analysis/bin/H4Reco /home/cmsdaq/DAQ/H4Analysis/cfg/VFE_adapter_dqm2.cfg $run $spill > /tmp/${run}_${spill}_h4reco.log 

    ###---Run DQM plots
    export run=$run
    export spill=$spill
    if [ $spill -eq 1 ]; then
        let prev_spill=$spill-1
    elif [ $spill -le $prescale ]; then
        let prev_spill=$spill-$prescale+1
    else
        let prev_spill=$spill-$prescale
    fi
    export prev_spill=$prev_spill  
    export outdir=/data/public_DQM_plots/$run/$spill/
    set +x
    parallel '/home/cmsdaq/DAQ/H4Analysis/FuriousPlotter/draw.py -c {} -p "current_spill /data/ntuples/${run}_${spill}.root h4","prev.spill /data/public_DQM_plots/${run}/${prev_spill}/","draw.outDir ${outdir}"' ::: `cat /home/cmsdaq/DAQ/H4DQM/data/current_dqm_plots.txt`

    rsync -aP /home/cmsdaq/DAQ/H4DQM/skel_DQM/ /data/public_DQM_plots/$run/
    rsync -aP /home/cmsdaq/DAQ/H4DQM/skel_DQM/ /data/public_DQM_plots/$run/$spill

	#clean unpack file
    [ "${keepUnpack}" == "1" ] || rm -rfv $unpackFolder/${run}/${spill.root}
  
    /home/cmsdaq/DAQ/H4DQM/scripts/link_last_spill.sh $run $spill
    
    if [ "$webDQM" != "localhost" ] ; then
	rsync -aP /data/public_DQM_plots/$run/ $webDQM:/data/public_DQM_plots/$run/
    fi

	#clean unpack file
    [ "${clean}" == "1" ] && rm -rfv ${output}/${run}/${spill}
    

fi