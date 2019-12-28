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
#include "SimulManager.h"
#include "Vertexer.h"

class RecManager : public TObject{
 public:
  static RecManager *GetInstance(double deltaPhi,double zBinWidth,double deltaZ,double deltaNentries,double zWidth); // singleton class method
  static RecManager *Destroy();                                                                                      // singleton class method
  void RunReconstruction(TTree *tree,VTX vert,TClonesArray *hitsFirstLayer,TClonesArray *hitsSecondLayer,TH3D *hZtrueMultRes,TH2D *hZtrueMultNrec,TH2D *hZtrueMultNsim) const;
  void Smearing(Point2D& hit,const double zResol,const double rPhiResol) const;
  void GenerateNoiseSoft(int meanNoiseNumber,Point2D *arrayHit) const;
 private:
  RecManager();                                                                                                      // (private) default constructor
  RecManager(const RecManager& gen);                                                                                 // (private) copy constructor
  RecManager(double deltaPhi,double zBinWidth,double deltaZ,double deltaNentries,double zWidth);                     // (private) standard constructor
  ~RecManager();                                                                                                     // (private) destructor
  static RecManager *fInstance;                                                                                      // The class instance
  Vertexer *vertxr;                                                                                                  // pointer to vertexer instance
  double fDeltaPhi;                                                                                                  // phi slice width
  double fZBinWidth;                                                                                                 // bin width  
  double fDeltaZ;                                                                                                    // horizontal cut
  double fDeltaNentries;                                                                                             // vertical cut
  double fZWidth;                                                                                                    // centroid width

 ClassDef(RecManager,1);
};

#endif // RECMANAGER_H 