#include <iostream>
#include <vector>

using namespace std;

template<class T>
void print_vector(const vector<T>&);

template<class R>
void swapInsideQueue(R& x, R& y) {
	R temp = x;
	x = y;
	y = temp;
}

template<class T>
class PriorityQueue {
public:
	bool empty() {
		return heap.empty();
	}
	int size() {
		return heap.size();
	}
	T& top() {
		return heap[0];
	}

	void push(T& newElement) {
		heap.push_back(newElement);
		push_heap();
	}

	void pop() {
		pop_heap();
	}

	void print_tree_aux(int pos, int level) const;

	void print_tree();

private:
	void push_heap();
	void pop_heap();
	void adjust_heap();
	vector<T> heap;

};

template<class T>
void PriorityQueue<T>::push_heap() {
	int position = heap.size() - 1;
	int parent = (position - 1) / 2;
	while (position > 0 && heap[position] > heap[parent]) {
		swap(heap[position], heap[parent]);
		position = parent;
		parent = (position - 1) / 2;
	}
}

template<class T>
void PriorityQueue<T>::pop_heap() {
	int lastpos = heap.size() - 1;
	swap(heap[0], heap[lastpos]);
	heap.pop_back();
	adjust_heap();
}

template<class T>
void PriorityQueue<T>::adjust_heap() {
	int position = 0;
	int heapSize = heap.size();
	while (position < heap.size()) {
		int childpos = position * 2 + 1;
		if (childpos < heapSize) {
			if ((childpos + 1 < heapSize)
					&& heap[childpos + 1] > heap[childpos]) {
				childpos++;
			}
			if (heap[position] > heap[childpos]) {
				return;
			} else {
				swap(heap[position], heap[childpos]);
			}
		}
		position = childpos;
	}
}

template<class T>
void PriorityQueue<T>::print_tree_aux(int pos, int level) const {
	if (heap.empty()) {
		cout << "Empty" << endl;
		return;
	}

	int i = 0;

	for (; i <= level; ++i) {
		cout << ".";
	}

	cout << heap[pos] << endl;

	if (2 * pos + 1 >= heap.size()) {
		return;
	}

	print_tree_aux(2 * pos + 1, level + 1);

	if (2 * pos + 2 < heap.size()) {
		print_tree_aux(2 * pos + 2, level + 1);
	}
}

template<class T>
void print_vector(const vector<T>& vec) {
	cout << endl;
	int i = 0;
	for (; i < vec.size(); ++i) {
		cout << vec[i] << " ";
	}
	cout << endl << endl;
}

template<class T>
void print_tree() {
}
