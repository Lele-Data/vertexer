// Layer.cxx
// Implementation of Layer.h
// 
// Authors: Mario Ciacco & Emanuele Data

#include <Riostream.h>
#include "Layer.h"

ClassImp(Layer);

Layer::Layer(double radius,double thickness,double length,double radLength,double zresol,double rphiresol):Cylinder(radius,thickness,radLength),
fLength(length),
fZresol(zresol),
fRphiResol(rphiresol){
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

double Layer::GetZresol() const{
  return fZresol;
}

double Layer::GetRphiResol() const{
  return fRphiResol;
}