#include <SoftwareSerial.h>

#define DEBUG_ENABLE // Comment this line before putting the code in the robot to remove serial printing

#ifdef DEBUG_ENABLE
  #define DEBUG_MACRO(STRING) Serial.println(STRING)
#else
  #define DEBUG_MACRO(STRING) // Nothing
#endif

/* Pin assignments */
#define ENABLE_1_PIN          6
#define MOTOR_1_INPUT_1       7
#define MOTOR_1_INPUT_2       8
#define MOTOR_2_INPUT_1       9
#define MOTOR_2_INPUT_2       10 
#define ENABLE_2_PIN          11
#define SERIAL_RX_PIN         2
#define SERIAL_TX_PIN         3

/* Constants */
#define MAX_MOTOR_SPEED       255
#define NORMAL_MOTOR_SPEED    180
#define SLOW_MOTOR_SPEED      80

// Software serial instance 
SoftwareSerial bluetoothSerial(SERIAL_RX_PIN, SERIAL_TX_PIN); // RX_PIN, TX_PIN
SoftwareSerial raspberrySerial(0,1);
char incomingData = 'r';
String data;

/* Functions prototypes */
void forward(uint8_t speed);
void reverse(uint8_t speed);
void right(uint8_t speed);
void left(uint8_t speed);
void sharpRightTurn(uint8_t speed);
void sharpLeftTurn(uint8_t speed);
void stopBot(uint8_t speed);
void scanBluetooth();

/* ---------------------------------------------------------- */
void setup() {
  /*
  #ifdef DEBUG_ENABLE
    Serial.begin(9600);
  #endif
  */
  bluetoothSerial.begin(9600); // This is the serial port we'll communicate with the bluetooth module through
  DEBUG_MACRO("--- Bluetooth RC robot started ---");
  
  pinMode(MOTOR_1_INPUT_1, OUTPUT);
  pinMode(MOTOR_1_INPUT_2, OUTPUT);
  pinMode(MOTOR_2_INPUT_1, OUTPUT);
  pinMode(MOTOR_2_INPUT_2, OUTPUT);
  pinMode(ENABLE_1_PIN, OUTPUT);
  pinMode(ENABLE_2_PIN, OUTPUT);
}

void loop() {
  scanBluetooth(); // Check if there's any incoming data through bluetooths
  
  switch(incomingData){ // Choose appropriate action
    case 'S': 
      stopBot(NORMAL_MOTOR_SPEED);
      DEBUG_MACRO("STOPPING");
      break;
    
    case 'F': 
      forward(NORMAL_MOTOR_SPEED); 
      DEBUG_MACRO("FORWARD");
      break;
      
    case 'L': 
      sharpLeftTurn(NORMAL_MOTOR_SPEED); 
      DEBUG_MACRO("LEFT");
      break;
    
    case 'R': 
      sharpRightTurn(NORMAL_MOTOR_SPEED);
      DEBUG_MACRO("RIGHT");
      break;
      
      
    case 'B':
      reverse(NORMAL_MOTOR_SPEED);
      DEBUG_MACRO("REVERSE");
      break;

    case 'V':
      incomingData='0';
      bluetoothSerial.end();
      scanRaspberry();
      raspberrySerial.begin(115200);
      raspberrySerial.println("Start");  
      //delay(100000);
      data = raspberrySerial.readStringUntil('\n');
      while(data!="end"){
        data = raspberrySerial.readStringUntil('\n');
      }
      raspberrySerial.print("you sent me: ");
      raspberrySerial.println(data);
      delay(1000);
      raspberrySerial.end();
      bluetoothSerial.begin(9600);
      break;
      
    default: 
      DEBUG_MACRO("UNKNOWN COMMAND");
      break;
  }
}
/* ---------------------------------------------------------- */

void forward(uint8_t speed){
  /* The pin numbers and high, low values might be different depending on your connections */
  analogWrite(ENABLE_1_PIN, speed); //Left Motor Speed
  analogWrite(ENABLE_2_PIN, speed); //Right Motor Speed
  digitalWrite(MOTOR_1_INPUT_1, LOW);
  digitalWrite(MOTOR_1_INPUT_2, HIGH);
  digitalWrite(MOTOR_2_INPUT_1, LOW);
  digitalWrite(MOTOR_2_INPUT_2, HIGH);
}

void reverse(uint8_t speed){
  /* The pin numbers and high, low values might be different depending on your connections */
  analogWrite(ENABLE_1_PIN, speed); //Left Motor Speed
  analogWrite(ENABLE_2_PIN, speed); //Right Motor Speed
  digitalWrite(MOTOR_1_INPUT_1, HIGH);
  digitalWrite(MOTOR_1_INPUT_2, LOW);
  digitalWrite(MOTOR_2_INPUT_1, HIGH);
  digitalWrite(MOTOR_2_INPUT_2, LOW);
}

void right(uint8_t speed){
  /* The pin numbers and high, low values might be different depending on your connections */
  analogWrite(ENABLE_1_PIN, speed); //Left Motor Speed
  analogWrite(ENABLE_2_PIN, speed); //Right Motor Speed
  digitalWrite(MOTOR_1_INPUT_1, LOW);
  digitalWrite(MOTOR_1_INPUT_2, HIGH);
  digitalWrite(MOTOR_2_INPUT_1, LOW);
  digitalWrite(MOTOR_2_INPUT_2, LOW);
}

void left(uint8_t speed){
  /* The pin numbers and high, low values might be different depending on your connections */
  analogWrite(ENABLE_1_PIN, speed); //Left Motor Speed
  analogWrite(ENABLE_2_PIN, speed); //Right Motor Speed
  digitalWrite(MOTOR_1_INPUT_1, LOW);
  digitalWrite(MOTOR_1_INPUT_2, LOW);
  digitalWrite(MOTOR_2_INPUT_1, LOW);
  digitalWrite(MOTOR_2_INPUT_2, HIGH);
}

void sharpRightTurn(uint8_t speed){
  /* The pin numbers and high, low values might be different depending on your connections */
  analogWrite(ENABLE_1_PIN, speed); //Left Motor Speed
  analogWrite(ENABLE_2_PIN, speed); //Right Motor Speed
  digitalWrite(MOTOR_1_INPUT_1, LOW);
  digitalWrite(MOTOR_1_INPUT_2, HIGH);
  digitalWrite(MOTOR_2_INPUT_1, HIGH);
  digitalWrite(MOTOR_2_INPUT_2, LOW);
}

void sharpLeftTurn(uint8_t speed){
  /* The pin numbers and high, low values might be different depending on your connections */
  analogWrite(ENABLE_1_PIN, speed); //Left Motor Speed
  analogWrite(ENABLE_2_PIN, speed); //Right Motor Speed
  digitalWrite(MOTOR_1_INPUT_1, HIGH);
  digitalWrite(MOTOR_1_INPUT_2, LOW);
  digitalWrite(MOTOR_2_INPUT_1, LOW);
  digitalWrite(MOTOR_2_INPUT_2, HIGH);
}

void stopBot(uint8_t speed){
  /* The pin numbers and high, low values might be different depending on your connections */
  analogWrite(ENABLE_1_PIN, speed); //Left Motor Speed
  analogWrite(ENABLE_2_PIN, speed); //Right Motor Speed
  digitalWrite(MOTOR_1_INPUT_1, LOW);
  digitalWrite(MOTOR_1_INPUT_2, LOW);
  digitalWrite(MOTOR_2_INPUT_1, LOW);
  digitalWrite(MOTOR_2_INPUT_2, LOW);
}

void scanBluetooth(){
  // There's data in the serial buffer (available() returns the number of bytes available)
  if(bluetoothSerial.available() > 0) incomingData = bluetoothSerial.read(); // Read a byte
}
void scanRaspberry(){
  // There's data in the serial buffer (available() returns the number of bytes available)
  if(raspberrySerial.available() > 0) incomingData = raspberrySerial.read(); // Read a byte
}
