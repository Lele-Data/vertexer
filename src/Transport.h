// Transport.h
// Class of Transport (SINGLETON)
//
// Author: Mario Ciacco & Emanuele Data

#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <TObject.h>
#include "Particle.h"
#include "Cylinder.h"
#include "Layer.h"
#include "Point2D.h"

const double kRMSscat=0.001; // rad

enum MultScatMethod{
  kOffScat=0, // turn off multiple scattering
  kOnScat=1   // turn on multiple scattering
};

class Transport : public TObject{
 public:
  static Transport *GetInstance();  // singleton class method
  static Transport *Destroy();      // singleton class method
  void MultipleScattering(Particle& particle,int nScatMethod);
  void Intersection(Particle& particle,Cylinder *cylinder);
  Point2D ComputeHit(Particle particle,Layer *layer);
  void ComputeHit(Particle particle,Layer *layer,double& zHit,double& phiHit);
 private:
  Transport();                       // (private) default constructor
  Transport(const Transport& transp);// (private) copy constructor
  ~Transport();                      // (private) destructor
  static Transport *fInstance;       // The class instance
  double (*RandomScat[2])();
  static double OffScattering(); // 0
  static double Onscattering();  // 1

 ClassDef(Transport,1);
};

#endif // TRANSPORT_H