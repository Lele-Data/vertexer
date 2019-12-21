// Propagator.h
// Class of Propagator (SINGLETON)
//
// Author: Mario Ciacco & Emanuele Data

#ifndef PROPAGATOR_H
#define PROPAGATOR_H

#include <TObject.h>
#include "Particle.h"
#include "Cylinder.h"
#include "Layer.h"
#include "Point2D.h"

const double kRMSscat=0.001; // rad

class Propagator : public TObject{
 public:
  static Propagator *GetInstance();  // singleton class method
  static Propagator *Destroy();      // singleton class method
  void MultipleScattering(Particle& particle,int nScatMethod);
  void Intersection(Particle& particle,Cylinder *cylinder);
  Point2D ComputeHit(Particle particle,Layer *layer);
  void ComputeHit(Particle particle,Layer *layer,double& zHit,double& phiHit);
 private:
  Propagator();                       // (private) default constructor
  Propagator(const Propagator& prop); // (private) copy constructor
  ~Propagator();                      // (private) destructor
  static Propagator *fInstance;       // The class instance
  double (*RandomScat[2])();
  static double OffScattering(); // 0
  static double Onscattering();  // 1

 ClassDef(Propagator,1);
};

#endif // PROPAGATOR_H