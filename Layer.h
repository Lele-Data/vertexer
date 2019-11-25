// Layer.h
// The class of a Layer
//
// Authors: Mario Ciacco & Emanuele Data

#ifndef LAYER_H
#define LAYER_H

#include "Cylinder.h"

class Layer : public Cylinder{
 public:
  Layer(double radius=0., double thickness = 0.,double length=0.);  
  ~Layer();  
  virtual TString GetCylinderType() const;
  double GetLength() const;

 private:
  double fLength;
  
 ClassDef(Layer,1);
};
#endif
