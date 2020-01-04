// Compile.C
// The compilation macro

#include <string>
#include <TSystem.h>
#include <TROOT.h>
#include <Riostream.h>

#include "../cfg/Constants.h"

void Compile(const char option='n') {
  gSystem->mkdir(FILE_DIR); // make build directory
  gSystem->mkdir(BUILD_DIR); // make build directory
  std::string opt="kg";
  if(option=='f') opt="kfg";
  // Classes for MC particles and intersections
  std::cout<<"Compile classes for MC particles and intersections..."<<std::endl;
  gSystem->CompileMacro("src/Particle.cxx",opt.data(),"",BUILD_DIR);
  gSystem->CompileMacro("src/Point2D.cxx",opt.data(),"",BUILD_DIR);
  // Classes for the detector definition
  std::cout<<"Compile classes for the detector definition..."<<std::endl;
  gSystem->CompileMacro("src/Cylinder.cxx",opt.data(),"",BUILD_DIR);
  gSystem->CompileMacro("src/Layer.cxx",opt.data(),"",BUILD_DIR);
  gSystem->CompileMacro("src/BeamPipe.cxx",opt.data(),"",BUILD_DIR);
  // Class for the MC generator and propagator
  std::cout<<"Compile classes for the MC generator and transport..."<<std::endl;
  gSystem->CompileMacro("src/Generator.cxx",opt.data(),"",BUILD_DIR);
  gSystem->CompileMacro("src/Transport.cxx",opt.data(),"",BUILD_DIR);
  // Class for the MC simulation manager
  std::cout<<"Compile class for the MC simulation manager..."<<std::endl;
  gSystem->CompileMacro("src/SimulManager.cxx",opt.data(),"",BUILD_DIR);
  // Class for the vertexer and vertex
  std::cout<<"Compile class for the vertexer and vertex..."<<std::endl;
  gSystem->CompileMacro("src/Vertexer.cxx",opt.data(),"",BUILD_DIR);
  gSystem->CompileMacro("src/Vertex.cxx",opt.data(),"",BUILD_DIR);
  // Class for the reconstruction manager
  std::cout<<"Compile class for the reconstruction manager..."<<std::endl;
  gSystem->CompileMacro("src/RecManager.cxx",opt.data(),"",BUILD_DIR);
  // load macros
  std::cout<<"Load macros..."<<std::endl;
  gROOT->LoadMacro("macros/SteerSimul.C");
  gROOT->LoadMacro("macros/SteerRec.C");
  gROOT->LoadMacro("macros/CreateHist.C");
  gROOT->LoadMacro("macros/DrawHist.C");
}