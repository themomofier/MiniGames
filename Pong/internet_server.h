#pragma once
// Copyright William Kerney
// based on:
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using namespace std;
using namespace boost;
using boost::asio::ip::tcp;

//A simple server that just sends a string and closes shop
//Use it like this: Internet_Server(2020,"Hello World!\n");
class Internet_Server {
	static const int BUFFER_SIZE = 1000000;
	asio::io_service io_service;
	tcp::socket socket;
	public:
	Internet_Server() : socket(io_service){}

	void start(int port){
		cout << "Waiting for someone to connect..." << endl;
		try {
			tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
			acceptor.accept(socket);
		} catch (std::exception& e) {
			cerr << e.what() << endl;
		}
	}

	void write(string s){
		system::error_code ignored_error;
		asio::write(socket, boost::asio::buffer(s), asio::transfer_all(), ignored_error);
	}

	string read() {
		try {
			boost::array<char, BUFFER_SIZE> buf;
			system::error_code error;

			size_t len = socket.read_some(asio::buffer(buf), error);

			if (error == asio::error::eof)
				return ""; // Connection closed cleanly by peer.
			else if (error)
				throw system::system_error(error); // Some other error.

			string s(buf.begin(),buf.begin()+len);
			return s;
			//cout.write(buf.data(), len);
		} catch (std::exception& e) {
			cerr << e.what() << endl;
			return "";
		}
	}
};
