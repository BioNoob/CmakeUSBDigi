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
TUVK type_uvk_last;
ASP type_asp_last;
bool _is_ok;
int last_read;

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
  if (type_uvk_last != type_uvk) {
    digitalWrite(0, LOW);
  } else if (type_asp_last != type_asp){ //&& type_asp != UN) {
    digitalWrite(0, LOW);
  }
}

void LocalDelay(int milsec) {
  //сюда запиздяшить опрос кнопки
  unsigned long last = millis();
  while (milsec > 0) {
    unsigned long now = millis();
    milsec -= now - last;
    last = now;
    last_read = analogRead(1);
    GetUvkAsp(last_read);
    DigiUSB.refresh();
    //refresh();
  }
}

void setup() {
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
  type_uvk = UNKN;
  type_asp = UN;
  type_uvk_last = UNKN;
  type_asp_last = UN;
  last_read = 0;
  analogReference(DEFAULT);
  DigiUSB.begin();
  _is_ok = false;
  while (DigiUSB.read() != -1) {
  }
}

void loop() {
  while (DigiUSB.read() != HELLO_GET) {
    //DigiUSB.delay(100);
    LocalDelay(100);
    return;
  }
  DigiUSB.write(HELLO_SEND);
  LocalDelay(300);
  //DigiUSB.delay(300);
  //HENDSHAKE
  uint8_t out = 0;
  uint8_t read = 0;
  out = type_uvk | type_asp;

  DigiUSB.write(out);
  for (int i = 0; i < 100; i++)  // 1sec
  {
    //DigiUSB.delay(100);
    LocalDelay(100);
    read = DigiUSB.read();
    if (read == out) {
      _is_ok = true;
      break;
    }
  }
  if (_is_ok) {
    //DigiUSB.delay(300);
    LocalDelay(300);
    digitalWrite(0, HIGH);
  } else {  //TIMEOUT
    for (int i = 0; i < 3; i++) {
      //DigiUSB.delay(100);
      LocalDelay(100);
      digitalWrite(0, LOW);
      //DigiUSB.delay(100);
      LocalDelay(100);
      digitalWrite(0, HIGH);
    }
  }
  //DigiUSB.delay(10);
  LocalDelay(10);
  _is_ok = false;
  type_asp_last = type_asp;
  type_uvk_last = type_uvk;
}