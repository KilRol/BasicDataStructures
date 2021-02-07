#pragma once
#include <iostream>
using namespace std;

class BooleanVector {
private:

	class Node {
	public:
		bool value;
		Node* next;
		Node(bool value = bool(), Node* next = nullptr) {
			this->value = value;
			this->next = next;
		}
	};

	Node* head;
public:

	class Iterator {
	public:
		friend class BooleanVector;
		Node* ptr;
		Iterator() {
			ptr = nullptr;
		}
		Iterator(const Iterator& iter) {
			ptr = iter.ptr;
		}
		Iterator& operator=(const Iterator& iter) {
			ptr = iter.ptr;
			return *this;
		}
		Iterator& operator++() {
			ptr = ptr->next;
			return *this;
		}
		Iterator operator++(int) {
			Iterator iter(*this);
			ptr = ptr->next;
			return iter;
		}
		bool& operator*() {
			return ptr->value;
		}
		bool* operator->() {
			return &(ptr->value);
		}
		bool operator!=(const Iterator& iter) {
			return (ptr != iter.ptr);
		}
		bool operator==(const Iterator& iter) {
			return (ptr == iter.ptr);
		}
	};

	BooleanVector() {
		head = nullptr;
	}
	BooleanVector(string& str) {
		head = nullptr;
		for (int i = 0; str[i]; i++) {
			if (str[i] == '0' || str[i] == '1') push_back(str[i] - 48);
		}
	}
	BooleanVector(const BooleanVector& v) {
		head = nullptr;
		Node* cur = v.head;
		while (cur) {
			push_back(cur->value);
			cur = cur->next;
		}
	}
	~BooleanVector() {
		bv_destroyer();
	}
	void bv_destroyer() {	
		while (head) {
			Node* t = head;
			head = head->next;
			delete t;
		}
	}
	Iterator begin() {
		Iterator iter;
		iter.ptr = head;
		return iter;
	}
	Iterator end() {
		Iterator iter;
		return iter;
	}
	BooleanVector& operator=(BooleanVector& v) {
		bv_destroyer();
		Node* cur = v.head;
		while (cur) {
			push_back(cur->value);
			cur = cur->next;
		}
		return *this;
	}
	void push_back(bool a) {
		Node* nd = new Node(a);
		if (!head) {
			head = nd;
		}
		else {
			Node* cur = head;
			while (cur->next)
			{
				cur = cur->next;
			}
			cur->next = nd;
		}
	}
	void del_tail() {
		Node* cur = head;
		while (cur->next->next) {
			cur = cur->next;
		}
		delete cur->next;
		cur->next = nullptr;
	}
	BooleanVector& operator<<(int count) {
		for (int i = 0; i < count; i++)
			push_back(false);
		return *this;
	}
	BooleanVector& operator>>(int count) {
		for (int i = 0; i < count; i++)
			del_tail();
		return *this;
	}
	bool isEmpty() {
		return (head == nullptr) ? true : false;
	}
	int dot_product(BooleanVector& v) {
		int res = false;
		Node* cur = head;
		Node* cur_v = v.head;

		while (cur && cur_v) {
			res = res + (cur->value & cur_v->value);
			cur = cur->next;
			cur_v = cur_v->next;
		}
		return res;
	}
	bool& operator[](int index) {
		Node* cur = head;
		for (int i = 0; i < index; i++) {
			cur = cur->next;
		}
		return cur->value;
	}
	bool operator==(BooleanVector& v) {
		Node* cur = head;
		Node* cur_v = v.head;
		while (cur && cur_v) {
			if (cur->value != cur_v->value) return false;
			cur = cur->next;
			cur_v = cur_v->next;
		}
		if (cur || cur_v) return false;
		return true;
	}
	bool operator!=(BooleanVector& v) {
		return !(*this == v);
	}
	BooleanVector conjunction(BooleanVector& v) {
		BooleanVector new_v;
		Node* cur = head;
		Node* cur_v = v.head;

		while (cur || cur_v) {
			if (!cur) {
				new_v.push_back(0 & cur_v->value);
				cur_v = cur_v->next;
			}
			else if (!cur_v) {
				new_v.push_back(0 & cur->value);
				cur = cur->next;
			}
			else {
				new_v.push_back(cur->value & cur_v->value);
				cur = cur->next;
				cur_v = cur_v->next;
			}
		}
		return new_v;
	}
	BooleanVector disjunction(BooleanVector& v) {
		BooleanVector new_v;
		Node* cur = head;
		Node* cur_v = v.head;

		while (cur || cur_v) {
			if (!cur) {
				new_v.push_back(0 | cur_v->value);
				cur_v = cur_v->next;
			}
			else if (!cur_v) {
				new_v.push_back(0 | cur->value);
				cur = cur->next;
			}
			else {
				new_v.push_back(cur->value | cur_v->value);
				cur = cur->next;
				cur_v = cur_v->next;
			}
		}
		return new_v;
	}
	BooleanVector subvector(int from, int to) {
		BooleanVector new_v;
		Node* cur = head;
		try {
			int i = 0;
			int j = 0;
			for (i; i < from; i++) {
				if (!cur) throw "Incorrect range";
				cur = cur->next;
			}
			for (j = i; i <= to; i++)
			{
				if (!cur) throw "Incorrect range";
				new_v.push_back(cur->value);
				cur = cur->next;
			}
		}
		catch (const char* e) {
			cout << e << endl;
			new_v.bv_destroyer();
		}
		return new_v;
	}
	BooleanVector concatenation(BooleanVector& v) {
		BooleanVector new_v = *this;
		Node* cur = v.head;
		while (cur) {
			new_v.push_back(cur->value);
			cur = cur->next;
		}
		return new_v;
	}
	friend istream& operator>>(istream& is, BooleanVector& v) {
		int pos = 0;
		try {
			char a;
			char b;
			is >> a;
			pos++;
			if (a != '<') throw "Symbol '<' expected.";
			is >> a;
			pos++;
			if (a != '<') throw "Symbol '<' expected.";
			while (true) {
				is >> a;
				pos++;
				is >> b;
				pos++;
				if (a == '0' || a == '1') {
					v.push_back(a - 48);
				}
				else {
					throw "Symbol '0' or '1' expected.";
				}

				if (b == ';') {
					continue;
				}
				else if (b == '>') {
					is >> b;
					if (b != '>') throw "Symbol '>' expected.";
					break;
				}
				else {
					throw "Symbol ';' expected.";
				}
			}
		}
		catch (const char* e) {
			cout << "Incorrect input in symbol " << pos << ". " << e << endl;
			v.bv_destroyer();
		}
		return is;
	}
	friend ostream& operator<<(ostream& os, const BooleanVector& b) {
		os << "<<" << b.head->value;
		for (Node* cur = b.head->next; cur; cur = cur->next) {
			os << ';' << cur->value;
		}
		os << ">>";
		return os;
	}
};

