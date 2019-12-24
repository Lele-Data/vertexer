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
#include "Transport.h"
#include "BeamPipe.h"
#include "Layer.h"

typedef struct{
  double X;
  double Y;
  double Z;
  double Mult;
}VTX;

class SimulManager : public TObject{
 public:
  static SimulManager *GetInstance(int nEvent,int nMult,int nEta,int nScat,uint seed=12345);
  static SimulManager *Destroy();
  void RunSimulation(TTree *tree,VTX& vert,TClonesArray& hitsFirstLayer,TClonesArray& hitsSecondLayer,BeamPipe *bpipe,Layer *layers[2]) const;
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
  Generator *gen;      // MC generator (vertex and particle generation)
  Transport *transp;    // MC propagator (particle transport)

 ClassDef(SimulManager,1);
};

#endif // SIMULMANAGER_H