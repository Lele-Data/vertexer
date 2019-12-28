// RecManager.cxx
// Implementation of RecManager.h
//
// Authors: Mario Ciacco & Emanuele Data

#include "RecManager.h"
#include <TRandom3.h>
#include <TMath.h>
ClassImp(RecManager);

RecManager *RecManager::fInstance=NULL;   // static data member    


RecManager::RecManager(double deltaPhi,double zBinWidth,double deltaZ,double deltaNentries,double zWidth):TObject(),
fDeltaPhi(deltaPhi),                                                                                                 // phi slice width
fZBinWidth(zBinWidth),                                                                                                 // bin width  
fDeltaZ(deltaZ),                                                                                                   // horizontal cut
fDeltaNentries(deltaNentries),                                                                                             // vertical cut
fZWidth(zWidth){
  vertxr=Vertexer::GetInstance();
}

RecManager::~RecManager(){
  vertxr=Vertexer::Destroy();
}

RecManager *RecManager::GetInstance(double deltaPhi,double zBinWidth,double deltaZ,double deltaNentries,double zWidth){
  if(!RecManager::fInstance) fInstance=new RecManager(deltaPhi,zBinWidth,deltaZ,deltaNentries,zWidth);
  return fInstance;
}

RecManager *RecManager::Destroy(){
  if(RecManager::fInstance) delete fInstance;
  fInstance=NULL;
  return fInstance;
}

void RecManager::RunReconstruction(TTree *tree,VTX vert,TClonesArray *hitsFirstLayer,TClonesArray *hitsSecondLayer,Layer *layer[2],TH3D *hZtrueMultRes,TH2D *hZtrueMultNrec,TH2D *hZtrueMultNsim) const{
  for(int iEvent=0;iEvent<tree->GetEntries();++iEvent){ // loop over events
    tree->GetEvent(iEvent);

    int nHitLayer1=hitsFirstLayer->GetEntries();
    int nHitLayer2=hitsSecondLayer->GetEntries();
    int nHitNoiseLayer1=GenerateNhitNoise(kMeanNnoise);            // generate the number of false hits
    int nHitNoiseLayer2=GenerateNhitNoise(kMeanNnoise);
    int nHitTotLayer1=nHitLayer1+nHitNoiseLayer1;                  // total number of hits
    int nHitTotLayer2=nHitLayer2+nHitNoiseLayer2;
    Point2D *arrayHitFirstLayer=new Point2D[nHitTotLayer1];        // allocate memory for hit array
    Point2D *arrayHitSecondLayer=new Point2D[nHitTotLayer2];

    // smearing on hits of first and second layer 
    int hitCounterL1=0;
    int hitCounterL2=0;
    for(int iHitL1=0;iHitL1<nHitLayer1;iHitL1++){ // loop over hits of first layer
      Point2D *hitTmp=(Point2D*)hitsFirstLayer->At(iHitL1);
      Smearing(*hitTmp,kzResol,krPhiResol,layer[1]);
      arrayHitFirstLayer[iHitL1]=*hitTmp;
      hitCounterL1++;
    } // end of loop over hits of first layer
    for(int iHitL2=0;iHitL2<nHitLayer2;iHitL2++){ // loop over hits of second layer
      Point2D *hitTmp=(Point2D*)hitsSecondLayer->At(iHitL2);
      Smearing(*hitTmp,kzResol,krPhiResol,layer[2]);
      arrayHitFirstLayer[iHitL2]=*hitTmp;
      hitCounterL2++;
    } // end of loop over hits of second layer
    GenerateHitNoiseSoft(nHitTotLayer1,arrayHitFirstLayer,hitCounterL1); // generation of false hits for layer 1
    GenerateHitNoiseSoft(nHitTotLayer1,arrayHitFirstLayer,hitCounterL2); // generation of false hits for layer 2
    
    int nBin=(zmax-zmin)/fZBinWidth;
    TH1D *hZrec=new TH1D("hZrec","hZrec",nBin,zmin,zmax);                // instantiate tracklets z intersection histogram
    double zIntersectionTrack[nMaxInter];                                // instantiate array for intersection
    int zInterCounter=0;                                                 // counter of tracklets intersection
    
    // loops to find tracklets intersection with z axis
    for(int iHitL1=0;iHitL1<nHitTotLayer1;iHitL1++){ // loop over hit layer 1
      for(int iHitL2=0;iHitL2<nHitTotLayer2;iHitL2++){ // loop over hit layer 2
        if(arrayHitFirstLayer[iHitL1].GetPhi()-0.5*fDeltaPhi<=arrayHitSecondLayer[iHitL2].GetPhi() && arrayHitSecondLayer[iHitL2].GetPhi()>=arrayHitFirstLayer[iHitL1].GetPhi()+0.5*fDeltaPhi){
          double zInter=vertxr->FindZintersect(arrayHitFirstLayer[iHitL1],arrayHitSecondLayer[iHitL2],layer[1]->GetRadius(),layer[2]->GetRadius());
          hZrec->Fill(zInter);
          zIntersectionTrack[zInterCounter++]=zInter;
        }
      } // end of loop over hit layer 2
    } // end of loop over hit layer 1

    // find vertex
    double zTmp=0;                                                    // variable to find the peak of histogram
    double mean=0;                                                    // variable to save mean position of vertex
    double rms=0;                                                     // variable to save rms of vertex
    if(vertxr->FindVertex(hZrec,zTmp,fDeltaZ,fDeltaNentries)){
      vertxr->FitVertex(zIntersectionTrack,mean,rms,zmin,zmax);
      // fill histograms hZtrueMultRes & hZtrueMultNrec
      double res=mean-vert.Z;
      hZtrueMultRes->Fill(vert.Z,vert.Mult,res);
      hZtrueMultNrec->Fill(vert.Z,vert.Mult);
    } 
    // fill hZtrueMultNsim histogram
    hZtrueMultNsim->Fill(vert.Z,vert.Mult);

    delete hZrec;
    delete[] arrayHitFirstLayer;                                      // deallocate memory for hit array
    delete[] arrayHitSecondLayer;

  } // end loop over events
}

void RecManager::Smearing(Point2D& hit,const double zResol,const double rPhiResol,Layer *layer) const{
  double z=hit.GetZ();
  double phi=hit.GetPhi();
  double new_z=z+gRandom->Gaus(0,zResol);
  double new_phi=phi+gRandom->Gaus(0,rPhiResol)/layer->GetRadius();
  hit.SetZ(new_z);
  hit.SetPhi(new_phi);
}

int RecManager::GenerateNhitNoise(int meanNoiseNumber) const{
  return gRandom->Poisson(meanNoiseNumber);
}

void RecManager::GenerateHitNoiseSoft(int nHitTot,Point2D *arrayHit,int hitCounter) const{
  for(int iHit=hitCounter;iHit<nHitTot;iHit++){
    double z=-27./2+27.*gRandom->Uniform();
    double phi=2*TMath::Pi()*gRandom->Uniform();
    Point2D newHit(z,phi);
    arrayHit[iHit]=newHit;
  }
}