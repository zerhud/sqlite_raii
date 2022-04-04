#define BOOST_TEST_DYN_LINK    
#define BOOST_TEST_MODULE common

#include <boost/test/unit_test.hpp>
#include "sqlite_raii/stmt.hpp"

BOOST_AUTO_TEST_SUITE(sqlite_raii)
BOOST_AUTO_TEST_SUITE(common)

BOOST_AUTO_TEST_CASE(create)
{
	BOOST_CHECK_NO_THROW( sqlite_raii::database{":memory:"} );
	sqlite_raii::database db{":memory:"};
	BOOST_TEST( db.raw() != nullptr );
	BOOST_TEST( db.error_code() == 0 );
}

BOOST_AUTO_TEST_CASE(create_table)
{
	sqlite_raii::database db{":memory:"};
	sqlite_raii::stmt st(&db, "CREATE TABLE foo( a int, b int );");
	BOOST_CHECK_NO_THROW( st.exec() );
	BOOST_CHECK_NO_THROW( db.exec("CREATE TABLE bar ( a int );") );
	BOOST_CHECK_THROW( db.exec("CREATE TABLE bar ( a int );"), std::exception );
}

BOOST_AUTO_TEST_CASE(get_src_sql)
{
	sqlite_raii::database db{":memory:"};
	sqlite_raii::stmt st(&db, "CREATE TABLE foo( a int, b int );");
	BOOST_TEST(st.sql() == "CREATE TABLE foo( a int, b int );");
}

BOOST_AUTO_TEST_CASE(bind)
{
	sqlite_raii::database db{":memory:"};
	db.exec("CREATE TABLE foo( a int, b int )");
	sqlite_raii::stmt st(&db, "INSERT INTO foo (a,b) VALUES(?,?)");
	sqlite_raii::query q(&st);
	q.bind(0, 1);
	q.bind(1, 2);
	BOOST_TEST( q.step() == false);
}

BOOST_AUTO_TEST_CASE(set_get)
{
	sqlite_raii::database db{":memory:"};
	db.exec("CREATE TABLE foo( a int, b text, c interger )");
	{
		sqlite_raii::stmt st(&db, "INSERT INTO foo (a,b,c) VALUES(?,?,?)");
		sqlite_raii::query q(&st);
		q.bind(0, 1);
		q.bind(1, "test");
		q.bind(2, 0.5);
		BOOST_TEST( q.step() == false );
	}
	{
		sqlite_raii::stmt st(&db, "SELECT * FROM foo WHERE a=?");
		sqlite_raii::query q(&st);
		q.bind(0, 1);
		BOOST_TEST( q.step() == true );
		BOOST_TEST(q.get<std::string>(1).value() == "test");
		BOOST_TEST( q.step() == false );
	}
	{
		sqlite_raii::stmt st(&db, "SELECT * FROM foo WHERE b=?");
		sqlite_raii::query q(&st);
		q.bind(0, "test");
		BOOST_TEST( q.step() == true );
		BOOST_TEST(q.get<std::int64_t>(0).value() == 1);
		BOOST_TEST( q.step() == false );
	}
	{
		sqlite_raii::stmt st(&db, "SELECT c FROM foo WHERE c=?");
		sqlite_raii::query q(&st);
		q.bind(0, 0.5);
		BOOST_TEST( q.step() == true );
		BOOST_TEST(q.get<double>(0).value() == 0.5);
		BOOST_TEST( q.step() == false );
	}
}

BOOST_AUTO_TEST_SUITE_END() // common
BOOST_AUTO_TEST_SUITE_END() // sqlite_raii
