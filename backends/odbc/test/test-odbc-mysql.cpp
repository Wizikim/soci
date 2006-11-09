//
// Copyright (C) 2004-2006 Maciej Sobczak, Stephen Hutton, David Courtney
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "soci.h"
#include "soci-odbc.h"
#include "common-tests.h"
#include <iostream>
#include <string>
#include <cassert>
#include <ctime>
#include <cmath>

using namespace SOCI;
using namespace SOCI::tests;

std::string connectString;
BackEndFactory const &backEnd = odbc;

// DDL Creation objects for common tests
struct TableCreator1 : public TableCreatorBase
{
    TableCreator1(Session& session)
        : TableCreatorBase(session) 
    {
        session << "create table soci_test(id integer, val integer, c char, "
                 "str varchar(20), sh int2, ul numeric(20), d float8, "
                 "tm datetime, i1 integer, i2 integer, i3 integer, " 
                 "name varchar(20))";
    }
};

struct TableCreator2 : public TableCreatorBase
{
    TableCreator2(Session& session)
        : TableCreatorBase(session)
    {
        session  << "create table soci_test(num_float float8, num_int integer,"
                     " name varchar(20), sometime datetime, chr char)";
    }
};

struct TableCreator3 : public TableCreatorBase
{
    TableCreator3(Session& session)
        : TableCreatorBase(session)
    {
        session << "create table soci_test(name varchar(100) not null, "
            "phone varchar(15))";
    }
};

//
// Support for SOCI Common Tests
//

class TestContext : public TestContextBase
{
public:
    TestContext(BackEndFactory const &backEnd, 
                std::string const &connectString)
        : TestContextBase(backEnd, connectString) {}

    TableCreatorBase* tableCreator1(Session& s) const
    {
        return new TableCreator1(s);
    }

    TableCreatorBase* tableCreator2(Session& s) const
    {
        return new TableCreator2(s);
    }

    TableCreatorBase* tableCreator3(Session& s) const
    {
        return new TableCreator3(s);
    }

    std::string toDateTime(std::string const &dateString) const
    {
        return "\'" + dateString + "\'";
    }

};

int main(int argc, char** argv)
{
    if (argc == 2)
    {
        connectString = argv[1];
    }
    else
    {
        connectString = "FILEDSN=./test-mysql.dsn";
    }
    try
    {
        TestContext tc(backEnd, connectString);
        CommonTests tests(tc);
        tests.run();
    }
    catch (SOCI::ODBCSOCIError const & e)
    {
        std::cout << "ODBC Error Code: " << e.odbcErrorCode() << std::endl
                  << "Native Error Code: " << e.nativeErrorCode() << std::endl
                  << "SOCI Message: " << e.what() << std::endl
                  << "ODBC Message: " << e.odbcErrorMessage() << std::endl;
    }
    catch (std::exception const & e)
    {
        std::cout << "STD::EXECEPTION " << e.what() << '\n';
    }
}