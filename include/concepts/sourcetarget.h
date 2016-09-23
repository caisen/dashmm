// =============================================================================
//  This file is part of:
//  Dynamic Adaptive System for Hierarchical Multipole Methods (DASHMM)
//
//  Copyright (c) 2015-2016, Trustees of Indiana University,
//  All rights reserved.
//
//  DASHMM is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  DASHMM is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with DASHMM. If not, see <http://www.gnu.org/licenses/>.
//
//  This software was created at the Indiana University Center for Research in
//  Extreme Scale Technologies (CREST).
// =============================================================================


/// Source concept in DASHMM
///
/// To qualify as a Source for DASHMM, a type should be trivially copyable,
/// and should provide one member accessible by name:
///
/// Point position;
///
/// For specific expansions, there will be further requirements on the contents
/// of the Source type for it to be compatible with the given Expansion.
/// In particular, sources will always have some required notion of 'charge'
/// for the Expansion in question. Often, this will be as simple as
///
/// double charge;
///
/// but could be more complicated in other cases. For details, please see the
/// specific Expansions in question.
///
/// Failure to provide a type with the required members will result in
/// compilation errors.
struct Source {
  Point position;
  double charge;
};




/// Target concept in DASHMM
///
/// To work as a Target type in DASHMM, a type must be trivially copyable and
/// must provide two (or more) members. The first is the location of the target
/// given by:
///
/// Point position
///
/// Second, one must provide a member for the output of the given expansion.
/// Often this is just
///
/// std::complex<double> phi
///
/// but it could be different for a given Expansion. See the documentation for
/// the Expansion of interest to determine the required members of Target for
/// using that Expansion. Some Expansion may produce multiple pieces of data
/// and so a target must supply each member.
///
/// Failure to provide the required members will result in compilation errors.
struct Target {
  Point position;
  std::complex<double> phi;   // for most, but not all cases
}