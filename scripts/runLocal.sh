#!/bin/bash

input="/tmp/kyee"
output="/tmp/kyee/output"
run="7290"
#run="5860"
spill="2"
prescale=1

TEMP=`getopt -o i:o:r:s:p: --long input:,output:,run:,spill:prescale: -n 'runDQM.sh' -- "$@"`
if [ $? != 0 ] ; then echo "Options are wrong..." >&2 ; exit 1 ; fi

eval set -- "$TEMP"


while true; do
case "$1" in
-i | --input ) input="$2"; shift 2 ;;
-o | --output ) output="$2"; shift 2 ;;
-r | --run ) run="$2"; shift 2;;
-s | --spill ) spill="$2"; shift 2;;
-p | --prescale ) prescale=$2; shift 2;;
-- ) shift; break ;;
* ) break ;;
esac
done

#/home/cmsdaq/DAQ/H4DQM/bin/unpack -i $input  -o $output -r $run -s $spill -p $prescale
#/home/cmsdaq/DAQ/H4DQM/bin/plotterTotal -i $output -o $output  -r $run -s $spill -t beam -I integrated.root 

#for runtype in beam ped led;do
for runtype in beam;do
echo $runtype
#local path
echo $input $output $run $spill $runtype 

    ./bin/plotterTotal -i $input -o $output  -r $run -s $spill -t $runtype -I "integrated.root" 
    
#!/bin/bash
#./bin/plotterTotal -i . -o . -r 318 -s 1 -t beam -I integratedBeam.root 
#./bin/plotterTotal -i . -o . -r 1504 -s 2 -t beam -I integrated.root 

    cd $output/$run/$spill/$dir/$runtype
    mkdir hodo
    mkdir DAQ
    mkdir ADC
    mkdir TDC 
    mkdir new
    mkdir root
    mkdir digitizer

    mv total/hodo_* hodo/
    mv total/DAQStatus_* DAQ/
    mv total/ADC_* ADC/
    mv total/TDC_* TDC/
    mv total/digitizer_new* new/
    mv total/digitizer_* digitizer/
    mv total/*.root root/

    rm -r total
    cd -

done
