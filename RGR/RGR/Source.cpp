#include "Tree.h"
#include <iostream>
using namespace std;

int main() {
	Tree a;
	cin >> a;
	cout << "|A| " << a << endl << endl;
	Tree b;
	cin >> b;
	cout << "|B| " << b << endl << endl;
	Tree c = a.merge(b);
	cout << "|C| " << c << endl << endl;
	Tree::postorder(c.getRoot(), 0);

	Tree d = a.composition(b);

	cout << d;

}