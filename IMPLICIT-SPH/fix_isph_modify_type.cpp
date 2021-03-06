/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
   ------------------------------------------------------------------------- */

#include "stdio.h"
#include "string.h"
#include "atom.h"
#include "comm.h"
#include "force.h"
#include "update.h"
#include "respa.h"
#include "error.h"
#include "neighbor.h"
#include "neigh_list.h"
#include "neigh_request.h"
#include "pair.h"
#include "domain.h"
#include "math.h"

#include <iostream>
#include "utils.h"

#include "pair_isph_corrected.h"
//#include "pair_isph_mls.h"

#include "fix_isph_modify_type.h"

using namespace LAMMPS_NS;
using namespace FixConst;
/* ---------------------------------------------------------------------- */
#undef  __FUNCT__
#define __FUNCT__ "FixISPH_ModifyType::FixISPH_ModifyType"
FixISPH_ModifyType::FixISPH_ModifyType(LAMMPS *lmp, int narg, char **arg) 
  : Fix(lmp, narg, arg) {
  FUNCT_ENTER(comm->me);

  if (atom->isph_flag != 1)
    error->all(FLERR, "Fix ipsh/modify/type command requires a matching atom_style, i.e., isph");

  if (narg < 3)
    error->all(FLERR,"Illegal number of arguments for fix isph/modify/type command");
  
  // default values
  _type_from = 0;
  _type_to   = 0;
  
  _xmin = 0.0; _xmax = 0.0;
  _ymin = 0.0; _ymax = 0.0;
  _zmin = 0.0; _zmax = 0.0;

  // over-ride shifting parameters
  switch (narg) {
  case 11: {
    _zmin = force->numeric(FLERR, arg[9]);
    _zmax = force->numeric(FLERR, arg[10]);
  }
  case 9: {
    _ymin = force->numeric(FLERR, arg[7]);
    _ymax = force->numeric(FLERR, arg[8]);
  }
  case 7: {
    _xmin = force->numeric(FLERR, arg[5]);
    _xmax = force->numeric(FLERR, arg[6]);
  }
  case 5: {
    _type_from = atoi(arg[3]);
    _type_to = atoi(arg[4]);
    break;
  }
  default: { 
    error->all(FLERR,"Illegal number of arguments for fix isph/modify/type command");   
    break;
  }
  }

  if (comm->me == 0) {
    const int w = 15; 
    cout << "FixISPH_ModifyType:: " << _type_from << " --> " << _type_to << endl;
    cout << scientific
         << setw(w) << _xmin << "   "
         << setw(w) << _xmax << "   "
         << setw(w) << _ymin << "   "
         << setw(w) << _ymax << "   "
         << setw(w) << _zmin << "   "
         << setw(w) << _zmax << "   "
         << fixed << endl;
  }

  time_integrate = 0;
  FUNCT_EXIT(comm->me);
}

/* ---------------------------------------------------------------------- */
#undef  __FUNCT__
#define __FUNCT__ "FixISPH_ModifyType::~FixISPH_ModifyType"
FixISPH_ModifyType::~FixISPH_ModifyType() {
  FUNCT_ENTER(comm->me);
  FUNCT_EXIT(comm->me);
}

/* ---------------------------------------------------------------------- */
#undef  __FUNCT__
#define __FUNCT__ "FixISPH_ModifyType::setmask"
int FixISPH_ModifyType::setmask() {
  FUNCT_ENTER(comm->me);

  int mask = 0;
  mask |= INITIAL_INTEGRATE;

  FUNCT_EXIT(comm->me);

  return mask;
}

/* ---------------------------------------------------------------------- */
#undef  __FUNCT__
#define __FUNCT__ "FixISPH_ModifyType::initial_integrate"
void FixISPH_ModifyType::initial_integrate(int dummy) {
  FUNCT_ENTER(comm->me);

  // get accesses to data structures
  const int nsize = (atom->nlocal + atom->nghost);

  double **x = atom->x;
  int *type = atom->type;

  bool check_x = (_xmin != _xmax);
  bool check_y = (_ymin != _ymax);
  bool check_z = (_zmin != _zmax);

  for (int i=0;i<nsize;++i) {
    double *x_at_i = x[i];
    bool is_modify = true;

    is_modify &= (check_x ? (_xmin < x_at_i[0] && x_at_i[0] < _xmax) : true);
    is_modify &= (check_y ? (_ymin < x_at_i[1] && x_at_i[1] < _ymax) : true);
    is_modify &= (check_z ? (_zmin < x_at_i[2] && x_at_i[2] < _zmax) : true);

    if (is_modify && type[i] == _type_from) 
      type[i] = _type_to;
  }

  // PairISPH *pair = dynamic_cast<PairISPH*>(force->pair_match("isph", 0));
  // if (pair != NULL) {
  //   pair->refreshParticles();
  // } else {
  //   error->all(FLERR, "Fail to retrive a pointer to pair: PairISPH");
  // }

  FUNCT_EXIT(comm->me);
}


