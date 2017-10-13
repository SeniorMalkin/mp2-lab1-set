// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len<0)
		throw invalid_argument("Trying to create bitfield with negative length");
	BitLen = len;
	MemLen = (BitLen - 1) / (sizeof(TELEM) * 8) + 1;
	pMem = new TELEM[MemLen];
	
	for(int i=0;i<MemLen;i++)
		pMem[i]=0;
		
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];

}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << n % (sizeof(TELEM) * 8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0)
		throw invalid_argument("Set bit with negative index");
	if (n > BitLen)
		throw invalid_argument("Set bit with too large index");

	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0)
		throw invalid_argument("Clear bit with negative index");
	if (n > BitLen)
		throw invalid_argument("Clear bit with too large index");

	pMem[GetMemIndex(n)] &= (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0)
		throw invalid_argument("Get bit with negative index");
	if (n > BitLen)
		throw invalid_argument("Get bit with too large index");

	if((pMem[GetMemIndex(n)]&GetMemMask(n))==0)
		return 0;
	return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen)
	{
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
	}
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 0;
	for (int i = 0; i < MemLen ; i++)
		if (pMem[i] != bf.pMem[i])
			return 0;
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 1;
	for (int i = 0; i < MemLen ; i++)
		if (pMem[i] != bf.pMem[i])
			return 1;
	return 0;
	//return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (MemLen > bf.MemLen)
	{
		TBitField result(BitLen);
		for (int i = 0; i < bf.MemLen; i++)
			result.pMem[i] = pMem[i] | bf.pMem[i];
		for (int i = bf.MemLen; i < MemLen; i++)
			result.pMem[i] = pMem[i];
		return result;
	}
	if (MemLen < bf.MemLen)
	{
		TBitField result(bf.BitLen);
		for (int i = 0; i < MemLen; i++)
			result.pMem[i] = pMem[i] | bf.pMem[i];
		for (int i = MemLen; i < bf.MemLen; i++)
			result.pMem[i] = bf.pMem[i];
		return result;
	}
	
	if (BitLen > bf.BitLen) {
		TBitField result(BitLen);
		for (int i = 0; i < bf.BitLen; i++)
		{
			if (bf.GetBit(i))
				result.SetBit(i);
			else
			{
				if (GetBit(i))
					result.SetBit(i);
			}
		}
		return result;
	}
	if (BitLen < bf.BitLen)
	{
		TBitField result(bf.BitLen);
		for (int i = 0; i < BitLen; i++)
		{
			if (bf.GetBit(i))
				result.SetBit(i);
			else
			{
				if (GetBit(i))
					result.SetBit(i);
			}
		}
		return result;
	}
	TBitField result(BitLen);
	for (int i = 0; i < bf.MemLen; i++)
		result.pMem[i] = pMem[i] | bf.pMem[i];
	return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (MemLen > bf.MemLen)
	{
		TBitField result(BitLen);
		for (int i = 0; i < bf.MemLen; i++)
			result.pMem[i] = pMem[i] & bf.pMem[i];
		return result;
	}
	if (MemLen < bf.MemLen)
	{
		TBitField result(bf.BitLen);
		for (int i = 0; i < MemLen; i++)
			result.pMem[i] = pMem[i] & bf.pMem[i];
		return result;
	}
	if (BitLen > bf.BitLen) 
	{
		TBitField result(BitLen);
		for (int i = 0; i < bf.BitLen; i++)
		{
			if (bf.GetBit(i))
				if (GetBit(i))
					result.SetBit(i);
		}
		return result;
	}
	if (BitLen < bf.BitLen)
	{
		TBitField result(bf.BitLen);
		for (int i = 0; i < BitLen; i++)
		{
			if (bf.GetBit(i))
				if (GetBit(i))
					result.SetBit(i);
		}
		return result;
	}
	TBitField result(BitLen);
	for (int i = 0; i < MemLen; i++)
		result.pMem[i] = pMem[i] & bf.pMem[i];
	return result;
	
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField result(BitLen);
	for (int i = 0; i < MemLen-1; i++)
		result.pMem[i] = (~pMem[i]);
	TELEM a = 1;
	for (int i = 0; i < (BitLen % (sizeof(TELEM) * 8)); i++)
	{
		if (!GetBit(i + ((MemLen - 1) * sizeof(TELEM) * 8)))
			result.SetBit(i + ((MemLen - 1) * sizeof(TELEM) * 8));
	}
/*
	for (int i = 0; i < (BitLen / (sizeof(TELEM) * 8)-1); i++)
	{
		a << 1;
		a += 1;
	}
	result.pMem[MemLen - 1] &= a;
	*/
	return result;

}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char sym;
	int i=0;
	istr>>sym;
	while((sym=='1')||(sym=='0'))
	{if (sym==0) bf.ClrBit(i);
	else 
	bf.SetBit(i);
	i++;
	istr>>sym;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.GetLength(); i++)
	{
		if(bf.GetBit(i))
			ostr<<1;
		else
			ostr<<0;
	}
	return ostr;
}
