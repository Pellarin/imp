#!/usr/bin/python

import sys,os,string
import IMP.multifit

from optparse import OptionParser
def usage():
    usage =  "usage %prog [options] <asmb.input> <proteomics> <mapping> <align param> \n"
    usage+="A script for calculate RMSD bewtween reference and fitting solutions"
    parser = OptionParser(usage)
    parser.add_option("-d", action="store_true", dest="use_dock",
                      help="if set the docking transformation is used (and not the fitting transformation)")
    (options, args) = parser.parse_args()
    if len(args) != 4:
        parser.error("incorrect number of arguments")
    return [options,args]

def run(asmb_fn,proteomics_fn,mapping_fn,params_fn,dock_trans):
    asmb=IMP.multifit.read_settings(asmb_fn)

    prot_data=IMP.multifit.read_proteomics_data(proteomics_fn)
    print "=========3"
    mapping_data=IMP.multifit.read_protein_anchors_mapping(prot_data,mapping_fn)
    alignment_params = IMP.multifit.AlignmentParams(params_fn)
    alignment_params.process_parameters()
    align=IMP.multifit.ProteomicsEMAlignmentAtomic(mapping_data,asmb,alignment_params)
    ensmb=IMP.multifit.Ensemble(asmb,mapping_data)
    print "=========6"
    #load all proteomics restraints
    mdl=align.get_model()
    mhs=align.get_molecules()
    gs=[]
    for i,mh in enumerate(mhs):
        fits_fn=asmb.get_component_header(i).get_transformations_fn()
        fits = IMP.multifit.read_fitting_solutions(fits_fn)
        print "calculating rmsd for",len(fits),"fits of protein",mh.get_name()
        xyz=IMP.core.XYZs(IMP.core.get_leaves(mh))
        mh_ref=IMP.atom.read_pdb(asmb.get_component_header(i).get_reference_fn(),mdl)
        rb=IMP.atom.setup_as_rigid_body(mh)
        xyz_ref=IMP.core.XYZs(IMP.core.get_leaves(mh_ref))
        new_fits=[]
        for i,rec in enumerate(fits):
            fit_t=rec.get_fit_transformation()
            if dock_trans:
                fit_t=rec.get_dock_transformation()
            IMP.core.transform(rb,fit_t)
            rmsd=IMP.atom.get_rmsd(xyz,xyz_ref)
            rec.set_rmsd_to_reference(rmsd)
            new_fits.append(rec)
            IMP.core.transform(rb,fit_t.get_inverse())
        IMP.multifit.write_fitting_solutions(fits_fn+".RMSD",new_fits)
if __name__=="__main__":
    (options,args) = usage()
    run(args[0],args[1],args[2],args[3],options.use_dock)
