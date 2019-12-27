// SimulManager.cxx
// Implementation of SimulManager.h
//
// Authors: Mario Ciacco & Emanuele Data

#include <TRandom3.h>
#include "SimulManager.h"
#include "Particle.h"
#include "Point2D.h"

ClassImp(SimulManager)

SimulManager *SimulManager::fInstance=NULL; // static data member

SimulManager::SimulManager(int nEvent,int nMult,int nEta,int nScat,uint seed):TObject(),
fEvent(nEvent),
fMultMethod(nMult),
fEtaMethod(nEta),
fMultScatMethod(nScat){
  gRandom->SetSeed(seed);
  gen=Generator::GetInstance();
  transp=Transport::GetInstance();
}

SimulManager::~SimulManager(){
  gen=Generator::Destroy();
  transp=Transport::Destroy();
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

// The simulation algorithm
void SimulManager::RunSimulation(TTree *tree,VTX& vert,TClonesArray& hitsFirstLayer,TClonesArray& hitsSecondLayer,BeamPipe *bpipe,Layer *layers[2]) const{
  double TmpZ=0.;                                        // temporary z used to compute hit
  double TmpPhi=0.;                                      // temporary phi used to compute hit
  for(int iEvent=0;iEvent<fEvent;++iEvent){ // loop over events
    hitsFirstLayer.Clear();                              // clear first array
    hitsSecondLayer.Clear();                             // clear second array
    vert.Mult=gen->GenerateMultiplicity(fMultMethod); // generate multiplicity
    gen->GenerateVertex(vert.X,vert.Y,vert.Z);           // generate collision vertex
    int iFirstLayer=0;                                   // counter
    int iSecondLayer=0;                                  // counter
    for(double iPart=0.;iPart<vert.Mult;++iPart){ // loop over particles
      Particle particle(vert.X,vert.Y,vert.Z);           // create particle with generated vertex
      gen->GenerateParticle(particle,fEtaMethod);        // generate particle direction
      // INTERACTION WITH BEAM PIPE
      transp->Intersection(particle,bpipe);              // intersect with beam pipe
      transp->MultipleScattering(particle,fMultScatMethod);// compute new direction (mult scat)
      // INTERACTION WITH LAYER 1
      transp->Intersection(particle,layers[0]);          // intersect with layer 1
      if(particle.GetZ()>(-layers[0]->GetLength()/2.)&&particle.GetZ()<(layers[0]->GetLength()/2.)){ // intersection check on first layer
        transp->ComputeHit(particle,layers[0],TmpZ,TmpPhi);          //compute hit on layer
        new(hitsFirstLayer[iFirstLayer++])Point2D(TmpZ,TmpPhi);      // add hit to TClonesArray
        transp->MultipleScattering(particle,fMultScatMethod);        // compute new direction
        // INTERACTION WITH LAYER 2
        transp->Intersection(particle,layers[1]);        // intersect with layer 2
        if(particle.GetZ()>(-layers[1]->GetLength()/2.)&&particle.GetZ()<(layers[1]->GetLength()/2.)){ // intersection check on second layer
          transp->ComputeHit(particle,layers[1],TmpZ,TmpPhi);        //compute hit on layer
          new(hitsSecondLayer[iSecondLayer++])Point2D(TmpZ,TmpPhi);  // add hit to TClonesArray
          transp->MultipleScattering(particle,fMultScatMethod);      // compute new direction
        } // end of intersection check on second layer
      } // end of intersection check on first layer
    } // end of loop over particles
    tree->Fill();                                        // fill the tree
  } // end of loop over events
}