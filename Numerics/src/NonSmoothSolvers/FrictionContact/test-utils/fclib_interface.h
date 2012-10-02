/* Siconos-Numerics, Copyright INRIA 2005-2012.
 * Siconos is a program dedicated to modeling, simulation and control
 * of non smooth dynamical systems.
 * Siconos is a free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * Siconos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Siconos; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Contact: Vincent ACARY, siconos-team@lists.gforge.inria.fr
 */
#ifndef FCLIB_INTERFACE_H
#define FCLIB_INTERFACE_H

#include "NumericsConfig.h"

#if defined(__cplusplus) && !defined(BUILD_AS_CPP)
extern "C"
{
#endif

#ifdef WITH_FCLIB
  FrictionContactProblem* frictionContact_fclib_read(const char *path);

  int frictionContact_fclib_write(FrictionContactProblem* problem, char * title, char * description,
                                  char * math_info,
                                  const char *path);

#endif

#if defined(__cplusplus) && !defined(BUILD_AS_CPP)
}
#endif

#endif


