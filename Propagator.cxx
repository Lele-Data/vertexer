// Propagator.cxx
// Implementation of Propagator.h
//
// Author: Mario Ciacco & Emanuele Data

#include "Propagator.h"
#include <TRandom3.h>
#include <Riostream.h>
#include <TMath.h>

ClassImp(Propagator);

double ComputeT(double,double,double,double,double); // helper function
void ComputePoint(Particle,double&,double&,double&,double);
void Rotate(double,double,double,double,double*);

Propagator::Propagator():TObject(){
  RandomScat[0]=OffScattering;
  RandomScat[1]=Onscattering;
}

Propagator::~Propagator(){}

void Propagator::MultipleScattering(Particle& particle,int nScatMethod){
  double u[3]; // direction 
  double thp=(RandomScat[nScatMethod])();

  std::cout<<"angolo generato "<<thp<<std::endl;

  double php=0.; 

  std::cout<<"theta "<<particle.GetTheta()<<", phi "<<particle.GetPhi()<<std::endl;

  Rotate(particle.GetTheta(),particle.GetPhi(),thp,php,u);

  std::cout<<"_______________in S_________________"<<std::endl;
  std::cout<<"x "<<u[0]<<" y "<<u[1]<<" z "<<u[2]<<std::endl;
  std::cout<<"____________________________________"<<std::endl;

  double new_theta=TMath::ATan(TMath::Sqrt(u[0]*u[0]+u[1]*u[1])/u[2]);
  if(new_theta<0) new_theta+=TMath::Pi();
  double new_phi=TMath::ATan(u[1]/u[0]);
  if(u[0]<0) new_phi+=TMath::Pi();
  else if(u[0]>=0 && u[1]<0) new_phi+=2.*TMath::Pi();

  std::cout<<"theta new "<<new_theta<<", phi new "<<new_phi<<std::endl;

  particle.SetDirection(new_theta,new_phi);
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

void Propagator::ComputeHit(Particle particle,Layer *layer,double& zHit,double& phiHit){
  double x,y,z;
  double layer_length=layer->GetLength();
  double t_val=ComputeT(particle.GetTheta(),particle.GetPhi(),layer->GetRadius(),particle.GetX(),particle.GetY());
  ComputePoint(particle,x,y,z,t_val);
  zHit=z;
  phiHit=particle.GetPhi();
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

void Rotate(double th,double ph,double thp,double php,double* u){
  double mr[3][3]; // rotation matrix

  mr[0][0]=-sin(ph);
  mr[1][0]=cos(ph);
  mr[2][0]=0.;
  mr[0][1]=-cos(th)*cos(ph);
  mr[1][1]=-sin(ph)*cos(th);
  mr[2][1]=sin(th);
  mr[0][2]=cos(ph)*sin(th);
  mr[1][2]=sin(th)*sin(ph);
  mr[2][2]=cos(th);

  double up[3];
  up[0]=sin(thp)*cos(php);
  up[1]=sin(thp)*sin(php);
  up[2]=cos(thp);

  std::cout<<"_______________in S'_________________"<<std::endl;
  std::cout<<"x' "<<up[0]<<" y' "<<up[1]<<" z' "<<up[2]<<std::endl;
  std::cout<<"_____________________________________"<<std::endl;


  for(int i=0;i<3;i++){
    u[i]=0;
    for(int j=0;j<3;j++) u[i]+=mr[i][j]*up[j];
  }
    
}