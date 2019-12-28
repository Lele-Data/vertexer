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
  static Vertexer *GetInstance(); // singleton class method
  static Vertexer *Destroy();     // singleton class method
  bool FindVertex(TH1D* hZrec,double& zTmp,const double deltaZ,const double deltaNentries) const;
  void FitVertex(Point2D *arrayHits,double& mean,double& rms,double zMin,double zMax) const;
  double FindZintersect(Point2D p1,Point2D p2,double r1,double r2) const; 
 private:
  Vertexer();                     // (private) default constructor
  Vertexer(const Vertexer& gen);  // (private) copy constructor
  ~Vertexer();                    // (private) destructor
  static Vertexer *fInstance;     // The class instance

 ClassDef(Vertexer,1);
};

#endif // VERTEXER_H 