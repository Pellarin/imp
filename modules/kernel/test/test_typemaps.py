import IMP
import IMP.test
import StringIO


class TypemapTests(IMP.test.TestCase):
    """Test particles"""

    def test_no_model(self):
        """Check particle typemaps"""
        m= IMP.Model("particle typemaps")
        ps= []
        for i in range(0,10):
            ps.append(IMP.Particle(m))
        num=IMP._take_particles(ps)
        self.assertEqual(num, len(ps))
        num=IMP._take_particles(m, ps)
        self.assertEqual(num, len(ps))
        num=IMP._take_particles(m, ps, StringIO.StringIO())
        self.assertEqual(num, len(ps))
        pps= IMP._pass_particles(ps)
        for i in range(len(ps)):
            self.assertEqual(pps[i], ps[i])
        pso= IMP._give_particles(m)
        self.assertEqual(len(pso), 10)
        print pso[0]
        di={}
        for p in ps:
            di[p]=IMP._TrivialDecorator.setup_particle(p)
        rps= IMP._take_particles(di.values())
    def test_particles_temps(self):
        """Test passing of ParticlesTemp"""
        m= IMP.Model("particle temps")
        ps= []
        for i in range(0,10):
            ps.append(IMP.Particle(m))
        inlist=[[ps[0], ps[2]],[ps[3], ps[4]]]
        outlist= IMP._pass_particles_temps(inlist)
        self.assertEqual(inlist, outlist)
    def _equal_lists(self, va, vb):
        print "testing"
        self.assertEqual(len(va), len(vb))
        for i in range(len(va)):
            print va[i]
            print vb[i]
            self.assertEqual(va[i], vb[i])
    def _almost_equal_lists(self, va, vb):
        print "testing"
        self.assertEqual(len(va), len(vb))
        for i in range(len(va)):
            print va[i]
            print vb[i]
            self.assertAlmostEqual(va[i], vb[i], delta=.01)
    def test_a(self):
        """Check float typemaps"""
        vs=[1.1, 2.1, 3]
        vso= IMP._pass_floats(vs)
        self._almost_equal_lists(vs, vso)
    def test_b(self):
        """Check int typemaps"""
        vs=[1, 2, 3]
        vso= IMP._pass_ints(vs)
        self._equal_lists(vs, vso)
    def test_bl(self):
        """Check ints list typemaps"""
        vs=[[1,2,3,4],[5,6,7,8]]
        vso= IMP._pass_ints_list(vs)
        self._equal_lists(vs, vso)
    def test_bls(self):
        """Check ints lists typemaps"""
        vs=[[[1,2],[3,4]],[[5,6],[7,8]]]
        vso= IMP._pass_ints_lists(vs)
        self._equal_lists(vs, vso)
    def test_intpairs(self):
        """Check int pairs typemap"""
        ips=[(1,2),(3,4)]
        self.assertEqual(IMP._test_intranges(ips), 2)
    def test_c(self):
        """Check string typemaps"""
        vs=["1.0", "2.0", "3"]
        vso= IMP._pass_strings(vs)
        self._equal_lists(vs, vso)
    def test_d(self):
        """Check floatkey typemaps"""
        vs=IMP.FloatKeys([IMP.FloatKey(1), IMP.FloatKey(2), IMP.FloatKey(3)])
        vso= IMP._pass_float_keys(vs)
        self._equal_lists(vs, vso)
    def _test_o(self):
        """Check overloaded"""
        m= IMP.Model("overloaded")
        ps= []
        for i in range(10):
            ps.append(IMP.Particle(m))
        pso=self._pass(ps)
        self._equal_lists(ps, pso)
    def test_dec(self):
        """Check decorators"""
        m= IMP.Model("decorators")
        ps= IMP._TrivialDecorators()
        for i in range(10):
            ps.append(IMP._TrivialDecorator.setup_particle(IMP.Particle(m)))
        pso=IMP._pass_decorators(ps)
        self._equal_lists(ps, pso)
    def test_dect(self):
        """Check decorators traits"""
        m= IMP.Model("traits decorator")
        ps= IMP._TrivialTraitsDecorators()
        tr= IMP.StringKey("ttd")
        for i in range(10):
            ps.append(IMP._TrivialTraitsDecorator.setup_particle(IMP.Particle(m), tr))
        pso=IMP._pass_decorator_traits(ps)
        self._equal_lists(ps, pso)

    def test_pairs(self):
        """Check pairs"""
        m= IMP.Model("pairs")
        ps= []
        for i in range(10):
            pp = (IMP.Particle(m), IMP.Particle(m));
        pso=IMP._pass_particle_pairs(ps)
        self._equal_lists(ps, pso)
    def test_index_pairs(self):
        """Check particle index pairs"""
        ps= []
        for i in range(10):
            pp = (IMP.ParticleIndex(i), IMP.ParticleIndex(i+1));
        pso=IMP._pass_particle_index_pairs(ps)
        self._equal_lists(ps, pso)
    def test_failure(self):
        """Checking bad list"""
        self.assertRaises(TypeError, IMP._pass_particle_pairs, [1])
    def test_dec_to_part(self):
        """Checking conversion of decorators to particles"""
        m= IMP.Model("particle decorator conversion")
        ps= IMP._TrivialDecorators()
        rps= []
        for i in range(10):
            ps.append(IMP._TrivialDecorator.setup_particle(IMP.Particle(m)))
            rps.append(ps[-1].get_particle())
        pso=IMP._pass_particles(ps)
        self._equal_lists(rps, pso)
    def test_dec_to_part_2(self):
        """Checking conversion of decorator to particle"""
        m= IMP.Model("decorator particle conversion")
        d=IMP._TrivialDecorator.setup_particle(IMP.Particle(m))
        pso=IMP._pass_particle(d)
        self.assertEqual(d.get_particle(), pso)

    def test_pair(self):
        """Checking that conversion to/from pairs is OK"""
        p= (1.0, 2.0)
        op= IMP._pass_pair(p)
        print "first"
        print p
        print op
        self.assertAlmostEqual(p[0], op[0], delta=.01)
        self.assertAlmostEqual(p[1], op[1], delta=.01)
        op= IMP._pass_plain_pair(p)
        print "second"
        print p
        print op
        self.assertAlmostEqual(p[0], op[0], delta=.01)
        self.assertAlmostEqual(p[1], op[1], delta=.01)
    def test_pair_overload(self):
        """Checking that conversion from IntRange is OK with overload"""
        ir=(1,5)
        r= IMP._test_intrange(ir)
        self.assertEqual(r, ir)
        rr= IMP._test_intrange()
        self.assertEqual(rr, (-1,-1))
    def test_overload(self):
        """Checking that overloading works"""
        m= IMP.Model("overloading")
        p = IMP.Particle(m)
        r = IMP._ConstRestraint(1)
        pi= IMP._test_overload([p])
        ri= IMP._test_overload([r])
        self.assertEqual(pi, 0)
        self.assertEqual(ri, 1)
    def test_dec_conv(self):
        """Checking decorator conversions work"""
        m= IMP.Model("decorator conversion")
        p0= IMP.Particle(m)
        p1= IMP.Particle(m)
        print p0
        print p1
        d0= IMP._TrivialDecorator.setup_particle(p0)
        d1= IMP._TrivialTraitsDecorator.setup_particle(p1)
        print d0
        print d1
        d1p= IMP._TrivialDecorator.setup_particle(p1)
        print d1p
        ps=[p0, d1]
        pso= IMP._pass_decorators(ps)
        print pso[0]
        print pso[1]
        self.assertEqual(pso[0], d0)
        self.assertEqual(pso[1], d1p)
    def test_tuple_conv(self):
        """Checking tuple conversions work"""
        m= IMP.Model("tuple conversion")
        p0= IMP.Particle(m)
        p1= IMP.Particle(m)
        pp= (p0, p1)
        ppo= IMP._pass_particle_pair(pp)
        print [x.get_name() for x in pp]
        print [x.get_name() for x in ppo]
        self.assertEqual(ppo[0], pp[0])
        self.assertEqual(ppo[1], pp[1])
    def test_overload_dec(self):
        """Check overloaded functions with decorators"""
        m= IMP.Model("overloaded decorators")
        d0= IMP._TrivialDecorator.setup_particle(IMP.Particle(m))
        d1= IMP._TrivialDerivedDecorator.setup_particle(IMP.Particle(m))
        self.assertEqual(IMP._overloaded_decorator(d1), 1)
        self.assertEqual(IMP._overloaded_decorator(d0), 0)


    class _TestRestraint(IMP.Restraint):
        def __init__(self, ps):
            IMP.Restraint.__init__(self)
            self.ps=ps
        def do_show(self, fh):
            fh.write("Test Particle")
        def unprotected_evaluate(self, da):
            return 0
        def get_version_info(self):
            return IMP.VersionInfo()
        def get_input_particles(self):
            return [self.ps[0], self.ps[1]]
        def get_input_containers(self):
            return []
        def get_interacting_particles(self):
            return [[self.ps[0], self.ps[1]]]
    def test_dir_tms_0(self):
        """Test director methods in restraint"""
        m= IMP.Model("directors in restraints")
        p0= IMP.Particle(m)
        p1= IMP.Particle(m)
        r= self._TestRestraint([p0, p1])
        m.add_restraint(r)
        m.evaluate(False)
        r.evaluate(False)
    def test_dir_tms(self):
        """Test that decorator particle methods can be called"""
        m= IMP.Model("decorator particle methos")
        p0= IMP.Particle(m)
        d= IMP._TrivialDecorator.setup_particle(p0)
        d.add_attribute(IMP.IntKey("Hi"), 1)
    def test_cast(self):
        """Test that casting objects works"""
        m=IMP.Model("object cast")
        r= IMP._ConstRestraint(1)
        m.add_restraint(r)
        rb= m.get_restraints()[0]
        #print rb.get_value()
        rc= IMP._ConstRestraint.get_from(rb)
        print rc.get_value()
    def test_model_objects(self):
        """Test that model objects can get passed from python"""
        m= IMP.Model("model objets")
        r= IMP._ConstRestraint(1)
        p= IMP.Particle(m)
        self.assertEqual([p],IMP._pass_model_objects([p]))
        self.assertEqual([r,p],IMP._pass_model_objects([r, p]))
    def test_restraint_cast(self):
        """Test that restraints can be cast"""
        m= IMP.Model("model objets")
        r= IMP._ConstRestraint(1)
        m.add_restraint(r)
        rb= m.get_restraints()[0]
        rbc= IMP._ConstRestraint.get_from(rb)
        self.assertEqual(rbc, rb)
    def test_particle_methods(self):
        """Test that decorators provide particle methods"""
        exclusions=["do_show",
                    "get_contained_particles",
                    "get_contained_particles_changed",
                    "get_input_containers",
                    "get_is_active",
                    "get_is_changed",
                    "get_is_valid",
                    "get_log_level",
                    "get_number_of_live_objects",
                    "get_prechange_particle",
                    "get_ref_count",
                    "get_string",
                    "get_type_name",
                    "get_version_info",
                    "set_log_level",
                    "set_was_used",
                    "get_is_up_to_date",
                    "get_is_scored",
                    "set_is_scored",
                    "get_from",
                    "get_is_shared",
                    "set_is_ref_counted",
                    "get_is_ref_counted",
                    "get_live_object_names",
                    "add_cache_attribute",
                    "get_index",
                    'get_float_keys',
                    'get_index',
                    'get_int_keys',
                    'get_object_keys',
                    'get_weak_object_keys',
                    'get_particle_keys',
                    'get_string_keys',
                    'do_update_dependencies',
                    'get_is_part_of_model',
                    'set_model',
                    'get_inputs',
                    'get_outputs',
                    'do_get_inputs',
                    'do_get_outputs',
                    'get_interactions',
                    'do_get_interactions',
                    'update_dependencies'
                    ]
        md= dir(IMP._TrivialDecorator)
        missing=[]
        for m in dir(IMP.Particle):
            if not m in md and m not in exclusions and not m.startswith("_"):
                print m
                missing.append(m)
        self.assertEqual(len(missing),0,
                         "The following methods are not found in decorators: "+str(missing))

if __name__ == '__main__':
    IMP.test.main()
