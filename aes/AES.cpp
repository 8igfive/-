#include "AES.h"

Cipher AES::genCipher(uint8_t size)
{
	srand(time(0));
	uint8_t byteSize = size / 8;
	uint8_t* cipherTemp = new uint8_t[byteSize];
	for (int i = 0; i < byteSize; ++i)
	{
		cipherTemp[i] = rand();
	}
	Cipher ret(cipherTemp, byteSize / 4, 4, 6 + byteSize / 4);
	delete[] cipherTemp;
	return ret;
}

AES::AES(uint8_t size)
	: size(size), blockWordNumber(4), cipherWordNumber(size / 32), round(cipherWordNumber + 6), cipher(genCipher(size)){ }

AES::AES(uint8_t cipher[], uint8_t byteSize)
	: size(byteSize * 8), blockWordNumber(4), cipherWordNumber(size / 32), round(cipherWordNumber + 6), cipher(cipher, byteSize / 4, 4, 6 + byteSize / 4) { }

Cipher& AES::getCipher()
{
	return cipher;
}

void AES::encrypt(uint8_t* in, uint8_t* out)
{
	// uint8_t temp[16];
	for (uint8_t w = 0; w < blockWordNumber; ++w)
	{
		for (uint8_t i = 0; i < 4; ++i)
		{
			out[blockWordNumber * i + w] = in[4 * w + i];
		}
	}
	State state(out, blockWordNumber);
	state.addRoundKey(cipher, 0);
	for (uint8_t r = 1; r < round; ++r)
	{
		state.subBytes();
		state.shiftRows();
		state.mixColumns();
		state.addRoundKey(cipher, r);
	}
	state.subBytes();
	state.shiftRows();
	state.addRoundKey(cipher, round);
}

void AES::decrypt(uint8_t* in, uint8_t* out)
{
	uint8_t* temp = new uint8_t[4 * blockWordNumber];
	for (uint8_t i = 0; i < 4; ++i)
	{
		for (uint8_t w = 0; w < blockWordNumber; ++w)
		{
			temp[blockWordNumber * i + w] = in[blockWordNumber * i + w];
		}
	}
	State state(temp, blockWordNumber);
	state.addRoundKey(cipher, round);
	state.shiftRowsInverse();
	state.subBytesInverse();
	for (uint8_t r = round - 1; r >= 1; --r)
	{
		state.addRoundKey(cipher, r);
		state.mixColumnsInverse();
		state.shiftRowsInverse();
		state.subBytesInverse();
	}
	state.addRoundKey(cipher, 0);
	for (uint8_t i = 0; i < 4; ++i)
	{
		for (uint8_t w = 0; w < blockWordNumber; ++w)
		{
			out[4 * w + i] = temp[blockWordNumber * i + w];
		}
	}
	delete[] temp;
}
