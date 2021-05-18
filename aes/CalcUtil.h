#ifndef __CALCUTIL_H__
#define __CALCUTIL_H__

#include <cstdint>

class CalcUtil
{
	static uint8_t gfMultCache[];
	static uint8_t substituteMatrix[];
	static uint8_t substituteMatrixInverse[];
	static uint8_t rCon[];
public:
	static uint8_t gfAdd(uint8_t a, uint8_t b);
	static uint8_t gfSub(uint8_t a, uint8_t b);
	static uint8_t gfMult(uint8_t a, uint8_t b);
	static uint8_t substitute(uint8_t origin);
	static uint8_t substituteInverse(uint8_t later);
	static uint8_t* rcon(uint8_t i);
};

#endif // !__CALCUTIL_H__
