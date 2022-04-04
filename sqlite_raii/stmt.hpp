#pragma once

/*************************************************************************
 * Copyright © 2018 Hudyaev Alexy <hudyaev.alexy@gmail.com>
 * This file is part of sqlite_raii.
 * Distributed under the MIT License.
 * See accompanying file LICENSE (at the root of this repository)
 *************************************************************************/

#include <vector>
#include <optional>
#include "database.hpp"

namespace sqlite_raii {

class stmt final {
	::sqlite3_stmt* rst;
	database* db;
public:
	stmt(database* db, std::string query);
	~stmt() noexcept ;

	::sqlite3_stmt* raw() ;
	void exec() ;
	std::string sql() const ;
};

class query final {
	stmt* st;
	std::vector<std::string> texts;

	bool is_null(std::size_t ind) const ;
public:
	query(stmt* st);
	~query() noexcept ;

	bool step();

	void bind(std::size_t ind);
	void bind(std::size_t ind, std::int64_t val);
	void bind(std::size_t ind, std::string val);
	template<typename T>
	std::enable_if_t<std::is_same_v<T,double>>
	bind(std::size_t ind, T val) { bind_double(ind, val); }

	void bind_double(std::size_t ind, double val);

	template<typename T>
	std::optional<T> get(std::size_t ind) const
	{
		if(is_null(ind)) return std::nullopt;
		if constexpr (std::is_same_v<T, std::string>)
		    return (const char*)sqlite3_column_text(st->raw(), ind);
		else if constexpr( std::is_same_v<T, std::int64_t>)
		    return sqlite3_column_int64(st->raw(), ind);
		else if constexpr( std::is_same_v<T, double>)
		    return sqlite3_column_double(st->raw(), ind);
		else return std::nullopt;
	}
};

} // namespace sqlite_raii
