// Vertex.cxx
// Implementation of Vertex.h
//
// Authors: Mario Ciacco & Emanuele Data

#include "Vertex.h"

ClassImp(Vertex);

Vertex::Vertex(double zrec,double zrecerr,double ztrue,double mult,bool reconstruct):TObject(),
fZrec(zrec),
fZrecErr(zrecerr),
fZtrue(ztrue),
fMult(mult),
fReconstruct(reconstruct)
{
  // constructor
}

Vertex::~Vertex(){
  // destructor
}

double Vertex::GetZrec() const{
  return fZrec;
}

double Vertex::GetZrecErr() const{
  return fZrecErr;
}

double Vertex::GetZtrue() const{
  return fZtrue;
}

double Vertex::GetMult() const{
  return fMult;
}

bool Vertex::IsReconstruct() const{
  return fReconstruct;
}