#include "Tree.h"
#include <iostream>
using namespace std;

int main() {
	Tree a;

	cin >> a;

	cout << "||A " << a << endl << endl;

	Tree b;

	cin >> b;
	cout << "||B " << b << endl << endl;

	a.merge(b);

	cout << "||A U B " << a << endl << endl;

	Tree c;
	cin >> c;
	cout << "||C " << c;
}