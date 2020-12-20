#include "Tree.h"
#include <iostream>
using namespace std;

int main() {
	Tree a, b; 
	cin >> a >> b;

	cout << endl << "|A| "<< a << endl << "|B| " << b << endl << endl;

	Tree c = a.merge(b);

	cout << c << endl << endl;
	cout << "At 2 " << c.at(2) << endl << endl;

	Tree d = a.composition(b);
	Tree v = b.composition(a);

	cout << "A(B)" << d << endl;
	cout << "B(A)" << v << endl << endl;

	Tree e;

	cout << "Empty tree " << e << endl;	
}