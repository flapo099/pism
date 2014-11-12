// Copyright (C) 2012, 2013, 2014 PISM Authors
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

#include "pism_const.hh"
#include "pism_options.hh"
#include "pismmerge.hh"

#include "PetscInitializer.hh"
#include "error_handling.hh"

using namespace pism;

static char help[] =
  "Tool for merging PISM output files produced using '-o_format quilt'.\n";

int process_one_variable(std::string var_name, std::string input_file, std::string output_file,
                         unsigned int compression_level) {
  NC4_Serial input(MPI_COMM_SELF, 0),
    output(MPI_COMM_SELF, compression_level);
  bool exists;
  int ierr;

  fprintf(stderr, "Merging variable %s from %s into %s, compression level %d...\n",
          var_name.c_str(), input_file.c_str(), output_file.c_str(), compression_level);

  // Fill the output file with metadata using the rank=0 "patch".
  input.open(patch_filename(input_file, 0), PISM_READONLY);

  // Create the output file
  output.create(output_file);

  // global attributes
  ierr = copy_attributes(input, output, "PISM_GLOBAL"); CHKERRQ(ierr);

  ierr = define_variable(input, output, var_name); CHKERRQ(ierr);

  input.inq_varid("time_bounds", exists);
  if (exists) {
    ierr = define_variable(input, output, "time_bounds"); CHKERRQ(ierr);
  }

  // mapping
  input.inq_varid("mapping", exists);
  if (exists) {
    ierr = define_variable(input, output, "mapping"); CHKERRQ(ierr);
  }

  // pism_override
  input.inq_varid("pism_override", exists);
  if (exists) {
    ierr = define_variable(input, output, "pism_override"); CHKERRQ(ierr);
  }

  // run_stats
  input.inq_varid("run_stats", exists);
  if (exists) {
    ierr = define_variable(input, output, "run_stats"); CHKERRQ(ierr);
  }

  // timestamp
  input.inq_varid("timestamp", exists);
  if (exists) {
    ierr = define_variable(input, output, "timestamp"); CHKERRQ(ierr);
  }

  // lat
  input.inq_varid("lat", exists);
  if (exists) {
    ierr = define_variable(input, output, "lat"); CHKERRQ(ierr);
  }

  // lon
  input.inq_varid("lon", exists);
  if (exists) {
    ierr = define_variable(input, output, "lon"); CHKERRQ(ierr);
  }

  input.close();

  ierr = copy_all_variables(input_file, output); CHKERRQ(ierr);

  output.close();

  fprintf(stderr, "Done.\n");

  return 0;
}

int process_all_variables(std::string input_file, std::string output_file,
                          unsigned int compression_level) {
  int ierr, n_vars;
  NC4_Serial input(MPI_COMM_SELF, 0),
    output(MPI_COMM_SELF, compression_level);

  fprintf(stderr, "Merging all variables from %s into %s, compression level %d...\n",
          input_file.c_str(), output_file.c_str(), compression_level);

  // Fill the output file with metadata using the rank=0 "patch".
  input.open(patch_filename(input_file, 0), PISM_READONLY);

  // Create the output file
  output.create(output_file);

  // global attributes
  ierr = copy_attributes(input, output, "PISM_GLOBAL"); CHKERRQ(ierr);

  // define all variables (except for {x,y}_patch)
  input.inq_nvars(n_vars);

  for (int j = 0; j < n_vars; ++j) {
    std::string var_name;

    input.inq_varname(j, var_name);

    if (var_name == "x_patch" || var_name == "y_patch") {
      continue;
    }

    ierr = define_variable(input, output, var_name); check(ierr);
  }

  ierr = copy_all_variables(input_file, output); CHKERRQ(ierr);

  output.close();

  fprintf(stderr, "Done.\n");

  return 0;
}

int main(int argc, char *argv[])
{
  PetscErrorCode  ierr;

  MPI_Comm    com = MPI_COMM_WORLD;
  int rank;

  PetscInitializer petsc(argc, argv, help);

  com = PETSC_COMM_WORLD;
  ierr = MPI_Comm_rank(com, &rank); CHKERRQ(ierr);

  /* This explicit scoping forces destructors to be called before PetscFinalize() */
  try {
    ierr = verbosityLevelFromOptions(); CHKERRQ(ierr);

    ierr = verbPrintf(2,com, "PISM-MERGE %s (output file merging tool)\n",
                      PISM_Revision); CHKERRQ(ierr);
    ierr = stop_on_version_option(); CHKERRQ(ierr);

    bool i_set, o_set, var_name_set, compression_level_set;
    std::string i_name, o_name, var_name;
    int compression_level = 0;
    ierr = OptionsString("-i", "Input file name",
                             i_name, i_set); CHKERRQ(ierr);
    ierr = OptionsString("-o", "Output file name",
                             o_name, o_set); CHKERRQ(ierr);
    ierr = OptionsString("-v", "Name of the variable to merge",
                             var_name, var_name_set); CHKERRQ(ierr);
    ierr = OptionsInt("-L", "Output compression level",
                          compression_level, compression_level_set); CHKERRQ(ierr);
    std::string usage =
      "  Merges output file created using '-o_format quilt'.\n\n"
      "  pismmerge {-i in.nc} [-o out.nc]\n"
      "where:\n"
      "  -i          in.nc is the name used with -extra_file or -o, e.g. ex-RANK.nc\n"
      "  -o          out.nc is the name of the output file that will contain merged data\n"
      "  -v var_name name of the variable to merge\n"
      "  -L <number> output compression level (from 0 to 9)\n"
      "notes:\n"
      "  * -o is optional\n";

    std::vector<std::string> required;
    required.push_back("-i");
    ierr = show_usage_check_req_opts(com, "pismmerge", required, usage); CHKERRQ(ierr);

    check_input_files(i_name);

    // Check the validity of the -L option.
    if (compression_level_set) {
      if (compression_level < 0 || compression_level > 9) {
        throw RuntimeError::formatted("invalid compression level: %d.",
                                      compression_level);
      }
    }

    // Set the output file name.
    if (o_set == false) {
      if (var_name_set == false) {
        o_name = output_filename(i_name, "ALL");
      } else {
        o_name = output_filename(i_name, var_name);
      }
    }

    if (rank == 0) {
      if (var_name_set) {
        ierr = process_one_variable(var_name, i_name, o_name, compression_level); CHKERRQ(ierr);
      } else {
        ierr = process_all_variables(i_name, o_name, compression_level); CHKERRQ(ierr);
      }
    }
  }
  catch (...) {
    handle_fatal_errors(com);
  }


  return 0;
}
