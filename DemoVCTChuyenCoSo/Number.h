#pragma once

#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

class Number {
private:
	bool sign;
	string absValue;
	int base;
	
	void reverseStr(string& str) const;
	string toString(const long double& value) const;
	bool isValid(const string& absValue, const int& _base) const;
	int decode(const string& digit) const;
	string encode(const int& digit) const;
	void standardize();
public:
	Number(const string& _value = "0", const int& _base = 10);

	void set(const string& _value, const int& _base);
	void convertBase(const int& newBase);
	void convertDecimal();

	friend ostream& operator<<(ostream& os, const Number& num);
	friend istream& operator>>(istream& is, Number& num);

	~Number();
};
