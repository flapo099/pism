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

#include "GivenInterface.hh"

#include "pism/coupler/util/options.hh"
#include "pism/util/IceGrid.hh"
#include "pism/util/ConfigInterface.hh"
#include "pism/util/Time.hh"


namespace pism {
namespace interface {

Given::Given(IceGrid::ConstPtr grid)
  : InterfaceModel(grid)
{
//  (void) input;

  ForcingOptions opt(*m_grid->ctx(), "interface.given");

  {
    unsigned int buffer_size = m_config->get_double("interface_forcing.buffer_size");
    unsigned int evaluations_per_year = m_config->get_double("interface_forcing.evaluations_per_year");
    bool periodic = opt.period > 0;

    PIO file(m_grid->com, "netcdf3", opt.filename, PISM_READONLY);

    m_delta = IceModelVec2T::ForcingField(m_grid,
                                          file,
                                          "interface_delta",
                                           "", // no standard name
                                           buffer_size,
                                           evaluations_per_year,
                                           periodic);

  }
  {
    m_delta->set_attrs("interface_forcing",
                       "till effective fraction overburden variable",
                       "1", "");
    m_delta->metadata().set_doubles("valid_range", {0.0, 1.0}); // fraction
  }
}

Given::~Given() {
  // empty
}

void Given::init_impl(const Geometry &geometry) {

  m_log->message(2,
                 "* Initializing the interface model reading delta at the interface of n"
                 "  ice and bed from a file...\n");

  ForcingOptions opt(*m_grid->ctx(), "interface.given");

  m_delta->init(opt.filename, opt.period, opt.reference_time);
 
  // read time-independent data right away:
  if (m_delta->n_records() == 1) {
    update(geometry, m_grid->ctx()->time()->current(), 0); // dt is irrelevant
  }
}

void Given::update_impl(const Geometry &geometry, double t, double dt) {
  (void) geometry;

  m_delta->update(t, dt);
  
  m_delta->average(t, dt);
}

const IceModelVec2S &Given::delta_impl() const {
  return *m_delta;
}

void Given::define_model_state_impl(const PIO &output) const {
  m_delta->define(output);
}

void Given::write_model_state_impl(const PIO &output) const {
  m_delta->write(output);
}

} // end of namespace interface
} // end of namespace pism