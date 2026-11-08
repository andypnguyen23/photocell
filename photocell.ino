// Light Meter
// A photoresistor controls how many of 8 LEDs are lit: the brighter the
// room, the more LEDs turn on. Uses an Arduino UNO R3 and a 74HC595
// shift register. Based on the Elegoo UNO R3 starter kit lesson (www.elegoo.com)
//
// Wiring:
//   Photoresistor and a 1k ohm resistor form a voltage divider, with the
//   point between them connected to A0
//   74HC595 drives the 8 LEDs, each through its own 220 ohm resistor

int lightPin = 0;    // photoresistor voltage divider connected to analog pin A0
int latchPin = 11;   // Arduino pin 11 -> 74HC595 ST_CP (latch): copies data to the outputs
int clockPin = 9;    // Arduino pin 9  -> 74HC595 SH_CP (clock): shift clock
int dataPin = 12;    // Arduino pin 12 -> 74HC595 DS (data): serial data in

int leds = 0;        // one bit per LED: 1 = on, 0 = off
void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
}

// Sends the current LED pattern to the 74HC595
void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);                       // start sending
   shiftOut(dataPin, clockPin, LSBFIRST, leds);       // send the 8 bits, lowest bit first
   digitalWrite(latchPin, HIGH);                      // done: LEDs update
}

int darkValue = 150;     // reading with the sensor covered, adjust these two to your liking
int brightValue = 400;   // reading with a flashlight on it

void loop()
{
  int reading = analogRead(lightPin);
  Serial.println(reading);

   // Convert the reading to a number of LEDs (0-8) and keep it in range
  int numLEDSLit = map(reading, darkValue, brightValue, 0, 8);
  numLEDSLit = constrain(numLEDSLit, 0, 8);

  // Build the LED pattern: set one bit for each LED that should be on.
  // Example: 3 LEDs lit = 00000111
  leds = 0;
  for (int i = 0; i < numLEDSLit; i++)
  {
    leds = leds + (1 << i);   // shifts a 1 left by i places, then adds it to turn on bit i's LED
  }
  updateShiftRegister();
}
