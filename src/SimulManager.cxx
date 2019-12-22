// SimulManager.cxx
// Implementation of SimulManager.h
//
// Authors: Mario Ciacco & Emanuele Data

#include <TRandom3.h>
#include "SimulManager.h"

ClassImp(SimulManager)

SimulManager *SimulManager::fInstance=NULL; // static data member

SimulManager::SimulManager(int nEvent,int nMult,int nEta,int nScat,uint seed):TObject(),
fEvent(nEvent),
fMultMethod(nMult),
fEtaMethod(nEta),
fMultScatMethod(nScat){
  gRandom->SetSeed(seed);
  gen=Generator::GetInstance();
  prop=Propagator::GetInstance();
}

SimulManager::~SimulManager(){
  gen=Generator::Destroy();
  prop=Propagator::Destroy();
}

SimulManager *SimulManager::GetInstance(int nEvent,int nMult,int nEta,int nScat,uint seed){
  if(!SimulManager::fInstance) fInstance=new SimulManager(nEvent,nMult,nEta,nScat,seed);
  return fInstance;
}

SimulManager *SimulManager::Destroy(){
  if(SimulManager::fInstance) delete fInstance;
  fInstance=NULL;
  return fInstance;
}

void SimulManager::RunSimulation(TTree *tree,TClonesArray& layerArr1,TClonesArray& layerArr2,BeamPipe *bpipe,Layer *layers[2]){

}