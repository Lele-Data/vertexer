// Compile.C
// The compilation macro

#include <TSystem.h>
#include <TROOT.h>
#include <Riostream.h>
#include <string>

const char *BUILD_DIR="build"; // build directory name

void Compile(const char option='n') {
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
  // Class for the vertexer
  std::cout<<"Compile class for the vertexer..."<<std::endl;
  gSystem->CompileMacro("src/Vertexer.cxx",opt.data(),"",BUILD_DIR);
  // Class for the reconstruction manager
  std::cout<<"Compile class for the reconstruction manager..."<<std::endl;
  gSystem->CompileMacro("src/RecManager.cxx",opt.data(),"",BUILD_DIR);
}