#!/bin/bash

function modification_time(){
	DATE="$(date -r $1 +%s )"
	echo $DATE
}


RUNS=$(ls | grep '^[0-9]*$' | awk '{if ($1> 154) print $1}')

CDIR=$PWD

for run in $RUNS; do
	cd $run
	echo "#### ------- ENTERING RUN $run ----------"
	rundir=$( ls | grep '^[0-9]*$' )
	files=$( echo $rundir | tr -s ' ' '\n'| while read dir ; do echo "$dir/dqmPlotstotal.root "; done)
	#echo "#### FILES=$files"
	isModified=0
	modTime=0
	[ -f dqmPlotstotal.root ] || isModified=1 ;
	[ $isModified -eq 0 ] && { 
				modTime=$(modification_time dqmPlotstotal.root );
				#echo "## modTime $modTime"
				for file in $files ; 
					do
					fileTime=$(modification_time $file )
					#echo "Times: file $file: $fileTime  vs $modTime"
					[ $fileTime -gt $modTime ] && isModified=1
					#[ $isModified -eq 0 ] || echo "is modified"
					done
				}
	echo "#### isModified=$isModified"
	[ $isModified -eq 0 ] || hadd -f dqmPlotstotal.root  $files ; 
	cd $CDIR
done

