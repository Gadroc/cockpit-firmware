
void setup() {
  // put your setup code here, to run once:
   Serial.begin(250000);
  Serial1.begin(250000);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  Serial1.println("Hello\r\n");
}

int data = -1;
void loop() {
  Serial1.println("Hello\r\n");
//  data = Serial.read();
//  if (data > -1) {
//    Serial1.write(data);
//  }
}
