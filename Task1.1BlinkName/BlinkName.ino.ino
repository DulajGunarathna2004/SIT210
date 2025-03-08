#include <Arduino.h>
#include <ctype.h>


const char* Alphabet[26] = {
  "..-",".-.","--.","-.-","--..",".---","-.-.","--","..","-...","----",
  "....","-.","...",".--.","-..-","...--","-....-","-...-","---.","--...",
  "...---","-","-.-.","..-.-","-.-.-." 
};

const char* firstName = "Dulaj";

void blinkingLED(const char* morse){
  while(*morse){
    if(*morse == '.'){
      digitalWrite (LED_BUILTIN, HIGH);
      delay(150);
    }else if ( *morse == '-' ){
      digitalWrite(LED_BUILTIN , HIGH);
      delay(900);
    }
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    morse++;
  }
  delay(1000);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  for(int i=0; firstName[i] != '\0' ; i++){
    int index = toupper(firstName[i]) - 'A';
    if (index >= 0 && index < 26){
      blinkingLED(Alphabet[index]);
    } 
  }
  delay(5000);
}
