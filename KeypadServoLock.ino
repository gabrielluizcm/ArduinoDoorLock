#include <Servo.h>
#include <Keypad.h>

#define LOCKEDPOSITION 60
#define UNLOCKEDPOSITION 0
#define UNLOCKERBUTTON 11
#define LEDPIN 10
#define PASSWORDLENGTH 4  //change this for longer or shorter passwords

Servo lockServo;
int isLocked = 1;

char keypadKey;
char keypadInput[PASSWORDLENGTH + 1]; //the +1 is a workaround for a weird array bug happening when the last position received value
const char definedPassword[PASSWORDLENGTH + 1] = "1111";  //change this to change the required password
int inputCount = 0;
int i = 0;

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//int servoPosition = 0;  used only for testing

void setup() {
  Serial.begin(9600);
  lockServo.attach(12);
  lockServo.write(LOCKEDPOSITION);
  pinMode(UNLOCKERBUTTON, INPUT);
  pinMode(LEDPIN, OUTPUT);
}

void blink()
{
  digitalWrite(LEDPIN, LOW);
  delay(100);
  digitalWrite(LEDPIN, HIGH);
}

void loop() {
  /*if (Serial.available()) Test for position values; use values in range (1, 180)
    {
      servoPosition = Serial.parseInt();
      if (servoPosition)
      {
        lockServo.write(servoPosition);
        Serial.println(servoPosition);
      }
    }*/
  keypadKey = keypad.getKey();
  
  if (isLocked)
    digitalWrite(LEDPIN, HIGH);
  else
    digitalWrite(LEDPIN, LOW);
  
  if (digitalRead(UNLOCKERBUTTON))
  {
    if (isLocked)
      lockServo.write(UNLOCKEDPOSITION);
    else
      lockServo.write(LOCKEDPOSITION);
    isLocked = !isLocked;
    delay(1000);
  }


  switch (keypadKey)
  {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '#':
    case '*':
      if (inputCount == PASSWORDLENGTH) //if you keep typing more characters than the password, the 'input cursor' will loop
        inputCount = 0;                 //eg. the current input is '1234', '5' is pressed, now the input is '5234'
      keypadInput[inputCount] = keypadKey;
      Serial.println(keypadInput);
      inputCount++;
      digitalWrite(LEDPIN, LOW);
      delay(100);
      digitalWrite(LEDPIN, HIGH);
      break;
    case 'A':
      if (!strcmp(keypadInput, definedPassword))
      {
        Serial.println("Unlocked");
        lockServo.write(UNLOCKEDPOSITION);
        isLocked = 0;
      }
      inputCount = 0;
      for (int i = 0; i < PASSWORDLENGTH; i++)
        keypadInput[i] = 0;
      Serial.println(keypadInput);
      blink();
      break;
    case 'B':
      inputCount--;
      keypadInput[inputCount] = 0;
      Serial.println(keypadInput);
      blink();
      break;
    case 'C':
      for (int i = 0; i < PASSWORDLENGTH; i++)
        keypadInput[i] = 0;
      inputCount = 0;
      blink();
      break;
    case 'D':
      if (!isLocked)
      {
        lockServo.write(LOCKEDPOSITION);
        isLocked = 1;
      }
  }
}

