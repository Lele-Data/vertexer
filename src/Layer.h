// Layer.h
// The class of a Layer
//
// Authors: Mario Ciacco & Emanuele Data

#ifndef LAYER_H
#define LAYER_H

#include "Cylinder.h"

class Layer : public Cylinder{
 public:
  Layer(double radius=0.,double thickness=0.,double length=0.,double zresol=0.,double rphiresol=0.);  
  ~Layer();  
  virtual void PrintCylinderType() const;
  double GetLength() const;
  double GetZresol() const;
  double GetRphiResol() const;
 private:
  double fLength;
  double fZresol;    // z resolution
  double fRphiResol; // r-phi resolution
  
 ClassDef(Layer,1);
};

#endif // LAYER_H