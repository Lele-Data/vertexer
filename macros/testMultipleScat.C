// This macro tests the multiple scattering method on a generated particle

#include <Riostream.h>
#include "../src/BeamPipe.h"
#include "../src/Particle.h"
#include "../src/Generator.h"
#include "../src/Transport.h"

void testMultipleScat(){
  Generator *g=Generator::GetInstance();
  Transport *transp=Transport::GetInstance();
  BeamPipe *bpipe=new BeamPipe(3,0.08,35.28); // cm, cm
  Particle p;
  g->GenerateParticle(p,EtaMethod::kHistEta);
  transp->MultipleScattering(p,(Cylinder*)bpipe,MultScatMethod::kOnScat);
  transp=Transport::Destroy();
  g=Generator::Destroy();
}