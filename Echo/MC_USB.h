#ifndef _MC_USB_
#define _MC_USB_

#include <DigiUSB.h>
#define HELLO_SEND 0x35  //'5'
#define HELLO_GET 0x53   //'S'
#define SYNC 0xEE
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
void LocalDelay(int milsec);
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
    LocalDelay(200);
    digitalWrite(0, HIGH);
    LocalDelay(200);
  }
}
uint8_t g_GetMaskedValue(uint32_t nValue, uint32_t mask)
{
	nValue = nValue & mask;
	uint32_t bMask = 1;
	while ((mask & bMask) == 0)
	{
		nValue = nValue >> 1;
		mask = mask >> 1;
	}
	return nValue;
}
void GetUvkAsp(int analog, ASP *t_asp, TUVK *t_uvk) {
  switch (*t_uvk) {
    case TUVK::MI28:
      if (analog < 770 && analog > 750) {
        *t_asp = ASP::PU;
      }
      if (analog < 610 && analog > 590) {
        *t_asp = ASP::C8;
      }
      if (analog < 510 && analog > 485) {
        *t_asp = ASP::UR;
      }
      break;
    case TUVK::KA52:
      if (analog < 685 && analog > 665) {
        *t_asp = ASP::PU;
      }
      if (analog < 510 && analog > 495) {
        *t_asp = ASP::C8;
      }
      if (analog < 410 && analog > 395) {
        *t_asp = ASP::UR;
      }
      break;
    case TUVK::SU25:
      if (analog < 515 && analog > 505) {
        *t_asp = ASP::PU;
      }
      if (analog < 345 && analog > 330) {
        *t_asp = ASP::C8;
      }
      if (analog < 260 && analog > 245) {
        *t_asp = ASP::FA;
      }
      break;
    case TUVK::UNKN:
      *t_asp = ASP::UN;
      break;
  }
  if (analog < 50) {
    *t_uvk = TUVK::UNKN;
    *t_asp = ASP::UN;
  } else if (analog < 435 && analog > 415) {
    *t_uvk = TUVK::MI28;
    *t_asp = ASP::UN;
  } else if (analog < 295 && analog > 275) {
    *t_uvk = TUVK::KA52;
    *t_asp = ASP::UN;
  } else if (analog < 150 && analog > 130) {
    *t_uvk = TUVK::SU25;
    *t_asp = ASP::UN;
  }
};
#endif