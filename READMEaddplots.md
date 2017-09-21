This README describes how to add and modify plots made by ./scripts/runLocal.sh
This is done by editting the src/plotterTools.cpp code. 

The DQM has a huge main class divided into groups, ADC, TDC, digitizer, and hodoscope. It works on unpacked root files (like those stored in the eos)

After line 1900 you can add a plot like so,

addPlot(1,"NEWPLOT", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F 
The first "1" means "plot it on a canvas and save it".

The next part depends on what you are looking for, but past line 746 you will have something along the lines of:

else if(name=="NEWPLOT"){
   varplots[name]->Fill(FILLVARIABLE,1.);
}

Note: be careful of the brackets. Just make sure the previous statement has brackets and the else if statement is closed. 

There are more complicated plots, some can be included within the iterator for loop,  for (std::map<TString,varPlot<float>*>::iterator it=varplots.begin();it!=varplots.end();++it)

   varplots[Form("%s_charge_integrated_vs_TDCrecoX",thisname.Data())]->Fill(tdc_recox,it->second->waveform->charge_integrated(0,900));

Or a simple 2D fill:

   varplots[name]->Fill2D(tdc_recox,tdc_recoy,1.);

The "Integrated" plots update with every spill. 

During data taking, ./scripts/runDQM.sh can be used (however, I myself haven't tried this yet). 


