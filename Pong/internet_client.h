#pragma once
// Copyright (c) 2017 William Kerney (william.kerney@scccd.edu)
// Adapted from:
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
using namespace std;
using namespace boost;
using boost::asio::ip::tcp;


//Use it like this:
//Internet_Client ic("localhost","2020");
//string s = ic.read();
class Internet_Client {
	static const int BUFFER_SIZE = 1000000;
	boost::asio::io_service io_service; //Handles networking
	tcp::socket socket; //Handles the internet connection

	public:
	//Constructor that takes an ip address and port number for the server to connect to
	Internet_Client(const char* ip, const char* port) : socket(io_service) {
		boost::asio::ip::tcp::resolver resolver(io_service);
		tcp::resolver::query query(ip, port);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;
		system::error_code error = asio::error::host_not_found;
		while (error && endpoint_iterator != end) {
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}
		if (error)
			throw system::system_error(error);
	}

	void write(string s){
		system::error_code ignored_error;
		asio::write(socket, boost::asio::buffer(s), asio::transfer_all(), ignored_error);
	}

	//Reads up to BUFFER_SIZE bytes from server at once
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
