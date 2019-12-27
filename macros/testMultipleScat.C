// This macro tests the multiple scattering method on a generated particle

#include <Riostream.h>
#include "../src/Particle.h"
#include "../src/Generator.h"
#include "../src/Transport.h"

void testMultipleScat(){
  Generator *g=Generator::GetInstance();
  Transport *transp=Transport::GetInstance();
  Particle p;
  g->GenerateParticle(p,EtaMethod::kHistEta);
  transp->MultipleScattering(p,MultScatMethod::kOnScat);
  transp=Transport::Destroy();
  g=Generator::Destroy();
}