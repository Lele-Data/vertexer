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

class SimulManager{
 public:
  SimulManager();
  ~SimulManager();
  void RunSimulation(TTree *tree,TClonesArray& layerArr1,TClonesArray& layerArr2,BeamPipe *bpipe,Layer *layers[2]);
 private:
  int fMultMethod;     // number of the method chosen for multiplicity generation
  int fEtaMethod;      // number of the method chosen for eta generation
  int fMultScatMethod; // number of the method chosen for multiple scattering
  int fEvent;          // number of events
  double fTmpZ;        // temporary z used to compute hit
  double fTmpPhi;      // temporary phi used to compute hit
  Generator *gen;      // MC generator (vertex and particle generation)
  Propagator *prop;    // MC propagator (particle transport)

 ClassDef(SimulManager,1);
};

#endif // SIMULMANAGER_H