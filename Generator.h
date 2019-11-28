// Generator.h
// The class of the MC Generator
//
// Authors: Mario Ciacco & Emanuele Data

#ifndef GENERATOR_H
#define GENERATOR_H

#include <TObject.h>
#include <TH1F.h>
#include "Particle.h"

const double kMaxMultiplicity=60.;
const double kSigX=0.01; // cm
const double kSigY=0.01; // cm
const double kSigZ=5.00; // cm

class Generator : public TObject{
 public:
  Generator();
  ~Generator();
  double GenerateMultiplicity(int nMultMethod) const;
  void GenerateVertex(double& x,double& y,double& z) const;
  void GenerateParticle(Particle& particle,int nEtaMethod) const; // generate the particle direction
 private:
  double (*RandomEta[2])();
  double (*RandomMultiplicity[3])();
  static double UniformEta();
  static double HistEta();
  static double UniformMultiplicity();
  static double HistMultiplicity();
  static double ConstantMultiplicity();
  static TH1F *hEta;
  static TH1F *hMult;

 ClassDef(Generator,1);
};

#endif // GENERATOR_H 