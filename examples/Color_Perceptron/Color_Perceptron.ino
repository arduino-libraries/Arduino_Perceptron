/*

  Perceptron example

  Hardware: Arduino Nano BLE Sense

  Usage: Follow the prompts in the Serial Monitor and show two objects of different colors to the color sensor onboard the Arduino.
  The sketch will train a perceptron using these examples, and then classify objects you show it in the future.

*/

#include <Arduino_APDS9960.h>
#include <Arduino_Perceptron.h>

const int   NUM_INPUTS          = 3;     // Classifier input is color sensor data; red, green and blue levels
const int   CLASSES             = 2;     // The perceptron only has 2 possible classes (-1 and 1)
const int   EXAMPLES            = 50;   // Number of samples for each object
const int   MAX_EPOCHS          = 250;   // Maximum training iterations
const float LEARNING_RATE       = 0.001; // Perceptron learning rate
const int   THRESHOLD           = 5;     // Color sensor light threshold

float       color[NUM_INPUTS];
String      label[CLASSES] = {"object A", "object B"};

Perceptron  perceptron(NUM_INPUTS, LEARNING_RATE);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!APDS.begin()) {
    Serial.println("Failled to initialized APDS!");
    while (1);
  }

  Serial.println("Arduino perceptron");

  // Get examples for the two possible outputs, -1 and 1
  // ---------------------------------------------------
  for (int output = -1; output < 2; output += 2) {

    Serial.print("Show me ");
    printLabel(output);

    // Wait for the object to move away again
    while (!APDS.proximityAvailable() || APDS.readProximity() == 0) {}

    for (int n = 0; n < EXAMPLES; n++) {

      // Sample object color - perceptron input
      readColor(color);

      // Add example color to the perceptron training set
      perceptron.addExample(color, output);
    }
  }

  // Train the perceptron
  // --------------------
  Serial.println("\nTraining perceptron");
  int epoch = 0;
  float accuracy = 0;

  while (epoch < MAX_EPOCHS && accuracy < 0.99) {
    accuracy = perceptron.train();
    epoch++;

    Serial.print("Accuracy: ");
    Serial.println(accuracy);
  }

  Serial.println("Training complete\n");
}


void loop() {

  int output;

  // Wait for the object to move away again
  while (!APDS.proximityAvailable() || APDS.readProximity() == 0) {}

  Serial.println("Let me guess your object");

  // Wait for an object then read its color
  readColor(color);

  output = perceptron.classify(color);

  Serial.print("You showed me ");
  printLabel(output);
}

void printLabel(int output) {
  if (output ==  -1) {
    Serial.println(label[0]);
  } else {
    Serial.println(label[1]);
  }
  Serial.println();
}

void readColor(float color[]) {
  int red, green, blue, proximity, colorTotal = 0;

  // Wait until we have a color bright enough
  while (colorTotal < THRESHOLD) {

    // Sample if color is available and object is close
    if (APDS.colorAvailable() && APDS.proximityAvailable() && APDS.readProximity() == 0) {

      // Read color and proximity
      APDS.readColor(red, green, blue);
      colorTotal = (red + green + blue);
    }
  }

  // Normalise the color sample data and put it in the classifier input array
  color[0] = (float)red / colorTotal;
  color[1] = (float)green / colorTotal;
  color[2] = (float)blue / colorTotal;

  // Print the red, green and blue values
  Serial.print(color[0]);
  Serial.print(",");
  Serial.print(color[1]);
  Serial.print(",");
  Serial.println(color[2]);
}
