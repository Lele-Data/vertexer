// SimulManager.h
// The class of the simulation manager
//
// Authors: Mario Ciacco & Emanuele Data

#ifndef SIMULMANAGER_H
#define SIMULMANAGER_H

#include <TObject.h>
#include <TTree.h>
#include <TClonesArray.h>
#include "Generator.h"
#include "Propagator.h"
#include "BeamPipe.h"
#include "Layer.h"

class SimulManager : public TObject{
 public:
  static SimulManager *GetInstance(int nEvent,int nMult,int nEta,int nScat,uint seed=12345);
  static SimulManager *Destroy();
  void RunSimulation(TTree *tree,TClonesArray& layerArr1,TClonesArray& layerArr2,BeamPipe *bpipe,Layer *layers[2]);
 private:
  SimulManager();                    // (private) default constructor
  SimulManager(int nEvent,int nMult,int nEta,int nScat,uint seed=12345);
  SimulManager(const SimulManager&); // (private) copy constructor
  ~SimulManager();                   // (private) destructor
  static SimulManager *fInstance;    // class instance
  int fEvent;          // number of events
  int fMultMethod;     // number of the method chosen for multiplicity generation
  int fEtaMethod;      // number of the method chosen for eta generation
  int fMultScatMethod; // number of the method chosen for multiple scattering
  double fTmpZ;        // temporary z used to compute hit
  double fTmpPhi;      // temporary phi used to compute hit
  Generator *gen;      // MC generator (vertex and particle generation)
  Propagator *prop;    // MC propagator (particle transport)

 ClassDef(SimulManager,1);
};

#endif // SIMULMANAGER_H