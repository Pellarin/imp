#-- File: simple_distance.py --#

import IMP
import IMP.core
import IMP.helper

imp_model = IMP.Model()

# Create particle point 1
p1 = IMP.Particle(imp_model)
p1.add_attribute(IMP.FloatKey("x"), 0, True)
p1.add_attribute(IMP.FloatKey("y"), 0, True)
p1.add_attribute(IMP.FloatKey("z"), 0, True)
p1.add_attribute(IMP.FloatKey("radius"), 1.0, False)

# Create particle point 2
p2 = IMP.Particle(imp_model)
p2.add_attribute(IMP.FloatKey("x"), 100, True)
p2.add_attribute(IMP.FloatKey("y"), 100, True)
p2.add_attribute(IMP.FloatKey("z"), 100, True)
p2.add_attribute(IMP.FloatKey("radius"), 1.0, False)

particles = IMP.Particles()
particles.append(p1)
particles.append(p2)

sd = IMP.helper.create_simple_distance(particles)

r = sd.get_restraint()
h = sd.get_harmonic_upper_bound()

sd.set_mean(10.0)
sd.set_stddev(3.5)
sd.set_k(0.1)

imp_model.add_restraint(r)
r.show()

imp_model.evaluate(False)
