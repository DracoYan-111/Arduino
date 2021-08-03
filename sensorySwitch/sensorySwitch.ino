int irSensorPin = D8 ;
bool irSensorOutput;
int count = 0;
bool switchs ;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(irSensorPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  irSensorOutput = digitalRead(irSensorPin);
  if (irSensorOutput == HIGH) {
    digitalWrite(LED_BUILTIN, LOW);
    count = 0;
    switchs = true;
  } else {
    if (count > 2000) {
      digitalWrite(LED_BUILTIN, HIGH);
      count = 0;
      switchs = false;
    } else if (switchs) {
      digitalWrite(LED_BUILTIN, LOW);
    }
    count++;
    Serial.println(count);
  }
  delay(100);
}
