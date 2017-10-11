Below are instructions on how to update the current core H4DQM (https://github.com/cmsromadaq/H4DQM) in order to work with newer runs as well as how to locally test the DQM code(s) from lxplus machines. 

To clone the core github repository run the following lines:

    git clone https://github.com/cmsromadaq/H4DQM.git

Pull from the repository changes other people committed:

    git pull

Change code, and commit

    git add new_file
    git commit -m "comment" new_file
    git push

Update the Makefile. It may not compile if this is not done. Change the LDFLAGS line to the below. 

    LDFLAGS         =-L/cvmfs/cms.cern.ch/slc6_amd64_gcc600/cms/cmssw/CMSSW_8_1_0/external/slc6_amd64_gcc600/ -lz -lm

In the scripts folder, if you want to run runLocal.sh on your lxplus machine, the code needs to be editted as well. 

In the first few lines edit to fit your needs (for example, a /tmp folder). These can vary, but mine look like: 

    input="/tmp/kyee"
    output="/tmp/kyee/output"
    run="7290"
    spill="2"

Line 33 must be revised to: 

    ./bin/plotterTotal -i $input -o $output -r $run -s $spill -t $runtype -I "integrated.root" 

In addition, 

To run code in the scripts folder, run the following command:

    ./scripts/CODE.sh

Depending on what you change the input and output to, you'll need to make designated folders and copy the data you want into those folders:

For runs from July 2017 between ~6000 and 6300, they are in a slightly different directory
    mkdir /tmp/kyee/6103
    /eos/cms/store/group/dpg_ecal/comm_ecal/upgrade/testbeam/ECALTB_H4_Jun2017/data/raw/DataTree/6103/2.root /tmp/kyee/6103

For newer runs,
    mkdir /tmp/kyee/7290
    cp  /eos/cms/store/group/dpg_ecal/comm_ecal/upgrade/testbeam/ECALTB_H4_Jun2017/raw/DataTree/7290/2.root /tmp/kyee/7290

For older runs, the eos directory is differentt
    mkdir /tmp/kyee/5860
    cp  /eos/cms/store/group/dpg_ecal/comm_ecal/upgrade/testbeam/ECALTB_H4_Jul2016/raw/DataTree/5860/2.root /tmp/kyee/5860

And then in the www folder, to make the page visible
    cp /eos/user/k/kyee/www/index.php /eos/user/k/kyee/www/DQMoutput/7290/2/beam/new

In order to work on newer runs, the line 53 in interface/Event.hpp needs to be editted from float digiSampleValue[MAX_DIGI_SAMPLES] to UShort_t digiSampleValue[MAX_DIGI_SAMPLES]

After all of this, the code should work. Compile in H4DQM with 'make' and then run codes from there. 

Note: If you make changes to the C++ code (not including the .sh codes as far as I know), you will need to recompile by executing 'make clean' followed by 'make'. 
