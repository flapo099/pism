// Copyright (C) 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018 PISM Authors
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

#ifndef _PIDELTALFORCING_H_
#define _PIDELTAFORCING_H_

#include "pism/coupler/InterfaceModel.hh"

namespace pism {

class ScalarForcing;

namespace interface {


class f_Delta : public InterfaceModel {
public:
  f_Delta(IceGrid::ConstPtr grid, std::shared_ptr<InterfaceModel> in);
  virtual ~f_Delta();

private:
  void init_impl(const Geometry &geometry);

  void update_impl(const Geometry &geometry, double t, double dt);

  const IceModelVec2S& delta_impl() const;

  IceModelVec2S::Ptr m_delta;
  std::unique_ptr<ScalarForcing> m_forcing;
};

} // end of namespace interface
} // end of namespace pism

#endif /* _PIDELTAFORCING_H_ */