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
#include "../cfg/Constants.h"

const double kDeltaZtrue=4;

void CreateHist(std::string inFilename="recResult",std::string outFilename="HistResult_Eff_Reso"){
  std::string inFilename_ext=FILE_DIR+inFilename+".root";      // filename with *.root extension
  std::string outFilename_ext=FILE_DIR+outFilename+".root";    // filename with *.root extension

  // SET HISTOGRAM STYLE
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerColor(kRed);
  gStyle->SetMarkerSize(0.8);
  gStyle->SetDrawOption("PE");
  
  // OPEN FILE AND GET HISTOGRAMS
  TFile inFile(inFilename_ext.c_str());
  if(!inFile.IsOpen()){
    std::cout<<"No input file!"<<std::endl;
    return;
  }
  TFile *outFile=new TFile(outFilename_ext.c_str(),"RECREATE");  // open a file (write mode)
  TH3D *hZtrueMultRes=(TH3D*)inFile.Get("hZtrueMultRes");
  hZtrueMultRes->GetXaxis()->SetTitle("Z_{true} (cm)");
  hZtrueMultRes->GetYaxis()->SetTitle("Multiplicity");
  hZtrueMultRes->GetZaxis()->SetTitle("Z_{rec}-Z_{true} (#mum)");
  TH2D *hZtrueMultNrec=(TH2D*)inFile.Get("hZtrueMultNrec");
  TH2D *hZtrueMultNsim=(TH2D*)inFile.Get("hZtrueMultNsim");
  
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
  for(int iZtrue=1+kDeltaZtrue;iZtrue<=kNzTrueBins-kDeltaZtrue;iZtrue++){
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

  // DECLARE RESOLUTION HISTOGRAMS
  TH1D *hMultResol=new TH1D("hMultResol","Resolution vs. Multiplicity",kNmultBins,kMultBins);
  TH1D *hZtrueResol=new TH1D("hZtrueResol","Resolution vs. Z_{true}",kNzTrueBins,kZtrueBins);

  TF1 *fitFun=new TF1("fitFun","gaus",kResMin,kResMax); // declare fit function
  for(int iMult=1;iMult<=kNmultBins;iMult++){
    sprintf(histNameRes,"hZtrueMultRes_projResMult_%d",iMult);
    sprintf(histTitleRes,"Residuals, %5.1f #leq mult < %5.1f",hZtrueMultRes->GetYaxis()->GetBinLowEdge(iMult),hZtrueMultRes->GetYaxis()->GetBinUpEdge(iMult));
    TH1D *projectionOnRes_mult=hZtrueMultRes->ProjectionZ(histNameRes,1,kNzTrueBins,iMult,iMult);
    projectionOnRes_mult->SetTitle(histTitleRes);
    // GAUSSIAN FIT TO GET RESOLUTION
    fitFun->SetParLimits(0,0.,1.e9);
    fitFun->SetParLimits(1,-100.,100.);
    fitFun->SetParLimits(2,0.,600.);
    projectionOnRes_mult->Fit(fitFun,"q");
    projectionOnRes_mult->Write();
    hMultResol->SetBinContent(iMult,fitFun->GetParameter(2));
    hMultResol->SetBinError(iMult,fitFun->GetParError(2));
  }
  hMultResol->Write();
  for(int iZtrue=1+kDeltaZtrue;iZtrue<=kNzTrueBins-kDeltaZtrue;iZtrue++){
    sprintf(histNameRes,"hZtrueMultRes_projResZtrue_%d",iZtrue);
    TH1D *projectionOnRes_ztrue=hZtrueMultRes->ProjectionZ(histNameRes,iZtrue,iZtrue,1,kNmultBins);
    // GAUSSIAN FIT TO GET RESOLUTION
    fitFun->SetParLimits(0,0.,1.e9);
    fitFun->SetParLimits(1,-100.,100.);
    fitFun->SetParLimits(2,0.,600.);
    projectionOnRes_ztrue->Fit(fitFun,"q");
    projectionOnRes_ztrue->Write();
    hZtrueResol->SetBinContent(iZtrue,fitFun->GetParameter(2));
    hZtrueResol->SetBinError(iZtrue,fitFun->GetParError(2));
  }
  hZtrueResol->Write();
  // WRITE AND CLOSE FILE
  outFile->Close();
  inFile.Close();
}

