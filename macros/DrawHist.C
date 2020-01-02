#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include "../cfg/Constants.h"

void DrawHist(std::string inFilename="HistResult_Eff_Reso"){
  std::string inFilename_ext=FILE_DIR+inFilename+".root";      // filename with *.root extension

  TFile *resultFile=new TFile(inFilename_ext.c_str());
  TH1D *projectionOnRes=(TH1D*)resultFile->Get("hZtrueMultRes_projRes_12");
  TH1D *projectionOnRes_Mult7=(TH1D*)resultFile->Get("hZtrueMultRes_projResMult_7");
  TH1D *hMultEff=(TH1D*)resultFile->Get("hMultEff");
  TH1D *hZtrueEff=(TH1D*)resultFile->Get("hZtrueEff");
  TH1D *hMultResol=(TH1D*)resultFile->Get("hMultResol");
  TH1D *hZtrueResol=(TH1D*)resultFile->Get("hZtrueResol");

  TCanvas *c1=new TCanvas("c1");
  c1->cd();
  projectionOnRes->Draw();
  TCanvas *c1a=new TCanvas("c1a");
  c1a->cd();
  projectionOnRes_Mult7->Draw();
  TCanvas *c2=new TCanvas("c2");
  c2->cd();
  hMultEff->Draw();
  TCanvas *c3=new TCanvas("c3");
  c3->cd();
  hZtrueEff->Draw();
  TCanvas *c4=new TCanvas("c4");
  c4->cd();
  hMultResol->Draw();
  TCanvas *c5=new TCanvas("c5");
  c5->cd();
  hZtrueResol->Draw();
}