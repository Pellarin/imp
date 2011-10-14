/**
 *  \file mc_membrane.cpp
 *  \brief Membrane MonteCarlo
 *
 *  Copyright 2011 IMP Inventors. All rights reserved.
 *
 */
#include <IMP/core.h>
#include <IMP/atom.h>
#include <IMP/membrane.h>
#include <IMP/rmf.h>
#include "mpi.h"
#include <boost/scoped_array.hpp>
#include <time.h>
#include <fstream>
#include <sstream>
#include <RMF/RootHandle.h>

using namespace IMP;
using namespace IMP::membrane;

int main(int argc, char* argv[])
{

MPI_Init(&argc,&argv);
int nproc, myrank;
MPI_Comm_size(MPI_COMM_WORLD, &nproc);
MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
MPI_Status status;

// initialize seed
unsigned int iseed = time(NULL);
// broadcast seed
MPI_Bcast(&iseed,1,MPI_UNSIGNED,0,MPI_COMM_WORLD);
// initialize random generator
srand (iseed);

// log file
std::ofstream logfile;
std::stringstream out;
out << myrank;
std::string names="log"+out.str();
char* name=(char*)malloc( sizeof( char ) *(names.length() +1) );;
strcpy(name, names.c_str());
logfile.open(name);

// parsing input
if(myrank==0) {std::cout << "Parsing input file" << std::endl;}
Parameters mydata=get_parameters("config.ini");

// create temperature and index array
double* temp=create_temperatures(mydata.MC.tmin,mydata.MC.tmax,nproc);
int*    index=create_indexes(nproc);

// create a new model
IMP_NEW(Model,m,());
// root hierarchy
IMP_NEW(Particle,ph,(m));
atom::Hierarchy all=atom::Hierarchy::setup_particle(ph);

// create representation
if(myrank==0) {std::cout << "Creating representation" << std::endl;}
core::TableRefiner* tbr=generate_TM(m,all,&mydata);

// reload
if(mydata.reload.length()>0){
 if(myrank==0){std::cout << "Restart from file" << std::endl;}
 std::string trajname=mydata.reload+out.str()+".rmf";
 RMF::RootHandle rh = RMF::open_rmf_file(trajname);
 atom::Hierarchies hs=all.get_children();
 rmf::set_hierarchies(rh, hs);
 // reload last frame
 for(unsigned int i=0;i<hs.size();++i){
  unsigned int iframe=rmf::get_number_of_frames(rh,hs[i]);
  rmf::load_frame(rh,iframe-1,hs[i]);
 }
}
//
// Prepare output file
std::string trajname="traj"+out.str()+".rmf";
RMF::RootHandle rh = RMF::create_rmf_file(trajname);
atom::Hierarchies hs=all.get_children();
for(unsigned int i=0;i<hs.size();++i) {rmf::add_hierarchy(rh, hs[i]);}
// adding key for score
RMF::Category my_kc= rh.add_category("my data");
RMF::FloatKey my_key=rh.add_float_key(my_kc,"my score",true);

// create restraints
if(myrank==0) {std::cout << "Creating restraints" << std::endl;}
create_restraints(m,all,tbr,&mydata);

// create sampler
if(myrank==0) {std::cout << "Creating sampler" << std::endl;}
Pointer<core::MonteCarlo> mc=
 setup_MonteCarlo(m,all,temp[index[myrank]],&mydata);
//mc->set_use_incremental_evaluate(true);

// wte restart
if(mydata.MC.do_wte && mydata.MC.wte_restart){
 Floats val;
 double bias;
 std::ifstream biasfile;
 std::string names="BIAS"+out.str();
 char* name=(char*)malloc( sizeof( char ) *(names.length() +1) );;
 strcpy(name, names.c_str());
 biasfile.open(name);
 while (biasfile >> bias){val.push_back(bias);}
 biasfile.close();
 Pointer<membrane::MonteCarloWithWte> ptr=
     dynamic_cast<membrane::MonteCarloWithWte*>(mc.get());
 ptr->set_bias(val);
}

// hot steps
if(mydata.MC.nhot>0){
 if(myrank==0) {std::cout << "High temperature initialization" << std::endl;}
 mc->set_kt(mydata.MC.tmax);
 mc->optimize(mydata.MC.nhot);
}
// set temperature
mc->set_kt(temp[index[myrank]]);

// sampling
if(myrank==0) {std::cout << "Sampling" << std::endl;}
// Monte Carlo loop
for(int imc=0;imc<mydata.MC.nsteps;++imc)
{
// run optimizer
 mc->optimize(mydata.MC.nexc);

// print statistics
 double myscore=m->evaluate(false);
 int    myindex=index[myrank];
 logfile << imc << " " << myindex << " " << myscore << " "
 << mydata.MC.nexc << " " << mc->get_number_of_forward_steps() << "\n";

// save configuration to file
 if(imc%mydata.MC.nwrite==0){
  rh.set_value(my_key,myscore,imc/mydata.MC.nwrite);
  for(unsigned int i=0;i<hs.size();++i){
   rmf::save_frame(rh,imc/mydata.MC.nwrite,hs[i]);
  }
 // dump bias on file if wte
  if(mydata.MC.do_wte){
   std::ofstream biasfile;
   std::string names="BIAS"+out.str();
   char* name=(char*)malloc( sizeof( char ) *(names.length() +1) );;
   strcpy(name, names.c_str());
   biasfile.open(name);
   Pointer<membrane::MonteCarloWithWte> ptr=
     dynamic_cast<membrane::MonteCarloWithWte*>(mc.get());
   double* mybias=ptr->get_bias_buffer();
   for(int i=0;i<ptr->get_nbin();++i){
    biasfile << mybias[i] << "\n";
   }
   biasfile.close();
  }
 }

// now it's time to try an exchange
 int    frank=get_friend(index,myrank,imc,nproc);
 int    findex=index[frank];
 double fscore;

// send and receive score
 MPI_Sendrecv(&myscore,1,MPI_DOUBLE,frank,myrank,
               &fscore,1,MPI_DOUBLE,frank,frank,
               MPI_COMM_WORLD, &status);

// if WTE, calculate U_mybias(myscore) and U_mybias(fscore) and exchange
 double delta_wte=0.0;

 if(mydata.MC.do_wte){
  Pointer<membrane::MonteCarloWithWte> ptr=
   dynamic_cast<membrane::MonteCarloWithWte*>(mc.get());
  double U_mybias[2]={ptr->get_bias(myscore),ptr->get_bias(fscore)};
  double U_fbias[2];
  MPI_Sendrecv(U_mybias,2,MPI_DOUBLE,frank,myrank,
                U_fbias,2,MPI_DOUBLE,frank,frank,
                MPI_COMM_WORLD, &status);
  delta_wte=(U_mybias[0]-U_mybias[1])/temp[myindex]+
            (U_fbias[0] -U_fbias[1])/ temp[findex];
 }

// calculate acceptance
 bool do_accept=get_acceptance(myscore,fscore,delta_wte,
                               temp[myindex],temp[findex]);

// if accepted exchange what is needed
 if(do_accept){
  myindex=findex;
  mc->set_kt(temp[myindex]);
// if WTE, rescale W0 and exchange bias
  if(mydata.MC.do_wte){
   Pointer<membrane::MonteCarloWithWte> ptr=
    dynamic_cast<membrane::MonteCarloWithWte*>(mc.get());
   ptr->set_w0(mydata.MC.wte_w0*temp[myindex]/mydata.MC.tmin);
   int     nbins=ptr->get_nbin();
   double* mybias=ptr->get_bias_buffer();
   double* fbias=new double[nbins];
   MPI_Sendrecv(mybias,nbins,MPI_DOUBLE,frank,myrank,
                 fbias,nbins,MPI_DOUBLE,frank,frank,
                MPI_COMM_WORLD, &status);
   Floats val(fbias, fbias+nbins);
   ptr->set_bias(val);
   delete(fbias);
  }
 }

// in any case, update index vector
 MPI_Barrier(MPI_COMM_WORLD);
 int sbuf[nproc],rbuf[nproc];
 for(int i=0;i<nproc;++i) {sbuf[i]=0;}
 sbuf[myrank]=myindex;
 MPI_Allreduce(sbuf,rbuf,nproc,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
 for(int i=0;i<nproc;++i){index[i]=rbuf[i];}
}

MPI_Barrier(MPI_COMM_WORLD);
// close rmf
rh.flush();
rh=RMF::RootHandle();
// flush and close logfile
logfile.flush();
logfile.close();
MPI_Barrier(MPI_COMM_WORLD);
// finalize MPI
MPI_Finalize();
return 0;
}
