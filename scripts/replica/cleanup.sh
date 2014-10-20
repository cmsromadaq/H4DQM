#!/bin/bash

LOCDATADIR=/home/cmsdaq/scripts-replica/test

DATETAG=$(date +%s)
LOGFILE=cleanup_log_${DATETAG}.txt

for file in $(find -H ${LOCDATADIR} -type f -mindepth 1 | grep '.raw' | grep -v ".transfer.done.")
do
if [[ -f ${file}.transfer.done.T2_IT_Rome && -f ${file}.transfer.done.T3_CH_PSI ]]
    then
    rm ${file}
    rm ${file}.transfer.done.T2_IT_Rome
    rm ${file}.transfer.done.T3_CH_PSI
    echo ${file} >> ${LOGFILE}
    fi
done


for mydir in $(find -H ${LOCDATADIR} -type d -mindepth 1)
do
    if [[ $(ls ${mydir} | wc -l) -eq 0 ]]
    then
	rmdir ${mydir}
    fi
done

