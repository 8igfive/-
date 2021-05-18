#ifndef __AES_H__
#define __AES_H__

#include <cstdlib>
#include <ctime>
#include <cstdint>
#include "Cipher.h"
#include "State.h"


class AES
{
	uint8_t size;
	uint8_t blockWordNumber;
	uint8_t cipherWordNumber;
	uint8_t round;
	Cipher cipher;
public:
	static Cipher genCipher(uint8_t size);
	AES(uint8_t size);
	AES(uint8_t cipher[], uint8_t byteSize);
	Cipher& getCipher();
	void encrypt(uint8_t* in, uint8_t* out);
	void decrypt(uint8_t* in, uint8_t* out);

};

#endif // !__AES_H__
