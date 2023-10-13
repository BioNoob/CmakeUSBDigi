#include "MC_USB.h"
TUVK type_uvk, type_uvk_old;
ASP type_asp, type_asp_old;
bool btn_pressed;


void LocalDelay(int milsec) {
  unsigned long last = millis();
  bool _conditions = false;
  while (milsec > 0) {
    unsigned long now = millis();
    milsec -= now - last;
    last = now;
    type_uvk_old = type_uvk;
    type_asp_old = type_asp;
    if (!btn_pressed) {
      GetUvkAsp(analogRead(1), &type_asp, &type_uvk);
      if (type_uvk == type_uvk_old) {
        if (type_asp != type_asp_old) {
          if (type_asp != ASP::UN) {
            _conditions = true;
          }
        }
      } else {
        _conditions = true;
      }
      if (_conditions) {
        btn_pressed = true;  //взвели флаг нажатой кнопки
        digitalWrite(0, LOW);
      }
    }
    DigiUSB.refresh();
  }
}

void setup() {
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
  type_uvk = type_uvk_old = TUVK::UNKN;
  type_asp = type_asp_old = ASP::UN;
  btn_pressed = false;
  analogReference(DEFAULT);
  DigiUSB.begin();
  while (DigiUSB.read() != -1) {
  }
  GetUvkAsp(analogRead(1), &type_asp, &type_uvk);
}

void loop() {
  LocalDelay(50);
  //TEST3
  int i = 0;
  uint8_t asp = 0;
  uint8_t tvk = 0;
  uint8_t read = 0;
  bool previos = false;
  bool _is_ok = true;
  previos = btn_pressed;
  while (asp != (uint8_t)type_asp || tvk != (uint8_t)type_uvk) {
    LocalDelay(50);
    if (btn_pressed != previos) {
      //DigiUSB.print("@");
      i = 0;
      previos = btn_pressed;
    }
    DigiUSB.write((uint8_t)type_uvk | (uint8_t)type_asp);
    //LocalDelay(10);
    read = 0;
    for (int ii = 0; ii < 10; ii++) {
      read = DigiUSB.read();
      tvk = read & 0xF0;
      asp = read & 0x0F;
      //i++;
      if (asp == (uint8_t)type_asp && tvk == (uint8_t)type_uvk){
        break;
      }
    }
    // DigiUSB.write((uint8_t)tvk);
    // DigiUSB.write((uint8_t)asp);
    // DigiUSB.write((uint8_t)asp != (uint8_t)type_asp || (uint8_t)tvk != (uint8_t)type_uvk);
    //DigiUSB.write(read);
    if (i > 60) {
      _is_ok = false;
      //DigiUSB.print('$');
      break;
    }
    i++;
  }
  if (_is_ok) {
    if (btn_pressed) {
      OkWrite();
    }
  } else {  //TIMEOUT
    if (btn_pressed) {
      TimeOut();
    }
  }
  _is_ok = false;
  btn_pressed = false;
  return;
}