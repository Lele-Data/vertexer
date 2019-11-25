// Point2D.cxx
// Implementation of Point2D.h
// 
// Authors: Mario Ciacco & Emanuele Data

#include <TObject.h>
#include "Point2D.h"


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

void Point2D::SetPoint2D(double z, double phi){
  fZ = z;
  fPhi = phi;
};
void Point2D::SetZ(double z){
  fZ = z;
};
void Point2D::SetPhi(double phi){
  fPhi = phi;
};

