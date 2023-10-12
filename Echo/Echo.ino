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
  LocalDelay(50);
  // //TEST2
  // uint8_t getter = 0;
  // uint8_t outter = 0;
  // outter = (uint8_t)type_uvk | (uint8_t)type_asp;
  // while (DigiUSB.read() != -1) {
  //   //LocalDelay(10);
  // }
  // DigiUSB.write(outter);  //отправляем данные
  // LocalDelay(500);
  // int iw = 0;
  // ASP asp = ASP::UN;
  // TUVK tvk = TUVK::UNKN;
  // _is_ok = true;
  // //GetUvkAsp(analogRead(1), &type_asp, &type_uvk);
  // while ((uint8_t)asp != (uint8_t)type_asp || (uint8_t)tvk != (uint8_t)type_uvk) {
  //   iw++;
  //   LocalDelay(50);
  //   //DigiUSB.print(iw,DEC);
  //   if (iw > 40) {
  //     _is_ok = false;
  //     break;
  //   } else {
  //     getter = DigiUSB.read();
  //     //DigiUSB.write(getter);
  //     tvk = (TUVK)((getter & 0xF0));
  //     asp = (ASP)((getter & 0xF0) << 4);
  //   }
  // }
  // // }
  // if (_is_ok) {
  //   DigiUSB.write(SYNC);
  //   if (btn_pressed) {
  //     OkWrite();
  //   }
  // } else {  //TIMEOUT
  //   //DigiUSB.write(0xED);
  //   TimeOut();
  // }
  // //DigiUSB.write(0xb8);
  // //_is_ok = false;
  // btn_pressed = false;
  // return;


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
    LocalDelay(50);
    i++;
    if (i > 20) {
      was_open = false;
      return;
    }
  }
  out = (uint8_t)type_uvk | (uint8_t)type_asp;
  DigiUSB.write(out);  //отправляем данные
  i = 0;
  while (DigiUSB.read() != SYNC) {
    LocalDelay(50);
    i++;
    if (i > 20) {
      was_open = false;
      return;
    }
  }
  //шлем синк
  DigiUSB.write((uint8_t)SYNC);
  //ждем синк

  i = 0;
  uint8_t asp = 0;
  uint8_t tvk = 0;
  _is_ok = true;
  while (asp != (uint8_t)type_asp || tvk != (uint8_t)type_uvk) {
    LocalDelay(10);
    read = 0;
    read = DigiUSB.read();
    tvk = read & 0xF0;
    asp = (read & 0x0F);
    // DigiUSB.write((uint8_t)tvk);
    // DigiUSB.write((uint8_t)asp);
    // DigiUSB.write((uint8_t)asp != (uint8_t)type_asp || (uint8_t)tvk != (uint8_t)type_uvk);
    //DigiUSB.write(read);
    if(i > 100)
    DigiUSB.write(out);
    if (i > 200) {
      _is_ok = false;
      break;
    }
    i++;
  }
  //ждем квиток

  if (_is_ok) {
    if (btn_pressed) {
      OkWrite();
    }
  } else {  //TIMEOUT
    was_open = false;
    if (btn_pressed) {
      TimeOut();
    }
  }
  _is_ok = false;
  btn_pressed = false;
}