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

#include "f_Delta.hh"
#include "pism/coupler/util/ScalarForcing.hh"

namespace pism {
namespace interface {

f_Delta::f_Delta(IceGrid::ConstPtr grid, std::shared_ptr<InterfaceModel> in)
  : InterfaceModel(grid, in) {

  m_forcing.reset(new ScalarForcing(grid->ctx(),
                                    "-interface_delta", 
				    "delta",
                                    "1", 
                                    "1",
                                    "till effective fraction overburden offsets"));
  m_delta = allocate_delta(grid);
}

f_Delta::~f_Delta() {
  // empty
}

void f_Delta::init_impl(const Geometry &geometry) {

  m_input_model->init(geometry);

  m_log->message(2, "* Initializing scalar interface forcing...\n");

  m_forcing->init();
}

void f_Delta::update_impl(const Geometry &geometry, double t, double dt) {
  m_input_model->update(geometry, t, dt);

  m_forcing->update(t, dt);

  //m_delta.copy_from(m_input_model->delta()); //possibly??
  //m_delta.shift(m_forcing->value());
  m_delta->copy_from(m_input_model->delta());
  m_delta->shift(m_forcing->value());
}

const IceModelVec2S& f_Delta::delta_impl() const {
  return *m_delta;
}

} // end of namespace interface
} // end of namespace pism
