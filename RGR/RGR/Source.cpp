#include "Tree.h"
#include <iostream>
using namespace std;

int main() {
	Tree a;
	a.push(a.getRoot(), MyPair(1, 3));
	a.push(a.getRoot(), MyPair(6, 2));
	a.push(a.getRoot(), MyPair(0, 2));
	a.push(a.getRoot(), MyPair(0, 0));

	a.infixTraverse(a.getRoot());

	cout << endl << "VERT" << endl << a.getValue(6) << endl << a.getValue(1);
	cout << endl << "INVERT" << endl << a.getInvertable(0) << endl << a.getInvertable(3);

	MyPair m1(1, 4);
	MyPair m2(2, 5);
	cout << endl;
	cout << "PAIR1 " << m1 << endl;
	cout << "PAIR2 " << m2 << endl;
	cout << "PAIR1 + PAIR2 " << m1 + m2 << endl;
	cout << "PAIR1 - PAIR2 " << m1 - m2 << endl;
	cout << "PAIR1 * PAIR2 " << m1 * m2 << endl;
	cout << "PAIR1 / PAIR2 " << m1 / m2 << endl;

}