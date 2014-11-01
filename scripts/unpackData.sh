#!/bin/bash

launchDir=`pwd`
input="/tmp/"
output="/tmp"
log="/tmp/${USER}"
queue="cmsan"
suffix="raw"
run="0"
spill="0"
dryrun=0
merge=0
batch=0
clean=0

waitForBatchJobs() {
    jobList=( ${jobList} )
    nJobs=${#jobList[@]}
    echo "Batch jobs are ${nJobs}"
    jobsDone=()
    nJobsDone=${#jobsDone[@]}
    while [ "${nJobsDone}" -ne "${nJobs}" ]; do
	jobsDone=""
	for aJob in ${jobList[@]}; do
	    jobStatus=`bjobs ${aJob} | sed 1d | awk '{print $3}'`
#	    echo "${aJob} ==> ${jobStatus}"
	    if [ "${jobStatus}" == "DONE" ]; then
		jobsDone="${aJob} ${jobsDone}"
	    fi
	done
	jobsDone=( ${jobsDone} )
	nJobsDone=${#jobsDone[@]}
#	echo "nJobsDone ${nJobsDone}"
	sleep 1
    done
}

TEMP=`getopt -o i:o:r:s:q:l:mbdc --long input:,output:,run:,spill:,queue:,log:,merge,batch,dryrun,clean -n 'unpackData.sh' -- "$@"`
if [ $? != 0 ] ; then echo "Options are wrong..." >&2 ; exit 1 ; fi

eval set -- "$TEMP"

while true; do
case "$1" in
-i | --input ) input="$2"; shift 2 ;;
-o | --output ) output="$2"; shift 2 ;;
-l | --log ) log="$2"; shift 2 ;;
-r | --run ) run="$2"; shift 2;;
-s | --spill ) spill="$2"; shift 2;;
-q | --queue ) queue="$2"; shift 2;;
-m | --merge ) merge=1; shift;;
-b | --batch ) batch=1; shift;;
-d | --dryrun ) dryrun=1; shift;;
-c | --clean ) clean=1; shift;;
-- ) shift; break ;;
* ) break ;;
esac
done

spillList="$spill"
[ "$spill" == "-1" ] && spillList=`find ${input}/${run} -regextype 'posix-basic' -regex ".*\.${suffix}" -type f | xargs -I {} basename {} .${suffix} | awk '{printf "%d ",$1}END{printf"\n"}' | sort -n`
spillList=( $spillList )
echo "===>unpackData: input ${input}, run ${run}, spills ${spillList[@]}"

runDir=`echo ${launchDir} | awk -F '/H4DQM' '{printf "%s/H4DQM\n",$1}'`

[ "${dryrun}" == "1" ] ||  mkdir -p ${output}/${run};  mkdir -p ${log}/${run}

jobList=""
for spills in ${spillList[@]}; do
    unpackCommand="${runDir}/bin/unpack -i ${input} -r ${run} -s ${spills} -o ${output}"
    command=""
    [ "${batch}" == "1" ] && command="bsub -q ${queue} -o ${log}/${run}/unpack_${spills}.log -e ${log}/${run}/unpack_${spills}.log"
    command="${command} ${unpackCommand}"
    echo ${command}
    [ "${dryrun}" == "1" ] && continue
    jobId=`${command} 2>&1`
    job=`echo ${jobId} | awk '{print $2}' | sed -e 's%<%%g' | sed -e 's%>%%g'`
    jobList="${job} ${jobList}"
#    echo "==>JOBS ${jobList}"
done

[ ${merge} == 1 ] || exit 0
[ ${batch} == 0 ] || echo "Waiting for batch jobs to finish..."; waitForBatchJobs
echo "Batch jobs completed successfully. Now merging all output root files"
hadd -f ${output}/${run}_RAW_MERGED.root `find ${output}/${run}/ -regextype 'posix-basic' -regex ".*\.root" -type f`
#need to check if we need some more intelligent merging to avoid too big files. In base some bookeping is required
[ ${clean} == 0 ] || echo "Cleaning directory ${output}/${run}"; rm -rf ${output}/${run} #cleaning the output dir if requested after merging
echo "Unpacking of run ${run} completed"