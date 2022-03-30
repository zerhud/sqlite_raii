/*************************************************************************
 * Copyright © 2018 Hudyaev Alexy <hudyaev.alexy@gmail.com>
 * This file is part of sqlitew.
 * Distributed under the MIT License.
 * See accompanying file LICENSE (at the root of this repository)
 *************************************************************************/

#include "database.hpp"
#include "exceptions.hpp"
#include "stmt.hpp"

sqlitew::database::database(std::string name)
{
	int r = sqlite3_open( name.c_str(), &db );
	error::throw_if( r );
	error::throw_if( *this );
}

sqlitew::database::~database() noexcept
{
	sqlite3_close(db);
}

sqlite3* sqlitew::database::raw()
{
	return db;
}

int sqlitew::database::error_code() const
{
	return sqlite3_errcode(db);
}

void sqlitew::database::exec(std::string q)
{
	stmt st(this, q);
	st.exec();
}
