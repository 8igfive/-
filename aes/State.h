#ifndef __STATE_H__
#define __STATE_H__
#include <cstdint>
#include "CalcUtil.h"
#include "Cipher.h"

class State
{
	/*
	* 数据布局如下：列优先布局
	* a e i m
	* b	f j n
	* c g k o
	* d h l p
	*/
	uint8_t* state;
	uint8_t blockWordNumber;
	void _mixColumns(uint8_t* a);
public:
	State(uint8_t* state, uint8_t blockWordNumber);
	uint8_t stateByte(uint8_t c, uint8_t i);
	void setStateByte(uint8_t c, uint8_t i, uint8_t data);
	void addRoundKey(Cipher& cipher, uint8_t round);
	void mixColumns();
	void mixColumnsInverse();
	void shiftRows();
	void shiftRowsInverse();
	void subBytes();
	void subBytesInverse();
};

#endif // !__STATE_H__

