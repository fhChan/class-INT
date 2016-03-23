#include <iostream>
#include <cstring>
using namespace std;

class INT
{
	int length_b, sign;
	char *buf;
	INT(char *p, int len, int s = 1);
	void Add(const INT &i);
	void Minus(const INT &i);

public:
	INT();
	INT(int i);
	INT(char *num);
	INT(const INT &i);
	~INT();

	INT &operator=(const INT &i);
	INT& INT::operator+=(const INT &i);
	INT& INT::operator-=(const INT &i);

	friend INT operator+(const INT &i1, const INT &i2);
	friend INT operator-(const INT &i1, const INT &i2);
	friend INT operator*(const INT &i1, const INT &i2);
	friend INT operator/(const INT &i1, const INT &i2);
	friend ostream &operator<<(ostream &out, const INT &i);
};

INT::INT()
{
	sign = 1;
	length_b = 10;
	buf = new char[length_b + 1];
	buf[0] = '0';
	buf[1] = '\0';
}

INT::INT(int i)
{
	if (i >= 0)
		sign = 1;
	else
	{
		sign = -1;
		i = -i;
	}
	int j = i, k = 0;
	do
	{
		k++;
		j /= 10;
	} while (j != 0);
	length_b = (k / 10 + 1) * 10;
	buf = new char[length_b + 1];
	j = 0;
	do
	{
		k = i % 10;
		buf[j++] = k + '0';
		i /= 10;
	} while (i != 0);
	buf[j] = '\0';
}

INT::INT(char *num)
{
	if (*num == '-')
	{
		sign = -1;
		num++;
	}
	else if (*num == '+')
	{
		sign = 1;
		num++;
	}
	else
		sign = 1;
	length_b = (strlen(num) / 10 + 1) * 10;
	buf = new char[length_b + 1];
	int i, j;
	for (i = 0, j = strlen(num) - 1; j >= 0; i++, j--)
		buf[i] = num[j];
	buf[i] = '\0';
}

INT::INT(const INT &i)
{
	sign = i.sign;
	length_b = i.length_b;
	buf = new char[length_b + 1];
	strcpy(buf, i.buf);
}

INT::INT(char *p, int len, int s)
{
	buf = p;
	length_b = len;
	sign = s;
}

INT::~INT()
{
	delete[]buf;
	buf = NULL;
	length_b = 0;
}

INT& INT::operator =(const INT &i)
{
	sign = i.sign;
	if (length_b < i.length_b)
	{
		delete[]buf;
		length_b = i.length_b;
		buf = new char[length_b + 1];
	}
	strcpy(buf, i.buf);
	return *this;
}

void INT::Add(const INT &i)
{
	int len1 = strlen(buf);
	int len2 = strlen(i.buf);
	int length_b1 = (((len1>len2 ? len1 : len2) + 1) / 10 + 1) * 10;
	if (length_b1 > length_b)
	{
		char *buf1 = new char[length_b1 + 1];
		strcpy(buf1, buf);
		delete[]buf;
		buf = buf1;
		length_b = length_b1;
	}

	char *p1, *p2;
	if (len1 >= len2)
	{
		p1 = buf;
		p2 = i.buf;
	}
	else
	{
		p1 = i.buf;
		p2 = buf;
	}

	int carry = 0, sum;
	char *p = buf;
	while (*p2 != '\0')
	{
		sum = (*p1 - '0') + (*p2 - '0') + carry;
		if (sum >= 10)
		{
			carry = 1;
			*p = (sum - 10) + '0';
		}
		else
		{
			carry = 0;
			*p = sum + '0';
		}
		p1++; p2++; p++;
	}
	while (*p1 != '\0')
	{
		if (carry == 0)
			*p = *p1;
		else
		{
			sum = (*p1 - '0') + carry;
			if (sum >= 10)
			{
				carry = 1;
				*p = (sum - 10) + '0';
			}
			else
			{
				carry = 0;
				*p = sum + '0';
			}
		}
		p1++; p++;
	}
	if (carry != 0)
	{
		*p = '1';
		p++;
	}
	*p = '\0';
}

void INT::Minus(const INT &i)
{
	int len1 = strlen(buf);
	int len2 = strlen(i.buf);
	int length_b1 = ((len1>len2 ? len1 : len2) / 10 + 1) * 10;
	if (length_b1 > length_b)
	{
		char *buf1 = new char[length_b1 + 1];
		strcpy(buf1, buf);
		delete[]buf;
		buf = buf1;
		length_b = length_b1;
	}

	char *p1, *p2;

	if (len1 > len2)
		p1 = buf;
	else if (len1 < len2)
		p1 = i.buf;
	else
	{
		int j;
		for (j = len1 - 1; j >= 0; j--)
		{
			if (buf[j] > i.buf[j])
			{
				p1 = buf;
				break;
			}
			else if (buf[j] < i.buf[j])
			{
				p1 = i.buf;
				break;
			}
		}
		if (j < 0)
		{
			sign = 1;
			strcpy(buf, "0");
			return;
		}
	}

	if (p1 == buf)
	{
		p2 = i.buf;
	}
	else
	{
		p2 = buf;
		sign = i.sign;
	}

	int carry = 0, diff;
	char *p = buf;
	while (*p2 != '\0')
	{
		diff = *p1 - *p2 - carry;
		if (diff < 0)
		{
			*p = (diff + 10) + '0';
			carry = 1;
		}
		else
		{
			*p = diff + '0';
			carry = 0;
		}
		p1++; p2++; p++;
	}
	while (*p1 != '\0')
	{
		if (carry == 0)
			*p = *p1;
		else
		{
			diff = (*p1 - '0') - carry;
			if (diff < 0)
			{
				*p = (diff + 10) + '0';
				carry = 1;
			}
			else
			{
				*p = diff + '0';
				carry = 0;
			}
		}
		p1++; p++;
	}
	p--;
	while (p != buf && *p == '0') p--;
	*(p + 1) = '\0';
}

INT& INT::operator+=(const INT &i)
{
	if (sign == i.sign)
		Add(i);
	else
		Minus(i);
	return *this;
}

INT operator+(const INT &i1, const INT &i2)
{
	INT temp(i1);
	temp += i2;
	return temp;
}

INT& INT::operator-=(const INT &i)
{
	INT *p = (INT *)&i;
	p->sign = -p->sign;
	*this += *p;
	p->sign = -p->sign;
	return *this;
}

INT operator-(const INT &i1, const INT &i2)
{
	INT temp(i1);
	temp -= i2;
	return temp;
}

INT operator*(const INT &i1, const INT &i2)
{
	int len1 = strlen(i1.buf);
	int len2 = strlen(i2.buf);
	int length_b = ((len1 + len2) / 10 + 1) * 10;
	char *buf = new char[length_b + 1];
	INT product, temp(buf, length_b, 1);

	for (int j = 0; j<len1; j++)
	{
		int carry = 0, mul, n;
		for (n = 0; n<j; n++)
			buf[n] = '0';
		for (int k = 0; k<len2; k++)
		{
			mul = (i1.buf[j] - '0')*(i2.buf[k] - '0') + carry;
			carry = mul / 10;
			buf[n++] = mul % 10 + '0';
		}
		if (carry != 0) buf[n++] = carry + '0';
		buf[n] = '\0';
		product += temp;
	}
	if (i1.sign == i2.sign)
		product.sign = 1;
	else
		product.sign = -1;
	return product;
}

INT operator/(const INT &i1, const INT &i2)
{
	INT div1(i1), div2(i2), quotient;
	div1.sign = 1;
	div2.sign = 1;

	div1 -= div2;
	while (div1.sign>0)
	{
		quotient += 1;
		div1 -= div2;
	}
	if (i1.sign == i2.sign)
		quotient.sign = 1;
	else
		quotient.sign = -1;
	return quotient;
}

ostream& operator<<(ostream &out, const INT &i)
{
	if (i.sign<0)	out << '-';
	for (int j = strlen(i.buf) - 1; j >= 0; j--)
		out << i.buf[j];
	return out;
}

int main()
{
	INT i1("123456"), i2, i3;
	i2 = i1 * 10;
	i3 = i1 * 11 - i2;
	cout << i1 << "\t" << i2 << "\t" << i3 << endl;
	return 0;
}
