/*
	Copyright (c) 2023 Tikhon Kozyrev (tikhon.kozyrev@gmail.com)
*/
#ifndef DB_SQLITE_QUERY_HPP
#define DB_SQLITE_QUERY_HPP

#include <sqlite3.h>
#include <string>

namespace DB {
	namespace SQLite {
		class Query {
			public:
				virtual ~Query();
				bool Bind(size_t index, int value);
				bool Bind(size_t index, const std::string &value);
				bool Exec();
				bool Reset();
				bool Next();
				bool ReadInt(size_t index, int &value);
				bool ReadString(size_t index, std::string &value);

			protected:
				Query(sqlite3_stmt *stmt);

			private:
				bool isValidColumn(size_t) const;
				void close();
				sqlite3_stmt *h;
		};
	}
}
#endif
