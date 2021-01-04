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

	cout << c << endl;
	
	Tree y = c.getInvertable();
	cout << y << endl;

	
} 