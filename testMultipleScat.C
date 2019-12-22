// This macro tests the multiple scattering method on a generated particle

#include <Riostream.h>
#include "Particle.h"
#include "Generator.h"
#include "Propagator.h"

void testMultipleScat(){
  Generator *g=Generator::GetInstance();
  Propagator *prop=Propagator::GetInstance();
  Particle p;
  g->GenerateParticle(p,EtaMethod::kHistEta);
  prop->MultipleScattering(p,MultScatMethod::kOnScat);
  prop=Propagator::Destroy();
  g=Generator::Destroy();
}