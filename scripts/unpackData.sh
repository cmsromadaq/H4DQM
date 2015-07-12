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
eos=0
cmssw=0
cmsswDir=/cvmfs/cms.cern.ch/slc6_amd64_gcc481/cms/cmssw/CMSSW_7_2_5/src/

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

mergeSpills()
{
    unpackedSpills=`find ${output}/${run}/ -regextype 'posix-basic' -regex ".*\.root" -type f | xargs -I {} basename {} .root | sort -n`
    unpackedSpills=( $unpackedSpills )

    maxsize=2000000

    mergeIndex=1
    mergedSize=0

    spillsToMerge=""
    nSpills=${#unpackedSpills[@]}

    spillIndex=0

    mkdir -p ${output}/MERGED/${run}/

    for aSpill in ${unpackedSpills[@]}; do

	let spillIndex+=1

	spillSize=`ls -la ${output}/${run}/${aSpill}.root | awk '{print $5}'`
	let spillSize=spillSize/1024
	let mergedSize+=spillSize
#	echo "Merged Size ${mergedSize} maxSize ${maxsize} nSpills ${nSpills} spillIndex ${spillIndex}"
	spillsToMerge="${output}/${run}/${aSpill}.root ${spillsToMerge}"
	if [[ ${mergedSize} -ge ${maxsize} || ${spillIndex} -ge ${nSpills} ]]; then 
	    echo "${spillsToMerge}" > ${output}/MERGED/${run}/RAW_MERGED_${mergeIndex}.list
	    hadd -f ${output}/MERGED/${run}/RAW_MERGED_${mergeIndex}.root ${spillsToMerge} 
	    spillsToMerge=""
	    mergedSize=0
	    let mergeIndex+=1
	fi
    done
}

launchJob()
{
    #prepareJobFile
    jobFile=${log}/${run}/${spills}.sh
    logFile=${log}/${run}/unpack_${spills}.log

    [ -e ${logFile} ] && mv ${logFile} ${logFile}.old
    rm -rf ${jobFile}
    touch ${jobFile}

    echo "#!/bin/sh" >>  ${jobFile}
    [ "${cmssw}" == "1" ] && echo "cd ${cmsswDir}; eval \`scram runtime -sh\`; cd -" >>  ${jobFile}

    [ "${eos}" == "1" ] &&  echo "mkdir -p /tmp/${run}; ${eosCommand} cp ${input}/${run}/${spills}.${suffix} /tmp/${run}/${spills}.${suffix}; mkdir -p /tmp/DataTree" >> ${jobFile}

    jobInputDir=${input}
    jobOutputDir=${output}
    
    #stagein for EOS
    [ "${eos}" == "1" ] &&  jobInputDir=/tmp; jobOutputDir=/tmp/DataTree
    unpackCommand="${runDir}/bin/unpack -i ${jobInputDir} -r ${run} -s ${spills} -o ${jobOutputDir}"
    echo "${unpackCommand}" >> ${jobFile}
    #stageout for EOS
    [ "${eos}" == "1" ] && echo "${eosCommand} cp ${jobOutputDir}/${run}/${spills}.root ${output}/${run}/${spills}.root"  >> ${jobFile} 

    #launch job
    command="${command} < ${jobFile}"
    [ "${batch}" == "1" ] && command="bsub -q ${queue} -o ${logFile} -e ${logFile} /bin/bash ${jobFile}"
    echo ${command}
    [ "${dryrun}" == "1" ] && continue
    jobId=`${command}`
    job=`echo ${jobId} | awk '{print $2}' | sed -e 's%<%%g' | sed -e 's%>%%g'`
    echo "${job} submitted"
    jobList="${job} ${jobList}"
}

TEMP=`getopt -o i:o:r:s:q:l:mbdce --long input:,output:,run:,spill:,queue:,log:,merge,batch,dryrun,clean,eos,cmssw -n 'unpackData.sh' -- "$@"`
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
-e | --eos ) eos=1; shift;;
--cmssw ) cmssw=1; shift;;
-- ) shift; break ;;
* ) break ;;
esac
done

eosCommand=""
[ "$eos" == "1" ] && eosCommand="/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select"

spillList="$spill"
if [ "$spill" == "-1" ] && [ "$eos" == "0" ]; then echo "Checking spill files in $input"; spillList=`find ${input}/${run} -regextype 'posix-basic' -regex ".*\.${suffix}" -type f | xargs -I {} basename {} .${suffix} | awk '{printf "%d ",$1}END{printf"\n"}' | sort -n`
fi
if [ "$spill" == "-1" ] && [ "$eos" == "1" ]; then echo "Checking spill files in EOS dir $input"; spillList=`${eosCommand} find -f ${input}/${run} | grep ".${suffix}" | xargs -I {} basename {} .${suffix} | awk '{printf "%d ",$1}END{printf"\n"}' | sort -n`
fi
spillList=( $spillList )
echo "===> unpackData: input ${input}, run ${run}, spills ${spillList[@]}"

runDir=`echo ${launchDir} | awk -F '/H4DQM' '{printf "%s/H4DQM\n",$1}'`

[ "${dryrun}" == "1" ] ||  ${eosCommand} mkdir -p ${output}/${run};  mkdir -p ${log}/${run}

jobList=""
for spills in ${spillList[@]}; do
    #prepare job 
    launchJob
done

[ ${merge} == 1 ] || exit 0
[ ${batch} == 0 ] || echo "Waiting for batch jobs to finish..."; waitForBatchJobs
echo "Batch jobs completed successfully. Now merging all output root files"
mergeSpills
[ ${clean} == 0 ] || echo "Cleaning directory ${output}/${run}"; rm -rf ${output}/${run} #cleaning the output dir if requested after merging
echo "Unpacking of run ${run} completed"