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
*/
#include "OneStepIntegratorXML.hpp"
using namespace std;

OneStepIntegratorXML::OneStepIntegratorXML():
  rootNode(NULL), DSConcernedNode(NULL), interactionsConcernedNode(NULL)
{}

OneStepIntegratorXML::OneStepIntegratorXML(xmlNodePtr OneStepIntegratorNode):
  rootNode(OneStepIntegratorNode), DSConcernedNode(NULL), interactionsConcernedNode(NULL)
{
  xmlNodePtr node;
  // get dsConcerned
  if ((node = SiconosDOMTreeTools::findNodeChild(rootNode, DS_CONCERNED)))
    DSConcernedNode = node;

  // get interactionsConcerned
  if ((node = SiconosDOMTreeTools::findNodeChild(rootNode, INTERACTIONS_CONCERNED)))
    interactionsConcernedNode = node;
}

void OneStepIntegratorXML::getDSNumbers(vector<int>& dsNumbers)
{
  if (!hasAllDS())
    SiconosDOMTreeTools::getVector(DSConcernedNode, dsNumbers);
  else
    XMLException::selfThrow("OneStepIntegratorXML::getDSNumbers - The list of dynamical systems is missing.");
}

void OneStepIntegratorXML::getInteractionsNumbers(vector<int>& inNumbers)
{
  if (!hasAllInteractions())
    SiconosDOMTreeTools::getVector(interactionsConcernedNode, inNumbers);
  else
    XMLException::selfThrow("OneStepIntegratorXML::getInteractionsNumbers - The list of interactions is missing.");
}
