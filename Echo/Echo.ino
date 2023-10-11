#include "MC_USB.h"
TUVK type_uvk;
ASP type_asp;
TUVK type_uvk_old;
ASP type_asp_old;
bool _is_ok;
bool btn_pressed;
bool was_open;


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
  was_open = false;
  analogReference(DEFAULT);
  DigiUSB.begin();
  _is_ok = false;
  while (DigiUSB.read() != -1) {
  }
  GetUvkAsp(analogRead(1), &type_asp, &type_uvk);
}

void loop() {
  //HENDSHAKE
  if (!was_open) {
    int ii = 0;
    unsigned char s = DigiUSB.read();
    while (s != HELLO_GET) {
      s = DigiUSB.read();
      if (btn_pressed) {
        ii++;
        if (ii > 10) {
          TimeOut();
          btn_pressed = false;
          ii = 0;
        } else {
          LocalDelay(100);
        }
      } else {
        LocalDelay(100);
      }
    }
    DigiUSB.write(HELLO_SEND);
    was_open = true;
  }
  uint8_t out = 0;
  uint8_t read = 0;
  int i = 0;
  //шлем синк
  DigiUSB.write((uint8_t)SYNC);
  //ждем синк
  while (DigiUSB.read() != SYNC) {
    LocalDelay(100);
    i++;
    if (i > 20) {
      was_open = false;
      return;
    }
  }

  LocalDelay(10);
  out = (uint8_t)type_uvk | (uint8_t)type_asp;
  DigiUSB.write(out);  //отправляем данные
                       //ждем синк
  i = 0;
  while (DigiUSB.read() != SYNC) {
    LocalDelay(100);
    i++;
    if (i > 20) {
      was_open = false;
      return;
    }
  }
  //шлем синк
  DigiUSB.write((uint8_t)SYNC);
  //ждем квиток
  for (int i = 0; i < 100; i++) {
    LocalDelay(10);
    read = DigiUSB.read();
    if (read == out) {
      _is_ok = true;
      break;
    }
  }

  if (_is_ok) {
    if (btn_pressed) {
      OkWrite();
    }
  } else {  //TIMEOUT
    was_open = false;
    TimeOut();
  }
  _is_ok = false;
  btn_pressed = false;
}