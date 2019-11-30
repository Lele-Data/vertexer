// Propagator.cxx
// Implementation of Propagator.h
//
// Author: Mario Ciacco & Emanuele Data

#include "Propagator.h"
#include <TRandom3.h>
#include <Riostream.h>

ClassImp(Propagator);

double ComputeT(double,double,double,double,double); // helper function
void ComputePoint(Particle,double&,double&,double&,double);

Propagator::Propagator():TObject(){
  RandomScat[0]=OffScattering;
  RandomScat[1]=Onscattering;
}

Propagator::~Propagator(){}

void Propagator::MultipleScattering(Particle& particle,int nScatMethod){
  
}

void Propagator::Intersection(Particle& particle,Cylinder *cylinder){
  double x,y,z;
  double t_val=ComputeT(particle.GetTheta(),particle.GetPhi(),cylinder->GetRadius()+cylinder->GetThickness(),particle.GetX(),particle.GetY());
  ComputePoint(particle,x,y,z,t_val);
  particle.SetPoint(x,y,z);
}

Point2D Propagator::ComputeHit(Particle particle,Layer *layer){
  double x,y,z;
  double layer_length=layer->GetLength();
  double t_val=ComputeT(particle.GetTheta(),particle.GetPhi(),layer->GetRadius(),particle.GetX(),particle.GetY());
  ComputePoint(particle,x,y,z,t_val);
  Point2D hit(z,particle.GetPhi());
  return hit;
}

double Propagator::OffScattering(){
  return 0.;
}

double Propagator::Onscattering(){
  return gRandom->Gaus(0.,kRMSscat);  
}  

double  ComputeT(double theta,double phi,double radius,double x_vert,double y_vert){
  double sin_theta=sin(theta);
  double cos_phi=cos(phi);
  double sin_phi=sin(phi);
  double a=sin_theta*sin_theta;
  double b=2*sin_theta*(x_vert*cos_phi+y_vert*sin_phi);
  double c=x_vert*x_vert+y_vert*y_vert-radius*radius;
  double bb_4ac=b*b-4*a*c;
  double sqrt_bb_4ac=sqrt(bb_4ac);
  return (-b+sqrt_bb_4ac)/(2*a); // choose the forward propagating particle
}

void ComputePoint(Particle particle,double& x,double& y,double& z,double t_val){
  double theta=particle.GetTheta();
  double phi=particle.GetPhi();
  x=sin(theta)*cos(phi)*t_val+particle.GetX();
  y=sin(theta)*sin(phi)*t_val+particle.GetY();
  z=cos(theta)*t_val+particle.GetZ();
}