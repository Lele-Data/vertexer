// Vertex.h
// The class of a vertex
//
// Authors: Mario Ciacco & Emanuele Data

#ifndef VERTEX_H
#define VERTEX_H

#include <TObject.h>

class Vertex : public TObject{
 public:
  Vertex(double zrec=-999.f,double zrecerr=-999.f,double ztrue=-999.f,double mult=-999.f,bool reconstruct=false);
  ~Vertex();
  double GetZrec() const;
  double GetZrecErr() const;
  double GetZtrue() const;
  double GetMult() const;
  bool IsReconstruct() const;
 private:
  double fZrec;
  double fZrecErr;
  double fZtrue;
  double fMult;
  bool fReconstruct;

 ClassDef(Vertex,1);
};

#endif // VERTEX_H