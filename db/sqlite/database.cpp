/*
	Copyright (c) 2023 Tikhon Kozyrev (tikhon.kozyrev@gmail.com)
*/
#include "db/sqlite/database.hpp"

namespace DB {
	namespace SQLite {
		namespace {
			class CQ: public Query {
				public:
					CQ(sqlite3_stmt *h):Query(h) {
					}
			};
			inline static std::shared_ptr<Query> makeQuery(sqlite3_stmt *h) {
				return std::make_shared<CQ>(h);
			}
		}
		std::string DataBase::Error() const {
			return std::string(sqlite3_errmsg(db));
		}
		bool DataBase::Exec(const std::string &sql) {
			return SQLITE_OK == sqlite3_exec(db, sql.c_str(), 0, 0, 0);
		}
		DataBase::DataBase():db(NULL) {
		}
		std::shared_ptr<Query> DataBase::Prepare(const std::string &sql) {
			std::shared_ptr<Query> res = nullptr;
			do {
				sqlite3_stmt *h = NULL;
				if (SQLITE_OK != sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &h, NULL)) {
					break;
				}
				if (NULL == h) {
					break;
				}
				res = makeQuery(h);
			} while (false);
			return res;
		}
		bool DataBase::Open(const std::string &dbname) {
			std::lock_guard<std::mutex> lock(mtx);
			close();
			return open(dbname);
		}
		bool DataBase::isOpen() const {
			std::lock_guard<std::mutex> lock(mtx);
			return isopen();
		}
		void DataBase::Close() {
			std::lock_guard<std::mutex> lock(mtx);
			close();
		}
		DataBase::~DataBase() {
			std::lock_guard<std::mutex> lock(mtx);
			close();
		}
		bool DataBase::isopen() const {
			return NULL != db;
		}
		void DataBase::close() {
			if (isopen()) {
				sqlite3_close(db);
				db = NULL;
			}
		}
		bool DataBase::open(const std::string &dbname) {
			return SQLITE_OK == sqlite3_open(dbname.c_str(), &db);
		}
	}
}
