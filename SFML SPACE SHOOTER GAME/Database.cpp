#include<Database\Database.h>
#include<string>
#include <stdio.h>
#include<iostream>

sqlite3 *db;
char *zErrMsg = 0;
int rc;
char *sql;
const char* data = "CallBack function called";

/***************************************************************************************
    *    Title:SQLite c++ Tutorial 
    *    Author: Tutorialspoint.com
    *    Date:  01 May 2016
    *    Availability: http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
    *
    ************************************************************************************/

int open() {

	rc = sqlite3_open("Scores.db", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}
}

static int callback(void *data, int argc, char **argv, char **azColName) {
	int i;
	fprintf(stderr, "%s: ", (const char*)data);
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int createTable() {
	sql = "Create TABLE Scores( " \
		"ID INTEGER PRIMARY KEY     AUTOINCREMENT, " \
		"NAME			TEXT	NOT NULL, "	\
		"LEVEL			TEXT	NOT NULL, " \
		"SCORE			INTEGER	);";

	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Table created successfully\n");
	}
	return 0;
}

int insert(std::string nIn, std::string lIn, int sIn) {
	std::string Player = "Player";
	if (nIn == "") {}
	else {
		Player = nIn;
	}

	std::string x = "insert into Scores(NAME, LEVEL, SCORE) values ('" + Player + "','" + lIn + "'," + std::to_string(sIn) + ")";

	char *y;

	sqlite3_stmt *statement;
	y = &x[0];
	int result = 1;

	if (sqlite3_prepare(db, y, -1, &statement, 0) == SQLITE_OK)
	{
		rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
		int res = sqlite3_step(statement);
		result = res;
		sqlite3_finalize(statement);
		fprintf(stdout, "inserted successfully\n");
	}
	else {
		fprintf(stdout, "not inserted \n");
	}
	return result;

	return 0;

}


std::string select() {

	std::string s;
	char *str = "SELECT NAME, LEVEL, SCORE FROM Scores ORDER BY SCORE DESC";
	sqlite3_stmt *statement;

	if (sqlite3_prepare(db, str, -1, &statement, 0) == SQLITE_OK) {
		int ctotal = sqlite3_column_count(statement);
		int res = 0;

		while (1) {
			res = sqlite3_step(statement);

			if (res == SQLITE_ROW) {
				for (int i = 0; i < ctotal; i++) {
					//The values going in + formatting
					s += (char*)sqlite3_column_text(statement, i);
					s += "\t";
				}
				s += "\n";
			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR) {
				std::cout << "done " << std::endl;
				break;
			}
		}
	}
	return s;
}