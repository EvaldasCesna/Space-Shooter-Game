#include<sqlite3.h>
#include<stdlib.h>
#include<stdio.h>
#include<sstream>

// Opens database
int open();
//Create Table Insert
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
//Select
static int callback(void *data, int argc, char **argv, char **azColName);

int createTable();
int insert(std::string nIn, std::string lIn, int sIn);
std::string select();

