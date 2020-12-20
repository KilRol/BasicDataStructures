#include <iostream>
#include "Tree.h"
using namespace std;


int main() {
	Tree a, b;
	cin >> a >> b;

	cout << endl << "|A| " << a << endl << "|B| " << b << endl << endl;

	Tree c = a.merge(b);

	cout << "|C| " << c << endl;

	Tree d = a.composition(b);
	Tree v = b.composition(a);

	cout << "A(B) " << d << endl;
	cout << "B(A) " << v << endl << endl;
}