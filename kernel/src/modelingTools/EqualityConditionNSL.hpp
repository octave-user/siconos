/* Siconos-Kernel, Copyright INRIA 2005-2012.
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
 *
 * Note: the size of this non smooth law is one by default, but can be set to any value.
 * Size > 1 is usefull when D matrix in the relation is not null and not diagonal, to write the y = CX +Dlambda with only one
 * interaction and one Interaction.
 */
/*! \file EqualityConditionNSL.hpp

*/
#ifndef EQUALITYCONDITIONNSLAW_H
#define EQUALITYCONDITIONNSLAW_H

#include "NonSmoothLaw.hpp"


/** Equality NonSmoothLaw
 *
 * \author SICONOS Development Team - copyright INRIA
 *  \version 3.0.0.
 *  \date (Creation) Apr 27, 2004
 *
 *
 **/
class EqualityConditionNSL : public NonSmoothLaw
{
private:
  /** serialization hooks
  */
  ACCEPT_SERIALIZATION(EqualityConditionNSL);

  /** default constructor
   */
  EqualityConditionNSL() {};

public:
  /** basic constructor
  *  \param size of the non smooth law
  */
  EqualityConditionNSL(unsigned int size);

  /** Destructor */
  ~EqualityConditionNSL();


  /** print the data to the screen
  */
  inline void display()const {};

  /** Visitors hook
   */
  ACCEPT_STD_VISITORS();
};

TYPEDEF_SPTR(EqualityConditionNSL)

#endif // EQUALITYCONDITIONNSLAW_H