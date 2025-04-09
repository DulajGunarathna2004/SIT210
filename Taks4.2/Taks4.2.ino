#define button_Pin 10 // for interrupt
#define Pir_Pin 13 // for interrupt
#define LED_Pin 6

bool ledState = false; 
int switchState = 0;

void ButtonInterrupt() {
  ledState = !ledState; 
  digitalWrite(LED_Pin, ledState);
  Serial.println("ButtonInterrution");

}

void MotionInterrupt() {
  ledState = !ledState;
  digitalWrite(LED_Pin, ledState);
  Serial.println("MotionInterrution");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 

  pinMode(button_Pin, INPUT_PULLUP);
  pinMode(Pir_Pin, INPUT);
  pinMode(LED_Pin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(button_Pin), ButtonInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(Pir_Pin), MotionInterrupt, RISING);

}

void loop() {
  // put your main code here, to run repeatedly:

}