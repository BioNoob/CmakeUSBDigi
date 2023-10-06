#include <DigiUSB.h>
#define HELLO_SEND 0x35  //'5'
#define HELLO_GET 0x53   //'S'
enum TUVK : uint8_t {
  UNKN = 0b00000000,
  MI28 = 0b00010000,
  KA52 = 0b00100000,
  SU25 = 0b01000000
};
enum ASP : uint8_t {
  UN = 0b0000,  //UNKNOWN
  PU = 0b0001,  //GUN
  C8 = 0b0010,  //C8
  UR = 0b0011,  //UR-A
  FA = 0b0100   //BOMBA
};
TUVK type_uvk;
ASP type_asp;
bool _is_ok;
int last_read;
bool btn_pressed;

void GetUvkAsp(int analog) {
  switch (type_uvk) {
    case MI28:
      if (analog < 770 && analog > 750) {
        type_asp = PU;
      }
      if (analog < 610 && analog > 590) {
        type_asp = C8;
      }
      if (analog < 510 && analog > 485) {
        type_asp = UR;
      }
      break;
    case KA52:
      if (analog < 685 && analog > 665) {
        type_asp = PU;
      }
      if (analog < 510 && analog > 495) {
        type_asp = C8;
      }
      if (analog < 410 && analog > 395) {
        type_asp = UR;
      }
      break;
    case SU25:
      if (analog < 515 && analog > 505) {
        type_asp = PU;
      }
      if (analog < 345 && analog > 330) {
        type_asp = C8;
      }
      if (analog < 260 && analog > 245) {
        type_asp = FA;
      }
      break;
    case UNKN:
      type_asp = UN;
      break;
  }
  if (analog < 50) {
    type_uvk = UNKN;
    type_asp = UN;
  } else if (analog < 435 && analog > 415) {
    type_uvk = MI28;
    type_asp = UN;
  } else if (analog < 295 && analog > 275) {
    type_uvk = KA52;
    type_asp = UN;
  } else if (analog < 150 && analog > 130) {
    type_uvk = SU25;
    type_asp = UN;
  }
}

void LocalDelay(int milsec) {
  //сюда запиздяшить опрос кнопки
  unsigned long last = millis();
  int helper = -1;
  while (milsec > 0) {
    unsigned long now = millis();
    milsec -= now - last;
    last = now;
    helper = analogRead(1);
    // DigiUSB.print("LAST");
    // DigiUSB.println(last_read, HEX);
    // DigiUSB.delay(500);
    // DigiUSB.print("HELP");
    // DigiUSB.println(helper, HEX);
    // DigiUSB.delay(500);
    if ((last_read + 10 >= helper) || (helper >= last_read - 10)) {
      if (!btn_pressed) {    //проверим есть ли нажатая уже.. необработанная. если нету будем менять значение
        btn_pressed = true;  //взвели флаг нажатой кнопки
        DigiUSB.println("CHANGE");
        last_read = helper;  //на отпрвку
        digitalWrite(0, LOW);
      }
    }
    DigiUSB.refresh();
  }
}

int ii;

void setup() {
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
  type_uvk = UNKN;
  type_asp = UN;
  last_read = 0;
  btn_pressed = false;
  analogReference(DEFAULT);
  DigiUSB.begin();
  _is_ok = false;
  while (DigiUSB.read() != -1) {
  }
  last_read = analogRead(1);
  ii = 0;
}

void loop() {
  while (DigiUSB.read() != HELLO_GET) {
    ii++;
    LocalDelay(100);
    DigiUSB.println(ii, DEC);
    if (ii > 5) {
      ii = 0;
      if (btn_pressed) {
        digitalWrite(0, HIGH);
        btn_pressed = false;
        DigiUSB.println("TIMEOUT");
      }
    }
    return;
  }
  DigiUSB.write(HELLO_SEND);
  //HENDSHAKE
  uint8_t out = 0;
  uint8_t read = 0;
  GetUvkAsp(last_read);
  out = type_uvk | type_asp;
  DigiUSB.write(out);
  digitalWrite(0, HIGH);
  btn_pressed = false;
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
    digitalWrite(0, HIGH);
  } else {  //TIMEOUT
    for (int i = 0; i < 3; i++) {
      LocalDelay(100);
      digitalWrite(0, LOW);
      LocalDelay(100);
      digitalWrite(0, HIGH);
    }
  }
  //DigiUSB.delay(10);
  LocalDelay(10);
  _is_ok = false;
}