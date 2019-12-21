// test generator and propagator

#include <TCanvas.h>
#include <TH3D.h>
#include <TLine.h>
#include <Riostream.h>
#include <TMath.h>
#include "Generator.h"
#include "Particle.h"
#include "Cylinder.h"
#include "Layer.h"
#include "BeamPipe.h"
#include "Point2D.h"
#include "Propagator.h"

void PrintParticle(Particle);

void testPropagator(){
  const int nBinsX=100;
  const int nBinsY=100;
  const int nBinsZ=100;
  const double xmin=-5., xmax=5;
  const double ymin=-5, ymax=5;
  const double zmin=-30, zmax=30;
  TH3D *hist=new TH3D("h","hist",nBinsX,xmin,xmax,nBinsY,ymin,ymax,nBinsZ,zmin,zmax);
  hist->SetMarkerStyle(21);
  hist->SetMarkerSize(1.0);
  double xv,yv,zv;
  Generator *g=Generator::GetInstance();
  BeamPipe *bp=new BeamPipe(3,0.08); // cm
  Layer *l1=new Layer(4,0.02,27); //cm
  Propagator *prop=Propagator::GetInstance();
  g->GenerateVertex(xv,yv,zv);
  hist->Fill(xv,yv,zv);
  for(int i=0;i<50;++i){
    Particle p1(0,0,0,0,0);
    p1.SetPoint(xv,yv,zv);
    PrintParticle(p1);
    g->GenerateParticle(p1,1);
    PrintParticle(p1);
    // intersezione beam pipe
    std::cout<<"-->intersezione beam pipe"<<std::endl;
    prop->Intersection(p1,bp);
    PrintParticle(p1);
    hist->Fill(p1.GetX(),p1.GetY(),p1.GetZ());
    // controllo che l'intersezione appartenga alla beampipe
    double R=TMath::Sqrt(p1.GetX()*p1.GetX()+p1.GetY()*p1.GetY());
    std::cout<<"controllo che l'intersezione appartenga alla beampipe "<<R<<std::endl;
    // intersezione layer 1
    std::cout<<"-->intersezione layer 1"<<std::endl;
    Point2D hit1;
    prop->Intersection(p1,l1);
    if(p1.GetZ()<l1->GetLength()/2.&&p1.GetZ()>-l1->GetLength()/2.){
      hit1=prop->ComputeHit(p1,l1);
      std::cout<<"Hit: Z="<<hit1.GetZ()<<" phi="<<hit1.GetPhi()<<std::endl;
      R=TMath::Sqrt(p1.GetX()*p1.GetX()+p1.GetY()*p1.GetY());
      std::cout<<"controllo che l'intersezione appartenga al layer 1 "<<R<<std::endl;
      PrintParticle(p1);
      hist->Fill(p1.GetX(),p1.GetY(),p1.GetZ());
    }
  }
  hist->Draw("P");
  prop=Propagator::Destroy();
  g=Generator::Destroy();
}

void PrintParticle(Particle p1){
  std::cout<<"----------------------- "<<std::endl;
  std::cout<<"particle x "<<p1.GetX()<<std::endl;
  std::cout<<"particle y "<<p1.GetY()<<std::endl;
  std::cout<<"particle z "<<p1.GetZ()<<std::endl;
  std::cout<<"particle theta "<<p1.GetTheta()<<std::endl;
  std::cout<<"particle phi "<<p1.GetPhi()<<std::endl;
  std::cout<<"----------------------- "<<std::endl;
}
