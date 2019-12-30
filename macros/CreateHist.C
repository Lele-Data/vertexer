#include <Riostream.h>
#include <TStyle.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TFile.h>
#include <TMath.h>
#include <TLatex.h>
#include <TCanvas.h>
#include <TF1.h>

#ifndef CONSTANT
#define CONSTANT
const double kResMin=-1000.; // um
const double kResMax=1000.;  // um
const int kNresBinLim=401;
const int kNzTrueBins=18;
const int kNmultBins=12;
const double kZtrueBins[]={-30.0,-27.0,-25.0,-23.0,-20.0,-15.0,-10.0,-5.0,-2.5,0.0,2.5,5.0,10.0,15.0,20.0,23.0,25.0,27.0,30.0};
const double kMultBins[]={2.5,3.5,4.5,5.5,7.5,9.5,14.5,24.5,34.5,44.5,54.5,70.0,100.0};
#endif // CONSTANT

#ifndef FILEDIR
#define FILEDIR
const char *FILE_DIR="results/";
#endif // FILEDIR

void CreateHist(std::string inFilename="recResult",std::string outFilename="HistResult_Eff_Reso"){
  std::string inFilename_ext=FILE_DIR+inFilename+".root";      // filename with *.root extension
  std::string outFilename_ext=FILE_DIR+outFilename+".root";    // filename with *.root extension

  // SET HISTOGRAM STYLE
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerColor(kRed);
  gStyle->SetMarkerSize(0.8);
  gStyle->SetDrawOption("PE");
  
  // OPEN FILE AND GET HISTOGRAMS
  TFile *inFile=new TFile(inFilename_ext.c_str());
  TFile *outFile=new TFile(outFilename_ext.c_str(),"RECREATE");  // open a file (write mode)
  TH3D *hZtrueMultRes=(TH3D*)inFile->Get("hZtrueMultRes");
  hZtrueMultRes->GetXaxis()->SetTitle("Z_{true} (cm)");
  hZtrueMultRes->GetYaxis()->SetTitle("Multiplicity");
  hZtrueMultRes->GetZaxis()->SetTitle("Z_{rec}-Z_{true} (#mum)");
  TH2D *hZtrueMultNrec=(TH2D*)inFile->Get("hZtrueMultNrec");
  TH2D *hZtrueMultNsim=(TH2D*)inFile->Get("hZtrueMultNsim");
  
  // CREATE HISTOGRAMS FOR EFFICIENCY
  TH1D *hZtrueNrec=hZtrueMultNrec->ProjectionX("hZtrueNrec",1,kNmultBins);
  TH1D *hMultNrec=hZtrueMultNrec->ProjectionY("hMultNrec",1,kNzTrueBins);
  TH1D *hZtrueNsim=hZtrueMultNsim->ProjectionX("hZtrueNsim",1,kNmultBins);
  TH1D *hMultNsim=hZtrueMultNsim->ProjectionY("hMultNsim",1,kNzTrueBins);
  TH1D *hZtrueEff=new TH1D("hZtrueEff","Efficiency vs. Z_{true}",kNzTrueBins,kZtrueBins);
  TH1D *hMultEff=new TH1D("hMultEff","Efficiency vs. Multiplicity",kNmultBins,kMultBins);
  hZtrueEff->GetXaxis()->SetTitle("Z_{true}");
  hMultEff->GetXaxis()->SetTitle("Multiplicity");
  
  // COMPUTE EFFICIENCY vs Ztrue
  for(int iZtrue=1;iZtrue<=kNzTrueBins;iZtrue++){
    if(hZtrueNsim->GetBinContent(iZtrue)<1.e-9) continue;
    double n_tot=hZtrueNsim->GetBinContent(iZtrue);
    double n_rec=hZtrueNrec->GetBinContent(iZtrue);
    double eff=n_rec/n_tot; // compute efficiency
    double eff_err=TMath::Sqrt(eff*(1-eff)/n_tot); // compute error on efficiency (binomial)
    hZtrueEff->SetBinContent(iZtrue,eff);
    hZtrueEff->SetBinError(iZtrue,eff_err);
  }
  hZtrueEff->Write();
  
  // COMPUTE EFFICIENCY vs MULTIPLICITY
  for(int iMult=1;iMult<=kNmultBins;iMult++){
    if(hMultNsim->GetBinContent(iMult)<1.e-9) continue;
    double n_tot=hMultNsim->GetBinContent(iMult);
    double n_rec=hMultNrec->GetBinContent(iMult);
    double eff=n_rec/n_tot; // compute efficiency
    double eff_err=TMath::Sqrt(eff*(1-eff)/n_tot); // compute error on efficiency (binomial)
    hMultEff->SetBinContent(iMult,eff);
    hMultEff->SetBinError(iMult,eff_err);
  }
  hMultEff->Write();
  
  // GET RESIDUES HISTOGRAM
  char histNameRes[50];
  char histTitleRes[50];
  double ResBins[kNresBinLim]; // define residues binning
  double ResStep=(kResMax-kResMin)/(kNresBinLim-1.);
  for(int iBin=0;iBin<kNresBinLim;++iBin)ResBins[iBin]=kResMin+ResStep*iBin;
  
  sprintf(histNameRes,"hZtrueMultRes_projRes_%d",kNmultBins);
  sprintf(histTitleRes,"Residues, %5.1f #leq mult < %5.1f",hZtrueMultRes->GetYaxis()->GetBinLowEdge(1),hZtrueMultRes->GetYaxis()->GetBinUpEdge(kNmultBins));
  TH1D *projectionOnRes=hZtrueMultRes->ProjectionZ(histNameRes,1,kNmultBins,1,kNzTrueBins);
  projectionOnRes->SetTitle(histTitleRes);
  projectionOnRes->Write();

  for(int iMult=1;iMult<=kNmultBins;iMult++){
    sprintf(histNameRes,"hZtrueMultRes_projRes_%d",iMult);
    sprintf(histTitleRes,"Residuals, %5.1f #leq mult < %5.1f",hZtrueMultRes->GetYaxis()->GetBinLowEdge(iMult),hZtrueMultRes->GetYaxis()->GetBinUpEdge(iMult));
    TH1D *projectionOnRes=hZtrueMultRes->ProjectionZ(histNameRes,1,kNzTrueBins,iMult,iMult);
    projectionOnRes->SetTitle(histTitleRes);
    TF1 *fitFun=new TF1("fitFun","gaus",kResMin,kResMax);
    // GAUSSIAN FIT TO GET RESOLUTION
    fitFun->SetParLimits(0,0.,1.e8);
    fitFun->SetParLimits(1,-50.,50.);
    fitFun->SetParLimits(2,0.,500.);
    projectionOnRes->Fit(fitFun);
    projectionOnRes->Write();
  }
  
  // WRITE AND CLOSE FILE
  outFile->Close();
}

