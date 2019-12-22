// Generator.cxx
// Implementation of Generator.h
// 
// Authors: Mario Ciacco & Emanuele Data

#include <TFile.h>
#include <TRandom3.h>
#include <TMath.h>
#include "Generator.h"

ClassImp(Generator);

Generator *Generator::fInstance=NULL; // static data member
TH1F *Generator::hEta=NULL;           // static data member
TH1F *Generator::hMult=NULL;          // static data member

TH1F* GetResizedHistogram(TH1F*,double,double); // helper function

Generator::Generator():TObject(){
  TFile kin("kinem.root");
  hMult=(TH1F*)kin.Get("hmul");
  TH1F* hEtaNotRes=(TH1F*)kin.Get("heta");
  hMult->SetDirectory(0); // ROOT is the owner of this histogram
  hEta=GetResizedHistogram(hEtaNotRes,-2.,2.);
  hEta->SetDirectory(0); // ROOT is the owner of this histogram
  kin.Close();
  RandomEta[0]=UniformEta;
  RandomEta[1]=HistEta;
  RandomMultiplicity[0]=UniformMultiplicity;
  RandomMultiplicity[1]=HistMultiplicity;
  RandomMultiplicity[2]=ConstantMultiplicity;
}

Generator::~Generator(){
}

Generator *Generator::GetInstance(){
  if(!Generator::fInstance) fInstance=new Generator();
  return fInstance;
}

Generator *Generator::Destroy(){
  if(Generator::fInstance) delete fInstance;
  fInstance=NULL;
  return fInstance;
}

double Generator::GenerateMultiplicity(int nMultMethod) const{
  return (RandomMultiplicity[nMultMethod])();
}

void Generator::GenerateVertex(double& x,double& y,double& z) const{
  x=gRandom->Gaus(0.,kSigX);  
  y=gRandom->Gaus(0.,kSigY);
  z=gRandom->Gaus(0.,kSigZ);
}

void Generator::GenerateParticle(Particle& particle,int nEtaMethod) const{
  double eta=RandomEta[nEtaMethod]();
  double theta=2.*TMath::ATan(TMath::Exp(-eta)); // theta is generated in (0,pi)
  double phi=gRandom->Rndm()*(2*TMath::Pi()); // phi is uniformly generated in (0,2*pi)
  particle.SetDirection(theta,phi);
}

double Generator::UniformEta(){
  double unif=gRandom->Rndm()*4.0;
  return unif-2.0;
}

double Generator::HistEta(){return hEta->GetRandom();}

double Generator::UniformMultiplicity(){
  return gRandom->Rndm()*kMaxMultiplicity;
}

double Generator::HistMultiplicity(){return hMult->GetRandom();}

double Generator::ConstantMultiplicity(){return kMaxMultiplicity;}

TH1F* GetResizedHistogram(TH1F* disteta,double xmin,double xmax){
  TAxis *xa=disteta->GetXaxis();
  double step=xa->GetBinWidth(1);
  int binMin=xa->FindBin(xmin);
  int binMax=xa->FindBin(xmax);
  double xLow=xa->GetBinLowEdge(binMin);
  double xHig=xa->GetBinUpEdge(binMax);
  int nBins=binMax-binMin+1;
  double step2=(xHig-xLow)/nBins;
  TH1F* heta2=new TH1F("heta2","#eta distribution 2",nBins,xLow,xHig);
  int j=1;
  for(int i=binMin;i<=binMax;i++) heta2->SetBinContent(j++,disteta->GetBinContent(i));
  return heta2;
}