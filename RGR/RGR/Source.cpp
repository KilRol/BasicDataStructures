#include <iostream>
#include "Tree.h"
using namespace std;

int main() {
	Tree a, b;
	cout << "Input format {1>2, 2>3, ...}" << endl;
	cin >> a >> b;

	cout << endl << "A| " << a << endl << "B| " << b << endl << endl;

	Tree c = a.merge(b);

	cout << "Merge " << c << endl;

	Tree d = a.composition(b);
	Tree v = b.composition(a);

	cout << "A(B) " << d << endl;
	cout << "B(A) " << v << endl << endl;

	cout << b.getInvertable(a, b.at(0)) << endl;

	a = c;

	Tree h = a * b;
	cout << h << endl;
	h = a + b;
	cout << h << endl;
} 