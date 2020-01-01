// SteerRec.C
// The reconstruction steering macro
//
// Authors: Mario Ciacco & Emanuele Data

#include <Riostream.h>
#include <string>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TClonesArray.h>
#include "../src/SimulManager.h"
#include "../src/RecManager.h"
#include "../cfg/Constants.h"

void SteerRec(std::string inFilename="simul",std::string outFilename="recResult",double deltaPhi=kDeltaPhi,double zBinWidth=kZbinWidth,double deltaZ=kDeltaZ,double zWidth=kZwidth){
  std::string inFilename_ext=FILE_DIR+inFilename+".root";      // filename with *.root extension
  std::string outFilename_ext=FILE_DIR+outFilename+".root";    // filename with *.root extension
  
  // DECLARE MEMORY LOCATION
  static VTX vert={0,0,0,0};
  TClonesArray *hitsFirstLayer=new TClonesArray("Point2D",100);
  TClonesArray *hitsSecondLayer=new TClonesArray("Point2D",100);

  // OPEN FILE AND GET TREE
  TFile inFile(inFilename_ext.c_str());
  if(!inFile.IsOpen()){
    std::cout<<"No input file!"<<std::endl;
    return;
  }
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
  layers[0]=new Layer(kFirstLayerRadius,kFirstLayerThick,kFirstLayerLength,kZresol,kRphiResol); // cm
  layers[1]=new Layer(kSecondLayerRadius,kSecondLayerThick,kSecondLayerLength,kZresol,kRphiResol); // cm

  // INSTANTIATE RECONSTRUCTION MANAGER AND RUN SIMULATION
  RecManager *manager=RecManager::GetInstance(deltaPhi,zBinWidth,deltaZ,zWidth);
  manager->RunReconstruction(tree,vert,hitsFirstLayer,hitsSecondLayer,layers,hZtrueMultRes,hZtrueMultNrec,hZtrueMultNsim);
  manager=RecManager::Destroy();
  
  // WRITE AND CLOSE FILE
  outFile.Write();
  outFile.Close();
  inFile.Close();
}