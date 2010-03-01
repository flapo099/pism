#include "../base/PISMComponent.hh"
#include "../base/iceModelVec.hh"
#include "deformation.hh"

class PISMBedDef : public PISMComponent {
public:
  PISMBedDef(IceGrid &g, const NCConfigVariable &conf);
  virtual ~PISMBedDef();
  virtual PetscErrorCode init(PISMVars &vars);
  virtual PetscErrorCode write_model_state(PetscReal t_years, PetscReal dt_years,
					   string filename)
  { return 0; }
  virtual PetscErrorCode write_fields(set<string> vars, PetscReal t_years,
				      PetscReal dt_years, string filename);

  virtual PetscErrorCode update(PetscReal t_years, PetscReal dt_years) = 0;
  virtual PetscErrorCode bed_elevation(PetscReal t_years, PetscReal dt_years, IceModelVec2 &result) = 0;
protected:
  PetscErrorCode pismbeddef_allocate(); // packaged to simplify error checking

  IceModelVec2 dtopgdt, topg_last, thk_last;
  IceModelVec2 *thk, *topg;
};

class PBPointwiseIsostasy : public PISMBedDef {
public:
  virtual PetscErrorCode init(PISMVars &vars);
  virtual PetscErrorCode update(PetscReal t_years, PetscReal dt_years);
  virtual PetscErrorCode bed_elevation(PetscReal t_years, PetscReal dt_years, IceModelVec2 &result) = 0;
};

#if (PISM_HAVE_FFTW==1)
#include <fftw3.h>

class PBLingleClark : public PISMBedDef {
  PBLingleClark(IceGrid &g, const NCConfigVariable &conf);
  virtual ~PBLingleClark();

  PetscErrorCode init(PISMVars &vars);
  PetscErrorCode update(PetscReal t_years, PetscReal dt_years);
protected:
  PetscErrorCode allocate();
  PetscErrorCode deallocate();
  PetscErrorCode transfer_to_proc0(IceModelVec2 &source, Vec result);
  PetscErrorCode transfer_from_proc0(Vec source, IceModelVec2 &result);
  IceModelVec2 topg_undeformed;
  Vec g2, g2natural;  // global Vecs used to transfer data to/from processor 0.
  VecScatter scatter; // VecScatter used to transfer data to/from processor 0.
  // Vecs on processor 0:
  Vec Hp0,			// ice thickness
    bedp0,			// bed elevation
    Hstartp0,			// initial (start-of-the-run) thickness
    bedstartp0,			// initial bed elevation
    upliftp0;			// bed uplift
  BedDeformLC bdLC;
};
#endif
