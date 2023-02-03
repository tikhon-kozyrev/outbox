/*
	Copyright (c) 2023 Tikhon Kozyrev (tikhon.kozyrev@gmail.com)
*/
#ifndef DB_SQLITE_DATABASE_HPP
#define DB_SQLITE_DATABASE_HPP

#include "db/sqlite/query.hpp"
#include <string>
#include <memory>
#include <mutex>

namespace DB {
	namespace SQLite {
		class DataBase {

			public:
				std::string Error() const;
				bool Exec(const std::string &sql);
				DataBase();
				std::shared_ptr<Query> Prepare(const std::string &sql);
				bool Open(const std::string &dbname = ":memory");
				bool isOpen() const;
				void Close();
				virtual ~DataBase();

			private:
				mutable std::mutex mtx;
				sqlite3 *db;
				bool isopen()const;
				void close();
				bool open(const std::string &dbname);
		};
	}
}
#endif
