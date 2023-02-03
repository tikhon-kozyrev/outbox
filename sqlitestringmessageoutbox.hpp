/*
	Copyright (c) 2023 Tikhon Kozyrev (tikhon.kozyrev@gmail.com)
*/
#ifndef SQLITESTRINGMESSAGEOUTBOX_HPP
#define SQLITESTRINGMESSAGEOUTBOX_HPP

#include "db/sqlite/all.hpp"
#include "patterns/outbox.hpp"
#include <memory>

class SQLiteStringMessageOutbox: public Patterns::TOutbox<int, std::string> {
	private:
		using DataBase = DB::SQLite::DataBase;
		using Query = DB::SQLite::Query;
		DataBase db;
		std::shared_ptr<Query> qinsert;
		std::shared_ptr<Query> qselect;
		std::shared_ptr<Query> qdelete;
		bool create_db();
		bool prepareQueries();

	public:
		SQLiteStringMessageOutbox();
		bool Open(const std::string &database) override;
		void Close() override;
		bool Send(const Message &msg) override;
		std::vector<KeyMessage> GetNext(size_t count) override;
		bool Done(const Key &key) override;
};

#endif
