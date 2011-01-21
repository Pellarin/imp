import IMP.test
import sys
import re

class LigandScoreApplicationTest(IMP.test.ApplicationTestCase):
    def test_simple(self):
        """Simple test of ligand score application"""
        p = self.run_application('ligand_score',
                              [self.get_input_file_name('1d3d-protein.pdb'),
                               self.get_input_file_name('1d3d-ligands.mol2')])
        p.stdin.close()
        out = p.stdout.readlines()
        ret, err = p.wait()
        sys.stderr.write(err)
        self.assertApplicationExitedCleanly(ret, err)
        for line, nligand, score in [(out[-2], '001', 8.39),
                                     (out[-1], '002', 6.54)]:
            m = re.match('Score for 1d3d\-ligand\.1d3d\.(\d+) is ([\d\.]+)',
                         line)
            self.assertNotEqual(m, None,
                                msg="Score output not found in " + str(out))
            self.assertEqual(m.group(1), nligand)
            self.assertAlmostEqual(float(m.group(2)), score, delta=0.01)

if __name__ == '__main__':
    IMP.test.main()
