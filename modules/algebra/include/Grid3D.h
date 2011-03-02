/**
 *  \file Grid3D.h   \brief A class to represent a voxel grid.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPALGEBRA_GRID_3D_H
#define IMPALGEBRA_GRID_3D_H

#include "algebra_config.h"

#include <IMP/base_types.h>
#include "Vector3D.h"
#include "GridD.h"
#include "BoundingBoxD.h"
#include "internal/grid_3d.h"
#include <boost/iterator/transform_iterator.hpp>
#include <IMP/internal/map.h>

#include <limits>
/** \namespace IMP::algebra::grids

    \brief Implementation for parameterized grids.

    First some terminology:
    - a voxel is the data stored at a given location is space
    - an Index is a way of identifying a particular voxel. That is, given
      an index, it is easy to get the voxel, but not vice-versa
    - an ExtendedIndex identifies a particular region in space, but
      it may not have a corresponding voxel (if it is outside of the
      region the grid is built on or if that voxel has not yet been
      added to the sparse grid).

    \imp provides support for a variety of spatial grids. The grid support in
    C++ is implemented by combining several different layers to specify
    what capabilities are desired. These layers are:
    - Data: any type of data can be stored in a voxel of the grid
    - Boundedness: By using UnboundedGridStorage3D or BoundedGridStorage3D,
      one can choose whether you want a grid over a finite region of space
      or over the whole space.
    - Storage: by choosing SparseGridStorage3D or DenseGridStorage3D, you can
      choose whether you want to store all voxels or only a subset of the
      voxels. The former is faster and more compact when most of the voxels are
      used, the latter when only a few are used (say <1/4).
    - Geometry: The Grid3D class itself provides a geometric layer, mapping
      Vector3D objects into voxels in the grid.

    These are implemented as mix-ins, so each layer provides a set of accessible
    functionality as methods/types in the final class.

   \par Basic operations
   Creating a grid with a given cell size and upper and lower
   bounds
   \code
   BoundingBox3D bb(VectorD<3>(10,10,10), VectorD<3>(100,100,100));
   typedef Grid3D<Ints> Grid;
   Grid grid(5, bb, 0.0);
   \endcode

   Iterate over the set of voxels incident on a bounding box:
   \code
   BoundingBoxD<3> bb(VectorD<3>(20.2,20.3,20.5), VectorD<3>(31.3,32.5,38.9));
   for (Grid::IndexIterator it= grid.voxels_begin(bb);
        it != grid.voxels_end(bb); ++it) {
        it->push_back(1);
   }
   \endcode
 */

IMPALGEBRA_BEGIN_NAMESPACE

//! Use trilinear interpolation to compute a smoothed value at v
/** The voxel values are assumed to be at the center of the voxel
    and the passed outside value is used for voxels outside the
    grid. The type Voxel must support get_linearly_interpolated().
    \relatesalso Grid3D
*/
template <class Storage>
const typename Storage::Value &
get_trilinearly_interpolated(const grids::GridD<3, Storage> &g,
                             const VectorD<3> &v,
                             const typename Storage::Value& outside=0);

// They are created with %template in swig to get around inclusion order issues
#ifndef SWIG
/** A dense grid of values. In python DenseFloatGrid3D and DenseDoubleGrid3D are
    provided. See IMP_GRID3D_FOREACH_VOXEL() for a useful macro when operating
    on all voxels of the grid.
*/
template <class VT>
struct DenseGrid3D:
  public grids::GridD<3, grids::DenseGridStorageD<3, VT> > {
  typedef grids::GridD<3, grids::DenseGridStorageD<3,  VT> > P;
  DenseGrid3D(double side,
                   const BoundingBoxD<3> &bb,
                   VT def=VT()): P(side, bb, def) {}
  DenseGrid3D(int xd, int yd, int zd,
                   const BoundingBoxD<3> &bb,
                   VT def=VT()): P(xd, yd, zd, bb, def) {}
  DenseGrid3D(){}

};

/** A sparse grid of values. In python SparseIntGrid3D is provided.*/
template <class VT>
struct SparseGrid3D:
  public grids::GridD<3, grids::SparseGridStorageD<3, VT,
                            grids::BoundedGridStorageD<3> > > {
  typedef grids::GridD<3, grids::SparseGridStorageD<3, VT,
                                  grids::BoundedGridStorageD<3> > > P;
  SparseGrid3D(double side,
                  const BoundingBoxD<3> &bb,
                  VT def=VT()): P(side, bb, def) {}
  SparseGrid3D(int xd, int yd, int zd,
                  const BoundingBoxD<3> &bb,
                  VT def=VT()): P(xd, yd, zd, bb, def) {}
  SparseGrid3D(){}
};

/** A sparse, infinite grid of values. In python SparseUnboundedIntGrid3D
    is provided.*/
template <class VT>
struct SparseUnboundedGrid3D:
  public grids::GridD<3, grids::SparseGridStorageD<3, int,
                          grids::UnboundedGridStorageD<3> > >{
  typedef grids::GridD<3, grids::SparseGridStorageD<3, int,
                           grids::UnboundedGridStorageD<3> > > P;
  SparseUnboundedGrid3D(double side,
                           const Vector3D &origin,
                           VT def=VT()): P(side, origin, def){}
  SparseUnboundedGrid3D(){}

};

#endif
IMPALGEBRA_END_NAMESPACE

#include "internal/grid_3d_impl.h"


/** Iterate over each voxel in grid. The voxel index is
    unsigned int voxel_index[3] and the coordinates of the center is
    VectorD<3> voxel_center and the index of the voxel is
    loop_voxel_index.
    \relatesalso Grid3D
 */
#define IMP_GRID3D_FOREACH_VOXEL(grid, action)                          \
  {                                                                     \
    unsigned int next_loop_voxel_index=0;                               \
    const algebra::Vector3D macro_map_unit_cell=g.get_unit_cell();      \
    const unsigned int macro_map_nx=g.get_number_of_voxels(0);          \
    const unsigned int macro_map_ny=g.get_number_of_voxels(1);          \
    const unsigned int macro_map_nz=g.get_number_of_voxels(2);          \
    const algebra::Vector3D macro_map_origin                            \
      =g.get_origin();                                                  \
    unsigned int voxel_index[3];                                        \
    IMP::algebra::VectorD<3> voxel_center;                              \
    for (voxel_index[0]=0; voxel_index[0]< macro_map_nx;                \
         ++voxel_index[0]) {                                            \
      voxel_center[0]= macro_map_origin[0]                              \
        +(ix+.5)*macro_map_unit_cell[0];                                \
      for (voxel_index[1]=0; voxel_index[1]< macro_map_ny;              \
           ++voxel_index[1]) {                                          \
        voxel_center[1]= macro_map_origin[1]                            \
          +(iy+.5)*macro_map_unit_cell[1];                              \
        for (voxel_index[2]=0; voxel_index[2]< macro_map_nz;            \
             ++voxel_index[2]) {                                        \
          voxel_center[2]= macro_map_origin[2]                          \
            +(iz+.5)*macro_map_unit_cell[2];                            \
          unsigned int loop_voxel_index=next_loop_voxel_index;          \
          ++next_loop_voxel_index;                                      \
          {action};                                                     \
        }                                                               \
      }                                                                 \
    }                                                                   \
  }                                                                     \


/** Iterate over each voxel in a subset of the grid that are less than
    center. The voxel index is unsigned int voxel_index[3]. Use this if,
    for example you want to find nearby pairs of voxels once each.
    \relatesalso Grid3D
*/
#define IMP_GRID3D_FOREACH_SMALLER_EXTENDED_INDEX_RANGE(grid, center,   \
                                                        lower_corner,   \
                                                        upper_corner,   \
                                                        action)         \
  { int voxel_index[3];                                                 \
    IMP_USAGE_CHECK(lower_corner <= upper_corner, "Inverted range "     \
                    << lower_corner << " " << upper_corner);            \
    IMP_USAGE_CHECK(lower_corner <= center, "Center not in range "      \
                    << lower_corner << " " << center);                  \
    IMP_USAGE_CHECK(center <= upper_corner, "Center not in range "      \
                    <<  center << upper_corner);                        \
    for (voxel_index[0]=lower_corner[0];                                \
         voxel_index[0] <= upper_corner[0]; ++voxel_index[0]) {         \
      if (voxel_index[0] > center[0]) break;                            \
      for (voxel_index[1]=lower_corner[1];                              \
           voxel_index[1] <= upper_corner[1]; ++voxel_index[1]) {       \
        if (voxel_index[0] == center[0]                                 \
            && voxel_index[1] > center[1]) break;                       \
        for (voxel_index[2]=lower_corner[2];                            \
             voxel_index[2] <= upper_corner[2]; ++voxel_index[2]) {     \
          if (voxel_index[0] == center[0] && voxel_index[1] == center[1]\
              && voxel_index[2] >= center[2]) break;                    \
          { action}                                                     \
        }                                                               \
      }                                                                 \
    }                                                                   \
  }

#endif  /* IMPALGEBRA_GRID_3D_H */
