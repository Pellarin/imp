/*
 * Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

/* Example of integrating CUDA functions into an existing
 * application / framework.
 * CPP code representing the existing application / framework.
 * Compiled with default CPP compiler.
 */

// includes, system
#include <iostream>
#include <stdlib.h>

// Required to include CUDA vector types
#include <cuda_runtime.h>
#include <vector_types.h>

#include <IMP/algebra/VectorD.h>
#include <IMP/Model.h>
#include <IMP/core/XYZ.h>
#include <IMP/atom/pdb.h>
#include <IMP/benchmark/benchmark_macros.h>
#include <boost/scoped_array.hpp>
#include <IMP/base/utility.h>

////////////////////////////////////////////////////////////////////////////////
// declaration, forward
extern "C" void get_distances_gpu(int argc, char **argv,
                                  int len, double3 *coords, double *out);
namespace {
IMP::Floats get_distances(const IMP::algebra::Vector3Ds &in) {
  IMP::Floats out(in.size()*in.size());
  for (unsigned int i = 0; i < in.size(); ++i) {
    for (unsigned int j = 0; j < in.size(); ++j) {
      out[i*in.size() + j] = IMP::algebra::get_distance(in[i], in[j]);
    }
  }
  return out;
}
}
////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int
main(int argc, char **argv)
{
  IMP::base::Pointer<IMP::Model> model = new IMP::Model();

  // check if file exists
  std::ifstream in_file(argv[1]);
  if(!in_file) {
    std::cerr << "Can't open file " << argv[1] << std::endl;
    exit(1);
  }

  // read pdb
  /*IMP::atom::Hierarchy mhd =
    IMP::atom::read_pdb(argv[1], model,
                        new IMP::atom::NonWaterNonHydrogenPDBSelector(),
                        // don't add radii
                        true, true);
  IMP::Particles particles
    = IMP::get_as<IMP::Particles>(get_by_type(mhd, IMP::atom::ATOM_TYPE));
  if(particles.size() > 0) { // pdb file
    std::cout << particles.size() << " atoms were read from PDB file "
              << argv[1] << std::endl;
              }*/
  unsigned int num = 1024;

  IMP::algebra::Vector3Ds coordinates(num);
  boost::scoped_array<double3> coordinates2(new double3[coordinates.size()]);

  for(unsigned int i = 0; i < coordinates.size(); i++) {
    coordinates[i] = IMP::algebra::Vector3D(i,i,i);
    coordinates2[i].x = coordinates[i][0];
    coordinates2[i].y = coordinates[i][1];
    coordinates2[i].z = coordinates[i][2];
  }

  boost::scoped_array<double>
    distances_gpu(new double[coordinates.size() * coordinates.size()]);
  double bad; bad = -1;
  std::fill(distances_gpu.get(),
            distances_gpu.get() + coordinates.size() * coordinates.size(),
            bad);
  {
    boost::timer gpu_time;

    get_distances_gpu(argc, argv, coordinates.size(), coordinates2.get(),
                      distances_gpu.get());
    std::cout << "GPU " << gpu_time.elapsed() << std::endl;
  }

  IMP::Floats distances_cpu;
  {
    boost::timer cpu_time;

    distances_cpu = get_distances(coordinates);
    std::cout << "CPU " << cpu_time.elapsed() << std::endl;
  }

  std::cout << distances_cpu.size() << " distances" << std::endl;
  int differ = 0;
  int garbage = 0;
  int fill = 0;
  int nan = 0;
  for (unsigned int i = 0; i < distances_cpu.size(); ++i) {
    //std::cout << distances_gpu[i] << " ";
    if (IMP::base::is_nan(distances_gpu[i]) ||
        IMP::base::is_nan(-distances_gpu[i])) {
      ++nan;
    } else if (distances_gpu[i] == -1) {
      ++fill;
    } else if (distances_gpu[i] > 40000
               || (distances_gpu[i] != 0 && distances_gpu[i] < .001)) {
      ++garbage;
    } else {
      double error = std::abs(distances_cpu[i] - distances_gpu[i]);
      if (error > .01) {
        ++differ;
        /*std::cerr << "Differ at " << i << " " << distances_cpu[i]
          << " vs " << distances_gpu[i].x << std::endl;*/
      }
    }
  }
  std::cout << "Errors: " << differ << " and garbage " << garbage
            << " fill " << fill << " nan " << nan << std::endl;
  return 0;
}
