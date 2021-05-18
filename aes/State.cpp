#include "State.h"


void State::_mixColumns(uint8_t* a)
{
	uint8_t column[4];
	for (uint8_t c = 0; c < blockWordNumber; ++c)
	{
		for (uint8_t i = 0; i < 4; ++i)
		{
			column[i] = stateByte(c, i);
		}
		for (uint8_t i = 0; i < 4; ++i)
		{
			setStateByte(c, i, CalcUtil::gfAdd
			(
				CalcUtil::gfAdd
				(
					CalcUtil::gfAdd
					(
						CalcUtil::gfMult(a[(4 - i) % 4], column[0]),
						CalcUtil::gfMult(a[(5 - i) % 4], column[1])
					),
					CalcUtil::gfMult(a[(6 - i) % 4], column[2])
				),
				CalcUtil::gfMult(a[(7 - i) % 4], column[3])
			));
		}
	}
}

State::State(uint8_t* state, uint8_t blockWordNumber)
	:state(state), blockWordNumber(blockWordNumber) { }

uint8_t State::stateByte(uint8_t c, uint8_t i)
{
	return state[blockWordNumber * i + c];
}

void State::setStateByte(uint8_t c, uint8_t i, uint8_t data)
{
	state[blockWordNumber * i + c] = data;
}

void State::addRoundKey(Cipher& cipher, uint8_t round)
{
	for (uint8_t c = 0; c < blockWordNumber; ++c)
	{
		for (uint8_t i = 0; i < 4; ++i)
		{
			setStateByte(c, i, stateByte(c, i) ^ cipher.cipherByte(round, c, i));
		}
	}
}

void State::mixColumns()
{
	uint8_t a[] = { 0x02, 0x03, 0x01, 0x01 };
	_mixColumns(a);
}

void State::mixColumnsInverse()
{
	uint8_t a[] = { 0x0e, 0x0b, 0x0d, 0x09 };
	_mixColumns(a);
}

void State::shiftRows()
{
	uint8_t* row = new uint8_t[blockWordNumber];
	for (uint8_t i = 1; i < 4; ++i)
	{
		for (uint8_t c = 0; c < blockWordNumber; ++c)
		{
			row[c] = stateByte((c + i) % blockWordNumber, i);
		}
		for (uint8_t c = 0; c < blockWordNumber; ++c)
		{
			setStateByte(c, i, row[c]);
		}
	}
	delete[] row;
}

void State::shiftRowsInverse()
{
	uint8_t* row = new uint8_t[blockWordNumber];
	for (uint8_t i = 1; i < 4; ++i)
	{
		for (uint8_t c = 0; c < blockWordNumber; ++c)
		{
			row[c] = stateByte((c - i + blockWordNumber) % blockWordNumber, i);
		}
		for (uint8_t c = 0; c < blockWordNumber; ++c)
		{
			setStateByte(c, i, row[c]);
		}
	}
	delete[] row;
}

void State::subBytes()
{
	for (uint8_t i = 0; i < 4; ++i)
	{
		for (uint8_t c = 0; c < blockWordNumber; ++c)
		{
			setStateByte(c, i, CalcUtil::substitute(stateByte(c, i)));
		}
	}
}

void State::subBytesInverse()
{
	for (uint8_t i = 0; i < 4; ++i)
	{
		for (uint8_t c = 0; c < blockWordNumber; ++c)
		{
			setStateByte(c, i, CalcUtil::substituteInverse(stateByte(c, i)));
		}
	}
}

