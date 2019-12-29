// Vertexer.cxx
// Implementation of Vertexer.h
//
// Authors: Mario Ciacco & Emanuele Data

#include <TMath.h>
#include "Vertexer.h"

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

bool Vertexer::FindVertex(TH1D* hZrec,double& zTmp,const double deltaZ,const double deltaNentries) const{
  int bFirstMax=FindFirstMaximum(hZrec);
  int bSecondMax=FindSecondMaximum(hZrec,bFirstMax);
  double deltaZobs=fabs((double)(bFirstMax-bSecondMax))*hZrec->GetBinWidth(1); // compute the distance between the two highest bins
  if(deltaZobs<deltaZ){ // horizontal (z) cut
    zTmp=hZrec->GetBinCenter(bFirstMax);
    return true;
  }
  else {
    double deltaNentrObs=hZrec->GetBinContent(bFirstMax)-hZrec->GetBinContent(bSecondMax);
    if(deltaNentrObs>deltaNentries) return true;
    return false;
  }
}

void Vertexer::FitVertex(double *arrayZ,double& mean,double& rms,double zMin,double zMax) const{
  int iArr=0;
  int iMin=0;
  double nZint=0;
  // compute mean
  while(arrayZ[iArr]<zMin)++iArr; // find the smallest z used to compute the mean
  iMin=iArr;
  while(!(arrayZ[iArr]>zMax)){
    ++nZint;
    mean+=arrayZ[iArr++];
  }
  mean/=nZint;
  // compute rms
  iArr=iMin;
  nZint=0.;
  while(!(arrayZ[iArr]>zMax)){
    ++nZint;
    rms+=(arrayZ[iArr]-mean)*(arrayZ[iArr]-mean);
    ++iArr;
  }
  rms/=(nZint-1);
  rms=TMath::Sqrt(rms);
}

double Vertexer::FindZintersect(Point2D p1,Point2D p2,double r1,double r2) const{
  double z1=p1.GetZ();
  double z2=p2.GetZ();
  return (z2-z1)/(r2-r1)*r2+z2; // the intersection of a 2D (in the z-r plane) straight line with the z axis
}

int Vertexer::FindFirstMaximum(TH1D* hist){
  int nBins=hist->GetNbinsX();
  int nBinMax=1;
  for(int iBins=2;iBins<nBins;++iBins){
    if(hist->GetBinContent(iBins)>hist->GetBinContent(nBinMax))
      nBinMax=iBins;
  }
  return nBinMax;
}

int Vertexer::FindSecondMaximum(TH1D* hist,int firstMaxBin){
  int nBins=hist->GetNbinsX();
  int nBinMax=1;
  for(int iBins=2;iBins<nBins;++iBins){
    if(hist->GetBinContent(iBins)>hist->GetBinContent(nBinMax)&&hist->GetBinContent(iBins)<hist->GetBinContent(firstMaxBin))
      nBinMax=iBins;
  }
  return nBinMax;
}