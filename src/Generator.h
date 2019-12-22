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

enum MultMethod{
  kUnifMult=0, // generate multiplicity uniformly between 0 and max multiplicity
  kHistMult=1, // generate multiplicity according to the histogram
  kConstMult=2 // generate constant multiplicity equal to the maximum value
};

enum EtaMethod{
  kUnifEta=0,  // generate pseudorapidity uniformly between -2 and 2
  kHistEta=1   // generate pseudorapidity according to the histogram
};

class Generator : public TObject{
 public:
  static Generator *GetInstance(); // singleton class method
  static Generator *Destroy();     // singleton class method
  double GenerateMultiplicity(int nMultMethod) const;
  void GenerateVertex(double& x,double& y,double& z) const;
  void GenerateParticle(Particle& particle,int nEtaMethod) const; // generate the particle direction
 private:
  Generator();                     // (private) default constructor
  Generator(const Generator& gen); // (private) copy constructor
  ~Generator();                    // (private) destructor
  static Generator *fInstance;     // The class instance
  double (*RandomEta[2])();
  double (*RandomMultiplicity[3])();
  static double UniformEta();          // 0
  static double HistEta();             // 1
  static double UniformMultiplicity(); // 0
  static double HistMultiplicity();    // 1
  static double ConstantMultiplicity();// 2
  static TH1F *hEta;
  static TH1F *hMult;

 ClassDef(Generator,1);
};

#endif // GENERATOR_H 