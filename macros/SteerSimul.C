// SteerSimul.C
// The simulation steering macro
//
// Authors: Mario Ciacco & Emanuele Data

#include <string>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TClonesArray.h>
#include <TStopwatch.h>
#include <Riostream.h>
#include "../src/SimulManager.h"
#include "../cfg/Constants.h"

void SteerSimul(std::string filename="simul",const int nEvent=100000,double nMult=MultMethod::kUnifMult,double nEta=EtaMethod::kUnifEta,double nScat=MultScatMethod::kOnScat,uint seed=42345){
  if(nEvent>1.e9){std::cout<<"Too many events!"<<std::endl; return;}
  const int nHits=kMaxMultiplicity;                       // the maximum number of expoected hits (in Generator.h)
  std::string filename_ext=FILE_DIR+filename+".root";     // filename with *.root extension
  TStopwatch swatch;

  // CREATE FILE AND TREE
  TFile file(filename_ext.c_str(),"RECREATE");            // open a file (write mode)
  TTree *tree=new TTree(SimulTreeName,"tree");            // create a tree

  // MEMORY LOCATIONS MAPPED TO TREE
  static VTX vert;                                        // create a structure containing information about the generated mult. & vertex
  TClonesArray *ArrayFirstLayer=new TClonesArray("Point2D",nHits); // ROOT array containing objects of the same class
  TClonesArray *ArraySecondLayer=new TClonesArray("Point2D",nHits);// ROOT array containing objects of the same class
  TClonesArray& HitsFirstLayer=*ArrayFirstLayer;          // reference to first layer TClonesArray
  TClonesArray& HitsSecondLayer=*ArraySecondLayer;        // reference to second layer TClonesArray

  // CONNECT TREE BRANCHES TO MEMORY LOCATIONS
  tree->Branch(SimVertBranchName,&vert.X,"X/D:Y:Z:mult"); // connect branch to the first memory location; specify types
  tree->Branch(SimHitFirstBranchName,&ArrayFirstLayer);   // connect branch to the TClonesArray of Point2D (first layer)
  tree->Branch(SimHitSecondBranchName,&ArraySecondLayer); // connect branch to the TClonesArray of Point2D (second layer)

  // INSTANTIATE THE BEAM PIPE AND DETECTOR
  BeamPipe *bpipe=new BeamPipe(kBpipeRadius,kBpipeThick,kRadLengthBe); // cm, cm
  Layer *layers[2];
  layers[0]=new Layer(kFirstLayerRadius,kFirstLayerThick,kFirstLayerLength,kRadLengthSi); // cm, cm
  layers[1]=new Layer(kSecondLayerRadius,kSecondLayerThick,kSecondLayerLength,kRadLengthSi); // cm, cm

  // GET THE SIMULATION MANAGER INSTANCE AND RUN THE SIMULATION
  SimulManager *manager=SimulManager::GetInstance(nEvent,nMult,nEta,nScat,seed);
  swatch.Start();
  manager->RunSimulation(tree,vert,HitsFirstLayer,HitsSecondLayer,bpipe,layers);
  swatch.Stop();
  swatch.Print("m");
  manager=SimulManager::Destroy();

  // WRITE AND CLOSE FILE
  file.Write();
  file.Close();
}