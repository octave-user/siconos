/* Siconos-Numerics, Copyright INRIA 2005-2011.
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
#include <stdio.h>
#include <stdlib.h>
#include "NonSmoothDrivers.h"
#include "frictionContact_test_function.h"
#include "misc.h"

int main(void)
{
  int info = 0 ;

  int nc = 4 ;
  int nb = 16 ;
  double mu[4] =
  {
    7.500000000000000000000000e-01,     7.500000000000000000000000e-01,     7.500000000000000000000000e-01,     7.500000000000000000000000e-01
  };
  unsigned int row[16] =
  {
    1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,   4
  };
  unsigned int column[16] =
  {
    1,  2,  3,  4,  1,  2,  3,  4,  1,  2,  3,  4,  1,  2,  3,   4
  };
  double q[12] =
  {
    -9.809999999999999276134588e-03,   0.000000000000000000000000e+00,  -6.006892553999999531711421e-19,  -9.809999999925425248625821e-03,   0.000000000000000000000000e+00,  -6.006892553999999531711421e-19,  -9.809999999927116604014898e-03,   0.000000000000000000000000e+00,  -6.006892553999999531711421e-19,  -9.809999999999999276134588e-03,   0.000000000000000000000000e+00,   -6.006892553999999531711421e-19
  };
  double W[144] =
  {
    9.906380157955265275404599e+00,    -2.190000014016017182427731e+00,    -2.166000013862399597996955e+00,    -2.190000014016017182427731e+00,     5.643518874808445495716569e+00,    -2.470593759882395712423886e+00,    -2.166000013862399597996955e+00,    -2.470593759882395712423886e+00,     5.697968875026285573426321e+00,
    2.087120107912005195771599e+00,    -2.190000014016017626516941e+00,     2.166000013862398709818535e+00,    -2.190000014016018070606151e+00,     7.564813552602958068860062e-01,     2.470593759882394380156256e+00,    -2.166000013862400042086165e+00,    -2.470593759882395712423886e+00,     5.697968875026285573426321e+00,
    -5.818779942685818440395451e+00,     2.142000013708781125387759e+00,     2.166000013862399597996955e+00,    -2.190000014016018070606151e+00,     7.564813552602958068860062e-01,     2.470593759882394380156256e+00,    -2.166000013862399597996955e+00,     2.416443759665754420495887e+00,     7.567813552614961469799937e-01,
    2.000480107357442527415969e+00,     2.142000013708781125387759e+00,    -2.166000013862398709818535e+00,    -2.190000014016017182427731e+00,     5.643518874808444607538149e+00,    -2.470593759882395268334676e+00,    -2.166000013862399153907745e+00,     2.416443759665754420495887e+00,     7.567813552614959249353888e-01,
    2.087120107912005639860809e+00,    -2.190000014016018070606151e+00,    -2.166000013862400042086165e+00,    -2.190000014016017626516941e+00,     7.564813552602958068860062e-01,    -2.470593759882396156513096e+00,     2.166000013862398709818535e+00,     2.470593759882394380156256e+00,     5.697968875026285573426321e+00,
    9.906380157955267051761439e+00,    -2.190000014016018514695361e+00,     2.166000013862399153907745e+00,    -2.190000014016018514695361e+00,     5.643518874808443719359730e+00,     2.470593759882394824245466e+00,     2.166000013862399153907745e+00,     2.470593759882394824245466e+00,     5.697968875026287349783161e+00,
    2.000480107357441639237550e+00,     2.142000013708782013566179e+00,     2.166000013862400042086165e+00,    -2.190000014016018514695361e+00,     5.643518874808443719359730e+00,     2.470593759882394824245466e+00,     2.166000013862399597996955e+00,    -2.416443759665753532317467e+00,     7.567813552614954808461789e-01,
    -5.818779942685818440395451e+00,     2.142000013708782013566179e+00,    -2.166000013862399153907745e+00,    -2.190000014016017626516941e+00,     7.564813552602962509752160e-01,    -2.470593759882395712423886e+00,     2.166000013862399153907745e+00,    -2.416443759665753532317467e+00,     7.567813552614952588015740e-01,
    -5.818779942685818440395451e+00,    -2.190000014016018070606151e+00,    -2.166000013862399597996955e+00,     2.142000013708781125387759e+00,     7.564813552602953627967963e-01,     2.416443759665754420495887e+00,     2.166000013862399597996955e+00,     2.470593759882394824245466e+00,     7.567813552614961469799937e-01,
    2.000480107357442083326760e+00,    -2.190000014016018514695361e+00,     2.166000013862399597996955e+00,     2.142000013708781569476969e+00,     5.643518874808444607538149e+00,    -2.416443759665753532317467e+00,     2.166000013862400042086165e+00,     2.470593759882394824245466e+00,     7.567813552614959249353888e-01,
    9.733100156846143491407020e+00,     2.142000013708782013566179e+00,     2.166000013862399597996955e+00,     2.142000013708781569476969e+00,     5.643518874808444607538149e+00,    -2.416443759665753532317467e+00,     2.166000013862399597996955e+00,    -2.416443759665753532317467e+00,     5.589668874593003877748743e+00,
    1.913840106802882079506389e+00,     2.142000013708782013566179e+00,    -2.166000013862399597996955e+00,     2.142000013708781125387759e+00,     7.564813552602958068860062e-01,     2.416443759665753976406677e+00,     2.166000013862399153907745e+00,    -2.416443759665753532317467e+00,     5.589668874593003877748743e+00,
    2.000480107357442971505179e+00,    -2.190000014016017182427731e+00,    -2.166000013862399153907745e+00,     2.142000013708781125387759e+00,     5.643518874808444607538149e+00,     2.416443759665754420495887e+00,    -2.166000013862398709818535e+00,    -2.470593759882395268334676e+00,     7.567813552614959249353888e-01,
    -5.818779942685818440395451e+00,    -2.190000014016017626516941e+00,     2.166000013862399153907745e+00,     2.142000013708781569476969e+00,     7.564813552602962509752160e-01,    -2.416443759665753532317467e+00,    -2.166000013862399153907745e+00,    -2.470593759882395712423886e+00,     7.567813552614957028907838e-01,
    1.913840106802882079506389e+00,     2.142000013708781125387759e+00,     2.166000013862399153907745e+00,     2.142000013708781569476969e+00,     7.564813552602962509752160e-01,    -2.416443759665753532317467e+00,    -2.166000013862399597996955e+00,     2.416443759665754420495887e+00,     5.589668874593003877748743e+00,
    9.733100156846143491407020e+00,     2.142000013708781125387759e+00,    -2.166000013862399153907745e+00,     2.142000013708781125387759e+00,     5.643518874808444607538149e+00,     2.416443759665753976406677e+00,    -2.166000013862399153907745e+00,     2.416443759665754420495887e+00,     5.589668874593003877748743e+00
  };


  double *reaction = (double*)malloc(3 * nc * sizeof(double));
  double *velocity = (double*)malloc(3 * nc * sizeof(double));
  for (int i = 0; i < 3 * nc; i++)
  {
    reaction[i] = 0.0;
    velocity[i] = 0.0;
  }



  int solver_id = SICONOS_FRICTION_3D_NSGS; // 500
  double tolerance = 1e-10;
  int itermax = 500;

  info = frictionContact3D_LmgcDriver(reaction,
                                      velocity,
                                      q,
                                      mu,
                                      W,
                                      row,
                                      column,
                                      nc,
                                      nb,
                                      solver_id,
                                      tolerance,
                                      itermax,
                                      2,
                                      2, 0);
  printf("reaction:");
  printm(1, 3 * nc, reaction);

  printf("velocity:");
  printm(1, 3 * nc, velocity);

  free(reaction);
  free(velocity);

  printf("info: %d\n", info);

  return info;
}
