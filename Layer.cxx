// Layer.cxx
// Implementation of Layer.cxx
// 
// Authors: Mario Ciacco & Emanuele Data

#include "Layer.h"

Layer::Layer(double radius, double thickness, double length):Cylinder(){
  fRadius = radius;
  fThickness = thickness;
  fLength = length;
}

Layer::~Layer(){
  //destructor
}

TString Layer::GetCylinderType() const{
  return "Layer";
}

double Layer::GetLength() const{
  return fLength;
}




