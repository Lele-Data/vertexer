// Generator.cxx
// Implementation of Generator.h
// 
// Authors: Mario Ciacco & Emanuele Data

#include <TObject.h>
#include <TFile.h>
#include <TH1F.h>
#include <TRandom3.h>
#include <TMath.h>
#include "Particle.h"
#include "Generator.h"

// TODO: implement the helper function 'ResizeHistogram'

ClassImp(Generator);

TH1F *Generator::hEta=NULL; // static data member
TH1F *Generator::hMult=NULL;

Generator::Generator(unsigned int seed):TObject()
{
  TFile kin("kinem.root");
  hMult=(TH1F*)kin.Get("hmul");
  hEta=(TH1F*)kin.Get("heta");
  hMult->SetDirectory(0); // ROOT is the owner of this histogram
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
  double theta=2.*TMath::ATan(TMath::Exp(-eta))+TMath::Pi()/2.; // theta is generated in (0,pi)
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

void Generator::ResizeHistogram(){}