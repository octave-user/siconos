/* Siconos is a program dedicated to modeling, simulation and control
 * of non smooth dynamical systems.
 *
 * Copyright 2020 INRIA.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include "SiconosConfig.h"

#include "SimpleMatrix.hpp"
#include "BlockMatrixIterators.hpp"
#include "BlockMatrix.hpp"

#include "SiconosAlgebra.hpp"

using namespace Siconos;


// =================================================
//                CONSTRUCTORS
// =================================================

using  std::cout;
using std::endl;

double& SimpleMatrix::operator()(unsigned int row, unsigned int col)
{
  assert((row < size(0) && col < size(1)) && "SimpleMatrix:operator(): Index out of range");

  if(_num == DENSE)
    return (*mat.Dense)(row, col);
  else if(_num == TRIANGULAR)
    return (*mat.Triang)(row, col);
  else if(_num == SYMMETRIC)
    return (*mat.Sym)(row, col);
  else if(_num == SPARSE)
  {
    double *d = (*mat.Sparse).find_element(row, col);
    if(d == nullptr)
      SiconosMatrixException::selfThrow("SimpleMatrix:operator(): Index out of range");
    double & ref = *d;
    return ref;
  }
  else if(_num == SPARSE_COORDINATE)
  {
    double *d = (*mat.SparseCoordinate).find_element(row, col);
    if(d == nullptr)
      SiconosMatrixException::selfThrow("SimpleMatrix:operator(): Index out of range");
    double & ref = *d;
    return ref;
  }
  else if(_num == BANDED)
    return (*mat.Banded)(row, col);
  else if(_num == ZERO)
    return const_cast<double&>((*mat.Zero)(row, col));
  else if(_num == IDENTITY)
    return const_cast<double&>((*mat.Identity)(row, col));
  else
  {
    SiconosMatrixException::selfThrow("SimpleMatrix::op () (unsigned int row, unsigned int col): invalid type of matrix");
    double *d = nullptr;
    double & ref = *d;
    return ref;
  }
}

double SimpleMatrix::operator()(unsigned int row, unsigned int col) const
{
  assert((row < size(0) && col < size(1)) && "SimpleMatrix:operator(): Index out of range");

  if(_num == DENSE)
    return (*mat.Dense)(row, col);
  else if(_num == TRIANGULAR)
    return (*mat.Triang)(row, col);
  else if(_num == SYMMETRIC)
    return (*mat.Sym)(row, col);
  else if(_num == SPARSE)
    return (*mat.Sparse)(row, col);
  else if(_num == SPARSE_COORDINATE)
    return (*mat.SparseCoordinate)(row, col);
  else if(_num == BANDED)
    return (*mat.Banded)(row, col);
  else if(_num == ZERO)
    return 0.0;
  else if(_num == IDENTITY)
    return (row == col);
  else
  {
    SiconosMatrixException::selfThrow("SimpleMatrix::op () (unsigned int row, unsigned int col): invalid type of matrix");
    return 0.0;
  }
}

//=============
// Assignment
//=============

SimpleMatrix& SimpleMatrix::operator = (const SiconosMatrix& m)
{

  if(&m == this) return *this;  // auto-assignment.

  unsigned int numM = m.num();

  if(size(0) != m.size(0) || size(1) != m.size(1))
  {
    resize(m.size(0), m.size(1));
  }
  // SiconosMatrixException::selfThrow("SimpleMatrix::operator = failed. Inconsistent sizes.");

  if(numM == 6)  // m = zero matrix
  {
    zero();
    return *this;
  }

  if(numM == 7)  // m = identity matrix
  {
    eye();
    return *this;
  }

  if(numM == 0)  // if m is a BlockMatrix
  {
    const BlockMatrix& mB = static_cast<const BlockMatrix&>(m);
    ConstBlocksIterator1 it;
    ConstBlocksIterator2 it2;
    unsigned int posRow = 0;
    unsigned int posCol = 0;

    for(it = mB._mat->begin1(); it != mB._mat->end1(); ++it)
    {
      for(it2 = it.begin(); it2 != it.end(); ++it2)
      {
        setBlock(posRow, posCol, **it2);
        posCol += (*it2)->size(1);
      }
      posRow += (*it)->size(0);
      posCol = 0;
    }
  }
  else
  {
    switch(_num)
    {
    case DENSE:
      switch(numM)
      {
      case DENSE:
        noalias(*(mat.Dense)) = *m.dense();
        break;
      case TRIANGULAR:
        noalias(*(mat.Dense)) = *m.triang();
        break;
      case SYMMETRIC:
        noalias(*(mat.Dense)) = *m.sym();
        break;
      case SPARSE:
        noalias(*(mat.Dense)) = *m.sparse();
        break;
      case SPARSE_COORDINATE:
        noalias(*(mat.Dense)) = *m.sparseCoordinate();
        break;
      case BANDED:
        noalias(*(mat.Dense)) = *m.banded();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::op= (const SimpleMatrix): invalid type of matrix");
        break;
      }
      break;
    case TRIANGULAR:
      switch(numM)
      {
      case TRIANGULAR:
        noalias(*(mat.Triang)) = *m.triang();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::assignment of a bad type of matrix into a triangular one.");
        break;
      }
      break;
    case SYMMETRIC:
      if(numM == 3)
        noalias(*(mat.Sym)) = *m.sym();
      else
        SiconosMatrixException::selfThrow("SimpleMatrix::bad assignment of matrix (symmetric one = dense or ...)");
      break;
    case SPARSE:
      switch(numM)
      {
      case DENSE:
      noalias(*(mat.Sparse)) = *m.dense();
      break;
      case TRIANGULAR:
        noalias(*(mat.Sparse)) = *m.triang();
        break;
      case SYMMETRIC:
        noalias(*(mat.Sparse)) = *m.sym();
        break;
      case SPARSE:
        noalias(*(mat.Sparse)) = *m.sparse();
        break;
      case SPARSE_COORDINATE:
        noalias(*(mat.Sparse)) = *m.sparseCoordinate();
        break;
      case BANDED:
        noalias(*(mat.Sparse)) = *m.banded();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::op= (const SimpleMatrix): invalid type of matrix");
        break;
      }
      break;
    case SPARSE_COORDINATE:
      switch(numM)
      {
      case DENSE:
        noalias(*(mat.SparseCoordinate)) = *m.dense();
        break;
      case TRIANGULAR:
        noalias(*(mat.SparseCoordinate)) = *m.triang();
        break;
      case SYMMETRIC:
        noalias(*(mat.SparseCoordinate)) = *m.sym();
        break;
      case SPARSE:
        noalias(*(mat.SparseCoordinate)) = *m.sparse();
        break;
      case SPARSE_COORDINATE:
        noalias(*(mat.SparseCoordinate)) = *m.sparseCoordinate();
        break;
      case BANDED:
        noalias(*(mat.SparseCoordinate)) = *m.banded();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::op= (const SimpleMatrix): invalid type of matrix");
        break;
      }
      break;
    case BANDED:
      switch(numM)
      {
      case BANDED:
        noalias(*(mat.Banded)) = *m.banded();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::op= (const SimpleMatrix): invalid type of matrix");
        break;
      }
      break;
    default:
      SiconosMatrixException::selfThrow("SimpleMatrix::op= (const SimpleMatrix): invalid type of matrix");
      break;
    }
    resetLU();
  }
  return *this;
}

SimpleMatrix& SimpleMatrix::operator = (const SimpleMatrix& m)
{

  if(&m == this) return *this;  // auto-assignment.

  unsigned int numM = m.num();

  if(size(0) != m.size(0) || size(1) != m.size(1))
    resize(m.size(0), m.size(1));

  //    SiconosMatrixException::selfThrow("SimpleMatrix::operator = failed. Inconsistent sizes.");

  if(numM == 6)  // m = zero matrix
  {
    zero();
    return *this;
  }
  else if(numM == 7)  // m = identity matrix
  {
    eye();
    return *this;
  }

  switch(_num)
  {
  case DENSE:
    switch(numM)
    {
    case DENSE:
      noalias(*(mat.Dense)) = *m.dense();
      break;
    case TRIANGULAR:
      noalias(*(mat.Dense)) = *m.triang();
      break;
    case SYMMETRIC:
      noalias(*(mat.Dense)) = *m.sym();
      break;
    case SPARSE:
      noalias(*(mat.Dense)) = *m.sparse();
      break;
    case SPARSE_COORDINATE:
      noalias(*(mat.Dense)) = *m.sparseCoordinate();
      break;
    case BANDED:
      noalias(*(mat.Dense)) = *m.banded();
      break;
    default:
      SiconosMatrixException::selfThrow("SimpleMatrix::op= (const SimpleMatrix): invalid type of matrix");
      break;
    }
    break;
  case TRIANGULAR:
    switch(numM)
    {
    case TRIANGULAR:
      noalias(*(mat.Triang)) = *m.triang();
      break;
    default:
      SiconosMatrixException::selfThrow("SimpleMatrix::assignment of a bad type of matrix into a triangular one.");
      break;
    }
    break;
  case SYMMETRIC:
    if(numM == 3)
      noalias(*(mat.Sym)) = *m.sym();
    else
      SiconosMatrixException::selfThrow("SimpleMatrix::bad assignment of matrix (symmetric one = dense or ...)");
    break;
  case SPARSE:
    switch(numM)
    {
    case DENSE:
      noalias(*(mat.Sparse)) = *m.dense();
      break;
    case TRIANGULAR:
      noalias(*(mat.Sparse)) = *m.triang();
      break;
    case SYMMETRIC:
      noalias(*(mat.Sparse)) = *m.sym();
      break;
    case SPARSE:
      noalias(*(mat.Sparse)) = *m.sparse();
      break;
    case BANDED:
      noalias(*(mat.Sparse)) = *m.banded();
      break;
    default:
      SiconosMatrixException::selfThrow("SimpleMatrix::op= (const SimpleMatrix): invalid type of matrix");
      break;
    }
    break;
  case SPARSE_COORDINATE:
    switch(numM)
    {
    case DENSE:
      noalias(*(mat.SparseCoordinate)) = *m.dense();
      break;
    case TRIANGULAR:
      noalias(*(mat.SparseCoordinate)) = *m.triang();
      break;
    case SYMMETRIC:
      noalias(*(mat.SparseCoordinate)) = *m.sym();
      break;
    case SPARSE:
      noalias(*(mat.SparseCoordinate)) = *m.sparse();
      break;
    case SPARSE_COORDINATE:
      noalias(*(mat.SparseCoordinate)) = *m.sparseCoordinate();
      break;
    case BANDED:
      noalias(*(mat.SparseCoordinate)) = *m.banded();
      break;
    default:
      SiconosMatrixException::selfThrow("SimpleMatrix::op= (const SimpleMatrix): invalid type of matrix");
      break;
    }
    break;

  case BANDED:
    switch(numM)
    {
    case BANDED:
      noalias(*(mat.Banded)) = *m.banded();
      break;
    default:
      SiconosMatrixException::selfThrow("SimpleMatrix::op= (const SimpleMatrix): invalid type of matrix");
      break;
    }
    break;
  default:
    SiconosMatrixException::selfThrow("SimpleMatrix::op= (const SimpleMatrix): invalid type of matrix");
    break;
  }
  resetLU();
  return *this;
}

SimpleMatrix& SimpleMatrix::operator = (const DenseMat& m)
{
  if(_num != 1)
    SiconosMatrixException::selfThrow("SimpleMatrix::operator = DenseMat : forbidden: the current matrix is not dense.");

  if(size(0) != m.size1() || size(1) != m.size2())
    SiconosMatrixException::selfThrow("SimpleMatrix::operator = DenseMat failed. Inconsistent sizes.");

  noalias(*(mat.Dense)) = m;

  resetLU();
  return *this;
}

//=================================
// Op. and assignment (+=, -= ... )
//=================================

SimpleMatrix& SimpleMatrix::operator +=(const SiconosMatrix& m)
{

  unsigned int numM = m.num();
  if(numM == 6)  // m = 0
    return *this;

  if(&m == this)  // auto-assignment
  {
    switch(_num)
    {
    case DENSE:
      *mat.Dense += *mat.Dense;
      break;
    case TRIANGULAR:
      *mat.Triang += *mat.Triang;
      break;
    case SYMMETRIC:
      *mat.Sym += *mat.Sym;
      break;
    case SPARSE:
      *mat.Sparse += *mat.Sparse;
      break;
    case SPARSE_COORDINATE:
      *mat.SparseCoordinate += *mat.SparseCoordinate;
      break;
    case BANDED:
      *mat.Banded += *mat.Banded;
      break;
    default:
      SiconosMatrixException::selfThrow("SimpleMatrix op+= invalid type of matrix");
    }
    resetLU();
    return *this;
  }

  if(size(0) != m.size(0) || size(1) != m.size(1))
    resize(m.size(0), m.size(1));

  //  SiconosMatrixException::selfThrow("SimpleMatrix op+= inconsistent sizes.");

  if(numM == 0)  // m is a BlockMatrix
  {
    const BlockMatrix& mB = static_cast<const BlockMatrix&>(m);
    ConstBlocksIterator1 it1;
    ConstBlocksIterator2 it2;
    unsigned int posRow = 0;
    unsigned int posCol = 0;
    // We scan all the blocks of m ...
    for(it1 = mB._mat->begin1(); it1 != mB._mat->end1(); ++it1)
    {
      for(it2 = it1.begin(); it2 != it1.end(); ++it2)
      {
        addBlock(posRow, posCol, **it2); // Each block of m is added into this.
        posCol += (*it2)->size(1);
      }
      posRow += (*it1)->size(0);
      posCol = 0;
    }
  }
  else // if m is a SimpleMatrix
  {
    switch(_num)
    {
    case DENSE:
      switch(numM)
      {
      case DENSE:
        noalias(*(mat.Dense)) += *m.dense();
        break;
      case TRIANGULAR:
        noalias(*(mat.Dense)) += *m.triang();
        break;
      case SYMMETRIC:
        noalias(*(mat.Dense)) += *m.sym();
        break;
      case SPARSE:
        noalias(*(mat.Dense)) += *m.sparse();
        break;
      case SPARSE_COORDINATE:
        noalias(*(mat.Dense)) += *m.sparseCoordinate();
        break;
      case BANDED:
        noalias(*(mat.Dense)) += *m.banded();
        break;
      case IDENTITY:
        noalias(*(mat.Dense)) += *m.identity();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::op+= (const SimpleMatrix): invalid type of matrix");
        break;
      }
      break;
    case TRIANGULAR:
      switch(numM)
      {
      case TRIANGULAR:
        noalias(*(mat.Triang)) += *m.triang();
        break;
      case IDENTITY:
        noalias(*(mat.Triang)) += *m.identity();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::op+= of a bad type of matrix into a triangular one.");
        break;
      }
      break;
    case SYMMETRIC:
      if(numM == 3)
        noalias(*(mat.Sym)) += *m.sym();
      else if(numM == 7)
        noalias(*(mat.Sym)) += *m.identity();
      else
        SiconosMatrixException::selfThrow("SimpleMatrix::op+= bad assignment of matrix (symmetric one = dense or ...)");
      break;
    case SPARSE:
      switch(numM)
      {
      case TRIANGULAR:
        noalias(*(mat.Sparse)) += *m.triang();
        break;
      case SYMMETRIC:
        noalias(*(mat.Sparse)) += *m.sym();
        break;
      case SPARSE:
        noalias(*(mat.Sparse)) += *m.sparse();
        break;
      case SPARSE_COORDINATE:
        noalias(*(mat.Sparse)) += *m.sparseCoordinate();
        break;
      case BANDED:
        noalias(*(mat.Sparse)) += *m.banded();
        break;
      case IDENTITY:
        noalias(*(mat.Sparse)) += *m.identity();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::op+=: invalid type of matrix");
        break;
      }
      break;
    case SPARSE_COORDINATE:
      switch(numM)
      {
      case TRIANGULAR:
        noalias(*(mat.SparseCoordinate)) += *m.triang();
        break;
      case SYMMETRIC:
        noalias(*(mat.SparseCoordinate)) += *m.sym();
        break;
      case SPARSE:
        noalias(*(mat.SparseCoordinate)) += *m.sparse();
        break;
      case SPARSE_COORDINATE:
        noalias(*(mat.SparseCoordinate)) += *m.sparseCoordinate();
        break;
      case BANDED:
        noalias(*(mat.SparseCoordinate)) += *m.banded();
        break;
      case IDENTITY:
        noalias(*(mat.SparseCoordinate)) += *m.identity();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::op+=: invalid type of matrix");
        break;
      }
      break;
    case BANDED:
      switch(numM)
      {
      case BANDED:
        noalias(*(mat.Banded)) += *m.banded();
        break;
      case IDENTITY:
        noalias(*(mat.Banded)) += *m.identity();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::op+= : invalid type of matrix");
        break;
      }
      break;
    default:
      SiconosMatrixException::selfThrow("SimpleMatrix::op+= : invalid type of matrix");
      break;
    }
    resetLU();
  }
  return *this;
}

SimpleMatrix& SimpleMatrix::operator -= (const SiconosMatrix& m)
{

  unsigned int numM = m.num();
  if(numM == 6)  // m = 0
    return *this;

  if(&m == this)  // auto-assignment
  {
    switch(_num)
    {
    case DENSE:
      *mat.Dense -= *mat.Dense;
      break;
    case TRIANGULAR:
      *mat.Triang -= *mat.Triang;
      break;
    case SYMMETRIC:
      *mat.Sym -= *mat.Sym;
      break;
    case SPARSE:
      *mat.Sparse -= *mat.Sparse;
      break;
    case SPARSE_COORDINATE:
      *mat.SparseCoordinate -= *mat.SparseCoordinate;
      break;
    case BANDED:
      *mat.Banded -= *mat.Banded;
      break;
    default:
      SiconosMatrixException::selfThrow("SimpleMatrix op-= invalid type of matrix");
    }
    resetLU();
    return *this;
  }
  if(size(0) != m.size(0) || size(1) != m.size(1))
    SiconosMatrixException::selfThrow("SimpleMatrix op-= inconsistent sizes.");

  if(numM == 0)  // m is a BlockMatrix
  {
    const BlockMatrix& mB = static_cast<const BlockMatrix&>(m);
    ConstBlocksIterator1 it1;
    ConstBlocksIterator2 it2;
    unsigned int posRow = 0;
    unsigned int posCol = 0;
    // We scan all the blocks of m ...
    for(it1 = mB._mat->begin1(); it1 != mB._mat->end1(); ++it1)
    {
      for(it2 = it1.begin(); it2 != it1.end(); ++it2)
      {
        subBlock(posRow, posCol, **it2); // Each block of m is added into this.
        posCol += (*it2)->size(1);
      }
      posRow += (*it1)->size(0);
      posCol = 0;
    }
  }
  else // if m is a SimpleMatrix
  {
    switch(_num)
    {
    case DENSE:
      switch(numM)
      {
      case DENSE:
        noalias(*(mat.Dense)) -= *m.dense();
        break;
      case TRIANGULAR:
        noalias(*(mat.Dense)) -= *m.triang();
        break;
      case SYMMETRIC:
        noalias(*(mat.Dense)) -= *m.sym();
        break;
      case SPARSE:
        noalias(*(mat.Dense)) -= *m.sparse();
        break;
      case SPARSE_COORDINATE:
        noalias(*(mat.Dense)) -= *m.sparseCoordinate();
        break;
      case BANDED:
        noalias(*(mat.Dense)) -= *m.banded();
        break;
      case IDENTITY:
        noalias(*(mat.Dense)) -= *m.identity();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::op-= (const SimpleMatrix): invalid type of matrix");
        break;
      }
      break;
    case TRIANGULAR:
      switch(numM)
      {
      case TRIANGULAR:
        noalias(*(mat.Triang)) -= *m.triang();
        break;
      case IDENTITY:
        noalias(*(mat.Triang)) -= *m.identity();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::op-= of a bad type of matrix into a triangular one.");
        break;
      }
      break;
    case SYMMETRIC:
      if(numM == 3)
        noalias(*(mat.Sym)) -= *m.sym();
      else if(numM == 7)
        noalias(*(mat.Sym)) -= *m.identity();
      else
        SiconosMatrixException::selfThrow("SimpleMatrix::op-= bad assignment of matrix (symmetric one = dense or ...)");
      break;
    case SPARSE:
      switch(numM)
      {
      case TRIANGULAR:
        noalias(*(mat.Sparse)) -= *m.triang();
        break;
      case SYMMETRIC:
        noalias(*(mat.Sparse)) -= *m.sym();
        break;
      case SPARSE:
        noalias(*(mat.Sparse)) -= *m.sparse();
        break;
      case SPARSE_COORDINATE:
        noalias(*(mat.Sparse)) -= *m.sparse();
        break;
      case BANDED:
        noalias(*(mat.Sparse)) -= *m.banded();
        break;
      case IDENTITY:
        noalias(*(mat.Sparse)) -= *m.identity();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::op-=: invalid type of matrix");
        break;
      }
      break;
    case SPARSE_COORDINATE:
      switch(numM)
      {
      case TRIANGULAR:
        noalias(*(mat.SparseCoordinate)) -= *m.triang();
        break;
      case SYMMETRIC:
        noalias(*(mat.SparseCoordinate)) -= *m.sym();
        break;
      case SPARSE:
        noalias(*(mat.SparseCoordinate)) -= *m.sparse();
        break;
      case SPARSE_COORDINATE:
        noalias(*(mat.SparseCoordinate)) -= *m.sparseCoordinate();
        break;
      case BANDED:
        noalias(*(mat.SparseCoordinate)) -= *m.banded();
        break;
      case IDENTITY:
        noalias(*(mat.SparseCoordinate)) -= *m.identity();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::op-=: invalid type of matrix");
        break;
      }
      break;

    case BANDED:
      switch(numM)
      {
      case BANDED:
        noalias(*(mat.Banded)) -= *m.banded();
        break;
      case IDENTITY:
        noalias(*(mat.Banded)) -= *m.identity();
        break;
      default:
        SiconosMatrixException::selfThrow("SimpleMatrix::op-= : invalid type of matrix");
        break;
      }
      break;
    default:
      SiconosMatrixException::selfThrow("SimpleMatrix::op-= : invalid type of matrix");
      break;
    }
    resetLU();
  }
  return *this;

}
