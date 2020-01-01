// SteerSimul.C
// The simulation steering macro
//
// Authors: Mario Ciacco & Emanuele Data

#include <string>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TClonesArray.h>
#include "../src/SimulManager.h"
#include "../cfg/Constants.h"

void SteerSimul(std::string filename="simul",const char *treename="tree",const int nEvent=10000,double nMult=MultMethod::kUnifMult,double nEta=EtaMethod::kUnifEta,double nScat=MultScatMethod::kOnScat,double seed=42345){
  const int nHits=kMaxMultiplicity;                  // the maximum number of expoected hits (in Generator.h)
  std::string filename_ext=FILE_DIR+filename+".root"; // filename with *.root extension
  
  // CREATE FILE AND TREE
  TFile file(filename_ext.c_str(),"RECREATE");       // open a file (write mode)
  TTree *tree=new TTree("T",treename);               // create a tree

  // MEMORY LOCATIONS MAPPED TO TREE
  static VTX vert;                                   // create a structure containing information about the generated mult. & vertex
  TClonesArray *ArrayFirstLayer=new TClonesArray("Point2D",nHits); // ROOT array containing objects of the same class
  TClonesArray *ArraySecondLayer=new TClonesArray("Point2D",nHits);// ROOT array containing objects of the same class
  TClonesArray& HitsFirstLayer=*ArrayFirstLayer;     // reference to first layer TClonesArray
  TClonesArray& HitsSecondLayer=*ArraySecondLayer;   // reference to second layer TClonesArray

  // CONNECT TREE BRANCHES TO MEMORY LOCATIONS
  tree->Branch("vert",&vert.X,"X/D:Y:Z:mult");       // connect branch to the first memory location; specify types
  tree->Branch("hitsFirstLayer",&ArrayFirstLayer);   // connect branch to the TClonesArray of Point2D (first layer)
  tree->Branch("hitsSecondLayer",&ArraySecondLayer); // connect branch to the TClonesArray of Point2D (second layer)

  // INSTANTIATE THE BEAM PIPE AND DETECTOR
  BeamPipe *bpipe=new BeamPipe(kBpipeRadius,kBpipeThick); // cm
  Layer *layers[2];
  layers[0]=new Layer(kFirstLayerRadius,kFirstLayerThick,kFirstLayerLength); // cm
  layers[1]=new Layer(kSecondLayerRadius,kSecondLayerThick,kSecondLayerLength); // cm

  // GET THE SIMULATION MANAGER INSTANCE AND RUN THE SIMULATION
  SimulManager *manager=SimulManager::GetInstance(nEvent,nMult,nEta,nScat,seed);
  manager->RunSimulation(tree,vert,HitsFirstLayer,HitsSecondLayer,bpipe,layers);
  manager=SimulManager::Destroy();

  // WRITE AND CLOSE FILE
  file.Write();
  file.Close();
}
