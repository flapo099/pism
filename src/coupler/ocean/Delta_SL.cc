// Copyright (C) 2011, 2012, 2013, 2014, 2015, 2016, 2017 PISM Authors
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

#include <gsl/gsl_math.h>

#include "Delta_SL.hh"
#include "pism/util/ConfigInterface.hh"
#include "pism/util/io/io_helpers.hh"
#include "pism/util/pism_utilities.hh"

namespace pism {
namespace ocean {

/// -ocean_delta_SL_file, ...

Delta_SL::Delta_SL(IceGrid::ConstPtr g, OceanModel* in)
  : PScalarForcing<OceanModel,OceanModifier>(g, in) {

  m_option_prefix = "-ocean_delta_SL";
  m_offset_name   = "delta_SL";

  m_offset = new Timeseries(*m_grid, m_offset_name, m_config->get_string("time.dimension_name"));

  m_offset->variable().set_string("units", "m");
  m_offset->variable().set_string("long_name", "sea level elevation offsets");
  m_offset->dimension().set_string("units", m_grid->ctx()->time()->units_string());

}

Delta_SL::~Delta_SL() {
  // empty
}

void Delta_SL::init_impl() {

  m_t = m_dt = GSL_NAN;  // every re-init restarts the clock

  m_input_model->init();

  m_log->message(2, "* Initializing sea level forcing...\n");

  init_internal();
}

MaxTimestep Delta_SL::max_timestep_impl(double t) const {
  (void) t;
  return MaxTimestep("ocean delta_SL");
}

void Delta_SL::sea_level_elevation_impl(double &result) const {
  result = m_input_model->sea_level_elevation() + m_current_forcing;
}

} // end of namespace ocean
} // end of namespace pism
