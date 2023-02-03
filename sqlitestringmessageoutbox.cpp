/*
	Copyright (c) 2023 Tikhon Kozyrev (tikhon.kozyrev@gmail.com)
*/
#include "sqlitestringmessageoutbox.hpp"

bool SQLiteStringMessageOutbox::create_db() {
	return db.Exec(R"(
				   CREATE TABLE IF NOT EXISTS outbox (
					   id      INTEGER PRIMARY KEY AUTOINCREMENT,
					   message TEXT    NOT NULL)
				  )");
}
bool SQLiteStringMessageOutbox::prepareQueries() {
	bool res = false;
	do {
		if (!(qinsert = db.Prepare("INSERT INTO outbox (message) VALUES (?)"))) {
			break;
		}
		if (!(qselect = db.Prepare("SELECT id, message FROM outbox ORDER BY id ASC LIMIT ?"))) {
			break;
		}
		if (!(qdelete = db.Prepare("DELETE FROM outbox WHERE id = ?"))) {
			break;
		}
		res = true;
	} while (false);
	if (!res) {
		qinsert = nullptr;
		qselect = nullptr;
		qdelete = nullptr;
	}
	return res;
}
SQLiteStringMessageOutbox::SQLiteStringMessageOutbox() {
}
bool SQLiteStringMessageOutbox::Open(const std::string &database) {
	bool res = false;
	do {
		if (db.isOpen()) {
			break;
		}
		if (!db.Open(database)) {
			break;
		}
		if (!create_db()) {
			db.Close();
			break;
		}
		if (!prepareQueries()) {
			db.Close();
			break;
		}
		res = true;
	} while (false);
	return res;
}
void SQLiteStringMessageOutbox::Close() {
	qdelete = nullptr;
	qselect = nullptr;
	qinsert = nullptr;
	db.Close();
}
bool SQLiteStringMessageOutbox::Send(const Message &msg) {
	bool res = false;
	do {
		if (!qinsert->Reset()) {
			break;
		}
		if (!qinsert->Bind(1, msg)) {
			break;
		}
		if (!qinsert->Exec()) {
			break;
		}
		res = true;
	} while (false);
	return res;
}
std::vector<SQLiteStringMessageOutbox::KeyMessage> SQLiteStringMessageOutbox::GetNext(size_t count) {
	std::vector<KeyMessage> res;
	do {
		if (!qselect->Reset()) {
			break;
		}

		if (!qselect->Bind(1, count)) {
			break;
		}
		while (qselect->Next()) {
			KeyMessage km;
			if (!qselect->ReadInt(0, km.key)) {
				continue;
			}
			if (!qselect->ReadString(1, km.value)) {
				continue;
			}
			res.push_back(km);
		}
	} while (false);
	return res;
}
bool SQLiteStringMessageOutbox::Done(const Key &key) {
	bool res = false;
	do {
		if (!qdelete->Reset()) {
			break;
		}

		if (!qdelete->Bind(1, key)) {
			break;
		}
		if (!qdelete->Exec()) {
			break;
		}
		res = true;
	} while (false);
	return res;
}
