// BeamPipe.cxx
// Implementation of BeamPipe.cxx
// 
// Authors: Mario Ciacco & Emanuele Data

#include "BeamPipe.h"

BeamPipe::BeamPipe(double radius, double thickness):Cylinder(){
  fRadius = radius;
  fThickness = thickness;
}

BeamPipe::~BeamPipe(){
  //destructor
}

TString BeamPipe::GetCylinderType() const{
  return "Beam Pipe";
}




