//
// Created by Libro on 2021/1/18.
//

#include "user.h"

void creat_file(std::string file_name) {
    std::fstream file(file_name, std::fstream::out);
    file.close();
}

void quit() {
    exit(0);
}

user::user(const char *user_id, const char *pswd, const char *user_name, int user_level) : level(user_level) {
    strcpy(id, user_id);
    strcpy(passwd, pswd);
    strcpy(name, user_name);
}

book::book(const char *isbn, const char *name_, const char *author_, const char *keywords_, const double &price,
           const int quantity):price(price),quantity(quantity) {
    strcpy(ISBN, isbn);
    strcpy(author, author_);
    strcpy(name, name_);
    strcpy(keywords,keywords_);
}
int user::user_num = 0;
int book::book_num = 0;

Markus::Markus(const std::string &_user_id, const std::string &_passwd, const std::string &_name):offset(0) {
    user_id = _user_id;
    passwd = _passwd;
    name = _name;
    while (1) {
        receive = &apollo.listen();
        switch (receive->command_type) {
            case -2:
            case -1:
                std::cout << "Invalid\n";
                break;
            case 0:
                su(receive->tokens, Markus::level);
            case 1:
                //logout
                return;
            case 2:
                useradd(receive->tokens, level);
            case 3:
                register_(receive->tokens);
            case 4:
                Delete(receive->tokens);
            case 5:
                change_passwd(receive->tokens, level);
            case 6:
                select(receive->tokens);
            case 7:
                modify(receive->tokens);
            case 8:
                import(receive->tokens);
            case 9:
                show(receive->tokens);
            case 10:
                show_finance(receive->tokens);
            case 11:
                buy(receive->tokens);
            case 12://report finance
            case 13://report employee
            case 14://log
            case 15:;//report myself
        }
        delete receive;
    }
}

Base::Base() {
    //initialization
    have_loaded.clear();
    std::fstream file("init.file", std::fstream::in);
    user root(root_name.c_str(), default_password.c_str(), root_name.c_str(), 7);
    if (!file.is_open()) {
        creat_file("init.file");
        creat_file("user.file");
        creat_file("books.file");
        creat_file("ISBN.file");
        creat_file("mem_ISBN.file");
        creat_file("ID.file");
        creat_file("mem_ID.file");
        creat_file("name.file");
        creat_file("mem_name.file");
        creat_file("author.file");
        creat_file("mem_author.file");
        creat_file("keywords.file");
        creat_file("mem_keywords.file");
        std::fstream file("user.file");
        ULL_ID.initialize(true);
        ULL_ISBN.initialize(true);
        ULL_author.initialize(true);
        ULL_name.initialize(true);
        ULL_key.initialize(true);
        ++user::user_num;
        file.write(reinterpret_cast<char *>(user::user_num), sizeof(int));
        file.write(reinterpret_cast<char *>(&root), user_size);
        file.close();
        char cid[len_id];
        strcpy(cid, root_name.c_str());
        ULL_ID.insert(cid, sizeof(int));
    }
    else {
        ULL_ID.initialize();
        ULL_ISBN.initialize();
        ULL_author.initialize();
        ULL_name.initialize();
        ULL_key.initialize();
    }
    while (1) {
        receive = &apollo.listen();
        switch (receive->command_type) {
            case -2:
                quit();
                exit(0);
            case 0:
                su(receive->tokens, level);
                break;//su
            case 3:
                register_(receive->tokens);
                break;//register
            default:
                std::cout << "Invalid\n";
        }
    }
}


Kara::Kara(const std::string &_user_id, const std::string &_passwd, const std::string &_name) {
    user_id = _user_id;
    passwd = _passwd;
    name = _name;
    while (1) {
        receive = &apollo.listen();
        switch (receive->command_type) {
            case -2:
                quit();
                break;
            case 0:
                su(receive->tokens, level);
                break;//su
            case 1://logout
                return;
            case 3:
                register_(receive->tokens);
                break;//register
            case 5://passwd
            case 9://show
            case 11://buy
            default:
                std::cout << "Invalid\n";
        }
        delete receive;
    }
}

void Kara::change_passwd(std::stringstream &tokens, int cur_level) {
    std::string _user_id,first,second;
    tokens>>_user_id>>first>>second;
    if (user_id.length()>len_id) return;//todo
    if (first.length()>len_pw) return;//todo
    if (second.length()>len_pw) return;//todo
    if (second==eol && _user_id!=root_name){
        return;
        //todo
    }
    int pos;
    std::vector<int>*result=&ULL_ID.find(_user_id.c_str());
    if (result->size()!=1) {
        delete result;
        return;
        //todo
    }
    pos=result->operator[](0);
    delete result;
    user tmp;
    std::fstream file("user.file");
    file.seekg(pos);
    file.read(rc(tmp),user_size);
    if (second==eol) {
        strcpy(tmp.passwd,first.c_str());
    }else {
        if (strcmp(tmp.passwd,first.c_str())!=0){
            return;
            //todo
        }
        strcpy(tmp.passwd,second.c_str());
    }
    file.seekp(pos);
    file.write(rc(tmp),user_size);
    file.close();
}

Conner::Conner(const std::string &_user_id, const std::string &_passwd, const std::string &_name):offset(0) {
    user_id = _user_id;
    passwd = _passwd;
    name = _name;
    while (1) {
        receive = &apollo.listen();
        switch (receive->command_type) {
            case -2:
                quit();
                break;
            case 0:
                break;//su
            case 1://logout
                return;
            case 2://useradd
            case 3:
                break;//register
            case 5://passwd
            case 6://select
            case 7://modify
            case 8://import
            case 9://show
            case 11://buy
            default:
                std::cout << "Invalid\n";
        }
    }
}

void Conner::useradd(std::stringstream &tokens, int cur_level) {
    std::string user_id, pswd, user_name;
    std::fstream file("user.file");
    char cid[len_id];
    int level, user_num;
    tokens >> user_id >> pswd >> level >> user_name;
    if (tokens.fail()) return;//todo Exceptions
    if (level >= cur_level) return;//todo Exceptions
    if (user_id.length() > len_id) return;
    if (pswd.length() > len_pw) return;
    if (user_name.length() > len_name) return;
    strcpy(cid, user_id.c_str());
    std::vector<int> *result = &ULL_ID.find(cid);
    if (result->size()) {
        delete result;
        return;//todo Exceptions
    }
    delete result;
    user tmp(user_id.c_str(), pswd.c_str(), user_name.c_str(), cur_level);
    file.read(rc(user_num), sizeof(int));
    ULL_ID.insert(cid, sizeof(int) + user_num * user_size);
    ++user_num;
    file.seekp(0);
    file.write(rc(user_num), sizeof(int));
    file.seekp(0, std::fstream::end);
    file.write(rc(tmp), user_size);
    file.close();
}

void Base::su(std::stringstream &tokens, int level_cur) {
    std::string user_id, pswd;
    std::fstream file;
    char cid[len_id];
    tokens >> user_id >> pswd;
    if (user_id.length() > len_id) return;//todo Exceptions
    if (pswd.length() > len_pw) return;//todo
    strcpy(cid, user_id.c_str());
    int offset;
    std::vector<int> *result = &ULL_ID.find(cid);
    if (result->size() != 1) {
        delete result;
        return;//todo
    }
    offset = result->operator[](0);
    delete result;
    user tmp;
    file.open("user.file");
    file.seekg(offset);
    file.read(rc(tmp), user_size);
    file.close();
    if (pswd == eol) {
        if (level_cur <= tmp.level) {
            return;//todo
        }
        if (!have_loaded.count(user_id)){
            have_loaded[user_id]=1;
        }else ++have_loaded[user_id];
        switch (tmp.level) {
            case 1:
                Kara(tmp.id, tmp.passwd, tmp.name);
                break;
            case 3:
                Conner(tmp.id, tmp.passwd, tmp.name);
                break;
            case 7:
                Markus(tmp.id, tmp.passwd, tmp.name);
                break;
        }
    }else {
        if (strcmp(pswd.c_str(),tmp.passwd)==0) {
            if (!have_loaded.count(user_id)){
                have_loaded[user_id]=1;
            }else ++have_loaded[user_id];
            switch (tmp.level) {
                case 1:
                    Kara(tmp.id, tmp.passwd, tmp.name);
                    break;
                case 3:
                    Conner(tmp.id, tmp.passwd, tmp.name);
                    break;
                case 7:
                    Markus(tmp.id, tmp.passwd, tmp.name);
                    break;
            }
        }else {
            return;
            //todo
        }
    }
}

void Base::register_(std::stringstream &tokens) {
    std::string user_id, pswd, user_name;
    std::fstream file("user.file");
    char cid[len_id];
    int user_num;
    tokens >> user_id >> pswd >> user_name;
    if (tokens.fail()) return;//todo Exceptions
    if (user_id.length() > len_id) return;
    if (pswd.length() > len_pw) return;
    if (user_name.length() > len_name) return;
    strcpy(cid, user_id.c_str());
    std::vector<int> *result = &ULL_ID.find(cid);
    if (result->size()) {
        delete result;
        return;//todo Exceptions
    }
    delete result;
    user tmp(user_id.c_str(), pswd.c_str(), user_name.c_str(), 1);
    file.read(rc(user_num), sizeof(int));
    ULL_ID.insert(cid, sizeof(int) + user_num * user_size);
    ++user_num;
    file.seekp(0);
    file.write(rc(user_num), sizeof(int));
    file.seekp(0, std::fstream::end);
    file.write(rc(tmp), user_size);
    file.close();
}

void Markus::Delete(std::stringstream &tokens) {
    std::string user_id;
    tokens >> user_id;
    if (user_id.length() > len_id) return;//todo
    if (!have_loaded.count(user_id)||have_loaded[user_id]==0) {
        char cid[len_id];
        strcpy(cid, user_id.c_str());
        std::vector<int> *result = &ULL_ID.find(cid);
        if (result->size() != 1) {
            delete result;
            return;//todo
        }
        delete result;
        ULL_ID.Delete(cid);
    }else {
        return;
        //todo
    }
}

void Conner::select(std::stringstream &tokens) {
    std::string ISBN;
    std::fstream file;
    tokens>>ISBN;
    std::vector<int>* result=&ULL_ISBN.find(ISBN.c_str());
    if (result->size()==0) {
        file.open("books.file");
        book tmp(ISBN.c_str());
        int book_num;
        file.read(rc(book_num),sizeof(int));
        offset=sizeof(int)+book_size*book_num;
        ++book_num;
        file.seekp(0);
        file.write(rc(book_num),sizeof(int));
        file.seekp(offset);
        file.write(rc(tmp),book_size);
        ULL_ISBN.insert(ISBN.c_str(),offset);
    }else if (result->size()==1){
        offset=result->operator[](0);
    }else {
        delete result;
        return;//todo
    }
    delete result;
}

void Conner::import(std::stringstream &tokens) {
    int quantity_in;
    double cost;
    tokens>>quantity_in>>cost;
    //todo finance
    if (offset==0) {
        return;
        //todo Exceptions
    }
    std::fstream file("books.file");
    file.seekg(offset);

}