// DrawHist.C
// This macro draws on canvas the histograms created by CreateHist.C
//
// Authors: Mario Ciacco & Emanuele Data

#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1D.h>
#include "../cfg/Constants.h"

void DrawHist(std::string inFilename="HistResult_Eff_Reso"){
  std::string inFilename_ext=FILE_DIR+inFilename+".root";      // filename with *.root extension

  gStyle->SetPadGridX(true);
  gStyle->SetPadGridY(true);
  gStyle->SetOptFit(1111);

  TFile *resultFile=new TFile(inFilename_ext.c_str());
  TH1D *projectionOnRes=(TH1D*)resultFile->Get("hZtrueMultRes_projRes_11");
  TH1D *hMultEff=(TH1D*)resultFile->Get("hMultEff");
  TH1D *hZtrueEff=(TH1D*)resultFile->Get("hZtrueEff");
  TH1D *hMultResol=(TH1D*)resultFile->Get("hMultResol");
  TH1D *hZtrueResol=(TH1D*)resultFile->Get("hZtrueResol");

  TCanvas *c1=new TCanvas("c1");
  c1->cd();
  projectionOnRes->Draw("PE");
  TCanvas *c2=new TCanvas("c2");
  c2->cd();
  hMultEff->Draw("PE1");
  TCanvas *c3=new TCanvas("c3");
  c3->cd();
  hZtrueEff->Draw("PE1");
  TCanvas *c4=new TCanvas("c4");
  c4->cd();
  hMultResol->Draw("PE1");
  TCanvas *c5=new TCanvas("c5");
  c5->cd();
  hZtrueResol->Draw("PE1");
}