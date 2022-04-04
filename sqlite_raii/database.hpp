#pragma once

/*************************************************************************
 * Copyright © 2018 Hudyaev Alexy <hudyaev.alexy@gmail.com>
 * This file is part of sqlite_raii.
 * Distributed under the MIT License.
 * See accompanying file LICENSE (at the root of this repository)
 *************************************************************************/

#include <string>
#include "sqlite3.h"

namespace sqlite_raii {

class database final {
	mutable ::sqlite3* db = nullptr;
public:
	database(std::string name);
	~database() noexcept ;

	sqlite3* raw() ;
	int error_code() const ;

	void exec(std::string q) ;
};

} // namespace sqlite_raii
