// Cylinder.h
// The class of an abstract cylinder
//
// Authors: Mario Ciacco & Emanuele Data

#ifndef CYLINDER_H
#define CYLINDER_H

#include <TObject.h>
#include <TString.h>

class Cylinder : public TObject{
 public:
  virtual TString PrintCylinderType() const = 0;
  double GetThickness() const;
  double GetRadius() const;
 protected:
  double fThickness;
  double fRadius;

 ClassDef(Cylinder,1);
};
#endif
