//
// Created by Libro on 2021/1/18.
//

#ifndef SRC_USER_H
#define SRC_USER_H

#include <fstream>
#include <string>
#include <cstring>
#include "Unrolled_Linked_List.hpp"
#include "Apollo.h"

Apollo apollo;
const int len_id = 30;
const int len_pw = 30;
const int len_name = 30;
const int len_ISBN = 20;
const int len_others = 60;
ULL::Unrolled_Linked_List<len_id> ULL_ID("ID.file", "mem_ID.file");
ULL::Unrolled_Linked_List<len_ISBN> ULL_ISBN("ISBN.file", "mem_ISBN.file");
ULL::Unrolled_Linked_List<len_others> ULL_name("name.file", "mem_name.file");
ULL::Unrolled_Linked_List<len_others> ULL_author("author.file", "mem_author.file");
ULL::Unrolled_Linked_List<len_others> ULL_key("keywords.file", "mem_keywords.file");
const std::string root_name = "root";
const std::string default_password = "sjtu";

class user {
public:
    static int user_num;

    user() = default;

    user(const char *user_id, const char *pswd, const char *user_name, int user_level);

    char id[len_id], passwd[len_pw];
    int level;
    char name[len_name];
};

class book {
public:
    static int book_num;

    book() = default;

    char ISBN[len_ISBN],name[len_others],author[len_others],keywords[len_others];
    double price;
    int quantity;
};

const int user_size = sizeof(user);

void quit();

void creat_file(std::string file_name);

class Base {
private:
    static const int level = 0;
protected:
    PaperCup *receive;
public:
    Base(bool op = false);

    void su(std::stringstream &tokens, int level_now);

    void register_(std::stringstream &tokens);
};

class Kara : public Base {
private:
    static const int level = 1;
protected:
    std::string user_id, passwd, name;
public:

    Kara()=default;

    Kara(const std::string& _user_id,const std::string& _passwd,const std::string& _name);
    void change_passwd(std::stringstream &tokens, int cur_level);

    void show(std::stringstream &tokens);

    void buy(std::stringstream &tokens);
};

class Conner : public Kara {
private:
    static const int level = 3;
    int offset;
public:
    Conner()=default;
    Conner(const std::string& _user_id,const std::string& _passwd,const std::string& _name);
    void useradd(std::stringstream &tokens, int cur_level);

    void select(std::stringstream &tokens);

    void modify(std::stringstream &tokens);

    void import(std::stringstream &tokens);
};

class Markus : public Conner {
private:
    static const int level = 7;
    int offset;
public:
    Markus(const std::string& _user_id,const std::string& _passwd,const std::string& _name);
    void Delete(std::stringstream &tokens);

    void show_finance(std::stringstream &tokens);
};


#endif //SRC_USER_H
