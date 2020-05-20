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

#include <float.h>
#include "Perceptron.h"
#include "utility/PerceptronNode.h"

Perceptron::Perceptron(const int inputLength, float learningRate) :
  _inputLength(inputLength+1), // inputs + bias
  _learningRate(learningRate),
  _weights(new float[_inputLength]),
  _examples(NULL)
{
  for (int i = 0; i < _inputLength; i++) {
  _weights[i] = (2.0*rand()/RAND_MAX)-1;;
  }
}

Perceptron::~Perceptron()
{
  clearAllClasses();
}

void Perceptron::addExample(const float input[], int class_)
{

  // create new node
  PerceptronNode* newNode = new PerceptronNode(input, _inputLength-1, class_);

  // add new node to examples list
  if (_examples == NULL) {
    _examples = newNode;
  } else {
    PerceptronNode* node = _examples;

    while (1) {
      PerceptronNode* next = node->next();

      if (next == NULL) {
        node->setNext(newNode);

        break;
      }

      node = next;
    }
  }
}

int Perceptron::classify(const float input[])
{
    float sum = 0;

    // Inputs
    for (int i = 0; i < _inputLength-1; i++) {
      sum += input[i] * _weights[i];
    }

    // Bias input is always 1
    sum += 1 * _weights[_inputLength-1];

    // Activation function
    if (sum > 0) return 1;
    else return 0;

}

void Perceptron::getWeights(float * weightArray,int arrayLength){
  for (int i = 0; i < arrayLength; i++) {
    weightArray[i] = _weights[i];
  }
}

float Perceptron::train()
{
  float count=0, correct=0;

  PerceptronNode* node = _examples;

  while (node != NULL) {
    count++;

    float inputs[_inputLength];

    // inputs
    for (int i = 0; i < _inputLength-1; i++) {
      inputs[i] = node->input(i);
    }

    // bias input is always 1
    inputs[_inputLength-1] = 1;

    int guess = classify(inputs);
    float error = node->class_() - guess;
    if (error==0) {correct++;}

    for (int i = 0; i < _inputLength; i++) {
      _weights[i] += _learningRate * error * inputs[i];
    }
        node = node->next();
}

  return correct/count;
}

void Perceptron::clearAllClasses()
{
  PerceptronNode* node = _examples;

  while (node != NULL) {
    PerceptronNode* next = node->next();

    delete node;

    node = next;
  }

  _examples = NULL;
}
