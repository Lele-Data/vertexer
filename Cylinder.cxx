// Cylinder.cxx
// Implementation of Cylinder.cxx
// 
// Authors: Mario Ciacco & Emanuele Data

#include "Cylinder.h"

ClassImp(Cylinder);

Cylinder::Cylinder(double radius,double thickness):TObject(),
fRadius(radius),
fThickness(thickness){
  // constructor
}

double Cylinder::GetThickness() const{
  return fThickness;
}

double Cylinder::GetRadius() const{
  return fRadius;
}