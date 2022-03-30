/*************************************************************************
 * Copyright © 2018 Hudyaev Alexy <hudyaev.alexy@gmail.com>
 * This file is part of sqlitew.
 * Distributed under the MIT License.
 * See accompanying file LICENSE (at the root of this repository)
 *************************************************************************/

#include <iostream>
#include "stmt.hpp"
#include "exceptions.hpp"

sqlitew::stmt::stmt(database* db, std::string query)
    : db(db)
{
	int r = sqlite3_prepare_v2(this->db->raw(), query.c_str(), query.size(), &rst, NULL);
	error::throw_if( r );
}

sqlitew::stmt::~stmt() noexcept
{
	sqlite3_finalize(rst);
}

sqlite3_stmt* sqlitew::stmt::raw()
{
	return rst;
}

void sqlitew::stmt::exec()
{
	query q(this);
	q.step();
}

bool sqlitew::query::is_null(std::size_t ind) const
{
	return sqlite3_column_bytes(st->raw(), ind) == 0;
}

sqlitew::query::query(stmt* st)
    : st(st)
{
}

sqlitew::query::~query() noexcept
{
	sqlite3_reset(st->raw());
}

bool sqlitew::query::step()
{
	int code = sqlite3_step(st->raw());
	if(code == SQLITE_DONE) return false;
	if(code == SQLITE_ROW) return true;
	error::throw_if(code);
	return false;
}

void sqlitew::query::bind(std::size_t ind)
{
	sqlite3_bind_null(st->raw(), ind+1);
}

void sqlitew::query::bind_double(std::size_t ind, double val)
{
	sqlite3_bind_double(st->raw(), ind+1, val);
}

void sqlitew::query::bind(std::size_t ind, int64_t val)
{
	sqlite3_bind_int64(st->raw(), ind+1, val);
}

void sqlitew::query::bind(std::size_t ind, std::string val)
{
	auto& iv = texts.emplace_back(std::move(val));
	sqlite3_bind_text(st->raw(), ind+1, iv.data(), iv.size(), [](void*){} );
}
