#ifndef __CIPHER_H__
#define __CIPHER_H__
#include <cstdint>
#include <iostream>
#include "CalcUtil.h"

class Cipher
{
	/*
	* 数据布局为行优先布局
	*/
	uint8_t* cipher;
	uint8_t cipherWordNumber;
	uint8_t blockWordNumber;
	uint8_t round;
	void shiftWord(uint8_t* word);
	void subWord(uint8_t* word);
	void rconXor(uint8_t* word, uint8_t* rcon);
	uint8_t* expand();
public:
	Cipher(uint8_t* cipher, uint8_t cipherWordNumber, uint8_t blockWordNumber, uint8_t round);
	~Cipher();
	Cipher(const Cipher& c);
	Cipher(Cipher&& c);
	Cipher& operator=(const Cipher& c);
	Cipher& operator=(Cipher&& c);
	uint8_t cipherByte(uint8_t r, uint8_t c, uint8_t i);
	
};

#endif // !__CIPHER_H__

