#pragma once
#include <iostream>
using namespace std;

class BooleanVector {
private:

	class Node {
	public:
		bool value;
		Node* next;
		Node* prev;
		Node(bool value = bool(), Node* next = nullptr, Node* prev = nullptr) {
			this->value = value;
			this->next = next;
			this->prev = prev;
		}
	};

	Node* head;
	Node* tail;
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
		Iterator& operator--() {
			ptr = ptr->prev;
			return *this;
		}
		Iterator operator++(int) {
			Iterator iter(*this);
			ptr = ptr->next;
			return iter;
		}
		Iterator operator--(int) {
			Iterator iter(*this);
			ptr = ptr->prev;
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
		tail = nullptr;
	}
	BooleanVector(const BooleanVector& v) {
		head = nullptr;
		tail = nullptr;
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
			tail = head->next;
			delete head;
			head = tail;
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
			tail = nd;
		}
		else {
			tail->next = nd;
			Node* cur = tail;
			tail = nd;
			tail->prev = cur;
		}
	}

	void push_front(bool a) {
		Node* nd = new Node(a);
		if (head == nullptr) {
			head = nd;
			tail = nd;
		}
		else {
			head->prev = nd;
			Node* cur = head;
			head = nd;
			head->next = cur;
		}
	}

	void del_tail() {
		Node* cur = tail;
		tail = tail->prev;
		tail->next = nullptr;
		delete cur;
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

	bool dot_product(BooleanVector& v) {
		bool res = false;
		Node* cur = tail;
		Node* cur_v = v.tail;

		while (cur && cur_v) {
			res = res | (cur->value & cur_v->value);
			cur = cur->prev;
			cur_v = cur_v->prev;
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
		Node* cur = tail;
		Node* cur_v = v.tail;
		while (cur && cur_v) {
			if (cur->value != cur_v->value) return false;
			cur = cur->prev;
			cur_v = cur_v->prev;
		}
		if (cur || cur_v) return false;
		return true;
	}

	bool operator!=(BooleanVector& v) {
		return !(*this == v);
	}

	BooleanVector conjunction(BooleanVector& v) {
		BooleanVector new_v;
		Node* cur = tail;
		Node* cur_v = v.tail;

		while (cur && cur_v) {
			new_v.push_front(cur->value & cur_v->value);
			cur = cur->prev;
			cur_v = cur_v->prev;
		}

		return new_v;
	}

	BooleanVector disjunction(BooleanVector& v) {
		BooleanVector new_v;
		Node* cur = tail;
		Node* cur_v = v.tail;

		while (cur || cur_v) {
			if (!cur) {
				new_v.push_front(0 | cur_v->value);
				cur_v = cur_v->prev;
			}
			else if (!cur_v) {
				new_v.push_front(0 | cur->value);
				cur = cur->prev;
			}
			else {
				new_v.push_front(cur->value | cur_v->value);
				cur = cur->prev;
				cur_v = cur_v->prev;
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
		for (Node* cur = b.head; cur; cur = cur->next) os << cur->value;
		return os;
	}
};

