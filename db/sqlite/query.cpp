/*
	Copyright (c) 2023 Tikhon Kozyrev (tikhon.kozyrev@gmail.com)
*/
#include "db/sqlite/query.hpp"

namespace DB {
	namespace SQLite {
		Query::~Query() {
			close();
		}
		Query::Query(sqlite3_stmt *stmt):h(stmt) {
		}
		bool Query::isValidColumn(size_t index) const {
			size_t  columns = sqlite3_column_count(h);
			return index < columns;
		}
		bool Query::Bind(size_t index, int value) {
			return SQLITE_OK == sqlite3_bind_int(h, index, value);
		}
		bool Query::Bind(size_t index, const std::string &value) {
			return SQLITE_OK == sqlite3_bind_text(h, index, value.c_str(), value.size(), 0);
		}
		bool Query::Exec() {
			bool res = false;
			switch (sqlite3_step(h)) {
				case SQLITE_ROW:
				case SQLITE_DONE: {
						res = true;
						break;
					};
			}
			return res;
		}
		bool Query::Reset() {
			return SQLITE_OK == sqlite3_reset(h);
		}
		bool Query::Next() {
			bool res = false;
			int rc = sqlite3_step(h);
			switch (rc) {
				case SQLITE_ROW: {
						res = true;
						break;
					};
			}
			return res;
		}
		bool Query::ReadInt(size_t index, int &value) {
			bool res = false;
			do {
				if (!isValidColumn(index)) {
					break;
				}
				res = true;
				switch (sqlite3_column_type(h, index)) {
					case SQLITE_INTEGER: {
							value = sqlite3_column_int(h, index);
							break;
						}
					case SQLITE_FLOAT: {
							value = sqlite3_column_double(h, index);
							break;
						}
#ifdef SQLITE3_TEXT
					case SQLITE3_TEXT: {
							value = atoi(reinterpret_cast<const char *>(sqlite3_column_text(h, index)));
							break;
						}
#else
					case SQLITE_TEXT: {
							value = atoi(reinterpret_cast<const char *>(sqlite3_column_text(h, index)));
							break;
						}
#endif
					default: {
							res = false;
						}
				}
			} while (false);
			return res;
		}
		bool Query::ReadString(size_t index, std::string &value) {
			bool res = false;
			do {
				if (!isValidColumn(index)) {
					break;
				}
				res = true;
				switch (sqlite3_column_type(h, index)) {
					case SQLITE_INTEGER: {
							value = std::to_string(sqlite3_column_int(h, index));
							break;
						}
					case SQLITE_FLOAT: {
							value = std::to_string(sqlite3_column_double(h, index));
							break;
						}
#ifdef SQLITE3_TEXT
					case SQLITE3_TEXT: {
							value = std::string(reinterpret_cast<const char *>(sqlite3_column_text(h, index)));
							break;
						}
#else
					case SQLITE_TEXT: {
							value = std::string(reinterpret_cast<const char *>(sqlite3_column_text(h, index)));
							break;
						}
#endif
					default: {
							res = false;
						}
				}
			} while (false);
			return res;
		}
		void Query::close() {
			if (NULL != h) {
				sqlite3_finalize(h);
				h = NULL;
			}
		}
	}
}
