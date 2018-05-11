#include <iostream>
using namespace std;

template <class T>
class BST{

	BST<T>* left = nullptr;  //pointer to left subtree
	BST<T>* right = nullptr; //pointer to right subtree
	T data;
	int size = 0; //Size of 0 indicates empty tree. 
				  //None of a tree's subtrees are ever empty.

	void insert_body(T new_data) {
		if(new_data < data) {
			if(left) left->insert_body(new_data);
			else left = new BST<T>(new_data);
		} else {
			if(right) right->insert_body(new_data);
			else right = new BST<T>(new_data);
		}
		size++;
	}
	
	void print_body() {
		if(left) left->print_body();
		cout << data << " ";
		if(right) right->print_body();
	}

	void print_structure_body(int tabs){
		if(left) left->print_structure_body(tabs + 1);
		cout << string(tabs, '\t') << data << endl;
		if(right) right->print_structure_body(tabs + 1);
	}
	
	bool find_body(T new_data){
		if(new_data == data) return true;
		if(new_data < data){
			if(left) return left->find_body(new_data);
		}else{
			if(right) return right->find_body(new_data);
		}
		return false;
	}
	

	public:
	BST(){} //empty tree constructor
	BST(T new_data) : data(new_data), size(1) {} //tree with one element constructor
	
	//check if size is 0, then call recursive insert
	void insert(T new_data){
		if(size == 0){
			data = new_data;
			size++;
		}else{
			insert_body(new_data);
		}
	}
	
	bool find(T new_data){
		if(size == 0) return false;
		return find_body(new_data);
	}
	
	//print with endl
	void print(){
		if(size == 0){
			cout << "Empty tree\n";
		}else{
			print_body();
			cout << endl;
		}
	}
	
	//print with indents to show tree structure
	void print_structure(){
		if(size == 0){
			cout << "Empty tree\n";
		}else{
			print_structure_body(0);
		}
	}
	
};
