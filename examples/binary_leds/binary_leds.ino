// This little sketch will read in a byte from the serial line, and
// then turn on/off some LEDs that correspond to the byte's base 2
// representation.

int pin[] = {2, 3, 4, 5, 8, 9, 10, 11};
int baudrate = 9600;
int num = 256;


void setup() {
  Serial.begin(baudrate);
  
  // Setup the pins
  for (int i = 0; i < 8; i++) {
    pinMode(pin[i], OUTPUT);
    digitalWrite(pin[i], LOW);
  }
}


void loop() {
  // get the data
  if (num < 256) {
      
      // set the LEDs to high via some boolean algebra
      for (int i = 0, j = 1; i < 8; i++, j <<= 1) {
        if ((num & j) == j)
          digitalWrite(pin[i], HIGH);
        else
          digitalWrite(pin[i], LOW);
      }
      
      Serial.write(num);  // Send it back
      num = 256;          // So we wait for another Serial event
  }
  
  delay(10);
}


void serialEvent() {
  if (Serial.available() != 0) {
    num = Serial.read();
  }
}
