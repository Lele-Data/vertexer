// Cylinder.h
// The class of an abstract cylinder
//
// Authors: Mario Ciacco & Emanuele Data

#ifndef CYLINDER_H
#define CYLINDER_H

#include <TObject.h>

class Cylinder : public TObject{
 public:
  Cylinder(double radius=0.,double thickness=0.,double radLength=0.);
  virtual void PrintCylinderType() const = 0;
  double GetThickness() const;
  double GetRadius() const;
  double GetRMSscat() const;
 private:
  double fRadius;
  double fThickness;
  double fRadLength;
  double fRMSscat;

 ClassDef(Cylinder,1);
};

#endif // CYLINDER_H