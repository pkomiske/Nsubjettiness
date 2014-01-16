// Nsubjettiness Package
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

#ifndef __FASTJET_CONTRIB_NSUBJETTINESS_HH__
#define __FASTJET_CONTRIB_NSUBJETTINESS_HH__

#include <fastjet/internal/base.hh>

#include "Njettiness.hh"

#include "fastjet/FunctionOfPseudoJet.hh"
#include <string>
#include <climits>

#ifndef G__DICTIONARY
typedef double Double32_t; // ROOT will store as 32-bit, but in code is double
#endif


FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

namespace contrib {

//------------------------------------------------------------------------
/// \class Nsubjettiness
/// Nsubjettiness extends the concept of Njettiness to a jet shape, but other
/// than the set of particles considered, they are identical.  This class
/// wraps the core Njettiness code to provide the fastjet::FunctionOfPseudoJet
/// interface for convenience in larger analyses.  See NjettinessPlugin.hh for
/// definitions of tau_N and the constructor options.

class Nsubjettiness : public FunctionOfPseudoJet<Double32_t> {
public:

   //moved constructor definition to here to clean up code -- TJW 12/25
   //removed normalization bool argument -- TJW 1/8
   //removed NsubParameters in Njettiness constructor -- TJW 1/9
   // updated to use new Njettiness constructor with normalized_cutoff_measure default -- TJW 1/13
   Nsubjettiness(int N, 
      Njettiness::AxesMode axes_mode, 
      double beta, 
      double R0, 
      double Rcutoff=std::numeric_limits<double>::max())
   : _njettinessFinder(axes_mode, Njettiness::normalized_cutoff_measure, beta, R0, Rcutoff), _N(N) {}

   //new constructor definition added by TJW 1/7
   // added possibility of 4th parameter -- TJW 1/15
   Nsubjettiness(int N, 
      Njettiness::AxesMode axes_mode, 
      Njettiness::MeasureMode measure_mode, 
      double para1 = NAN, double para2 = NAN, double para3 = NAN, double para4 = NAN) 
   : _njettinessFinder(axes_mode, measure_mode, para1, para2, para3, para4), _N(N) {}

   /// returns tau_N, measured on the constituents of this jet 
   Double32_t result(const PseudoJet& jet) const;

   //To set axes for manual use 
   void setAxes(std::vector<fastjet::PseudoJet> myAxes) {
      // TODO:  Have this test that manual axes are being used
    	_njettinessFinder.setAxes(myAxes);
   }

   //TODO: add TauComponents function that returns all necessary tau components if desired -- TJW

private:

   mutable Njettiness _njettinessFinder; // should muck with this so result can be const without this mutable

   int _N;
   //_normalization bool removed -- TJW 1/8
};

//result definition moved to Nsubjettiness.cc -- TJW 12/22

//------------------------------------------------------------------------
/// \class NsubjettinessRatios
// NsubjettinessRatios Calculates uses the results from Nsubjettiness to calculate the ratio
// tau_N/tau_M, where N and M are specified by the user. The ratio of different tau values
// proves to be an interesting value for analysis, and this class allows these values
// to be calculated with ease. -- comment added by TJW

class NsubjettinessRatio : public FunctionOfPseudoJet<Double32_t> {
public:

   // old constructor removed since it isn't necessary for backwards compatibility -- TJW 1/13

   // new constructor to match new Nsubjettiness constructor -- TJW 1/10
   // added possibility of 4th parameter -- TJW 1/15
   NsubjettinessRatio(int N, int M, 
      Njettiness::AxesMode axes_mode, 
      Njettiness::MeasureMode measure_mode,
      double para1 = NAN, double para2 = NAN, double para3 = NAN, double para4 = NAN)
   : _nsub_numerator(N, axes_mode, measure_mode, para1, para2, para3, para4), _nsub_denominator(M, axes_mode, measure_mode, para1, para2, para3, para4) {};

   //changed return value from double to Double32_t to match Nsubjettiness class -- TJW 12/22
   //returns tau_N/tau_M based off the input jet using result function from Nsubjettiness 
   Double32_t result(const PseudoJet& jet) const;

private: 
   Nsubjettiness _nsub_numerator;
   Nsubjettiness _nsub_denominator;
};

//ratio result definition moved to Nsubjettiness.cc --TJW 12/22

} // namespace contrib

FASTJET_END_NAMESPACE

#endif  // __FASTJET_CONTRIB_NSUBJETTINESS_HH__
