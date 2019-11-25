// Point2D.h
// The class of a 2D point
//
// Authors: Mario Ciacco & Emanuele Data

#ifndef POINT2D_H
#define POINT2D_H

#include <TObject.h>

class Point2D : public TObject{
 public:
  Point2D(double z=0.,double phi=0.);
  ~Point2D();
  void SetPoint2D(double z, double phi);
  void SetZ(double z);
  void SetPhi(double phi);
  double GetZ() const;
  double GetPhi() const;
 private:
  double fZ;
  double fPhi;

 ClassDef(Point2D,1);
};
#endif
