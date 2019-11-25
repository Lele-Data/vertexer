// Particle.h
// The class of an MC particle
//
// Authors: Mario Ciacco & Emanuele Data

#ifndef PARTICLE_H
#define PARTICLE_H

#include <TObject.h>

class Particle : public TObject{
 public:
  Particle(double x=0.,double y=0.,double z=0.,double theta=0.,double phi=0.);
  ~Particle();
  void SetPoint(double x,double y,double z);
  void SetDirection(double theta,double phi);
  double GetX() const{return fX;};
  double GetY() const{return fY;};
  double GetZ() const{return fZ;};
  double GetTheta() const{return fTheta;};
  double GetPhi() const{return fPhi;};
 private:
  double fX;
  double fY;
  double fZ;
  double fTheta;
  double fPhi;

 ClassDef(Particle,1);
};

#endif // PARTICLE_H  