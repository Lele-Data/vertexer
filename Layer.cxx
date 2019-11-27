// Layer.cxx
// Implementation of Layer.cxx
// 
// Authors: Mario Ciacco & Emanuele Data

#include <Riostream.h>
#include "Layer.h"

ClassImp(Layer);

Layer::Layer(double radius,double thickness,double length):Cylinder(radius,thickness),
fLength(length){
  // constructor
}

Layer::~Layer(){
  // destructor
}

void Layer::PrintCylinderType() const{
  std::cout<<"Layer"<<std::endl;
}

double Layer::GetLength() const{
  return fLength;
}