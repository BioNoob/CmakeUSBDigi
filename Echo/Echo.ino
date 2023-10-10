#include <DigiUSB.h>
#define HELLO_SEND 0x35  //'5'
#define HELLO_GET 0x53   //'S'
enum class TUVK : uint8_t {
  UNKN = 0b00000000,
  MI28 = 0b00010000,
  KA52 = 0b00100000,
  SU25 = 0b01000000
};
enum class ASP : uint8_t {
  UN = 0b0000,  //UNKNOWN
  PU = 0b0001,  //GUN
  C8 = 0b0010,  //C8
  UR = 0b0011,  //UR-A
  FA = 0b0100   //BOMBA
};
TUVK type_uvk;
ASP type_asp;
TUVK type_uvk_old;
ASP type_asp_old;
bool _is_ok;
int last_read;
bool btn_pressed;
int ii;
bool was_open;

void GetUvkAsp(int analog) {
  switch (type_uvk) {
    case TUVK::MI28:
      if (analog < 770 && analog > 750) {
        type_asp = ASP::PU;
      }
      if (analog < 610 && analog > 590) {
        type_asp = ASP::C8;
      }
      if (analog < 510 && analog > 485) {
        type_asp = ASP::UR;
      }
      break;
    case TUVK::KA52:
      if (analog < 685 && analog > 665) {
        type_asp = ASP::PU;
      }
      if (analog < 510 && analog > 495) {
        type_asp = ASP::C8;
      }
      if (analog < 410 && analog > 395) {
        type_asp = ASP::UR;
      }
      break;
    case TUVK::SU25:
      if (analog < 515 && analog > 505) {
        type_asp = ASP::PU;
      }
      if (analog < 345 && analog > 330) {
        type_asp = ASP::C8;
      }
      if (analog < 260 && analog > 245) {
        type_asp = ASP::FA;
      }
      break;
    case TUVK::UNKN:
      type_asp = ASP::UN;
      break;
  }
  if (analog < 50) {
    type_uvk = TUVK::UNKN;
    type_asp = ASP::UN;
  } else if (analog < 435 && analog > 415) {
    type_uvk = TUVK::MI28;
    type_asp = ASP::UN;
  } else if (analog < 295 && analog > 275) {
    type_uvk = TUVK::KA52;
    type_asp = ASP::UN;
  } else if (analog < 150 && analog > 130) {
    type_uvk = TUVK::SU25;
    type_asp = ASP::UN;
  }
}

void LocalDelay(int milsec) {
  //сюда запиздяшить опрос кнопки
  unsigned long last = millis();
  bool _conditions = false;
  int helper = -1;
  while (milsec > 0) {
    unsigned long now = millis();
    milsec -= now - last;
    last = now;
    type_uvk_old = type_uvk;
    type_asp_old = type_asp;
    helper = analogRead(1);
    GetUvkAsp(helper);  //заполнили тувк и асп
    if ((last_read + 10 >= helper) ^ (helper >= last_read - 10)) { //ТУТ МОЖНО ТАКЖЕ ПО ТИПУ АСП УВК посмотреть не проверяя значения
      if (!btn_pressed) {
        //проверим есть ли нажатая уже.. необработанная. если нету будем менять значение
        if (type_uvk == type_uvk_old) {
          if (type_asp != type_asp_old) {
            //Взести флаг на мигание
            _conditions = true;
          }
        } else {
          _conditions = true;
        }
        if (_conditions) {
          btn_pressed = true;  //взвели флаг нажатой кнопки
          //last_read = helper;  //на отпрвку
          digitalWrite(0, LOW);
        }
        _conditions = false;
      }
    }
    DigiUSB.refresh();
  }
}


void TimeOut() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(0, LOW);
    LocalDelay(200);
    digitalWrite(0, HIGH);
    LocalDelay(200);
  }
}
void OkWrite() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(0, LOW);
    LocalDelay(100);
    digitalWrite(0, HIGH);
    LocalDelay(100);
  }
}



void setup() {
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
  type_uvk = type_uvk_old = TUVK::UNKN;
  type_asp = type_asp_old = ASP::UN;
  last_read = 0;
  btn_pressed = false;
  was_open = false;
  analogReference(DEFAULT);
  DigiUSB.begin();
  _is_ok = false;
  while (DigiUSB.read() != -1) {
  }
  last_read = analogRead(1);
  GetUvkAsp(last_read);
  ii = 0;
}



void loop() {
  if (!was_open) {
    unsigned char s = DigiUSB.read();
    while (s != HELLO_GET) {
      if (btn_pressed) {
        ii++;
        if (ii > 10) {
          TimeOut();
          //last_read = analogRead(1);
          btn_pressed = false;
        }
      } else {
        ii = 0;
      }
      LocalDelay(100);
      return;
    }
    DigiUSB.write(HELLO_SEND);
  }
  was_open = true;

  //HENDSHAKE
  uint8_t out = 0;
  uint8_t read = 0;
  //GetUvkAsp(last_read);
  LocalDelay(100);
  out = (uint8_t)type_uvk | (uint8_t)type_asp;
  DigiUSB.write(out);
  LocalDelay(200);
  //digitalWrite(0, HIGH);
  //btn_pressed = false;
  for (int i = 0; i < 100; i++)  // 1sec
  {
    read = DigiUSB.read();
    if (read == out) {
      _is_ok = true;
      break;
    }
    LocalDelay(100);
  }
  if (_is_ok) {
    if (btn_pressed) {
      OkWrite();
    }
  } else {  //TIMEOUT
    was_open = false;
    TimeOut();
  }
  //
  LocalDelay(200);
  _is_ok = false;
  btn_pressed = false;
}