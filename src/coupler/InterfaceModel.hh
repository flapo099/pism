// Copyright (C) 2008-2011, 2013, 2014, 2015, 2016, 2017, 2018 Ed Bueler, Constantine Khroulev, Ricarda Winkelmann,
// Gudfinna Adalgeirsdottir and Andy Aschwanden
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

#ifndef __PISMInterfaceModel_hh
#define __PISMInterfaceModel_hh

#include <memory>

#include "pism/util/Component.hh"

namespace pism {

class IceModelVec2S;
class Geometry;

//! @brief Interface models and modifiers: provide ts for 'delta',
//! in basalstrength
namespace interface {
//! A very rudimentary PISM interface ice/bed model.
class InterfaceModel : public Component {
public:
  // "modifier" constructor
  InterfaceModel(IceGrid::ConstPtr g, std::shared_ptr<InterfaceModel> input);
  // "model" constructor
  InterfaceModel(IceGrid::ConstPtr g);

  virtual ~InterfaceModel();

  void init(const Geometry &geometry);

  void update(const Geometry &geometry, double t, double dt);

  const IceModelVec2S& delta() const; 
  
protected:
  virtual void init_impl(const Geometry &geometry);
  // provides default (pass-through) implementations for "modifiers"
  virtual void update_impl(const Geometry &geometry, double t, double dt);
  virtual MaxTimestep max_timestep_impl(double t) const;
  virtual void define_model_state_impl(const PIO &output) const;
  virtual void write_model_state_impl(const PIO &output) const;

  virtual DiagnosticList diagnostics_impl() const;
  virtual TSDiagnosticList ts_diagnostics_impl() const;

  virtual const IceModelVec2S& delta_impl() const;
  
protected:
  std::shared_ptr<InterfaceModel> m_input_model;

  static IceModelVec2S::Ptr allocate_delta(IceGrid::ConstPtr g);
  
};

} // end of namespace interface
} // end of namespace pism

#endif  // __PISMInterfaceModel_hh
