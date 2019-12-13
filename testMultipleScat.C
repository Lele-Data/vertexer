// This macro tests the multiple scattering method on a generated particle

#include <Riostream.h>
#include "Particle.h"
#include "Generator.h"
#include "Propagator.h"

void testMultipleScat(){
    Generator g;
    Propagator prop;
    Particle p;
    g.GenerateParticle(p,1);
    prop.MultipleScattering(p,1);
}