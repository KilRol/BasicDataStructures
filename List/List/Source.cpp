#include <iostream>
#include "List.h"

using namespace std;

int main() {
	List<int> a;
	a.push_back(5);
	a.push_back(7);
	a.push_back(9);

	cout << a << endl;

	List<int> b(a);
	cout << b << endl;

	b.push_back(199);
	cout << a << endl;
	cout << b << endl;
}