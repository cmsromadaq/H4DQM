#!/bin/bash

### crate symlinks in run directory to last spill plots

run=${1}
spill=${2}

for file in `ls /data/public_DQM_plots/${run}/${spill}/ | grep '.png\|.txt\|.root\|.pdf'`;
do
    unlink /data/public_DQM_plots/$run/$file
    ln -s /data/public_DQM_plots/$run/$spill/$file /data/public_DQM_plots/$run/$file 
done

