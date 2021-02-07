#include "BooleanVector.h"

int main()
{
	BooleanVector a, b;
	cin >> a;
	cin >> b;
	cout << a << endl;
	cout << b << endl;

	cout << "CONJUNCTION " << a.conjunction(b) << endl;
	cout << "DISJUNCTION " << a.disjunction(b) << endl;

	cout << "DOT PRODUCT " << a.dot_product(b) << endl;

	cout << (a.subvector(2, 3)) << endl;
	cout << (a.subvector(0, 3)) << endl;
}