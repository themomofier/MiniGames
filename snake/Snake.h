class Snake {
	int x = 0;
	int y = 0;
	Snake* prev = nullptr;
	Snake* next = nullptr;

	public:
	Snake(int new_x, int new_y) : x(new_x), y(new_y) {}
	int get_x() 					{ return x; 		}
	int get_y() 					{ return y; 		}
	Snake* get_next() 				{ return next; 		}
	Snake* get_prev() 				{ return prev; 		}
	void set_x(int new_x) 			{ x = new_x; 		}
	void set_y(int new_y) 			{ y = new_y; 		}
	void set_next(Snake* new_next) 	{ next = new_next; 	}
	void set_prev(Snake* new_prev) 	{ prev = new_prev; 	}

};

class Body {
	Snake* head = nullptr;
	Snake* tail = nullptr;
	int size = 0;

	public:
	Body() {}

	int get_size() { return size;}

	Snake* get_front() { return head; }
	Snake* get_back() { return tail; }

	void add_front(int x, int y) {
		Snake* new_snake = new Snake(x, y);
		if (head == nullptr) {
			head = new_snake;
			tail = new_snake;
		}
		else {
			new_snake->set_next(head);
			head->set_prev(new_snake);
			head = new_snake;
		}
		size++;
	}

	void add_back(int x, int y) {
		Snake* new_snake = new Snake(x, y);
		if (head == nullptr) {
			head = new_snake;
			tail = new_snake;
		}
		else {
			new_snake->set_prev(tail);
			tail->set_next(new_snake);
			tail = new_snake;
		}
		size++;
	}
	
	void pop_back() {
		Snake* temp = head;
		if (head->get_next() == nullptr) {
			head = nullptr;
			delete temp;
			size--;
		}
		else {
			temp = tail;
			tail = tail->get_prev();
			tail->set_next(nullptr);
			delete temp;
			size--;
		}
	}
};
