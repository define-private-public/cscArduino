// Variables we need for the application.
int left_paddle = A0;
int right_paddle = A1;
int cur_l, cur_r, last_l, last_r;
int delta = 4;
byte buf[4];

int baudrate = 9600;

const int REQ = 2;


void setup() {
  pinMode(left_paddle, INPUT);
  pinMode(right_paddle, INPUT);
  
  cur_l = last_l = cur_r = last_r = 0;
  
  Serial.begin(baudrate);
}


void loop() {
  if (buf[0] == REQ) {
    cur_l = analogRead(left_paddle);
    cur_r = analogRead(right_paddle);
  
    buf[0] = (byte)(cur_l >> 8);
    buf[1] = (byte)cur_l;
    buf[2] = (byte)(cur_r >> 8);
    buf[3] = (byte)cur_r;
    
    Serial.write(buf, 4);
  
    buf[0] = 0;
  }  
  
  delay(10);  // Small nap (10 millisecond)
}

void serialEvent() {
  if (Serial.available() > 0) {
    buf[0] = Serial.read();
  } 
}
