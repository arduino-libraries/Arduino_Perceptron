/*
  This file is part of the Arduino_KNN library.
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

#ifndef _KNN_NODE_H_
#define _KNN_NODE_H_

class PerceptronNode {
public:
  PerceptronNode(const float values[], int numValues, int class_);
  virtual ~PerceptronNode();

  int class_() const;

  int desired() const;
  float input(int i) const;

  PerceptronNode* next() const;

  void setNext(PerceptronNode* next);

private:
  float* _values;
  int _class;
  PerceptronNode* _next;
};

#endif
