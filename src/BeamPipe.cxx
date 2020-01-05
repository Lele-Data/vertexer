// BeamPipe.cxx
// Implementation of BeamPipe.cxx
// 
// Authors: Mario Ciacco & Emanuele Data

#include <Riostream.h>
#include "BeamPipe.h"

ClassImp(BeamPipe);

BeamPipe::BeamPipe(double radius,double thickness,double radLength):Cylinder(radius,thickness,radLength){
  // constructor
}

BeamPipe::~BeamPipe(){
  // destructor
}

void BeamPipe::PrintCylinderType() const{
  std::cout<<"Beam Pipe"<<std::endl;
}