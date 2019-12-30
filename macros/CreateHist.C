#include <Riostream.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TFile.h>
#include <TProfile.h>
#include <TCanvas.h>

const double kResMin=-10.0;  // cm
const double kResMax=10.0;   // cm
const int kNresBinLim=401;
const int kNzTrueBins=18;
const int kNmultBins=12;
const double kZtrueBins[]={-30.0,-27.0,-25.0,-23.0,-20.0,-15.0,-10.0,-5.0,-2.5,0.0,2.5,5.0,10.0,15.0,20.0,23.0,25.0,27.0,30.0};
const double kMultBins[]={2.5,3.5,4.5,5.5,7.5,9.5,14.5,24.5,34.5,44.5,54.5,70.0,100.0};

void CreateHist(std::string inFilename="recResult",std::string outFilename="HistResult_Eff_Reso"){
  std::string inFilename_ext=inFilename+".root";      // filename with *.root extension
  std::string outFilename_ext=outFilename+".root";    // filename with *.root extension

  // OPEN FILE AND GET HISTOGRAMS
  TFile *inFile=new TFile(inFilename_ext.c_str());
  
  TH3D *hZtrueMultRes=(TH3D*)inFile->Get("hZtrueMultRes");
  hZtrueMultRes->GetXaxis()->SetTitle("Z_{true}");
  hZtrueMultRes->GetYaxis()->SetTitle("Multiplicity");
  hZtrueMultRes->GetZaxis()->SetTitle("Z_{rec}-Z_{true}");
  TH2D *hZtrueMultNrec=(TH2D*)inFile->Get("hZtrueMultNrec");
  TH2D *hZtrueMultNsim=(TH2D*)inFile->Get("hZtrueMultNsim");
  
  // CREATE 2D HISTOGRAMS FOR EFFICIENCY
  TH2D *hZtrueMultEff=new TH2D("hZtrueMultEff","hZtrueMultEff",kNzTrueBins,kZtrueBins,kNmultBins,kMultBins);
  hZtrueMultEff->SetTitle("Efficiency");
  hZtrueMultEff->GetXaxis()->SetTitle("Z_{true}");
  hZtrueMultEff->GetYaxis()->SetTitle("Multiplicity");
  for(int iZtrue=1;iZtrue<=kNzTrueBins;iZtrue++){
    for(int iMult=1;iMult<=kNmultBins;iMult++){
      double eff;
      if((double)hZtrueMultNsim->GetBinContent(iZtrue,iMult)!=0)
          eff=(double)hZtrueMultNrec->GetBinContent(iZtrue,iMult)/(double)hZtrueMultNsim->GetBinContent(iZtrue,iMult);
      else eff=0;
      hZtrueMultEff->SetBinContent(iZtrue,iMult,eff);
      // std::cout<<hZtrueMultEff->GetBinContent(iZtrue,iMult)<<"\t";
    }
    // std::cout<<""<<std::endl;
  }

  // GET EFFICIENCY VS Z TRUE WITH INTEGRATED MULTIPLICITY FROM multBinMin TO multBinMax
  int multBinMin=11;
  int multBinMax=11;
  TH1D *projectionOnZ=new TH1D("projectionOnZ","projectionOnZ",kNzTrueBins,kZtrueBins);
  projectionOnZ=hZtrueMultEff->ProjectionX("px",multBinMin,multBinMax);
  

  // GET EFFICIENCY VS MULT WITH ZTRUE FROM zTrueBinMin TO zTrueBinMax
  int zTrueBinMin=12;
  int zTrueBinMax=12;
  TH1D *projectionOnMult=new TH1D("projectionOnMult","projectionOnMult",kNmultBins,kMultBins);
  projectionOnMult=hZtrueMultEff->ProjectionY("py",zTrueBinMin,zTrueBinMax);
  
  // GET RESIDUES HISTOGRAM
  double ResBins[kNresBinLim];                        // define residues binning
  double ResStep=(kResMax-kResMin)/(kNresBinLim-1.);
  for(int iBin=0;iBin<kNresBinLim;++iBin)ResBins[iBin]=kResMin+ResStep*iBin;
  TH1D *projectionOnRes=new TH1D("projectionOnRes","projectionOnRes",kNresBinLim,ResBins);
  projectionOnRes=hZtrueMultRes->ProjectionZ("pz",multBinMin,multBinMax,zTrueBinMin,zTrueBinMax);
  
  // DRAW HISTOGRAM
  TCanvas *c0=new TCanvas();
  c0->cd();
  projectionOnRes->Draw(); 
  TCanvas *c1=new TCanvas();
  c1->cd();
  projectionOnZ->Draw();
  TCanvas *c2=new TCanvas();
  c2->cd();
  projectionOnMult->Draw();

  // WRITE AND CLOSE FILE
  TFile *outFile=new TFile(outFilename_ext.c_str(),"RECREATE");  // open a file (write mode)
  hZtrueMultEff->Write();
  projectionOnZ->Write();
  projectionOnMult->Write();
  projectionOnRes->Write();
  outFile->Close();
}