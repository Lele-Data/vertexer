// Constants.h
// The sowftware constants and parameters
//
// Authors: Mario Ciacco & Emanuele Data

#ifndef CONSTANTS
#define CONSTANTS

// DIRECTORIES
const char *BUILD_DIR="build"; // build directory name
const char *FILE_DIR="results/"; // result files directory

// TREE NAMES
const char *SimulTreeName="T";
const char *RecTreeName="V";

// BRAMCH NAMES
const char *SimVertBranchName="vert";
const char *SimHitFirstBranchName="hitsFirstLayer";
const char *SimHitSecondBranchName="hitsSecondLayer";
const char *RecVertBaranchName="vtx";

// DETECTOR AND BEAM PIPE
const double kBpipeRadius=3.0; // cm
const double kBpipeThick=0.08; // cm
const double kFirstLayerRadius=4.0; // cm
const double kFirstLayerThick=0.02; // cm
const double kFirstLayerLength=27.0; // cm
const double kSecondLayerRadius=7.0; // cm
const double kSecondLayerThick=0.0; // cm
const double kSecondLayerLength=27.0; // cm
const double kZresol=0.0120;   // cm
const double kRphiResol=0.0030;// cm

// GENERATOR PARAMETERS
const double kMaxMultiplicity=60.;
const double kConstMultiplicity=3.;
const double kSigX=0.01; // cm
const double kSigY=0.01; // cm
const double kSigZ=5.30; // cm

// HISTOGRAM BINNING
const double kResMin=-1000.; // um
const double kResMax=1000.;  // um
const int kNresBinLim=401;
const int kNzTrueBins=18;
const int kNmultBins=12;
const double kZtrueBins[]={-30.0,-27.0,-25.0,-23.0,-20.0,-15.0,-10.0,-5.0,-2.5,0.0,2.5,5.0,10.0,15.0,20.0,23.0,25.0,27.0,30.0};
const double kMultBins[]={2.5,3.5,4.5,5.5,7.5,9.5,14.5,24.5,34.5,44.5,54.5,70.0,100.0};

// RECONSTRUCTION PARAMETERS
const int kMeanNnoise=30.;     // mean number of generated false hits
const double kZmax=30.0;       // lower limit for the histogram  
const double kZmin=-30.0;      // upper limit for the histogram
const int nMaxInter=10000;     // size of array for tracklets intersection with z

// VERTEXER ALGORITHM PARAMETERS
const double kDeltaPhi=0.005;  // phi slice width
const double kZbinWidth=0.05;  // bin width
const double kDeltaZ=0.10;     // horizontal cut
const double kZwidth=0.20;     // centroid width

#endif // CONSTANTS