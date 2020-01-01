// RecManager.cxx
// Implementation of RecManager.h
//
// Authors: Mario Ciacco & Emanuele Data

#include <TRandom3.h>
#include <TMath.h>
#include <Riostream.h>
#include "RecManager.h"
#include "Vertex.h"

ClassImp(RecManager);

RecManager *RecManager::fInstance=NULL; // static data member    

void BubbleSort(double arr[],int size); // helper function

RecManager::RecManager(double deltaPhi,double zBinWidth,double deltaZ,double zWidth):TObject(),
fDeltaPhi(deltaPhi),           // phi slice width
fZBinWidth(zBinWidth),         // bin width  
fDeltaZ(deltaZ),               // horizontal cut
fZWidth(zWidth){               // centroid width
  vertxr=Vertexer::GetInstance();
}

RecManager::~RecManager(){
  vertxr=Vertexer::Destroy();
}

RecManager *RecManager::GetInstance(double deltaPhi,double zBinWidth,double deltaZ,double zWidth){
  if(!RecManager::fInstance) fInstance=new RecManager(deltaPhi,zBinWidth,deltaZ,zWidth);
  return fInstance;
}

RecManager *RecManager::Destroy(){
  if(RecManager::fInstance) delete fInstance;
  fInstance=NULL;
  return fInstance;
}

void RecManager::RunReconstruction(TTree *inTree,TTree *vtxTree,VTX& vert,Vertex& vtx,TClonesArray *hitsFirstLayer,TClonesArray *hitsSecondLayer,Layer *layer[2]) const{
  for(int iEvent=0;iEvent<inTree->GetEntries();++iEvent){ // loop over events
    inTree->GetEvent(iEvent);                             // process current event
    
    // DEFINE AND INSTANTIATE ARRAYS OF HIT POINTS
    int nHitLayer1=hitsFirstLayer->GetEntries();
    int nHitLayer2=hitsSecondLayer->GetEntries();
    int nHitNoiseLayer1=GenerateNhitNoise(kMeanNnoise);               // generate the number of false hits
    int nHitNoiseLayer2=GenerateNhitNoise(kMeanNnoise);
    int nHitTotLayer1=nHitLayer1+nHitNoiseLayer1;                     // total number of hits
    int nHitTotLayer2=nHitLayer2+nHitNoiseLayer2;
    Point2D **arrayHitFirstLayer=new Point2D*[nHitTotLayer1];         // allocate memory for hit array
    Point2D **arrayHitSecondLayer=new Point2D*[nHitTotLayer2];

    // APPLY SMEARING TO HITS ON LAYER 1 AND 2 AND GENERATE NOISE/SOFT HITS
    int hitCounterL1=0;
    int hitCounterL2=0;
    for(int iHitL1=0;iHitL1<nHitLayer1;++iHitL1){ // loop over hits of first layer
      Point2D *hitTmp=(Point2D*)hitsFirstLayer->At(iHitL1);
      arrayHitFirstLayer[iHitL1]=new Point2D(hitTmp->GetZ(),hitTmp->GetPhi());
      Smearing(arrayHitFirstLayer[iHitL1],layer[0]);
      hitCounterL1++;
    } // end of loop over hits of first layer
    for(int iHitL2=0;iHitL2<nHitLayer2;++iHitL2){ // loop over hits of second layer
      Point2D *hitTmp=(Point2D*)hitsSecondLayer->At(iHitL2);
      arrayHitSecondLayer[iHitL2]=new Point2D(hitTmp->GetZ(),hitTmp->GetPhi());
      Smearing(arrayHitSecondLayer[iHitL2],layer[1]);
      hitCounterL2++;
    } // end of loop over hits of second layer
    GenerateHitNoiseSoft(arrayHitFirstLayer,layer[0],hitCounterL1,nHitTotLayer1); // generation of false hits for layer 1
    GenerateHitNoiseSoft(arrayHitSecondLayer,layer[1],hitCounterL2,nHitTotLayer2);// generation of false hits for layer 2

    // DEFINE Z INTERSECTION HISTOGRAM PARAMENTERS
    int nBin=(kZmax-kZmin)/fZBinWidth;
    TH1D *hZrec=new TH1D("hZrec","hZrec",nBin,kZmin,kZmax);           // instantiate tracklets z intersection histogram
    hZrec->SetDirectory(0);                                           // avoid segmentation fault giving owner
    double zIntersectionTrack[nMaxInter]={0};                         // instantiate array for intersection
    int zInterCounter=0;                                              // counter of tracklets intersection

    // LOOP TO FIND TRACKLETS INTERSECTIONS WITH Z AXIS
    for(int iHitL1=0;iHitL1<nHitTotLayer1;++iHitL1){ // loop over hit layer 1
      for(int iHitL2=0;iHitL2<nHitTotLayer2;++iHitL2){ // loop over hit layer 2
        double phi1=arrayHitFirstLayer[iHitL1]->GetPhi();
        double phi2=arrayHitSecondLayer[iHitL2]->GetPhi();
        if(phi2>=phi1-0.5*fDeltaPhi && phi2<=phi1+0.5*fDeltaPhi){
          double zInter=vertxr->FindZintersect(arrayHitFirstLayer[iHitL1]->GetZ(),arrayHitSecondLayer[iHitL2]->GetZ(),layer[0]->GetRadius(),layer[1]->GetRadius());
          hZrec->Fill(zInter);
          zIntersectionTrack[zInterCounter++]=zInter;
        }
      } // end of loop over hit layer 2
    } // end of loop over hit layer 1
    
    // FIND AND FIT VERTEX
    double zTmp=0.;                                                   // variable to find the peak of histogram
    double zMean=-999.f;                                              // variable to save mean position of vertex
    double zRms=-999.f;                                               // variable to save rms of vertex
    bool rec=false;
    if(vertxr->FindVertex(hZrec,zTmp,fDeltaZ)){         // check if vertex is found from histogram
      BubbleSort(zIntersectionTrack,nMaxInter);                                         // sort array of intersections with the z axis
      vertxr->FitVertex(zIntersectionTrack,zMean,zRms,zTmp-fZWidth/2.,zTmp+fZWidth/2.); // fit vertex (within centroid region) if found
      rec=true;
    }

    // FILL OUTPUT TREE
    vtx=Vertex(zMean,zRms,vert.Z,vert.Mult,rec);
    vtxTree->Fill();

    // FREE MEMORY
    delete hZrec;
    for(int iHit1=0;iHit1<nHitTotLayer1;++iHit1) {if(arrayHitFirstLayer[iHit1]){delete arrayHitFirstLayer[iHit1];}}
    for(int iHit2=0;iHit2<nHitTotLayer2;++iHit2) {if(arrayHitSecondLayer[iHit2])delete arrayHitSecondLayer[iHit2];}
    delete[] arrayHitFirstLayer;
    delete[] arrayHitSecondLayer;
  } // end loop over events
}

void RecManager::Smearing(Point2D *hit,Layer *layer) const{
  double z=hit->GetZ();
  double phi=hit->GetPhi();
  double Zres=layer->GetZresol();
  double new_z=gRandom->Gaus(z,Zres);
  double new_phi=phi+gRandom->Gaus(0,layer->GetRphiResol())/layer->GetRadius();
  hit->SetZ(new_z);
  hit->SetPhi(new_phi);
}

int RecManager::GenerateNhitNoise(int meanNoiseNumber) const{
  return gRandom->Poisson(meanNoiseNumber);
}

void RecManager::GenerateHitNoiseSoft(Point2D **arrayHit,Layer *layer,int hitCounter,int nHitTot) const{
  double length=layer->GetLength();
  for(int iHit=hitCounter;iHit<nHitTot;iHit++){
    double z=-length/2.+length*gRandom->Rndm();
    double phi=2.*TMath::Pi()*gRandom->Rndm();
    arrayHit[iHit]=new Point2D(z,phi);
  }
}

// This function is used to sort the array of z intersections
void BubbleSort(double arr[],int size){
  int arr_index=0;
  int num_unsor=1;
  while (num_unsor>0) {
    num_unsor=0;
    arr_index=0;
    for (;arr_index<size-1;++arr_index){
      if (arr[arr_index]>arr[arr_index+1]){
        arr[arr_index+1]+=arr[arr_index];
        arr[arr_index]=arr[arr_index+1]-arr[arr_index];
        arr[arr_index+1]-=arr[arr_index];
        num_unsor+=1;
      }
    }
  }
}