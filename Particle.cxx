// Particle.cxx
// Implementation of Particle.h
// 
// Authors: Mario Ciacco & Emanuele Data

#include "Particle.h"

ClassImp(Particle);

Particle::Particle(double x,double y,double z,double theta,double phi):TObject(),
fX(x),
fY(y),
fZ(z),
fTheta(theta),
fPhi(phi){
  // constructor
} 

Particle::~Particle(){
  // destructor
}

void Particle::SetPoint(double x,double y, double z){
  fX = x;
  fY = y;
  fZ = z;
}

void Particle::SetDirection(double theta, double phi){
  fTheta = theta;
  fPhi = phi;
}