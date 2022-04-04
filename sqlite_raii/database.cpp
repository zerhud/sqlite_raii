/*************************************************************************
 * Copyright © 2018 Hudyaev Alexy <hudyaev.alexy@gmail.com>
 * This file is part of sqlite_raii.
 * Distributed under the MIT License.
 * See accompanying file LICENSE (at the root of this repository)
 *************************************************************************/

#include "database.hpp"
#include "exceptions.hpp"
#include "stmt.hpp"

sqlite_raii::database::database(std::string name)
{
	int r = sqlite3_open( name.c_str(), &db );
	error::throw_if( r );
	error::throw_if( *this );
}

sqlite_raii::database::~database() noexcept
{
	sqlite3_close(db);
}

sqlite3* sqlite_raii::database::raw()
{
	return db;
}

int sqlite_raii::database::error_code() const
{
	return sqlite3_errcode(db);
}

void sqlite_raii::database::exec(std::string q)
{
	stmt st(this, q);
	st.exec();
}
