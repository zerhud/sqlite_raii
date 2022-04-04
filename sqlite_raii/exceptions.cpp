/*************************************************************************
 * Copyright © 2018 Hudyaev Alexy <hudyaev.alexy@gmail.com>
 * This file is part of sqlite_raii.
 * Distributed under the MIT License.
 * See accompanying file LICENSE (at the root of this repository)
 *************************************************************************/

#include "exceptions.hpp"

using namespace std::literals;

void sqlite_raii::error::throw_if(const database& db)
{
	int code = db.error_code();
	if(code!=0) throw error(db);
}

void sqlite_raii::error::throw_if(int code)
{
	if(code != SQLITE_OK) throw error(code);
}

sqlite_raii::error::error(int code)
    : desc(std::to_string(code) + ": "s + sqlite3_errstr(code))
{
}

sqlite_raii::error::error(const database& db)
    : desc(sqlite3_errstr(db.error_code()))
{
}

const char* sqlite_raii::error::what() const noexcept
{
	return desc.c_str();
}
