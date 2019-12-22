// SimulManager.cxx
// Implementation of SimulManager.h
//
// Authors: Mario Ciacco & Emanuele Data

#include "SimulManager.h"

// default constructor
SimulManager::SimulManager():TObject(),
fEvent(0),
fMultMethod(0),
fEtaMethod(0),
fMultScatMethod(0){
  gen=NULL;
  prop=NULL;
}

// standard constructor
SimulManager::SimulManager(int nEvent,int nMult,int nEta,int nScat,uint seed=12345):TObject(),
fEvent(nEvent),
fMultMethod(nMult),
fEtaMethod(nEta),
fMultScatMethod(nScat){
  gen=Generator::GetInstance();
  prop=Propagator::GetInstance();
}

SimulManager::~SimulManager(){
}

void SimulManager::RunSimulation(TTree *tree,TClonesArray& layerArr1,TClonesArray& layerArr2,BeamPipe *bpipe,Layer *layers[2]){

}