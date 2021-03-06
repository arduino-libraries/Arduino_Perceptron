/*
  This file is part of the Arduino_Perceptron library.
  Copyright (c) 2020 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <math.h>
#include <stdlib.h>

#include "PerceptronNode.h"

PerceptronNode::PerceptronNode(const float values[], int numValues, int class_) :
  _class(class_),
  _next(NULL)
{
  _values = new float[numValues];

  for (int i = 0; i < numValues; i++) {
    _values[i] = values[i];
  }
}

PerceptronNode::~PerceptronNode()
{
  if (_values) {
    delete[] _values;
    _values = NULL;
  }
}


float PerceptronNode::input(int i) const
{
  return _values[i];
}

int PerceptronNode::class_() const
{
  return _class;
}

PerceptronNode* PerceptronNode::next() const
{
  return _next;
}

void PerceptronNode::setNext(PerceptronNode* next)
{
  _next = next;
}
