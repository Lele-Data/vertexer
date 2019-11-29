// Propagator.h
// Class of Propagator
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
  Propagator();
  ~Propagator();
  void MultipleScattering(Particle& particle,int nScatMethod);
  void Intersection(Particle& particle,Cylinder *cylinder);
  Point2D Intersection(Particle& particle,Layer *layer);
 private:
  double (*RandomScat[2])();
  static double OffScattering(); // 0
  static double Onscattering();  // 1

 ClassDef(Propagator,1);
};

#endif // PROPAGATOR_H