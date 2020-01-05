// Vertexer.h
// The class of the Vertexer
//
// Authors: Mario Ciacco & Emanuele Data

#ifndef VERTEXER_H
#define VERTEXER_H

#include <TObject.h>
#include <TH1D.h>
#include "Point2D.h"

class Vertexer : public TObject{
 public:
  static Vertexer *GetInstance();                           // singleton class method
  static Vertexer *Destroy();                               // singleton class method
  bool FindVertex(TH1D* hZrec,double& zTmp,const double deltaZ) const;
  void FitVertex(double *arrayZ,int size,double& mean,double& rms,double zMin,double zMax,double binWidth) const;
  double FindZintersect(double z1,double z2,double r1,double r2) const; 
 private:
  Vertexer();                                               // (private) default constructor
  Vertexer(const Vertexer& gen);                            // (private) copy constructor
  ~Vertexer();                                              // (private) destructor
  static int FindFirstMaximum(TH1D* hist);                  // find the bin of the first highest maximum
  static int FindSecondMaximum(TH1D* hist);                 // find the bin of the second highest maximum
  static Vertexer *fInstance;                               // The class instance

 ClassDef(Vertexer,1);
};

#endif // VERTEXER_H 