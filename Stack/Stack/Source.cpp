#include "Stack.h";

int main() {
	Stack<int> A;

	A.push_front(3);
	A.push_front(6);
	A.push_front(8);

	cout << "A old " << A << endl;

	cin >> A >> A >> A;
	cout << "A new " << A << endl;
}