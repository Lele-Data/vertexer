// Point2D.cxx
// Implementation of Point2D.cxx
// 
// Authors: Mario Ciacco & Emanuele Data

#include <TObject.h>
#include "Point2D.h"

ClassImp(Point2D);

Point2D::Point2D(double z,double phi):TObject(),
fZ(z),
fPhi(phi){
  //constructor
} 

Point2D::~Point2D(){
  //Destructor
}

double Point2D::GetZ() const{
  return fZ;
};

double Point2D::GetPhi() const{
  return fPhi;
};