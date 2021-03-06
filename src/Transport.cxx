// Transport.cxx
// Implementation of Transport.h
//
// Author: Mario Ciacco & Emanuele Data

#include <TRandom3.h>
#include <Riostream.h>
#include <TMath.h>
#include "Transport.h"

ClassImp(Transport);

Transport *Transport::fInstance=NULL; // static data member

double ComputeT(double,double,double,double,double); // helper function
void ComputePoint(Particle,double&,double&,double&,double);
void Rotate(double,double,double,double,double*);

Transport::Transport():TObject(){
  RandomScat[0]=OffScattering;
  RandomScat[1]=Onscattering;
}

Transport::~Transport(){}

Transport *Transport::GetInstance(){
  if(!Transport::fInstance) fInstance=new Transport();
  return fInstance;
}

Transport *Transport::Destroy(){
  if(Transport::fInstance) delete fInstance;
  fInstance=NULL;
  return fInstance;
}

void Transport::MultipleScattering(Particle& particle,Cylinder *cyl,int nScatMethod){
  double u[3]; // direction 
  double rms_scat=cyl->GetRMSscat();
  double thp=(RandomScat[nScatMethod])(rms_scat); // generate theta according to the multiple scattering distribution
  double php=gRandom->Rndm()*2*TMath::Pi(); // generate phi according to a uniform distribution in (0,2*pi
  Rotate(particle.GetTheta(),particle.GetPhi(),thp,php,u);
  // compute the theta and phi of the particle in the lab frame
  double new_theta=TMath::ATan(TMath::Sqrt(u[0]*u[0]+u[1]*u[1])/u[2]);
  if(new_theta<0) new_theta+=TMath::Pi();
  double new_phi=TMath::ATan(u[1]/u[0]);
  if(u[0]<0) new_phi+=TMath::Pi();
  else if(u[0]>=0 && u[1]<0) new_phi+=2.*TMath::Pi();
  particle.SetDirection(new_theta,new_phi);
}

void Transport::Intersection(Particle& particle,Cylinder *cylinder){
  double x,y,z;
  double t_val=ComputeT(particle.GetTheta(),particle.GetPhi(),cylinder->GetRadius()+cylinder->GetThickness(),particle.GetX(),particle.GetY());
  ComputePoint(particle,x,y,z,t_val);
  particle.SetPoint(x,y,z);
}

Point2D Transport::ComputeHit(Particle particle,Layer *layer){
  double x,y,z;
  double layer_length=layer->GetLength();
  double t_val=ComputeT(particle.GetTheta(),particle.GetPhi(),layer->GetRadius(),particle.GetX(),particle.GetY());
  ComputePoint(particle,x,y,z,t_val);
  Point2D hit(z,particle.GetPhi());
  return hit;
}

void Transport::ComputeHit(Particle particle,Layer *layer,double& zHit,double& phiHit){
  double x,y,z;
  double layer_length=layer->GetLength();
  double t_val=ComputeT(particle.GetTheta(),particle.GetPhi(),layer->GetRadius(),particle.GetX(),particle.GetY());
  ComputePoint(particle,x,y,z,t_val);
  zHit=z;
  phiHit=particle.GetPhi();
}

double Transport::OffScattering(double rms){
  return 0.;
}

double Transport::Onscattering(double rms){
  return gRandom->Gaus(0.,rms);  
}  

// This function computes the parameter t to find the intersection of a line (particle) with a cylinder (layer/beam pipe)
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

// This function computes the cylindrical coordinates of the intersection of the particle with the cylinder
void ComputePoint(Particle particle,double& x,double& y,double& z,double t_val){
  double theta=particle.GetTheta();
  double phi=particle.GetPhi();
  x=sin(theta)*cos(phi)*t_val+particle.GetX();
  y=sin(theta)*sin(phi)*t_val+particle.GetY();
  z=cos(theta)*t_val+particle.GetZ();
}

// This function rotates a direction given the angles of rotation
void Rotate(double th,double ph,double thp,double php,double* u){
  double mr[3][3]; // rotation matrix
  // define the entries of the rotation matrix
  mr[0][0]=sin(ph);
  mr[1][0]=-cos(ph);
  mr[2][0]=0.;
  mr[0][1]=cos(th)*cos(ph);
  mr[1][1]=sin(ph)*cos(th);
  mr[2][1]=-sin(th);
  mr[0][2]=cos(ph)*sin(th);
  mr[1][2]=sin(th)*sin(ph);
  mr[2][2]=cos(th);
  double up[3]; // direction
  // define the direction in the primed frame
  up[0]=sin(thp)*cos(php);
  up[1]=sin(thp)*sin(php);
  up[2]=cos(thp);
  // matrix multiplication
  for(int i=0;i<3;i++){
    u[i]=0;
    for(int j=0;j<3;j++) u[i]+=mr[i][j]*up[j];
  }
}