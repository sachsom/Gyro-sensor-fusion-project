int counter =0;

void setup() {
  Serial.begin(38400); 
  delay(50);
}

void loop() {
  counter++;
  Serial.print("Arduino counter: ");
  Serial.println(counter);
   // wait half a sec
}
