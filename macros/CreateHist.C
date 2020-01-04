// CreateHist.h
// This macro reads the tree of vertices and creates histograms
//
// Authors: Mario Ciacco & Emanuele Data

#include <Riostream.h>
#include <TStyle.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TLatex.h>
#include <TCanvas.h>
#include <TF1.h>
#include "../src/Vertex.h"
#include "../cfg/Constants.h"

void CreateHist(std::string inFilename="recResult",std::string outFilename="HistResult_Eff_Reso"){
  std::string inFilename_ext=FILE_DIR+inFilename+".root";      // filename with *.root extension
  std::string outFilename_ext=FILE_DIR+outFilename+".root";    // filename with *.root extension

  // SET HISTOGRAM STYLE
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerColor(kRed);
  gStyle->SetMarkerSize(0.8);
  gStyle->SetDrawOption("PE");
  gStyle->SetOptFit(1111);

  // MEMORY LOCATION MAPPED FROM (INPUT) TREE
  Vertex *vtx=new Vertex();

  // OPEN FILE AND GET TREE
  TFile inFile(inFilename_ext.c_str());
  TFile *outFile=new TFile(outFilename_ext.c_str(),"RECREATE");  // open a file (write mode)
  if(!inFile.IsOpen()){
    std::cout<<"No input file!"<<std::endl;
    return;
  }
  TTree *inTree=(TTree*)inFile.Get(RecTreeName);
  TBranch *bVert=inTree->GetBranch(RecVertBaranchName);
  bVert->SetAddress(&vtx);

  // INSTANTIATE HISTOGRAMS
  double ResBins[kNresBinLim];                        // define residual binning
  double ResStep=(kResMax-kResMin)/(kNresBinLim-1.);
  for(int iBin=0;iBin<kNresBinLim;++iBin)ResBins[iBin]=kResMin+ResStep*iBin;

  TH3D *hZtrueMultRes=new TH3D("hZtrueMultRes","hZtrueMultRes",kNzTrueBins,kZtrueBins,kNmultBins,kMultBins,kNresBinLim-1,ResBins);
  TH2D *hZtrueMultNrec=new TH2D("hZtrueMultNrec","hZtrueMultNrec",kNzTrueBins,kZtrueBins,kNmultBins,kMultBins);
  TH2D *hZtrueMultNsim=new TH2D("hZtrueMultNsim","hZtrueMultNsim",kNzTrueBins,kZtrueBins,kNmultBins,kMultBins);
  hZtrueMultRes->GetXaxis()->SetTitle("Z_{true} (cm)");
  hZtrueMultRes->GetYaxis()->SetTitle("Multiplicity");
  hZtrueMultRes->GetZaxis()->SetTitle("Z_{rec}-Z_{true} (#mum)");

  // PROCESS EVENTS (VERTICES) IN TREE AND FILL HISTOGRAMS
  for(int iEvent=0;iEvent<inTree->GetEntries();++iEvent){
    inTree->GetEvent(iEvent);
    if(vtx->IsReconstruct()){
      double res=(vtx->GetZrec()-vtx->GetZtrue())*10000.; // um
      hZtrueMultRes->Fill(vtx->GetZtrue(),vtx->GetMult(),res);
      hZtrueMultNrec->Fill(vtx->GetZtrue(),vtx->GetMult());
    }
    hZtrueMultNsim->Fill(vtx->GetZtrue(),vtx->GetMult());
  }

  hZtrueMultRes->Write();
  hZtrueMultNrec->Write();
  hZtrueMultNsim->Write();

  // CREATE HISTOGRAMS FOR EFFICIENCY
  TH1D *hZtrueNrec=hZtrueMultNrec->ProjectionX("hZtrueNrec",1,kNmultBins);
  TH1D *hMultNrec=hZtrueMultNrec->ProjectionY("hMultNrec",1,kNzTrueBins);
  TH1D *hZtrueNsim=hZtrueMultNsim->ProjectionX("hZtrueNsim",1,kNmultBins);
  TH1D *hMultNsim=hZtrueMultNsim->ProjectionY("hMultNsim",1,kNzTrueBins);
  TH1D *hZtrueEff=new TH1D("hZtrueEff","Efficiency vs. Z_{true}",kNzTrueBins,kZtrueBins);
  TH1D *hMultEff=new TH1D("hMultEff","Efficiency vs. Multiplicity",kNmultBins,kMultBins);
  hZtrueEff->GetXaxis()->SetTitle("Z_{true} (cm)");
  hZtrueEff->GetYaxis()->SetTitle("Efficiency");
  hMultEff->GetXaxis()->SetTitle("Multiplicity");
  hMultEff->GetYaxis()->SetTitle("Efficiency");
  
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
  
  // GET RESIDUAL HISTOGRAM
  char histNameRes[50];
  char histTitleRes[50];
  
  sprintf(histNameRes,"hZtrueMultRes_projRes_%d",kNmultBins);
  sprintf(histTitleRes,"Residuals, %5.1f #leq mult < %5.1f",hZtrueMultRes->GetYaxis()->GetBinLowEdge(1),hZtrueMultRes->GetYaxis()->GetBinUpEdge(kNmultBins));
  TH1D *projectionOnRes=hZtrueMultRes->ProjectionZ(histNameRes,1,kNmultBins,1,kNzTrueBins);
  projectionOnRes->SetTitle(histTitleRes);
  projectionOnRes->Write();

  // DECLARE RESOLUTION HISTOGRAMS
  TH1D *hMultResol=new TH1D("hMultResol","Resolution vs. Multiplicity",kNmultBins,kMultBins);
  TH1D *hZtrueResol=new TH1D("hZtrueResol","Resolution vs. Z_{true}",kNzTrueBins,kZtrueBins);
  hMultResol->GetXaxis()->SetTitle("Multiplicity");
  hMultResol->GetYaxis()->SetTitle("Resolution (#mum)");
  hZtrueResol->GetXaxis()->SetTitle("Z_{true} (cm)");
  hZtrueResol->GetYaxis()->SetTitle("Resolution (#mum)");

  for(int iMult=1;iMult<=kNmultBins;iMult++){
    sprintf(histNameRes,"hZtrueMultRes_projResMult_%d",iMult);
    sprintf(histTitleRes,"Residuals, %5.1f #leq mult < %5.1f",hZtrueMultRes->GetYaxis()->GetBinLowEdge(iMult),hZtrueMultRes->GetYaxis()->GetBinUpEdge(iMult));
    TH1D *projectionOnRes_mult=hZtrueMultRes->ProjectionZ(histNameRes,1,kNzTrueBins,iMult,iMult);
    projectionOnRes_mult->SetTitle(histTitleRes);
    // GAUSSIAN FIT TO GET RESOLUTION
    TF1 *fitFun=new TF1("fitFun","gaus",kResMin,kResMax); // declare fit function
    fitFun->SetParLimits(0,0.,1.e5);
    fitFun->SetParLimits(1,-50.,50.);
    fitFun->SetParLimits(2,0.,600.);
    fitFun->SetLineColor(kBlue+3);
    double hist_rms=projectionOnRes_mult->GetRMS();
    projectionOnRes_mult->Fit(fitFun,"QLM+","",-1.5*hist_rms,1.5*hist_rms);
    projectionOnRes_mult->GetXaxis()->SetRangeUser(-4.*fitFun->GetParameter(2),4.*fitFun->GetParameter(2));
    projectionOnRes_mult->SetMarkerStyle(20);
    projectionOnRes_mult->SetMarkerColor(kRed);
    projectionOnRes_mult->SetMarkerSize(0.8);
    projectionOnRes_mult->SetDrawOption("PE");
    projectionOnRes_mult->Write();
    hMultResol->SetBinContent(iMult,fitFun->GetParameter(2));
    hMultResol->SetBinError(iMult,fitFun->GetParError(2));
  }
  hMultResol->Write();

  for(int iZtrue=1;iZtrue<=kNzTrueBins;iZtrue++){
    sprintf(histNameRes,"hZtrueMultRes_projResZtrue_%d",iZtrue);
    TH1D *projectionOnRes_ztrue=hZtrueMultRes->ProjectionZ(histNameRes,iZtrue,iZtrue,1,kNmultBins);
    // GAUSSIAN FIT TO GET RESOLUTION
    TF1 *fitFun=new TF1("fitFun","gaus",kResMin,kResMax); // declare fit function
    fitFun->SetParLimits(0,0.,1.e5);
    fitFun->SetParLimits(1,-50.,50.);
    fitFun->SetParLimits(2,0.,600.);
    fitFun->SetLineColor(kBlue+3);
    projectionOnRes_ztrue->Fit(fitFun,"QLM+","",-190.,190.);
    projectionOnRes_ztrue->Write();
    projectionOnRes_ztrue->SetMarkerStyle(20);
    projectionOnRes_ztrue->SetMarkerColor(kRed);
    projectionOnRes_ztrue->SetMarkerSize(0.8);
    projectionOnRes_ztrue->SetDrawOption("PE");
    hZtrueResol->SetBinContent(iZtrue,fitFun->GetParameter(2));
    hZtrueResol->SetBinError(iZtrue,fitFun->GetParError(2));
  }
  hZtrueResol->Write();

  // WRITE AND CLOSE FILE
  outFile->Close();
  inFile.Close();
}