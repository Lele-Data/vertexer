// RecManager.h
// The class of the RecManager
//
// Authors: Mario Ciacco & Emanuele Data

#ifndef RECMANAGER_H
#define RECMANAGER_H

#include <TObject.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TH3D.h>
#include <TH2D.h>
#include <TH1D.h>
#include "SimulManager.h"
#include "Vertex.h"
#include "Vertexer.h"
#include "../cfg/Constants.h"

class RecManager : public TObject{
 public:
  static RecManager *GetInstance(double deltaPhi,double zBinWidth,double deltaZ,double zWidth);                      // singleton class method
  static RecManager *Destroy();                                                                                      // singleton class method
  void RunReconstruction(TTree *inTree,TTree *vtxTree,VTX& vert,Vertex& vtx,TClonesArray *hitsFirstLayer,TClonesArray *hitsSecondLayer,Layer *layer[2]) const;
 private:
  RecManager();                                                                                                      // (private) default constructor
  RecManager(const RecManager& gen);                                                                                 // (private) copy constructor
  RecManager(double deltaPhi,double zBinWidth,double deltaZ,double zWidth);                                          // (private) standard constructor
  ~RecManager();                                                                                                     // (private) destructor
  void Smearing(Point2D *hit,Layer *layer) const;                                                                    // apply smearing to hit points
  int GenerateNhitNoise(int meanNoiseNumber) const;                                                                  // generate the number of false hits
  void GenerateHitNoiseSoft(Point2D **arrayHit,Layer *layer,int hitCounter,int nHitTot) const;                       // generate false hits
  static RecManager *fInstance;                                                                                      // The class instance
  Vertexer *vertxr;                                                                                                  // pointer to vertexer instance
  double fDeltaPhi;                                                                                                  // phi slice width
  double fZBinWidth;                                                                                                 // bin width  
  double fDeltaZ;                                                                                                    // horizontal cut
  double fZWidth;                                                                                                    // centroid width

 ClassDef(RecManager,1);
};

#endif // RECMANAGER_H 