// Simple RGB LED sketch.
// Please note that I'm using a common anode LED.  If you have common cathode, you
// should be able to modify this sketch to fit your needs.

#define SEND_OK 1

// The LED pins
int r_led = 9;
int g_led = 10;
int b_led = 11;
int r, g, b;    // Intensity
int baudrate = 9600;
byte buffer[3];


void setup()  {
  Serial.begin(baudrate);
  
  // Set the pin modes
  pinMode(r_led, OUTPUT);
  pinMode(g_led, OUTPUT);
  pinMode(b_led, OUTPUT);
  r = g = b = 255;
} 


void loop()  {
  // Set the colors and wait a bit
  analogWrite(r_led, r);
  analogWrite(g_led, g);
  analogWrite(b_led, b);
  delay(1);                            
}


void serialEvent() {
  if (Serial.available() >= 3) {
    // I do the "255 -" because of the common anode RGB LED
    r = 255 - Serial.read();
    g = 255 - Serial.read();
    b = 255 - Serial.read();
  }
}
