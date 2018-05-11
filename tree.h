#include <iostream>
#include <fstream>
#include <sstream>
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
	
	bool find_body(T new_data){
		if(new_data == data) return true;
		if(new_data < data){
			if(left) return left->find_body(new_data);
		}else{
			if(right) return right->find_body(new_data);
		}
		return false;
	}
	
	void save_body(ofstream& filestream) {
		if(right) right->save_body(filestream);
		filestream << data << endl;
		if(left) left->save_body(filestream);
	}
	

	public:
	BST(){} //empty tree constructor
	BST(T new_data) : data(new_data), size(1) {} //tree with one element constructor
	BST(string filename){
		ifstream file(filename);
		int score;
		while(file >> score){
			insert(score);	
		}
		file.close();
	}
	
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
	
	void save(string filename){
		ofstream file(filename);
		save_body(file);
		file.close();
	}

	string to_string(){
		stringstream ss;
		if(right) ss << right->to_string() << " ";
		ss << data << " ";
		if(left) ss << left->to_string() << " ";
		return ss.str();
	}
};
