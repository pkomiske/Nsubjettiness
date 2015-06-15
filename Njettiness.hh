//  Nsubjettiness Package
//  Questions/Comments?  jthaler@jthaler.net
//
//  Copyright (c) 2011-14
//  Jesse Thaler, Ken Van Tilburg, Christopher K. Vermilion, and TJ Wilkason
//
//  $Id$
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

#ifndef __FASTJET_CONTRIB_NJETTINESS_HH__
#define __FASTJET_CONTRIB_NJETTINESS_HH__


#include "MeasureDefinition.hh"
#include "AxesDefinition.hh"
#include "TauComponents.hh"

#include "fastjet/PseudoJet.hh"
#include "fastjet/SharedPtr.hh"
#include <fastjet/LimitedWarning.hh>

#include <cmath>
#include <vector>
#include <list>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

namespace contrib {

/** \mainpage Nsubjettiness Documentation
 *
 * These Doxygen pages provide automatically generated documentation for the
 * Nsubjettiness FastJet contrib.  This documentation is being slowly improved.
 *
 * \section key_classes Key Classes
 *
 * - Nsubjettiness:  Calculating the N-subjettiness jet shapes
 *
 * - NsubjettinessRatio:  Calculating N-subjettiness ratios
 *
 * - XConePlugin:  Running the XCone Jet Algorithm
 *
 * - NjettinessPlugin:  Running N-jettiness as a Jet Algorithm more generically
 */
   


///////
//
// Main Njettiness Class
//
///////

///------------------------------------------------------------------------
/// \class Njettiness
/**
 * The N-jettiness event shape.
 *
 * This is the core class used to perform N-jettiness jet finding (via NjettinessPlugin or XConePluggin)
 * as well as find the N-subjettiness jet shape (via Nsubjettiness).
 *
 * In general, the user should never need to call this object.  In addition, its API should not be considered
 * fixed, since all code improvements effectively happen from reorganizing this class.
 *
 * Njettiness uses AxesDefinition and MeasureDefinition together in order to find tau_N for the event.
 * It also can return information about the axes and jets it used in the calculation, as well as
 * information about how the event was partitioned.
 */
///------------------------------------------------------------------------

class Njettiness {
public:
   
   /// Main constructor that uses AxesMode and MeasureDefinition to specify measure
   /// Unlike Nsubjettiness or NjettinessPlugin, the value N is not chosen
   Njettiness(const AxesDefinition & axes_def, const MeasureDefinition & measure_def);

   /// Destructor
   ~Njettiness() {};
   
   /// setAxes for Manual mode
   void setAxes(const std::vector<fastjet::PseudoJet> & myAxes);
   
   /// Calculates and returns all TauComponents that user would want.
   /// This information is stored in _current_tau_components for later access as well.
   TauComponents getTauComponents(unsigned n_jets, const std::vector<fastjet::PseudoJet> & inputJets) const;

   /// Calculates the value of N-subjettiness,
   /// but only returns the tau value from _current_tau_components
   double getTau(unsigned n_jets, const std::vector<fastjet::PseudoJet> & inputJets) const {
      return getTauComponents(n_jets, inputJets).tau();
   }

   /// Return all relevant information about tau components
   TauComponents currentTauComponents() const {return _current_tau_components;}
   /// Return axes found by getTauComponents.
   std::vector<fastjet::PseudoJet> currentAxes() const { return _currentAxes;}
   /// Return seedAxes used if onepass minimization (otherwise, same as currentAxes)
   std::vector<fastjet::PseudoJet> seedAxes() const { return _seedAxes;}
   /// Return jet partition found by getTauComponents.
   std::vector<fastjet::PseudoJet> currentJets() const {return _currentPartition.jets();}
   /// Return beam partition found by getTauComponents.
   fastjet::PseudoJet currentBeam() const {return _currentPartition.beam();}
   /// Return beam partition found by getTauComponents.
   TauPartition currentPartition() const {return _currentPartition;}
   

private:
   
   /// AxesDefinition to use.  Implemented as SharedPtrs to avoid memory management headaches
   SharedPtr<const AxesDefinition> _axes_def;
   /// MeasureDefinition to use.  Implemented as SharedPtrs to avoid memory management headaches
   SharedPtr<const MeasureDefinition> _measure_def;

   
   // Information about the current information
   // Defined as mutables, so user should be aware that these change when getTau is called.
   // TODO:  These are not thread safe and should be fixed somehow
   mutable TauComponents _current_tau_components; //automatically set to have components of 0; these values will be set by the getTau function call
   mutable std::vector<fastjet::PseudoJet> _currentAxes; //axes found after minimization
   mutable std::vector<fastjet::PseudoJet> _seedAxes; // axes used prior to minimization (if applicable)
   mutable TauPartition _currentPartition; //partitioning information

   /// Warning if the user tries to use v1.0.3 measure style.
   static LimitedWarning _old_measure_warning;
   /// Warning if the user tries to use v1.0.3 axes style.
   static LimitedWarning _old_axes_warning;

   
public:
   
   // These interfaces are included for backwards compability, and will be deprecated in a future release (scheduled for deletion in v3.0)
   
   /// Deprecated enum to determine axes mode
   /// The various axes choices available to the user
   /// It is recommended to use AxesDefinition instead of these.
   enum AxesMode {
      kt_axes,             // exclusive kt axes
      ca_axes,             // exclusive ca axes
      antikt_0p2_axes,     // inclusive hardest axes with antikt-0.2
      wta_kt_axes,         // Winner Take All axes with kt
      wta_ca_axes,         // Winner Take All axes with CA
      onepass_kt_axes,     // one-pass minimization from kt starting point
      onepass_ca_axes,     // one-pass minimization from ca starting point
      onepass_antikt_0p2_axes,  // one-pass minimization from antikt-0.2 starting point
      onepass_wta_kt_axes, //one-pass minimization of WTA axes with kt
      onepass_wta_ca_axes, //one-pass minimization of WTA axes with ca
      min_axes,            // axes that minimize N-subjettiness (100 passes by default)
      manual_axes,         // set your own axes with setAxes()
      onepass_manual_axes  // one-pass minimization from manual starting point
   };
   
   /// Deprecated enum to determine measure mode
   /// The measures available to the user.
   /// "normalized_cutoff_measure" was the default in v1.0 of Nsubjettiness
   /// "unnormalized_measure" is now the recommended default usage
   /// But it is recommended to use MeasureDefinition instead of these.
   enum MeasureMode {
      normalized_measure,           //default normalized measure
      unnormalized_measure,         //default unnormalized measure
      geometric_measure,            //geometric measure
      normalized_cutoff_measure,    //default normalized measure with explicit Rcutoff
      unnormalized_cutoff_measure,  //default unnormalized measure with explicit Rcutoff
      geometric_cutoff_measure      //geometric measure with explicit Rcutoff
   };
   
   /// Intermediate constructor (needed to enable v1.0.3 backwards compatibility?)
   Njettiness(AxesMode axes_mode, const MeasureDefinition & measure_def);
   
   /// Old-style constructor which takes axes/measure information as enums with measure parameters
   /// This version absolutely is not recommended
   Njettiness(AxesMode axes_mode,
              MeasureMode measure_mode,
              int num_para,
              double para1 = std::numeric_limits<double>::quiet_NaN(),
              double para2 = std::numeric_limits<double>::quiet_NaN(),
              double para3 = std::numeric_limits<double>::quiet_NaN())
   : _axes_def(createAxesDef(axes_mode)), _measure_def(createMeasureDef(measure_mode, num_para, para1, para2, para3)) {
   }
  
   /// Convert old style enums into new style MeasureDefinition
   AxesDefinition* createAxesDef(AxesMode axes_mode) const;
   
   /// Convert old style enums into new style MeasureDefinition
   MeasureDefinition* createMeasureDef(MeasureMode measure_mode, int num_para, double para1, double para2, double para3) const;

};
   
} // namespace contrib

FASTJET_END_NAMESPACE

#endif  // __FASTJET_CONTRIB_NJETTINESS_HH__

