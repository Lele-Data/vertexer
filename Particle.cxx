// Particle.cxx
// Implementation of Particle.h
// 
// Authors: Mario Ciacco & Emanuele Data

#include <TObject.h>
#include "Particle.h"


Particle::Particle(double x,double y,double z,double theta,double phi):TObject(),
fX(x),
fY(y),
fZ(z),
fTheta(theta),
fPhi(phi){
  //constructor
} 

Particle::~Particle(){
  //Destructor
}

double Particle::GetX() const{
  return fX;
}
double Particle::GetY() const{
  return fY;
};
double Particle::GetZ() const{
  return fZ;
};
double Particle::GetTheta() const{
  return fTheta;
};
double Particle::GetPhi() const{
  return fPhi;
};

void Particle::SetPoint(double x,double y, double z){
  fX = x;
  fY = y;
  fZ = z;
}

void Particle::SetDirection(double theta, double phi){
  fTheta = theta;
  fPhi = phi;
}

