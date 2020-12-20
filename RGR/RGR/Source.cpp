#include "Tree.h"
#include <iostream>
using namespace std;

int main() {
	Tree a, b; 
	cin >> a >> b;

	cout << endl << "|A| "<< a << endl << "|B| " << b << endl << endl;

	Tree c = a.merge(b);

	cout << c << endl << endl;

	Tree d = a.composition(b);
	Tree v = b.composition(a);
	cout << "a.composition(b)" << d << endl << endl;
	cout << "b.composition(a)" << v << endl << endl;

	Tree e;

	cout << e << endl;
}