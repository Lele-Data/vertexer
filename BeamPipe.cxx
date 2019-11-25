// BeamPipe.cxx
// Implementation of BeamPipe.cxx
// 
// Authors: Mario Ciacco & Emanuele Data

#include <Riostream.h>
#include "BeamPipe.h"

ClassImp(BeamPipe);

BeamPipe::BeamPipe(double radius, double thickness):Cylinder(radius,thickness){
  // constructor
}

BeamPipe::~BeamPipe(){
  // destructor
}

void BeamPipe::PrintCylinderType() const{
  std::cout<<"Beam Pipe"<<std::endl;
}