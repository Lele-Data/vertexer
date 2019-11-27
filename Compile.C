// Compile.C
// The compilation macro

#include <TSystem.h>
#include <TROOT.h>
#include <Riostream.h>
#include <string>

void Compile(const char option='n') {
  std::string opt="kg";
  if(option=='f') opt="kfg";
  // Classes for MC particles and intersections
  std::cout<<"Compile classes for MC particles and intersections..."<<std::endl;
  gSystem->CompileMacro("Particle.cxx",opt.data());
  gSystem->CompileMacro("Point2D.cxx",opt.data());
  // Classes for the detector definition
  std::cout<<"Compile classes for the detector definition..."<<std::endl;
  gSystem->CompileMacro("Cylinder.cxx",opt.data());
  gSystem->CompileMacro("Layer.cxx",opt.data());
  gSystem->CompileMacro("BeamPipe.cxx",opt.data());
  // Class for the MC generator
  gSystem->CompileMacro("Generator.cxx",opt.data());
}


