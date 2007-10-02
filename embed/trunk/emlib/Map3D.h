#ifndef _MAP3D_H
#define _MAP3D_H

/*
  CLASS

  KEYWORDS

  AUTHORS
  Keren Lasker (mailto: kerenl@salilab.org)

  OVERVIEW TEXT
  responsibilities: holds and provides access to a 3D grid

*/

#include "Vector3.h"

#include <math.h>
#include <stdlib.h>

using namespace std;


template<class dataItemT> class Map3D;

template <class dataItemT>
class Map3D {
public:

  // constructors

  Map3D() {}
  Map3D(int nx_,int ny_,int nz_,float *voxelsize_);
  //TODO: should the copy cont copy all the data?
  //Map3D(const Map3D &copy_map); // copy constuctor
  ~Map3D();


  // handle the data allocation/release
  int Release(); // free the allocated grid


  int threeD2oneD_index(int x_ind, int y_ind, int z_ind) const{
    // TODO: should we add here a validation check
    return x_ind+y_ind*nx+z_ind*nx*ny;
  }

   const dataItemT  get_voxeldata(int i, int j,int k) const;

   void setData(int i,int j,int k, dataItemT);


protected:
  void free_1d_array_range(dataItemT *t);
  dataItemT * allocate_1d_array_range(int zdim,int ydim,int xdim);

  // orientation data
  Vector3 translateGrid; // the left-buttom voxel of the grid is assumed to be located at (0,0,0). The exact location of the grid is found in the XPLOR file. We insert this data to the translateGrid vector

  dataItemT *data; // the order is ZYX (Z-slowest)
  float voxelsize[3];
  int nx,ny,nz;
private:
  bool allocated;
};






#define INCLUDE_SHEILD
#include "Map3D.cc"
#undef INCLUDE_SHEILD


#endif //_MAP3D_H
