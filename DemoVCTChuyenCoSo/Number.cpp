#include "Number.h"
#define _EPSILON 1e-10

void Number::reverseStr(string& str) const {
	reverse(str.begin(), str.end());
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == ')') str[i] = '(';
		else if (str[i] == '(') str[i] = ')';
	}
}

string Number::toString(const long double& value) const {
	stringstream ss;
	ss << std::setprecision(std::numeric_limits<long double>::digits10 + 1);
	ss << value;
	return ss.str();
}

bool Number::isValid(const string& inValue, const int& inBase) const {
	if (inBase <=1 || inValue.size() == 0) return false;
	// Check error: ".abcde" and "abcde."
	if (inValue.front() == '.' || inValue.back() == '.') return false;
	// Check error: 2 dots in a real number
	int dot1 = inValue.find(".");
	int dot2 = inValue.rfind(".");
	if (dot1 != 1 && dot1 != dot2) return false;
	// Check if exist a digit not in the base-N's digit set. ie: if exist '2' in base-2 number
	for (int begin = 0; begin < inValue.length(); begin++) {
		if (inValue[begin] == '.') continue;
		int end = (inValue[begin] == '(') ? inValue.find(')', begin) : begin;
		if (end == -1) return false;
		int digit = decode(inValue.substr(begin, end - begin + 1));
		if (digit >= inBase) {
			return false;
		}
		else {
			begin = end;		
		}
	}
	return true;
}

int Number::decode(const string& digit) const {
	// Digits like: '0'-'9' and 'a'-'z' and 'A'-'Z'
	if (digit.length() == 1) {
		if (digit[0] >= '0' && digit[0] <= '9') {
			return digit[0] - '0';
		}
		if (digit[0] >= 'A' && digit[0] <= 'Z') {
			return digit[0] - 'A' + 10;
		}
		if (digit[0] >= 'a' && digit[0] <= 'z') {
			return digit[0] - 'a' + 10;
		}
	}
	// Digits like: '(0)','(1)', .... <=> a_0, a_1, ...
	if (digit.length() > 2 && digit.front() == '(' && digit.back() == ')') {
		return stoi(digit.substr(1, digit.length() - 2)) + 36;
	}
	throw "Invalid number!";
}

string Number::encode(const int& digitVal) const {
	string res = "";
	if (digitVal >= 0 && digitVal <= 9) {
		res += ('0' + digitVal);
		return res;
	}
	if (digitVal >= 10 && digitVal <= 35) {
		res += ('A' + digitVal - 10);
		return res;
	}
	if (digitVal >= 36) {
		res += '(' + to_string(digitVal - 36) + ')';
		return res;
	}
	throw "Invalid number!";
}

void Number::standardize() {
	if (absValue.length() > 1) {
		// ie: 00001, 001.5, ...
		while (absValue[0] == '0' && absValue[1] != '.') {
			absValue = absValue.substr(1);
		}
		// ie: 0.100, 1.000, ...
		while (absValue.find('.') != -1 && absValue.back() == '0') {
			absValue.pop_back();
			if (absValue.back() == '.') {
				absValue.pop_back();
			}
		}
	}
}

Number::Number(const string& inValue, const int& inBase) {
	set(inValue, inBase);
}

void Number::set(const string& inValue, const int& inBase) {
	if (inValue.size() <= 0)
		throw "Invalid input value!";

	// Set the sign: positive or negative number
	sign = inValue[0] == '-';

	// Set the value string and the base with checking if it's a valid number
	if (isValid(inValue.substr(sign), inBase)) {
		absValue = inValue.substr(sign);
		base = inBase;
		// Beautify/Standardize the value string
		standardize();
	}
	else {
		throw "Invalid value!";
	}
}

void Number::convertBase(const int& newBase) {
	if (newBase < 2) throw "Invalid base!";
	if (base == newBase) return;
	if (base != 10) convertDecimal();
	if (newBase == 10) return;

	string strInt, strFrac;
	long long valInt = 0;
	long double valFrac = 0.0;

	// Split the integral (before dot) and fractional (after dot)
	int dot = absValue.find('.');
	if (dot != -1) {
		strInt = absValue.substr(0, dot);
		strFrac = absValue.substr(dot + 1);
	}
	else {
		strInt = absValue;
	}

	// Convert the integral
	valInt = stoll(strInt);
	strInt.clear();
	if (valInt == 0) {
		strInt.append("0");
	}
	else {
		while (valInt > 0) {
			string digit = encode(valInt % newBase);
			strInt.append(digit);
			valInt /= newBase;
		}
		reverseStr(strInt);
	}
	// Convert the fractional
	valFrac = stold("0." + strFrac);
	strFrac.clear();
	while (valFrac > 0 && strFrac.length() <= 20) {
		string digit = encode(floor(valFrac * newBase));
		strFrac.append(digit);
		valFrac = (valFrac * newBase) - floor(valFrac * newBase);
	}
	// Combine the integral and fractional parts
	string result = sign ? "-" : "";
	result += strInt + (!strFrac.empty()?"." + strFrac:"");
	set(result, newBase);
}

void Number::convertDecimal() {
	string strInt, strFrac;
	long long valInt = 0;
	long double valFrac = 0.0;

	// Split the integral (before dot) and fractional (after dot)
	int dot = absValue.find('.');
	if (dot != -1) {
		strInt = absValue.substr(0, dot);
		strFrac = absValue.substr(dot + 1);
	}
	else {
		strInt = absValue;
	}

	// Convert the integral
	double radix = 1;
	for (int end = strInt.length() - 1; end >= 0; end--) {
		int begin = (strInt[end] == ')') ? strInt.rfind('(', end) : end;
		int digit = decode(strInt.substr(begin, end - begin + 1));
		valInt += radix * digit;
		end = begin;
		radix *= base;
	}
	// Convert the fractional
	radix = 1.0 / base;
	for (int begin = 0; begin < strFrac.length(); begin++) {
		int end = (strFrac[begin] == '(') ? strFrac.find(')', begin) : begin;
		int digit = decode(strFrac.substr(begin, end - begin + 1));
		valFrac += radix * digit;
		begin = end;
		radix *= 1.0 / base;
	}
	// Combine the integral and fractional part
	string result = sign ? "-" : "";
	result += to_string(valInt) + (fabsl(valFrac) > _EPSILON ? "." + toString(valFrac).substr(2) : "");
	set(result, 10);
}

Number::~Number() {
	absValue.clear();
	base = 10;
	sign = false;
}

ostream& operator<<(ostream& os, const Number& num) {
	return os << (num.sign ? "-" : "") << num.absValue << "_" << num.base;
}

istream& operator>>(istream& is, Number& num) {
	string inValue = "";
	int inBase = 10;
	is >> inValue >> inBase;
	num.set(inValue, inBase);
	return is;
}
