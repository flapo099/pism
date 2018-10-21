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

#ifndef _PIGIVEN_H_
#define _PIGIVEN_H_

#include "pism/coupler/InterfaceModel.hh"
#include "pism/util/iceModelVec2T.hh"

namespace pism {
namespace interface {

class Given : public InterfaceModel {
public:
  Given(IceGrid::ConstPtr g);
  virtual ~Given();
protected:
  void init_impl(const Geometry &geometry);
  void update_impl(const Geometry &geometry, double t, double dt);

  const IceModelVec2S &delta_impl() const;

  void define_model_state_impl(const PIO &output) const;
  void write_model_state_impl(const PIO &output) const;

  IceModelVec2T::Ptr m_delta;
};

} // end of namespace interface
} // end of namespace pism

#endif /* _PIGIVEN_H_ */
