#pragma once
#include <iostream>
using namespace std;

template <class T>

struct Node {
public: 
	T info;
	Node<T>* left;
	Node<T>* right;
	Node<T>(T info = T(), Node<T>* left = nullptr, Node<T>* right = nullptr)
	{
		this->info = info;
		this->left = left;
		this->right = right;
	}
	Node<T>(const Node<T>& node)
	{
		this->info = node.info;
		this->left = node.left;
		this->right = node.right;
	}
};

template <class T>

class BinarySearchTree
{
private:
	Node<T>* root;
public:
	BinarySearchTree() {
		this->root = nullptr;
	}
	Node<T>*& getRoot() {
		return this->root;
	}
	void add(Node<T>*& root, T value) {

		if (!root)
		{
			root = new Node<T>(value);
		}
		else
		{
			if (value < root->info)
				add(root->left, value);
			else {
				add(root->right, value);
			}
		}
	}
	void infixTraverse(Node<T>* root) {
		if (root) {
			
			infixTraverse(root->left);
			cout << root->info << " ";
			infixTraverse(root->right);
		}
	}
	void prefixTraverse(Node<T>* root) {
		if (root) {

			cout << root->info << " ";
			prefixTraverse(root->left);
			prefixTraverse(root->right);
		}
	}
	void postfixTraverse(Node<T>* root) {
		if (root) {

			postfixTraverse(root->left);
			postfixTraverse(root->right);
			cout << root->info << " ";
		}
	}
	Node<T>*& find(Node<T>*& root, T value) {
		if (root->info == value) return root;
		if (value < root->info) find(root->left, value);
		else find(root->right, value);
	}

};

