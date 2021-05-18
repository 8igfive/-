#include "Cipher.h"

void Cipher::shiftWord(uint8_t* word)
{
	uint8_t temp = word[0];
	word[0] = word[1];
	word[1] = word[2];
	word[2] = word[3];
	word[3] = temp;
}

void Cipher::subWord(uint8_t* word)
{
	word[0] = CalcUtil::substitute(word[0]);
	word[1] = CalcUtil::substitute(word[1]);
	word[2] = CalcUtil::substitute(word[2]);
	word[3] = CalcUtil::substitute(word[3]);
}

void Cipher::rconXor(uint8_t* word, uint8_t* rcon)
{
	word[0] = word[0] ^ rcon[0];
	word[1] = word[1] ^ rcon[1];
	word[2] = word[2] ^ rcon[2];
	word[3] = word[3] ^ rcon[3];
}

uint8_t* Cipher::expand()
{
	uint8_t* expandCipher = new uint8_t[4 * blockWordNumber * (1 + round)];
	for (uint8_t w = 0; w < cipherWordNumber; ++w)
	{
		expandCipher[4 * w + 0] = cipher[4 * w + 0];
		expandCipher[4 * w + 1] = cipher[4 * w + 1];
		expandCipher[4 * w + 2] = cipher[4 * w + 2];
		expandCipher[4 * w + 3] = cipher[4 * w + 3];
	}
	uint8_t temp[4];
	uint8_t len = blockWordNumber * (round + 1);
	for (uint8_t w = cipherWordNumber; w < len; ++w)
	{
		temp[0] = expandCipher[4 * (w - 1) + 0];
		temp[1] = expandCipher[4 * (w - 1) + 1];
		temp[2] = expandCipher[4 * (w - 1) + 2];
		temp[3] = expandCipher[4 * (w - 1) + 3];
		if (!(w % cipherWordNumber))
		{
			shiftWord(temp);
			subWord(temp);
			rconXor(temp, CalcUtil::rcon(w / cipherWordNumber));
		}
		expandCipher[4 * (w) + 0] = expandCipher[4 * (w - cipherWordNumber) + 0] ^ temp[0];
		expandCipher[4 * (w) + 1] = expandCipher[4 * (w - cipherWordNumber) + 1] ^ temp[1];
		expandCipher[4 * (w) + 2] = expandCipher[4 * (w - cipherWordNumber) + 2] ^ temp[2];
		expandCipher[4 * (w) + 3] = expandCipher[4 * (w - cipherWordNumber) + 3] ^ temp[3];
	}
	return expandCipher;
}

Cipher::Cipher(uint8_t* cipher, uint8_t cipherWordNumber, uint8_t blockWordNumber, uint8_t round)
	:cipher(cipher), cipherWordNumber(cipherWordNumber), blockWordNumber(blockWordNumber), round(round)
{
	this->cipher = expand();
}

uint8_t Cipher::cipherByte(uint8_t r, uint8_t c, uint8_t i)
{
	return cipher[4 * blockWordNumber * r + 4 * c + i];
}

Cipher::~Cipher()
{
	delete[] cipher;
}

Cipher::Cipher(const Cipher& c)
	:cipherWordNumber(c.cipherWordNumber), blockWordNumber(c.blockWordNumber), round(c.round)
{
	//std::cout << 1 << std::endl;
	uint8_t len = 4 * blockWordNumber * (1 + round);
	cipher = new uint8_t[len];
	for (uint8_t i = 0; i < len; ++i)
	{
		cipher[i] = c.cipher[i];
	}
}

Cipher::Cipher(Cipher&& c)
	:cipher(c.cipher), cipherWordNumber(c.cipherWordNumber), blockWordNumber(c.blockWordNumber), round(c.round)
{
	//std::cout << 2 << std::endl;
	c.cipher = nullptr;
}

Cipher& Cipher::operator=(const Cipher& c)
{
	cipherWordNumber = c.cipherWordNumber;
	blockWordNumber = c.blockWordNumber;
	round = c.round;
	uint8_t len = 4 * blockWordNumber * (1 + round);
	for (uint8_t i = 0; i < len; ++i)
	{
		cipher[i] = c.cipher[i];
	}
	return *this;
}

Cipher& Cipher::operator=(Cipher&& c)
{
	cipherWordNumber = c.cipherWordNumber;
	blockWordNumber = c.blockWordNumber;
	round = c.round;
	cipher = c.cipher;
	c.cipher = nullptr;
	return *this;
}
