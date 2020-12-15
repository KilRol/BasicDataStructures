#pragma once
#include <iostream>
using namespace std;

template <class T>

struct Node {
	T info;
	Node* next;
	Node<T>(T info = T(), Node<T>* next = nullptr) {
		this->info = info;
		this->next = next;
	}
	Node<T>(const Node<T>& node) {
		this->info = node.info;
		this->next = nullptr;
	}
};

template <class T>

class Stack
{
private:
	Node<T>* head;
	int size;
public:
	Stack<T>() {
		head = nullptr;
		size = 0;
	}

	Stack<T>(const Stack<T>& stack) {
		this->size = stack.size;

		Node<T>* cur = stack.head->next;
		Node<T>* cur2 = new Node<T>(stack.head);
		
		this->head = cur2;

		while (cur)
		{
			cur2->next = cur;
			cur = cur->next;
			cur2 = cur2->next;
		}
	}

	~Stack<T>() {
		clear();
	}

	void clear() {
		while (size) pop();
	}

	void push_front(T info) {
		Node<T>* nd = new Node<T>(info, this->head);
		this->head = nd;
	}

	bool operator=(const Stack<T>& stack)
	{
		clear();
		this->size = stack.size;

		if (stack.head == nullptr) return *this;

		Node<T>* cur = stack.head->next;
		Node<T>* cur2 = new Node<T>(stack.head);

		this->head = cur2;

		while (cur)
		{
			cur2->next = cur;
			cur = cur->next;
			cur2 = cur2->next;
		}

		return *this;
	}

	bool operator!() 
	{
		return !(this->head);
	}


	bool operator==(const Stack<T>& stack) 
	{
		if (this->size != stack.size) return false;

		Node<T>* cur = this->head; 
		Node<T>* cur2 = stack.head;

		while (cur) 
		{
			if (cur->info != cur2->info)
				return false;
			cur = cur->next;
			cur2 = cur2->next;
		}

		return true;
	}

	bool operator!=(const Stack<T>& stack)
	{
		return !(*this == stack);
	}

	T pop() 
	{
		Node<T>* nd = this->head;
		T value = nd->info;

		this->head = this->head->next;
		this->size--;
		delete nd;

		return value;
	}

	friend ostream& operator<<(ostream& os, const Stack<T>& stack)
	{
		Node<T>* cur = stack.head;
		while (cur) 
		{
			os << cur->info << " ";
			cur = cur->next;
		}
		return os;
	}

	friend istream& operator>>(istream& is, Stack<T>& stack)
	{
		T value;
		is >> value;

		Node<T>* nd = new Node<T>(value, stack.head);
		stack.head = nd;
		return is;
	}

};

