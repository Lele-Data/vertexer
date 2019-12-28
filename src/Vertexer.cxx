// Vertexer.cxx
// Implementation of Vertexer.h
//
// Authors: Mario Ciacco & Emanuele Data

#include "Vertexer.h"

ClassImp(Vertexer);

Vertexer *Vertexer::fInstance;     // static data member

Vertexer::Vertexer(){

}

Vertexer::~Vertexer(){

}

Vertexer *Vertexer::GetInstance(){

}

Vertexer *Vertexer::Destroy(){

}

bool Vertexer::FindVertex(TH1D* hZrec,double& zTmp,const double deltaZ,const double deltaNentries) const{

}

void Vertexer::FitVertex(Point2D *arrayHits,double& mean,double& rms,double zMin,double zMax) const{

}

double Vertexer::FindZintersect(Point2D p1,Point2D p2,double r1,double r2) const{

}