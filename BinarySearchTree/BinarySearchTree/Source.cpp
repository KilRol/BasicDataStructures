#include "BinarySearchTree.h"

int main() {
	BinarySearchTree<int> tree;

	tree.add(tree.getRoot(), 8);
	tree.add(tree.getRoot(), 3);
	tree.add(tree.getRoot(), 1);
	tree.add(tree.getRoot(), 6);
	tree.add(tree.getRoot(), 4);

	BinarySearchTree<int> tree1;

	tree1.add(tree1.getRoot(), 7);
	tree1.add(tree1.getRoot(), 10);
	tree1.add(tree1.getRoot(), 14);
	tree1.add(tree1.getRoot(), 13);
	tree1.add(tree1.getRoot(), 8);

	tree.infixTraverse(tree.getRoot());
	cout << endl;
	tree.prefixTraverse(tree.getRoot());
	cout << endl;
	tree.postfixTraverse(tree.getRoot());
	cout << endl;
	cout << endl;
	tree1.infixTraverse(tree1.getRoot());
	cout << endl;
	tree1.prefixTraverse(tree1.getRoot());
	cout << endl;
	tree1.postfixTraverse(tree1.getRoot());

}