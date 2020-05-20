/*

Perceptron example

Hardware: Arduino Nano BLE Sense

Usage: Follow the prompts in the Serial Monitor and show two objects of different colors to the color sensor onboard the Arduino.
The sketch will train a perceptron using these examples, and then classify objects you show it in the future.

The color sensor works better in a well lit area

*/

#include <Arduino_APDS9960.h>
#include <Arduino_Perceptron.h>

const int   NUM_INPUTS          = 3;     // Red, green and blue
const int   CLASSES             = 2;     // The perceptron only has 2 possible classes
const int   EXAMPLES            = 30;    // Number of samples for each object
const int   NUM_EPOCHS          = 100;   // Maximum training iterations
const float LEARNING_RATE       = 0.001; // Perceptron learning rate
const int   THRESHOLD           = 5;     // Color sensor light threshold

float       input[NUM_INPUTS];
float       weights[NUM_INPUTS+1];       // Weights for each color input + the bias
String      label[CLASSES] = {"object A", "object B"};

Perceptron  perceptron(NUM_INPUTS, LEARNING_RATE);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!APDS.begin()) {
    Serial.println("Failed to initialize APDS color sensor!");
    while (1);
  }

  Serial.println("Arduino perceptron");

  // Get example color data
  // ----------------------
  for (int thisClass = 0; thisClass <= 1; thisClass++) {

    Serial.print("Show me ");
    Serial.println(label[thisClass]);

    // Wait for the object to move away again
    while (!APDS.proximityAvailable() || APDS.readProximity() == 0) {}

    // Get n color samples from the object
    for (int n = 0; n < EXAMPLES; n++) {
      readColor(input); // red, green and blue
      perceptron.addExample(input, thisClass); // Add to training set
    }
  }

  // Train the perceptron
  // --------------------
  Serial.println("\nTraining perceptron");
  int epoch = 0;
  float accuracy = 0;

  Serial.print("Start weights:");
  printWeights();

  for (int epoch = 0; epoch < NUM_EPOCHS; epoch++) {
    accuracy = perceptron.train();
    Serial.print("Accuracy: ");
    Serial.println(accuracy);
  }

  Serial.print("End weights:");
  printWeights();

  Serial.println("Training complete\n");
}


// Classify objects shown
// ----------------------
void loop() {

  int output;

  // Wait for the object to move away again
  while (!APDS.proximityAvailable() || APDS.readProximity() == 0) {}

  Serial.println("Let me guess your object");

  readColor(input);

  output = perceptron.classify(input);

  Serial.print("You showed me ");
  Serial.println(label[output]);
}


void readColor(float color[]) {
  int red, green, blue;

  // Loop until we have a color sample bright enough
  while (red+green+blue < THRESHOLD) {
    // Sample if color sensor is available and the object is close
    if (APDS.colorAvailable() && APDS.proximityAvailable() && APDS.readProximity() == 0) {
      APDS.readColor(red, green, blue);
    }
  }

  // Put color sample data in input array
  color[0] = (float)red;
  color[1] = (float)green;
  color[2] = (float)blue;

  // Print the red, green and blue values
  printArray(color,NUM_INPUTS);
}


void printWeights() {
  perceptron.getWeights(weights,NUM_INPUTS+1); // input weights plus the bias
  printArray(weights,NUM_INPUTS+1);
}


void printArray(float myArray[], int length) {
  Serial.print(myArray[0]);
  for (int n = 1; n < length; n++) {
    Serial.print(",");
    Serial.print(myArray[n]);
  }
  Serial.println();
}
