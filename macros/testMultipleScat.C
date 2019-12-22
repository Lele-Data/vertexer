// This macro tests the multiple scattering method on a generated particle

#include <Riostream.h>
#include "../src/Particle.h"
#include "../src/Generator.h"
#include "../src/Propagator.h"

void testMultipleScat(){
  Generator *g=Generator::GetInstance();
  Propagator *prop=Propagator::GetInstance();
  Particle p;
  g->GenerateParticle(p,EtaMethod::kHistEta);
  prop->MultipleScattering(p,MultScatMethod::kOnScat);
  prop=Propagator::Destroy();
  g=Generator::Destroy();
}