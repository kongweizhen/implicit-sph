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


#include "comm.h"

#include <iostream>
#include "utils.h"
#include "bond_isph_fene.h"

using namespace LAMMPS_NS;

/* ---------------------------------------------------------------------- */
#undef  __FUNCT__
#define __FUNCT__ "BondISPH_FENE::BondISPH_FENE"
BondISPH_FENE::BondISPH_FENE(LAMMPS *lmp)
  : BondFENE(lmp),
    BondISPH() {
  FUNCT_ENTER(comm->me);
  FUNCT_EXIT(comm->me);
}

#undef  __FUNCT__
#define __FUNCT__ "BondISPH_FENE::~BondISPH_FENE"
BondISPH_FENE::~BondISPH_FENE() {
  FUNCT_ENTER(comm->me);
  FUNCT_EXIT(comm->me);
}

#undef  __FUNCT__
#define __FUNCT__ "BondISPH_FENE::compute"
void BondISPH_FENE::compute(int eflag, int vflag) {
  if (BondISPH::isComputeEnabled()) {
    FUNCT_ENTER(comm->me);
    BondFENE::compute(eflag, vflag);
    FUNCT_EXIT(comm->me);
  }
}
