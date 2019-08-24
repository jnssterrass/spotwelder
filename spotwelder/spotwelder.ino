// These constants won't change. They're used to give names to the pins used:
const int potentiometerPin = A0;  // Analog input pin that the potentiometer is attached to
const int buttonPin = 2;
const int pedalPin = 3;
const int weldPin = 4;

/*STATES*/
const int NOT_WELD = 0;
const int WELD = 1;

int sensorValue = 0;        // value read from the pot
int weldPulseDuration = 0;        // value output to the PWM (analog out)

int STATE = NOT_WELD;

void readInterruptFunc() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 2000) {
    if (STATE == NOT_WELD) STATE = WELD;
  }
  last_interrupt_time = interrupt_time;
}

void readPot() {
  // read the analog in value:
  sensorValue = analogRead(potentiometerPin);
  // map it to the range of the analog out:
  weldPulseDuration = map(sensorValue, 0, 1023, 0, 50);
}

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(weldPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), readInterruptFunc, FALLING);
  attachInterrupt(digitalPinToInterrupt(pedalPin), readInterruptFunc, FALLING);
}

void loop() {

  readPot();

  if (STATE == WELD) {
    Serial.println("WELDING...");
    digitalWrite(weldPin, HIGH);
    delay(weldPulseDuration);
    digitalWrite(weldPin, LOW);
    STATE = NOT_WELD;
    delay(2000);
  }

  // print the results to the Serial Monitor:
  /*
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.print(weldPulseDuration);
  Serial.print("\t STATE = ");
  Serial.println(STATE);
  */
  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
}
