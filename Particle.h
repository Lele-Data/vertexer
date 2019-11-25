// Particle.h
// The class of an MC particle
//
// Authors: Mario Ciacco & Emanuele Data

#include <TObject.h>

class Particle : public TObject{
 public:
  Particle(double x=0.,double y=0.,double z=0.,double theta=0.,double phi=0.);
  ~Particle();
  void SetPoint(double x, double y, double z);
  void SetDirection(double theta, double phi);
  double GetX() const;
  double GetY() const;
  double GetZ() const;
  double GetTheta() const;
  double GetPhi() const;
 private:
  double fX;
  double fY;
  double fZ;
  double fTheta;
  double fPhi;

 ClassDef(Particle,1);
};
