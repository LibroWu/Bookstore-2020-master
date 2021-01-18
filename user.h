//
// Created by Libro on 2021/1/18.
//

#ifndef SRC_USER_H
#define SRC_USER_H
#include <string>
#include "Apollo.h"
class Base{
public:
    void su(std::string user_id,std::string passwd="");
    void register_(std::string user_id,std::string passwd,std::string name);
};

class Kara: public Base{
public:
    void change_passwd(std::string user_id,std::string new_passwd,std::string old_passwd="");
    void logout();
    void show();
};

class Conner: public Kara{
public:
    virtual void useradd(std::string user_id,std::string passwd,int level,std::string name);
    void select(std::string ISBN);
    void modify();
    void import();
};

class Markus: public Conner{
public:
    Markus();
    ~Markus();
    void Delete(std::string user_id);
    virtual void useradd(std::string user_id,std::string passwd,int level,std::string name);
    void show_finance();
};


#endif //SRC_USER_H
