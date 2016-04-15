#!/usr/bin/env python

# Copyright (C) 2012, 2014, 2016 Moritz Huetten and Torsten Albrecht

# create MISMIP config override file

try:
    from netCDF4 import Dataset as NC
except:
    print "netCDF4 is not installed!"
    sys.exit(1)

filename = "MISMIP3D_conf.nc"

print '  creating MISMIP3D_conf.nc for configuration parameters (overrides) ...'

nc = NC(filename, 'w', format="NETCDF3_CLASSIC")

var = nc.createVariable("pism_overrides", 'i')

attrs = {"is_dry_simulation": "no",
         "include_bmr_in_continuity": "no",
         "ssa.compute_surface_gradient_inward": "no",
         "flow_law.isothermal_Glen.ice_softness": 1.0e-25,
         "ice.density": 900.,
         "sea_water.density": 1000.,
         "bootstrapping.defaults.geothermal_flux": 0.0,
         "Glen_exponent": 3.,
         "standard_gravity": 9.81,
         "ocean.sub_shelf_heat_flux_into_ice": 0.0,
         "sia.bed_smoother_range": 0.0,
         }

for name, value in attrs.iteritems():
    var.setncattr(name, value)

nc.close()
