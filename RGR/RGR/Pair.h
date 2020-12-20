#pragma once
#include <iostream>
using namespace std;

class Pair
{
private:
	int arg;
	int value;

public:

	Pair(int arg = int(), int num2 = int()) {
		this->arg = arg;
		this->value = num2;
	}
	Pair(const Pair& p) {
		this->arg = p.arg;
		this->value = p.value;
	}
	bool operator<(Pair& p) {
		if (this->arg < p.arg) return true;
		else if (this->arg > p.arg) return false;
		else if (this->value < p.value) return true;
		else return false;
	}

	bool operator>(Pair& p) {
		if (this->arg > p.arg) return true;
		else if (this->arg < p.arg) return false;
		else if (this->value > p.value) return true;
		else return false;
	}

	bool operator==(Pair& p) {
		if (this->arg == p.arg) return true;
		else return false;
	}

	bool operator!=(Pair& p) {
		return !(*this == p);
	}

	Pair& operator=(Pair& p) {
		this->arg = p.arg;
		this->value = p.value;
		return *this;
	}

	//Arithmetic operations
	Pair operator+(Pair& p) {
		Pair a(*this);
		a.value = a.value + p.value;
		return a;
	}
	Pair operator-(Pair& p) {
		Pair a(*this);
		a.value = a.value - p.value;
		return a;
	}
	Pair operator*(Pair& p) {
		Pair a(*this);
		a.value = a.value * p.value;
		return a;
	}
	Pair operator/(Pair& p) {
		Pair a(*this);
		a.value = a.value / p.value;
		return a;
	}

	int getValue() {
		return this->value;
	}
	int getArg() {
		return this->arg;
	}

	friend ostream& operator<<(ostream& os, const Pair& p) {
		return os << p.arg << ">" << p.value;
	}
	friend istream& operator>>(istream& is, Pair& p) {
		char a;
		return is >> p.arg >> a >> p.value;
	}
};

