//
// Created by Libro on 2021/1/18.
//

#ifndef SRC_USER_H
#define SRC_USER_H

#include <fstream>
#include <iomanip>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include "Unrolled_Linked_List.hpp"
#include "BPT.hpp"
#include "Apollo.h"
#include "Watcher.h"
#include "error.h"

const int len_id = 30;
const int len_pw = 30;
const int len_name = 30;
const int len_ISBN = 20;
const int len_others = 60;
const int len_hash=7;
const std::string root_name = "root";
const std::string default_password = "sjtu";

class user {
public:
    static int user_num;

    user() = default;

    user(const char *user_id, const char *pswd, const char *user_name, int user_level);
    //bool operator<(const user& other) ;
    char id[len_id+1], passwd[len_pw+1];
    int level;
    char name[len_name+1];
};

class book {
public:
    static int book_num;
    bool operator<(const book& other);
    book() = default;
    explicit book(const char* isbn,const char * name_="",const char * author_="",const char* keywords_="",const double& price=0,const int quantity=0);
    book(const book& other);
    int quantity;
    char ISBN[len_ISBN+1], name[len_others+1], author[len_others], keywords[len_others+1];
    double price;
};

const int user_size = sizeof(user);
const int book_size = sizeof(book);


void quit();
void creat_file(std::string file_name);

class Base {
private:
    static const int level = 0;
protected:
    PaperCup *receive;
public:
    void ferry();

    void su(std::stringstream &tokens, int level_cur);

    void register_(std::stringstream &tokens);
};

class Kara : public Base {
private:
    static const int level = 1;
protected:
    std::string user_id, passwd, name;
public:

    Kara() = default;

    Kara(const std::string &_user_id, const std::string &_passwd, const std::string &_name);

    void change_passwd(std::stringstream &tokens, int cur_level);

    void show(std::stringstream &tokens);

    void buy(std::stringstream &tokens);
};

class Conner : public Kara {
private:
    static const int level = 3;
protected:
    int offset;
    std::string _ISBN;
public:
    Conner();

    Conner(const std::string &_user_id, const std::string &_passwd, const std::string &_name);

    void useradd(std::stringstream &tokens, int cur_level);

    void select(std::stringstream &tokens);

    void modify(std::stringstream &tokens);

    void import(std::stringstream &tokens);

    void report_myself();
};

class Markus : public Conner {
private:
    static const int level = 7;
public:
    Markus(const std::string &_user_id, const std::string &_passwd, const std::string &_name);

    void Delete(std::stringstream &tokens);

    void show_finance(std::stringstream &tokens);

    void report_employee();

    void report_log();

    void report_finance();
};


#endif //SRC_USER_H
