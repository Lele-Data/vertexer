// BeamPipe.h
// The class of a beam pipe. (we could/should improve as a singleton)
//
// Authors: Mario Ciacco & Emanuele Data

#ifndef BEAMPIPE_H
#define BEAMPIPE_H

#include "Cylinder.h"

class BeamPipe : public Cylinder{
 public:
  BeamPipe(double radius=0.,double thickness = 0.);  
  ~BeamPipe();  
  virtual void PrintCylinderType() const;
 
 ClassDef(BeamPipe,1);
};

#endif // BEAMPIPE_H