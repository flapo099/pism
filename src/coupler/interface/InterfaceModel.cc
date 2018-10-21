/* Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018 PISM Authors
 *
 * This file is part of PISM.
 *
 * PISM is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
 * version.
 *
 * PISM is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PISM; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <gsl/gsl_math.h>       // GSL_NAN

#include "pism/coupler/InterfaceModel.hh"
#include "pism/util/iceModelVec.hh"
#include "pism/util/MaxTimestep.hh"
#include "pism/util/pism_utilities.hh"

namespace pism {
namespace interface {

IceModelVec2S::Ptr InterfaceModel::allocate_delta(IceGrid::ConstPtr g) {
  IceModelVec2S::Ptr result(new IceModelVec2S(g, "delta", WITHOUT_GHOSTS));
  result->set_attrs("climate_forcing",
                    "till effective fraction overburden at the interface ice/bed",
                    "1", "");
  result->metadata().set_doubles("valid_range", {0.0, 1.0});??
  return result;
}

// "modifier" constructor
InterfaceModel::InterfaceModel(IceGrid::ConstPtr g, std::shared_ptr<InterfaceModel> input)
  : Component(g), m_input_model(input) {
  // empty
}

// "model" constructor
InterfaceModel::InterfaceModel(IceGrid::ConstPtr g)
  : InterfaceModel(g, nullptr) {
  // empty
}

InterfaceModel::~InterfaceModel() {
  // empty
}

void InterfaceModel::init(const Geometry &geometry) {
  this->init_impl(geometry);
}

void InterfaceModel::init_impl(const Geometry &geometry) {
  if (m_input_model) {
    m_input_model->init(geometry);
  }
}

void InterfaceModel::update(const Geometry &geometry, double t, double dt) {
  this->update_impl(geometry, t, dt);
}


const IceModelVec2S& InterfaceModel::delta() const {
  return delta_impl();
}


// pass-through default implementations for "modifiers"

void InterfaceModel::update_impl(const Geometry &geometry, double t, double dt) {
  if (m_input_model) {
    m_input_model->update(geometry, t, dt);
  } else {
    throw RuntimeError::formatted(PISM_ERROR_LOCATION, "no input model");
  }
}

MaxTimestep InterfaceModel::max_timestep_impl(double t) const {
  if (m_input_model) {
    return m_input_model->max_timestep(t);
  } else {
    throw RuntimeError::formatted(PISM_ERROR_LOCATION, "no input model");
  }
}

void InterfaceModel::define_model_state_impl(const PIO &output) const {
  if (m_input_model) {
    return m_input_model->define_model_state(output);
  } else {
    // no state to define
  }
}

void InterfaceModel::write_model_state_impl(const PIO &output) const {
  if (m_input_model) {
    return m_input_model->write_model_state(output);
  } else {
    // no state to write
  }
}

const IceModelVec2S& InterfaceModel::delta_impl() const {
  if (m_input_model) {
    return m_input_model->delta();
  } else {
    throw RuntimeError::formatted(PISM_ERROR_LOCATION, "no input model");
  }
}


namespace diagnostics {

/*! @brief Delta. */
class PI_delta : public Diag<InterfaceModel>
{
public:
  PI_delta(const InterfaceModel *m)
    : Diag<InterfaceModel>(m) {

    /* set metadata: */
    m_vars = {SpatialVariableMetadata(m_sys, "del")};

    set_attrs("till effective fraction overburden at the interface ice/bed", "",
              "1", "1", 0);
  }
protected:
  IceModelVec::Ptr compute_impl() const {

    IceModelVec2S::Ptr result(new IceModelVec2S(m_grid, "del", WITHOUT_GHOSTS));
    result->metadata(0) = m_vars[0];

    result->copy_from(model->delta());

    return result;
  }
};



} // end of namespace diagnostics

DiagnosticList InterfaceModel::diagnostics_impl() const {
  using namespace diagnostics;
  DiagnosticList result = {
    {"delta",                     Diagnostic::Ptr(new PI_delta(this))}
  };

  if (m_input_model) {
    return combine(m_input_model->diagnostics(), result);
  } else {
    return result;
  }
}

TSDiagnosticList InterfaceModel::ts_diagnostics_impl() const {
  if (m_input_model) {
    return m_input_model->ts_diagnostics();
  } else {
    return {};
  }
}


} // end of namespace interface
} // end of namespace pism
