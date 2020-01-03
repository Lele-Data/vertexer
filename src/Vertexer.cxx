// Vertexer.cxx
// Implementation of Vertexer.h
//
// Authors: Mario Ciacco & Emanuele Data

#include <TMath.h>
#include "Vertexer.h"
#include <Riostream.h>

ClassImp(Vertexer);

Vertexer *Vertexer::fInstance=NULL;     // static data member

Vertexer::Vertexer():TObject(){
  // constructor
}

Vertexer::~Vertexer(){
  // destructor
}

Vertexer *Vertexer::GetInstance(){
  if(!Vertexer::fInstance) fInstance=new Vertexer();
  return fInstance;
}

Vertexer *Vertexer::Destroy(){
  if(Vertexer::fInstance) delete fInstance;
  fInstance=NULL;
  return fInstance;
}

bool Vertexer::FindVertex(TH1D* hZrec,double& zTmp,const double deltaZ) const{
  int bFirstMax=FindFirstMaximum(hZrec);
  int bSecondMax=FindSecondMaximum(hZrec);
  zTmp=(hZrec->GetBinCenter(bFirstMax)+hZrec->GetBinCenter(bFirstMax))/2.;
  double deltaZobs=fabs((double)(bFirstMax-bSecondMax))*hZrec->GetBinWidth(1); // compute the distance between the two highest bins
  if(deltaZobs<deltaZ) // horizontal (z) cut
    return true;
  return false;
}

void Vertexer::FitVertex(double *arrayZ,double& mean,double& rms,double zMin,double zMax) const{
  int iArr=0;
  int iMin=0;
  double nZint=1.;
  // compute mean
  while(arrayZ[iArr]<zMin)++iArr; // find the smallest z used to compute the mean
  iMin=iArr++;
  mean=arrayZ[iMin];
  // std::cout<<mean<<std::endl;
  while(arrayZ[iArr]<zMax){
    ++nZint;
    // std::cout<<arrayZ[iArr]<<std::endl;
    mean+=arrayZ[iArr];
    ++iArr;
  }
  mean/=nZint;
  // compute rms
  iArr=iMin+1;
  nZint=1.;
  rms=(arrayZ[iMin]-mean)*(arrayZ[iMin]-mean);
  while(!(arrayZ[iArr]>zMax)){
    ++nZint;
    rms+=(arrayZ[iArr]-mean)*(arrayZ[iArr]-mean);
    ++iArr;
  }
  if(nZint>1.5)rms/=(nZint-1.);
  rms=TMath::Sqrt(rms);
}

// ^ r
// .        .
// .       .   (r2,z2)
// .      .
// .     .   (r1,z1)
// .    .
// -------------> z
// (z-z2)=(z2-z1)/(r2-r1)*(r-r2)

double Vertexer::FindZintersect(double z1,double z2,double r1,double r2) const{
  return -(z2-z1)/(r2-r1)*r2+z2; // the intersection of a 2D (in the z-r plane) straight line with the z axis
}

int Vertexer::FindFirstMaximum(TH1D* hist){
  int nBins=hist->GetNbinsX();
  int nBinMax=1;
  for(int iBins=2;iBins<=nBins;++iBins){
    if(hist->GetBinContent(iBins)>hist->GetBinContent(nBinMax))
      nBinMax=iBins;
  }
  return nBinMax;
}

int Vertexer::FindSecondMaximum(TH1D* hist){
  int nBins=hist->GetNbinsX();
  int nBinMax=nBins;
  for(int iBins=nBins-1;iBins>0;--iBins){
    if(hist->GetBinContent(iBins)>hist->GetBinContent(nBinMax)) 
      nBinMax=iBins;
  }
  return nBinMax;
}