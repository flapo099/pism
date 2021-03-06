// Copyright (C) 2011, 2014, 2015, 2016, 2017 Constantine Khroulev and David Maxwell
//
// This file is part of PISM.
//
// PISM is free software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation; either version 3 of the License, or (at your option) any later
// version.
//
// PISM is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License
// along with PISM; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include <cassert>

#include "Mask.hh"
#include "IceGrid.hh"

namespace pism {

void GeometryCalculator::compute(double sea_level, const IceModelVec2S &bed, const IceModelVec2S &thickness,
                                 IceModelVec2Int &out_mask, IceModelVec2S &out_surface) const {
  compute_mask(sea_level, bed, thickness, out_mask);
  compute_surface(sea_level, bed, thickness, out_surface);
}

void GeometryCalculator::compute(const IceModelVec2S& sea_level,
                                 const IceModelVec2S& bed,
                                 const IceModelVec2S& thickness,
                                 IceModelVec2Int& out_mask,
                                 IceModelVec2S& out_surface) const {
  compute_mask(sea_level, bed, thickness, out_mask);
  compute_surface(sea_level, bed, thickness, out_surface);
}

void GeometryCalculator::compute_mask(const IceModelVec2S &sea_level,
                                      const IceModelVec2S &bed,
                                      const IceModelVec2S &thickness,
                                      IceModelVec2Int &result) const {
  IceModelVec::AccessList list{&sea_level, &bed, &thickness, &result};

  const IceGrid &grid = *bed.get_grid();

  const unsigned int stencil = result.get_stencil_width();
  assert(sea_level.get_stencil_width() >= stencil);
  assert(bed.get_stencil_width()       >= stencil);
  assert(thickness.get_stencil_width() >= stencil);

  for (PointsWithGhosts p(grid, stencil); p; p.next()) {
    const int i = p.i(), j = p.j();

    result(i,j) = this->mask(sea_level(i, j), bed(i, j), thickness(i, j));
  }
}

void GeometryCalculator::compute_mask(double sea_level,
                                      const IceModelVec2S &bed,
                                      const IceModelVec2S &thickness,
                                      IceModelVec2Int &result) const {
  IceModelVec::AccessList list{&bed, &thickness, &result};

  const IceGrid &grid = *bed.get_grid();

  const unsigned int stencil = result.get_stencil_width();
  assert(bed.get_stencil_width()       >= stencil);
  assert(thickness.get_stencil_width() >= stencil);

  for (PointsWithGhosts p(grid, stencil); p; p.next()) {
    const int i = p.i(), j = p.j();

    result(i,j) = this->mask(sea_level, bed(i,j), thickness(i,j));
  }
}

void GeometryCalculator::compute_surface(const IceModelVec2S &sea_level,
                                         const IceModelVec2S &bed,
                                         const IceModelVec2S &thickness,
                                         IceModelVec2S &result) const {
  IceModelVec::AccessList list{&sea_level, &bed, &thickness, &result};

  const IceGrid &grid = *bed.get_grid();

  const unsigned int stencil = result.get_stencil_width();
  assert(sea_level.get_stencil_width() >= stencil);
  assert(bed.get_stencil_width()       >= stencil);
  assert(thickness.get_stencil_width() >= stencil);

  for (PointsWithGhosts p(grid, stencil); p; p.next()) {
    const int i = p.i(), j = p.j();

    result(i,j) = this->surface(sea_level(i, j), bed(i, j), thickness(i, j));
  }
}

void GeometryCalculator::compute_surface(double sea_level,
                                         const IceModelVec2S &bed,
                                         const IceModelVec2S &thickness,
                                         IceModelVec2S &result) const {
  IceModelVec::AccessList list{&bed, &thickness, &result};

  const IceGrid &grid = *bed.get_grid();

  const unsigned int stencil = result.get_stencil_width();
  assert(bed.get_stencil_width()       >= stencil);
  assert(thickness.get_stencil_width() >= stencil);

  for (PointsWithGhosts p(grid, stencil); p; p.next()) {
    const int i = p.i(), j = p.j();

    result(i,j) = this->surface(sea_level, bed(i,j), thickness(i,j));
  }
}

} // end of namespace pism
