#pragma once
#include <iostream>
using namespace std;


template <class T>

struct Node {
public:
	T info;
	Node* next;
	Node<T>(T info = T(), Node<T>* next = nullptr) 
	{
		this->info = info;
		this->next = next;
	}
	Node<T>(const Node<T>& node) 
	{
		this->info = node.info;
		this->next = nullptr;
	}
};

template <class T>

class List
{
private:
	Node<T>* head;
	int size;
public:

	List<T>() 
	{
		this->head = nullptr;
		this->size = 0;
	}

	List<T>(const List<T>& list)
	{
		this->size = list.size;

		Node<T>* cur = list.head->next;
		Node<T>* cur2 = new Node<T>(*list.head);

		this->head = cur2;

		while (cur)
		{
			Node<T>* h = new Node<T>(*cur);
			cur2->next = h;
			cur = cur->next;
			cur2 = cur2->next;
		}
	}

	~List<T>() 
	{
		clear();
	}

	void push_back(T info) 
	{
		Node<T>* nd = new Node<T>(info);
		if (!this->head) 
		{
			this->head = nd;
		}
		else 
		{
			Node<T>* cur = this->head;
			while (cur->next) {
				cur = cur->next;
			}
			cur->next = nd;
		}
		size++;
	}

	void push_front(T info)
	{
		Node<T>* nd = new Node<T>(info, this->head);
		head = nd;
	}

	void delete_front() 
	{
		Node<T>* tmp = this->head;
		this->head = this->head->next;
		this->size--;
		delete tmp;
	}

	void clear()
	{
		while (size) delete_front();
	}

	List<T>& operator= (const List& list) 
	{
		clear();
		if (list.head == nullptr) return *this;

		this->size = list.size;

		Node<T>* cur = list.head->next;
		Node<T>* cur2 = new Node<T>(*list.head);

		this->head = cur2;
		
		while (cur)
		{
			Node<T>* h = new Node<T>(*cur);
			cur2->next = h;
			cur = cur->next;
			cur2 = cur2->next;
		}

		return *this;
	}
	
	bool operator==(const List<T>& list)
	{
		if (this->size != list.size) return false;
		Node<T>* cur = this->head;
		Node<T>* cur2 = list.head;
		while (cur) 
		{
			if (cur->info != cur2->info)
				return false;
			cur = cur->next;
			cur2 = cur2->next;
		}
		return true;
	}
	bool operator!=(const List<T>& list) {
		return !(*this == list);
	}
	bool operator!() {
		return !(this->head);
	}

	friend istream& operator>> (istream& is, List<T>& list);
	friend ostream& operator<< (ostream& os, List<T>& list) 
	{
		Node<T>* cur = list.head;
		while (cur) {
			os << cur->info << " ";
			cur = cur->next;
		}
		return os;
	}
};