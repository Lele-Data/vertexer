// SteerRec.C
// The reconstruction steering macro
//
// Authors: Mario Ciacco & Emanuele Data

#include <string>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TClonesArray.h>
#include "../src/SimulManager.h"
#include "../src/RecManager.h"

#ifndef CONSTANT
#define CONSTANT
const double kResMin=-1000.; // um
const double kResMax=1000.;  // um
const int kNresBinLim=501;
const int kNzTrueBins=18;
const int kNmultBins=12;
const double kZtrueBins[]={-30.0,-27.0,-25.0,-23.0,-20.0,-15.0,-10.0,-5.0,-2.5,0.0,2.5,5.0,10.0,15.0,20.0,23.0,25.0,27.0,30.0};
const double kMultBins[]={2.5,3.5,4.5,5.5,7.5,9.5,14.5,24.5,34.5,44.5,54.5,70.0,100.0};
#endif // CONSTANT

const double kDeltaPhi=0.0005;
const double kZbinWidth=0.01;
const double kDeltaZ=0.05;
const double kDeltaNentries=2.;
const double kZwidth=0.1;

void SteerRec(std::string inFilename="simul",std::string outFilename="recResult",double deltaPhi=kDeltaPhi,double zBinWidth=kZbinWidth,double deltaZ=kDeltaZ,double deltaNentries=kDeltaNentries,double zWidth=kZwidth){
  std::string inFilename_ext=inFilename+".root";      // filename with *.root extension
  std::string outFilename_ext=outFilename+".root";    // filename with *.root extension
  
  // DECLARE MEMORY LOCATION
  static VTX vert;
  TClonesArray *hitsFirstLayer=new TClonesArray("Point2D",100);
  TClonesArray *hitsSecondLayer=new TClonesArray("Point2D",100);

  // OPEN FILE AND GET TREE
  TFile inFile(inFilename_ext.c_str());
  TFile outFile(outFilename_ext.c_str(),"RECREATE");  // open a file (write mode)
  TTree *tree=(TTree*)inFile.Get("T");
  
  // GET TREE BRANCHES FROM EXISTING TREE
  TBranch *bVertMult=tree->GetBranch("vert");
  TBranch *bFirstLayer=tree->GetBranch("hitsFirstLayer");
  TBranch *bSecondLayer=tree->GetBranch("hitsSecondLayer");

  // SET ADDRESSES OF BRANCHES
  bVertMult->SetAddress(&vert.X);
  bVertMult->SetAutoDelete(kTRUE);
  bFirstLayer->SetAddress(&hitsFirstLayer);
  bFirstLayer->SetAutoDelete(kTRUE);
  bSecondLayer->SetAddress(&hitsSecondLayer);
  bSecondLayer->SetAutoDelete(kTRUE);

  // INSTANTIATE HISTOGRAMS
  double ResBins[kNresBinLim];                        // define residues binning
  double ResStep=(kResMax-kResMin)/(kNresBinLim-1.);
  for(int iBin=0;iBin<kNresBinLim;++iBin)ResBins[iBin]=kResMin+ResStep*iBin;
  TH3D *hZtrueMultRes=new TH3D("hZtrueMultRes","hZtrueMultRes",kNzTrueBins,kZtrueBins,kNmultBins,kMultBins,kNresBinLim-1,ResBins);
  TH2D *hZtrueMultNrec=new TH2D("hZtrueMultNrec","hZtrueMultNrec",kNzTrueBins,kZtrueBins,kNmultBins,kMultBins);
  TH2D *hZtrueMultNsim=new TH2D("hZtrueMultNsim","hZtrueMultNsim",kNzTrueBins,kZtrueBins,kNmultBins,kMultBins);
  
  // INSTANTIATE LAYERS
  Layer *layers[2];
  layers[0]=new Layer(4.,0.2,27.,kZresol,kRphiResol); // cm
  layers[1]=new Layer(7.,0.0,27.,kZresol,kRphiResol); // cm

  // INSTANTIATE RECONSTRUCTION MANAGER AND RUN SIMULATION
  RecManager *manager=RecManager::GetInstance(deltaPhi,zBinWidth,deltaZ,deltaNentries,zWidth);
  manager->RunReconstruction(tree,vert,hitsFirstLayer,hitsSecondLayer,layers,hZtrueMultRes,hZtrueMultNrec,hZtrueMultNsim);
  manager=RecManager::Destroy();
  
  // WRITE AND CLOSE FILE
  outFile.Write();
  outFile.Close();
}