// Cylinder.cxx
// Implementation of Cylinder.cxx
// 
// Authors: Mario Ciacco & Emanuele Data

#include <TMath.h>
#include "Cylinder.h"

const double kMomentum=700.; // MeV/c

ClassImp(Cylinder);

Cylinder::Cylinder(double radius,double thickness,double radLength):TObject(),
fRadius(radius),
fThickness(thickness),
fRadLength(radLength){
  // constructor
  if(radLength>0){
    double fracxX0=thickness/radLength;
    fRMSscat=TMath::Sqrt(2)*((13.6)/(kMomentum))*TMath::Sqrt(fracxX0)*(1.+0.038*TMath::Log(fracxX0));
  }
}

double Cylinder::GetThickness() const{
  return fThickness;
}

double Cylinder::GetRadius() const{
  return fRadius;
}

double Cylinder::GetRMSscat() const{
  return fRMSscat;
}