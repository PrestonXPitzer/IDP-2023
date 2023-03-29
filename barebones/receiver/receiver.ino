#define THRESHOLD 700
#define PIN_NUM A5

int read_val = -1;
char test = 0;
char previous = 1, current = 1;

char pseudoDigiRead(int pin) {
  read_val = analogRead(PIN_NUM);
  if (read_val >= THRESHOLD) {
    return 1;
  } else {
    return 0;
  }
}

void setup() {
  pinMode(PIN_NUM, INPUT);
  Serial.begin(9600);
}

void loop() {
  previous = current;
  current = pseudoDigiRead(PIN_NUM);
  if (current == 0 && previous == 1) {
    delay(15);
    for (int i = 7; i >= 0; i--) {
      test |= ((pseudoDigiRead(PIN_NUM)) << i);
      delay(11);
    }
    Serial.print(test);
    test = 0;
  }

}
