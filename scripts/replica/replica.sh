#!/bin/bash

LOCDATADIR=/rawdata
REMSITE=T3_CH_PSI
REMDATADIR=/store/.....
# the data will be copied in ${REMDATADIR}/${LOCDATADIR}/....


REPLICA=/opt/data-replica/data_replica.py
REPLICA_OPTIONS="--from-site=LOCAL --to-site=${REMSITE} --delete --recreate-subdirs"
DATETAG=$(date +%s)
LOGNAME=data_replica_${DATETAG}
WDIR=${PWD}

if [ ! -f ${REPLICA} ]
then
    echo 'Install data_replica.py first!!!'
    exit 1
fi


for file in $(find -H ${LOCDATADIR} -type f -mindepth 1 | grep '.raw' | grep -v '.transfer.done.')
do
    echo ${file} >> totransfer_${DATETAG}.txt
done

${REPLICA} ${REPLICA_OPTIONS} --logfile=${LOGNAME}.txt totransfer_${DATETAG}.txt ${REMDATADIR}
NFAILED=$?

SUCCESSFILES=${LOGNAME}_successList.log
FAILEDFILES=${LOGNAME}_failedList.log

for file in $(cat ${SUCCESSFILES})
do
    touch ${file}.transfer.done.${REMSITE}
done

if [ -f ${FAILEDFILES} ]
then
    for file in $(cat ${FAILEDFILES})
    do
	echo "TRANSFER FAILED: ${file}"
    done
fi
