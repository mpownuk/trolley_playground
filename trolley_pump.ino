#include <SoftwareSerial.h>

SoftwareSerial bluetooth(10, 11);
#define trolley_sensor 2
#define pump_runner 3

int oil_amount = 0;

short trolley_user_data = 0;
short trolley = 3;
int timer = 500;
bool isCountingTrolley = true;
// bool isPumpRunning = false;

void setup() {
  pinMode(trolley_sensor, INPUT_PULLUP);
  pinMode(pump_runner, OUTPUT);

  Serial.begin(9600);
  bluetooth.begin(9600);
  Serial.println("Gotowy do połączenia z Bluetooth HC-05");
}

void loop() {
  if (digitalRead(trolley_sensor) == LOW) {
    if(isCountingTrolley){
      trolley--;
      isCountingTrolley = false;
      if (trolley <=0){
        digitalWrite(pump_runner, HIGH);
        if(trolley_user_data != 0){
          trolley = trolley_user_data;
        } else {
          trolley = 3;
        }
        delay(timer);
        digitalWrite(pump_runner, LOW);
      }
    }
  } else {
    digitalWrite(pump_runner, LOW);
    isCountingTrolley = true;
  }

  if (bluetooth.available()) {
    // char c = bluetooth.read();
    String command = bluetooth.readStringUntil('\n');
    // Serial.write(c);
    handleBluetoothCommand(command);
  }

  if (Serial.available()) {
    char c = Serial.read();
    bluetooth.write(c);
  }
}

void handleBluetoothCommand(String command){
  command.trim();
  int equalsIndex = command.indexOf("=");
  if(equalsIndex == -1){
    Serial.println("unknown command mendo");
    return;
  }

  String variable = command.substring(0, equalsIndex);
  String valueStr = command.substring(equalsIndex +1);

  int value = valueStr.toInt();
  if(variable.equalsIgnoreCase("trolley")){
    trolley_user_data = value;
    Serial.print("trolley set on: ");
    Serial.println(trolley_user_data);
  } else if(variable.equalsIgnoreCase("timer")){
    timer = value;
    Serial.print("timer set on: ");
    Serial.println(timer);
  } else {
    Serial.println("unknown variable");
  }
}