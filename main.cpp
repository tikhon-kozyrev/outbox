/*
	Copyright (c) 2023 Tikhon Kozyrev (tikhon.kozyrev@gmail.com)
*/
#include <iostream>
#include <thread>

#include "sqlitestringmessageoutbox.hpp"

namespace Demo {
	void Outbox(const std::string &dbname) {
		using namespace std;
		SQLiteStringMessageOutbox outbox;

		if (!outbox.Open(dbname)) {
			cout << "Outbox opening failed" << endl;
		}

		std::thread writer([&outbox]() {
			cout << "Start MessageWriter" << endl;

			std::string text = "MSG# ";
			std::string msg;
			for (size_t i = 0; i<10; ++i) {
				msg = text+std::to_string(i);
				if (outbox.Send(msg)) {
					cout << "Message [" << msg << "] sent" << endl;
				} else {
					cout << "Message [" << msg << "] sending error" << endl;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(100*i));
			}
			cout << "Stop MessageWriter" << endl;
		});
		std::thread reader([&outbox]() {
			cout << "Start MessageReader" << endl;
			for (size_t i = 7; i-->0;) {
				std::this_thread::sleep_for(std::chrono::seconds(1));
				cout << "Reading up qo 10 messages..." << endl;
				for (const SQLiteStringMessageOutbox::KeyMessage &km: outbox.GetNext(10)) {
					if (outbox.Done(km.key)) {
						cout << "Message [" << km.value << "] processd" << endl;
					} else {
						cout << "Message [" << km.value << "] processing error" << endl;
					}
				}
				cout << "No messages..." << endl;
			}
			cout << "Stop MessageReader" << endl;
		});
		if (writer.joinable()) {
			writer.join();
		}
		if (reader.joinable()) {
			reader.join();
		}

		outbox.Close();
	}
}

int main(int argc, char *argv[]) {
	using namespace std;

	int res = 1;
	do {
		std::string dbname = ":memory:";
		if (argc > 2) {
			cout << "Usage: " << argv[0] << " [database_name]" << endl;
			break;
		}
		if (2 == argc) {
			dbname = argv[1];
		}
		Demo::Outbox(dbname);
		res = 0;
	} while (false);

	cout << "Done." << endl;
	return res;
}
