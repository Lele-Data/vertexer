// RecManager.cxx
// Implementation of RecManager.h
//
// Authors: Mario Ciacco & Emanuele Data

#include "RecManager.h"

ClassImp(RecManager);

RecManager *RecManager::fInstance;   // static data member    

RecManager::RecManager(double deltaPhi,double zBinWidth,double deltaZ,double deltaNentries,double zWidth){

}

RecManager::~RecManager(){

}

RecManager *RecManager::GetInstance(double deltaPhi,double zBinWidth,double deltaZ,double deltaNentries,double zWidth){

}

RecManager *RecManager::Destroy(){

}

void RecManager::RunReconstruction(TTree *tree,VTX vert,TClonesArray *hitsFirstLayer,TClonesArray *hitsSecondLayer,TH3D *hZtrueMultRes,TH2D *hZtrueMultNrec,TH2D *hZtrueMultNsim) const{

}

void RecManager::Smearing(Point2D& hit,const double zResol,const double rPhiResol) const{

}

void RecManager::GenerateNoiseSoft(int meanNoiseNumber,Point2D *arrayHit) const{

}