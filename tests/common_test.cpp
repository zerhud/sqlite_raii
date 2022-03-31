#define BOOST_TEST_DYN_LINK    
#define BOOST_TEST_MODULE common

#include <boost/test/unit_test.hpp>
#include "sqlitew/stmt.hpp"

BOOST_AUTO_TEST_SUITE(sqlitew)
BOOST_AUTO_TEST_SUITE(common)

BOOST_AUTO_TEST_CASE(create)
{
	BOOST_CHECK_NO_THROW( sqlitew::database{":memory:"} );
	sqlitew::database db{":memory:"};
	BOOST_TEST( db.raw() != nullptr );
	BOOST_TEST( db.error_code() == 0 );
}

BOOST_AUTO_TEST_CASE(create_table)
{
	sqlitew::database db{":memory:"};
	sqlitew::stmt st(&db, "CREATE TABLE foo( a int, b int );");
	BOOST_CHECK_NO_THROW( st.exec() );
	BOOST_CHECK_NO_THROW( db.exec("CREATE TABLE bar ( a int );") );
	BOOST_CHECK_THROW( db.exec("CREATE TABLE bar ( a int );"), std::exception );
}

BOOST_AUTO_TEST_CASE(bind)
{
	sqlitew::database db{":memory:"};
	db.exec("CREATE TABLE foo( a int, b int )");
	sqlitew::stmt st(&db, "INSERT INTO foo (a,b) VALUES(?,?)");
	sqlitew::query q(&st);
	q.bind(0, 1);
	q.bind(1, 2);
	BOOST_TEST( q.step() == false);
}

BOOST_AUTO_TEST_CASE(set_get)
{
	sqlitew::database db{":memory:"};
	db.exec("CREATE TABLE foo( a int, b text )");
	{
		sqlitew::stmt st(&db, "INSERT INTO foo (a,b) VALUES(?,?)");
		sqlitew::query q(&st);
		q.bind(0, 1);
		q.bind(1, "test");
		BOOST_TEST( q.step() == false );
	}
	{
		sqlitew::stmt st(&db, "SELECT * FROM foo WHERE a=?");
		sqlitew::query q(&st);
		q.bind(0, 1);
		BOOST_TEST( q.step() == true );
		BOOST_TEST(q.get<std::string>(1).value() == "test");
		BOOST_TEST( q.step() == false );
	}
	{
		sqlitew::stmt st(&db, "SELECT * FROM foo WHERE b=?");
		sqlitew::query q(&st);
		q.bind(0, "test");
		BOOST_TEST( q.step() == true );
		BOOST_TEST(q.get<std::int64_t>(0).value() == 1);
		BOOST_TEST( q.step() == false );
	}
}

BOOST_AUTO_TEST_SUITE_END() // common
BOOST_AUTO_TEST_SUITE_END() // sqlitew
