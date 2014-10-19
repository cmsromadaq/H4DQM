#include "interface/StandardIncludes.hpp"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"

#define nPlanesHodo 4
#define nFibersHodo 64
//check if it is true this schema
#define hodoX1 0
#define hodoY1 1
#define hodoX2 2
#define hodoY2 3

#define hodoSmallX 0
#define hodoSmallY 1

#define nPlanesSmallHodo 2
#define nFibersSmallHodo 8

#include <vector>
#define MAX_VEC 1024
using namespace std;

// fwd decl because it's a long list
vector<int> fiberOrderA;
vector<int> fiberOrderB;
vector<int> fiberOrderC;
vector<int> fiberOrderD;
vector<int> fiberOrderI;
void fillFiberOrder();

int main(int argc, char**argv){

string fileName=argv[1];
string outName=argv[2];

TFile *f=TFile::Open(fileName.c_str());
TTree *t=(TTree*)f->Get("H4tree");

UInt_t nPatterns,patternBoard[MAX_VEC],pattern[MAX_VEC],patternChannel[MAX_VEC];

t->SetBranchAddress("nPatterns"		,&nPatterns);
t->SetBranchAddress("patternBoard"	,patternBoard);
t->SetBranchAddress("pattern"	,pattern);
t->SetBranchAddress("patternChannel"	,patternChannel);

fillFiberOrder();
bool fibersOn_[nPlanesHodo][nFibersHodo];

TFile *out=TFile::Open(outName.c_str(),"RECREATE");
TH1D *hX1=new TH1D("X1","X1",64,0,64);
TH1D *hY1=new TH1D("Y1","Y1",64,0,64);
TH1D *hX2=new TH1D("X2","X2",64,0,64);
TH1D *hY2=new TH1D("Y2","Y2",64,0,64);
TH2D *h_X=new TH2D("Xcorr","Xcorr",64,0,64,64,0,64);
TH2D *h_Y=new TH2D("Ycorr","Ycorr",64,0,64,64,0,64);
TH2D *h_X1Y1=new TH2D("X1Y1corr","1corr",64,0,64,64,0,64);
TH2D *h_X2Y2=new TH2D("X2Y2corr","1corr",64,0,64,64,0,64);
TH2D *h_X1Y2=new TH2D("X1Y2corr","X1X2corr",64,0,64,64,0,64);
TH2D *h_X2Y1=new TH2D("X2Y1corr","X1X2corr",64,0,64,64,0,64);

// loop over entries
for (long long iEntry=0;iEntry<t->GetEntries();++iEntry)
{
	t->GetEntry(iEntry);
	if ( (iEntry &1023) ==0 ) printf("Entry=%lld/%lld\n",iEntry,t->GetEntries());
	//if (iEntry >50000 ) { printf("Stopping at 50 000 \n");break;}
	//fill 
	for(UInt_t i=0;i<nPatterns;++i){

		if(patternBoard[i]==0x08030001 || patternBoard[i]==0x08030002){
			int planecouple = (patternBoard[i]==0x08030001) ? 0 : 1;


			for (unsigned int j=0; j<32; j++){

				bool isX=(planecouple ==0);
				bool isY=(planecouple ==1);

				bool is1= ( ( patternChannel[i]/2) == 0);
				bool is2= ( ( patternChannel[i]/2) == 1);
				bool isA= ( ( patternChannel[i]%2) == 0);
				bool isB= ( ( patternChannel[i]%2) == 1);
				bool isI=false;
				bool isC=false;
				bool isD=false;
			
				int offset=0;	
				if (isX and is1 ) 
					{
					if (isA) { 
						isA=false;  //offset=32;
						isB=false;
						isC=false; //offset=32;
						isD=true; 
						}
					else if (isB) { 
						isB=false; //isA=true; 
						isA=false;
						isC=true;
						}
					}

				if (isY and is2)
					{
					if (isB) { isB=false;isD=true; }
					}
				/// -------PIANO 2--------------
				if ( (isY and is1) or (isX and is2) ) 
					{
					//if (isA) { isB=true; isA=false;}
					//else if (isB) { isA=true; isB=false;}
					}

				if (!isA and !isB and !isC and !isD and !isI)
					{
						printf("No Fiber mapping\n");
						exit(0);
					}

				bool thisfibon = (pattern[i]>>j) &0b1;
				//std::vector<int> *fiberorder = ((bool)( patternChannel[i]&0b1) ) ? &fiberOrderB : &fiberOrderA;
				std::vector<int> *fiberorder ;
				int n=0;
				if (isI) { fiberorder = &fiberOrderI;  ++n;}
				if (isA) { fiberorder = &fiberOrderA; ++n;}
				if (isB) { fiberorder = &fiberOrderB; ++n;}
				if (isC) { fiberorder = &fiberOrderC; ++n;}
				if (isD) { fiberorder = &fiberOrderD; ++n;}

				if (fiberorder->size() <31) 
					{
						printf("No complete Mapping isA %d | isB %d |isC %d | isD %d | isI %d\n",int(isA),int(isB),int(isC),int(isD),int(isI));
					}
				if (n != 1) printf("Number of matching is %d\n",n);
				//fibersOn_[2*planecouple+patternChannel[i]/2][fiberorder->at(j)-1]=thisfibon;
				int pos=0;
				if (isY) pos+=2;
				if (is2) pos+=1;
				//if(isB && isX && is1) printf("filling fiber %d\n",fiberorder->at(j)-1);
				fibersOn_[pos][fiberorder->at(j)-1 +offset]=thisfibon;
			}
		}
	}


	int fibersOn=0;
	for(int i=0;i<64;i++){
		if(fibersOn_[hodoX1][i]==1) {
			fibersOn++;
			hX1->Fill(i);
			for(int j=0;j<64;j++)if(fibersOn_[hodoX2][j]==1) h_X->Fill(i,j); //X1X2
			for(int j=0;j<64;j++)if(fibersOn_[hodoY1][j]==1) h_X1Y1->Fill(i,j); //X1Y1
			for(int j=0;j<64;j++)if(fibersOn_[hodoY2][j]==1) h_X1Y2->Fill(i,j); //X1Y2
		}
	}

	fibersOn=0;
	for(int i=0;i<64;i++){
		if(fibersOn_[hodoY1][i]==1){
			fibersOn++;
			hY1->Fill(i);
			for(int j=0;j<64;j++)if(fibersOn_[hodoY2][j]==1) h_Y->Fill(i,j); // Y1Y2
			for(int j=0;j<64;j++)if(fibersOn_[hodoX2][j]==1) h_X2Y1->Fill(j,i); // X2Y1
		}
	}

	fibersOn=0;
	for(int i=0;i<64;i++){
		if(fibersOn_[hodoX2][i]==1) {
			fibersOn++;
			hX2->Fill(i);
			for(int j=0;j<64;j++)if(fibersOn_[hodoY2][j]==1) h_X2Y2->Fill(i,j); //X2Y2
		}
	}

	fibersOn=0;
	for(int i=0;i<64;i++){
		if(fibersOn_[hodoY2][i]==1) 
		{
			fibersOn++;
			hY2->Fill(i);
		}
	}



}// entries loop

out->cd();
hX1->Write();
hY1->Write();
hX2->Write();
hY2->Write();
h_X->Write();
h_Y->Write();
h_X1Y1->Write();
h_X2Y2->Write();
h_X1Y2->Write();
h_X2Y1->Write();

return 0;
}// main


void fillFiberOrder(){

	fiberOrderA.clear();
	fiberOrderB.clear();
	fiberOrderC.clear();
	fiberOrderD.clear();
	fiberOrderI.clear();

	fiberOrderA.push_back(31);
	fiberOrderA.push_back(29);
	fiberOrderA.push_back(23);
	fiberOrderA.push_back(21);
	fiberOrderA.push_back(5);
	fiberOrderA.push_back(7);
	fiberOrderA.push_back(15);
	fiberOrderA.push_back(13);
	fiberOrderA.push_back(1);
	fiberOrderA.push_back(3);
	fiberOrderA.push_back(11);
	fiberOrderA.push_back(9);
	fiberOrderA.push_back(6);
	fiberOrderA.push_back(8);
	fiberOrderA.push_back(16);
	fiberOrderA.push_back(14);
	//
	fiberOrderA.push_back(17);
	fiberOrderA.push_back(27);
	fiberOrderA.push_back(19);
	fiberOrderA.push_back(25);
	fiberOrderA.push_back(24);
	fiberOrderA.push_back(22);
	fiberOrderA.push_back(32);
	fiberOrderA.push_back(30);
	fiberOrderA.push_back(4);
	fiberOrderA.push_back(2);
	fiberOrderA.push_back(12);
	fiberOrderA.push_back(10);
	fiberOrderA.push_back(20);
	fiberOrderA.push_back(18);
	fiberOrderA.push_back(28);
	fiberOrderA.push_back(26);

	fiberOrderB.push_back(54);
	fiberOrderB.push_back(64);
	fiberOrderB.push_back(56);
	fiberOrderB.push_back(62);
	fiberOrderB.push_back(49);
	fiberOrderB.push_back(51);
	fiberOrderB.push_back(59);
	fiberOrderB.push_back(57);
	fiberOrderB.push_back(53);
	fiberOrderB.push_back(55);
	fiberOrderB.push_back(63);
	fiberOrderB.push_back(61);
	fiberOrderB.push_back(45);
	fiberOrderB.push_back(47);
	fiberOrderB.push_back(37);
	fiberOrderB.push_back(39);
	fiberOrderB.push_back(34);
	fiberOrderB.push_back(42);
	fiberOrderB.push_back(36);
	fiberOrderB.push_back(44);
	fiberOrderB.push_back(50);
	fiberOrderB.push_back(52);
	fiberOrderB.push_back(58);
	fiberOrderB.push_back(60);
	fiberOrderB.push_back(38);
	fiberOrderB.push_back(48);
	fiberOrderB.push_back(40);
	fiberOrderB.push_back(46);
	fiberOrderB.push_back(41);
	fiberOrderB.push_back(43);
	fiberOrderB.push_back(33);
	fiberOrderB.push_back(35);

	fiberOrderI.push_back(1);
	fiberOrderI.push_back(2);
	fiberOrderI.push_back(3);
	fiberOrderI.push_back(4);
	fiberOrderI.push_back(5);
	fiberOrderI.push_back(6);
	fiberOrderI.push_back(7);
	fiberOrderI.push_back(8);
	fiberOrderI.push_back(9);
	fiberOrderI.push_back(10);
	fiberOrderI.push_back(11);
	fiberOrderI.push_back(12);
	fiberOrderI.push_back(13);
	fiberOrderI.push_back(14);
	fiberOrderI.push_back(15);
	fiberOrderI.push_back(16);
	fiberOrderI.push_back(17);
	fiberOrderI.push_back(18);
	fiberOrderI.push_back(19);
	fiberOrderI.push_back(20);
	fiberOrderI.push_back(21);
	fiberOrderI.push_back(22);
	fiberOrderI.push_back(23);
	fiberOrderI.push_back(24);
	fiberOrderI.push_back(25);
	fiberOrderI.push_back(26);
	fiberOrderI.push_back(27);
	fiberOrderI.push_back(28);
	fiberOrderI.push_back(29);
	fiberOrderI.push_back(30);
	fiberOrderI.push_back(31);
	fiberOrderI.push_back(32);

	// ORDER A  [A1,A2]-> [A2,A1]	
	fiberOrderC.push_back(17);//12
	fiberOrderC.push_back(27);//22
	fiberOrderC.push_back(19);//15 --14
	fiberOrderC.push_back(25); //21 --20
	fiberOrderC.push_back(24); //20 -- 19
	fiberOrderC.push_back(22); // 17
	fiberOrderC.push_back(32);
	fiberOrderC.push_back(30);//26,25
	fiberOrderC.push_back(4  );// XXXX
	fiberOrderC.push_back(2  );// XXXX
	fiberOrderC.push_back(12);
	fiberOrderC.push_back(10);//6
	fiberOrderC.push_back(20); 
	fiberOrderC.push_back(18);//13
	fiberOrderC.push_back(28);//24,23
	fiberOrderC.push_back(26);//21

	fiberOrderC.push_back(31);//26
	fiberOrderC.push_back(29);//24
	fiberOrderC.push_back(23);
	fiberOrderC.push_back(21);
	fiberOrderC.push_back(5);
	fiberOrderC.push_back(7);
	fiberOrderC.push_back(15);
	fiberOrderC.push_back(13);
	fiberOrderC.push_back(1   ); //XXXX
	fiberOrderC.push_back(3   ); //XXXX
	fiberOrderC.push_back(11);
	fiberOrderC.push_back(9);
	fiberOrderC.push_back(6);
	fiberOrderC.push_back(8);
	fiberOrderC.push_back(16); //12
	fiberOrderC.push_back(14);

	// B [B1,B2] -> [B2,B1]
	fiberOrderD.push_back(34);
	fiberOrderD.push_back(42);
	fiberOrderD.push_back(36);
	fiberOrderD.push_back(44);
	fiberOrderD.push_back(50);
	fiberOrderD.push_back(52);
	fiberOrderD.push_back(58);
	fiberOrderD.push_back(60);
	fiberOrderD.push_back(38);
	fiberOrderD.push_back(48);
	fiberOrderD.push_back(40);
	fiberOrderD.push_back(46);
	fiberOrderD.push_back(41);
	fiberOrderD.push_back(43);
	fiberOrderD.push_back(33);
	fiberOrderD.push_back(35);

	fiberOrderD.push_back(54);
	fiberOrderD.push_back(64);
	fiberOrderD.push_back(56);
	fiberOrderD.push_back(62);
	fiberOrderD.push_back(49);
	fiberOrderD.push_back(51);
	fiberOrderD.push_back(59);
	fiberOrderD.push_back(57);
	fiberOrderD.push_back(53);
	fiberOrderD.push_back(55);
	fiberOrderD.push_back(63);
	fiberOrderD.push_back(61);
	fiberOrderD.push_back(45);
	fiberOrderD.push_back(47);
	fiberOrderD.push_back(37);
	fiberOrderD.push_back(39);
	return;
}

