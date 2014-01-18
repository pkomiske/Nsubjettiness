//  Nsubjettiness Package
//  Questions/Comments?  jthaler@jthaler.net
//
//  Copyright (c) 2011-14
//  Jesse Thaler, Ken Van Tilburg, Christopher K. Vermilion, and TJ Wilkason
//
//----------------------------------------------------------------------
// This file is part of FastJet contrib.
//
// It is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or (at
// your option) any later version.
//
// It is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this code. If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------

#include "WinnerTakeAllRecombiner.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

namespace contrib{

std::string WinnerTakeAllRecombiner::description() const {
   return "Winner Take All scheme recombination";
}

// recombine pa and pb by creating pab with energy of the sum of particle energies in the direction of the harder particle
void WinnerTakeAllRecombiner::recombine(const fastjet::PseudoJet & pa, const fastjet::PseudoJet & pb, fastjet::PseudoJet & pab) const {
   if (pa.perp() >= pb.perp()) {
      pab.reset_PtYPhiM(pa.perp() + pb.perp(), pa.rap(), pa.phi());
   }
   else if (pb.perp() > pa.perp()) {
      pab.reset_PtYPhiM(pa.perp() + pb.perp(), pb.rap(), pb.phi());
   }
}

} //namespace contrib

FASTJET_END_NAMESPACE
