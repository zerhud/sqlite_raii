#pragma once

/*************************************************************************
 * Copyright © 2018 Hudyaev Alexy <hudyaev.alexy@gmail.com>
 * This file is part of sqlitew.
 * Distributed under the MIT License.
 * See accompanying file LICENSE (at the root of this repository)
 *************************************************************************/

#include <exception>
#include "database.hpp"

namespace sqlitew {

class error : public std::exception {
	std::string desc;
public:
	static void throw_if(const database& db);
	static void throw_if(int code) ;

	error(int code);
	error(const database& db);
	const char* what() const noexcept override ;
};

} // namespace sqlitew
