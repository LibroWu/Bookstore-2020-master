//
// Created by Libro on 2021/1/18.
//

#include "user.h"

struct CMP{
    bool operator()(const long long &a,const long long &b){
        return a>b;
    }
};
class whatDoIKnow {
public:
    long long key;
    int sec;

    whatDoIKnow(long long key = 0, int sec = 0) : key(key), sec(sec) {}

    bool operator<(const whatDoIKnow &other) const {
        return (key < other.key || key == other.key && sec < other.sec);
    }

    bool operator==(const whatDoIKnow &other) const {
        return (key==other.key && sec==other.sec);
    }
};
Apollo apollo;
Watcher Arya("Nights_watch.file", "wall.file");
//ULL::Unrolled_Linked_List<len_hash> ULL_ID("ID.file", "mem_ID.file");
BPT<long long, int, 288, 288,CMP> BPT_ID("ID.file", "mem_ID.file");
//ULL::Unrolled_Linked_List<len_hash> ULL_ISBN("ISBN.file", "mem_ISBN.file");
BPT<long long, int ,288, 288> BPT_ISBN("ISBN.file", "mem_ISBN.file");
//ULL::Unrolled_Linked_List<len_hash> ULL_name("name.file", "mem_name.file");
BPT<whatDoIKnow,int,288,288> BPT_name("name.file", "mem_name.file");
//ULL::Unrolled_Linked_List<len_hash> ULL_author("author.file", "mem_author.file");
BPT<whatDoIKnow,int,288,288> BPT_author("author.file", "mem_author.file");
//ULL::Unrolled_Linked_List<len_hash> ULL_key("keywords.file", "mem_keywords.file");
BPT<whatDoIKnow,int,288,288> BPT_key("keywords.file", "mem_keywords.file");
std::unordered_map<std::string, int> have_loaded;

void create_file(std::string file_name) {
    std::fstream file(file_name, std::fstream::out);
    file.close();
}

void quit() {
   // BPT_ID.print();
    exit(0);
}

/*
bool user::operator<(const user &other) {
    if (this->level != other.level)
        return this->level < other.level;
    return strcmp(this->id, other.id);
}*/

user::user(const char *user_id, const char *pswd, const char *user_name, int user_level) : level(user_level) {
    strcpy(id, user_id);
    strcpy(passwd, pswd);
    strcpy(name, user_name);
}

book::book(const char *isbn, const char *name_, const char *author_, const char *keywords_, const double &price,
           const int quantity) : price(price), quantity(quantity) {
    strcpy(ISBN, isbn);
    strcpy(author, author_);
    strcpy(name, name_);
    strcpy(keywords, keywords_);
}

book::book(const book &other) : quantity(other.quantity), price(other.price) {
    strcpy(ISBN, other.ISBN);
    strcpy(name, other.name);
    strcpy(author, other.author);
    strcpy(keywords, other.keywords);
}


bool book::operator<(const book &other) {
    return (strcmp(ISBN, other.ISBN) < 0);
}

int user::user_num = 0;
int book::book_num = 0;

Conner::Conner() : offset(0) {}

Markus::Markus(const std::string &_user_id, const std::string &_passwd, const std::string &_name) {
#ifdef logs
    Arya.add_log(_user_id, "User:" + _user_id, "log in", "Successfully");
#endif
    user_id = _user_id;
    passwd = _passwd;
    name = _name;
    while (1) {
        receive = &apollo.listen();
        try {
            switch (receive->command_type) {
                case -2:
                    quit();
                case -1:
                    std::cout << "Invalid\n";
                    break;
                case 0:
                    su(receive->tokens, Markus::level);
                    break;
                case 1:
                    //logout
                    --have_loaded[user_id];
                    return;
                case 2:
                    useradd(receive->tokens, level);
                    break;
                case 3:
                    register_(receive->tokens);
                    break;
                case 4:
                    Delete(receive->tokens);
                    break;
                case 5:
                    change_passwd(receive->tokens, level);
                    break;
                case 6:
                    select(receive->tokens);
                    break;
                case 7:
                    modify(receive->tokens);
                    break;
                case 8:
                    import(receive->tokens);
                    break;
                case 9:
                    show(receive->tokens);
                    break;
                case 10:
                    show_finance(receive->tokens);
                    break;
                case 11:
                    buy(receive->tokens);
                    break;
                case 12:
                    report_finance();
                    break;
                case 13:
                    report_employee();
                    break;
                case 14:
                    report_log();
                case 15:
                    report_myself();
                    break;
            }
        } catch (ErrorException) {
            std::cout << "Invalid\n";
        }
        delete receive;
    }
}

void Base::ferry() {
    //initialization
    std::cout.setf(std::ios::fixed);
    have_loaded.clear();
    std::fstream file("init.file", std::fstream::in);
    user root(root_name.c_str(), default_password.c_str(), root_name.c_str(), 7);
    if (!file.is_open()) {
        create_file("init.file");
        create_file("user.file");
        create_file("books.file");
        create_file("ISBN.file");
        create_file("mem_ISBN.file");
        create_file("ID.file");
        create_file("mem_ID.file");
        create_file("name.file");
        create_file("mem_name.file");
        create_file("author.file");
        create_file("mem_author.file");
        create_file("keywords.file");
        create_file("mem_keywords.file");
        create_file("Nights_watch.file");
#ifdef logs
        create_file("wall.file");
        file.open("wall.file");
        int x = 0;
        file.write(reinterpret_cast<char *>(&x), sizeof(int));
        file.close();
#endif
        BPT_ID.initialise();
        BPT_ISBN.initialise();
        BPT_name.initialise();
        BPT_author.initialise();
        BPT_key.initialise();
//        ULL_ISBN.initialize(true);
//        ULL_author.initialize(true);
//        ULL_name.initialize(true);
//        ULL_key.initialize(true);
        Arya.init();
        std::fstream file("user.file");
        ++user::user_num;
        file.write(reinterpret_cast<char *>(&user::user_num), sizeof(int));
        file.write(reinterpret_cast<char *>(&root), user_size);
        file.close();
        file.open("books.file");
        file.write(rc(book::book_num), sizeof(int));
        file.close();
        std::string after_hash;
        //Get_Hash(root_name, after_hash);
        BPT_ID.insert(Get_Hash(root_name), sizeof(int));
    } else {
        //ULL_ISBN.initialize();
        //ULL_author.initialize();
        //ULL_name.initialize();
        //ULL_key.initialize();
    }
    while (1) {
        receive = &apollo.listen();
        try {
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
        } catch (ErrorException) {
            std::cout << "Invalid\n";
        }
        delete receive;
    }
}


Kara::Kara(const std::string &_user_id, const std::string &_passwd, const std::string &_name) {
#ifdef logs
    Arya.add_log(_user_id, "User:" + _user_id, "log in", "Successfully");
#endif
    user_id = _user_id;
    passwd = _passwd;
    name = _name;
    while (1) {
        receive = &apollo.listen();
        try {
            switch (receive->command_type) {
                case -2:
                    quit();
                    break;
                case 0:
                    su(receive->tokens, level);
                    break;//su
                case 1://logout
                    --have_loaded[user_id];
                    return;
                case 3:
                    register_(receive->tokens);
                    break;//register
                case 5:
                    change_passwd(receive->tokens, level);
                    break;//passwd
                case 9:
                    show(receive->tokens);
                    break;//show
                case 11:
                    buy(receive->tokens);
                    break;//buy
                default:
                    std::cout << "Invalid\n";
            }
        } catch (ErrorException) {
            std::cout << "Invalid\n";
        }
        delete receive;
    }
}

void Kara::change_passwd(std::stringstream &tokens, int cur_level) {
    std::string _user_id, first, second;
    tokens >> _user_id >> first >> second;
    if (user_id.length() > len_id) error();
    if (first.length() > len_pw) error();
    if (second.length() > len_pw) error();
    if (second == eol && cur_level != 7) {
        error();
    }
    std::string after_hash;
    int pos;
    //Get_Hash(_user_id, after_hash);
    pos=BPT_ID.Find(Get_Hash(_user_id));
//    std::vector<int> *result = &ULL_ID.find(after_hash.c_str());
    if (pos==0) {
        error();
    }
    user tmp;
    std::fstream file("user.file");
    file.seekg(pos);
    file.read(rc(tmp), user_size);
    std::string old(tmp.passwd);
    if (second == eol) {
        strcpy(tmp.passwd, first.c_str());
    } else {
        if (strcmp(tmp.passwd, first.c_str()) != 0) {
            error();
        }
        strcpy(tmp.passwd, second.c_str());
    }
    file.seekp(pos);
    file.write(rc(tmp), user_size);
    file.close();
#ifdef logs
    std::string now(tmp.passwd);
    Arya.add_log(user_id, "User:" + _user_id, "password: " + old + "->" + now, "Successfully");
#endif
}

Conner::Conner(const std::string &_user_id, const std::string &_passwd, const std::string &_name) : offset(0) {
#ifdef logs
    Arya.add_log(_user_id, "User:" + _user_id, "log in", "Successfully");
#endif
    user_id = _user_id;
    passwd = _passwd;
    name = _name;
    while (1) {
        receive = &apollo.listen();
        try {
            switch (receive->command_type) {
                case -2:
                    quit();
                    break;
                case 0:
                    su(receive->tokens, level);
                    break;//su
                case 1://logout
                    --have_loaded[user_id];
                    return;
                case 2://useradd
                    useradd(receive->tokens, level);
                    break;
                case 3:
                    register_(receive->tokens);
                    break;//register
                case 5://passwd
                    change_passwd(receive->tokens, level);
                    break;
                case 6://select
                    select(receive->tokens);
                    break;
                case 7://modify
                    modify(receive->tokens);
                    break;
                case 8://import
                    import(receive->tokens);
                    break;
                case 9://show
                    show(receive->tokens);
                    break;
                case 11://buy
                    buy(receive->tokens);
                    break;
                case 15:
                    report_myself();
                    break;
                default:
                    std::cout << "Invalid\n";
            }
        } catch (ErrorException) {
            std::cout << "Invalid\n";
        }
    }
}

void Conner::useradd(std::stringstream &tokens, int cur_level) {
    std::string user_id, pswd, user_name;
    std::fstream file("user.file");
    int level, user_num;
    tokens >> user_id >> pswd >> level >> user_name;
    if (tokens.fail()) error();
    if (level >= cur_level) error();
    if (user_id.length() > len_id) error();
    if (pswd.length() > len_pw) error();
    if (user_name.length() > len_name) error();
    std::string after_hash;/*
    Get_Hash(user_id, after_hash);
    std::vector<int> *result = &ULL_ID.find(after_hash.c_str());*/
    int pos=BPT_ID.Find(Get_Hash(user_id));
    if (pos) {
        error();
    }
    user tmp(user_id.c_str(), pswd.c_str(), user_name.c_str(), level);
    file.read(rc(user_num), sizeof(int));
    BPT_ID.insert(Get_Hash(user_id), sizeof(int) + user_num * user_size);
    ++user_num;
    file.seekp(0);
    file.write(rc(user_num), sizeof(int));
    file.seekp(sizeof(int) + (user_num - 1) * user_size);
    file.write(rc(tmp), user_size);
    file.close();
#ifdef logs
    Arya.add_log(user_id, "User:" + user_id, "has been added", "Successfully");
#endif
}

void Base::su(std::stringstream &tokens, int level_cur) {
    std::string user_id, pswd;
    std::fstream file;
    tokens >> user_id >> pswd;
    if (user_id.length() > len_id) error();
    if (pswd.length() > len_pw) error();
    std::string after_hash;
    Get_Hash(user_id, after_hash);
    int offset=BPT_ID.Find(Get_Hash(user_id));
    if (offset==0) {
        error();
    }
    user tmp;
    file.open("user.file");
    file.seekg(offset);
    file.read(rc(tmp), user_size);
    file.close();
    if (pswd == eol) {
        if (level_cur <= tmp.level) {
            error();
        }
        if (!have_loaded.count(user_id)) {
            have_loaded[user_id] = 1;
        } else ++have_loaded[user_id];
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
    } else {
        if (strcmp(pswd.c_str(), tmp.passwd) == 0) {
            if (!have_loaded.count(user_id)) {
                have_loaded[user_id] = 1;
            } else ++have_loaded[user_id];
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
        } else {
            error();
        }
    }
}

void Base::register_(std::stringstream &tokens) {
    std::string user_id, pswd, user_name;
    std::fstream file("user.file");
    int user_num;
    tokens >> user_id >> pswd >> user_name;
    if (tokens.fail()) error();
    if (user_id.length() > len_id) error();
    if (pswd.length() > len_pw) error();
    if (user_name.length() > len_name) error();
    std::string after_hash;
    if (BPT_ID.Find(Get_Hash(user_id))) {
        error();
    }
    user tmp(user_id.c_str(), pswd.c_str(), user_name.c_str(), 1);
    file.read(rc(user_num), sizeof(int));
    Get_Hash(user_id, after_hash);
    BPT_ID.insert(Get_Hash(user_id), sizeof(int) + user_num * user_size);
    ++user_num;
    file.seekp(0);
    file.write(rc(user_num), sizeof(int));
    file.seekp(sizeof(int) + (user_num - 1) * user_size);
    file.write(rc(tmp), user_size);
    file.close();
}

void Markus::Delete(std::stringstream &tokens) {
    std::string user_id, after_hash;
    tokens >> user_id;
    if (user_id.length() > len_id) error();
    if (!have_loaded.count(user_id) || have_loaded[user_id] == 0) {
        int pos = BPT_ID.Find(Get_Hash(user_id));
        if (pos==0) {
            error();
        }
        BPT_ID.Delete(Get_Hash(user_id));
#ifdef logs
        Arya.add_log(user_id, "User:" + user_id, "has been deleted", "Successfully");
#endif
    } else {
        error();
    }
}

void Conner::select(std::stringstream &tokens) {
    std::string ISBN, after_hash;
    std::fstream file;
    tokens >> ISBN;
    int pos=BPT_ISBN.Find(Get_Hash(ISBN));
    //Get_Hash(ISBN, after_hash);
    //std::vector<int> *result = &ULL_ISBN.find(after_hash.c_str());
    if (pos==0) {
        file.open("books.file");
        book tmp(ISBN.c_str());
        int book_num;
        file.read(rc(book_num), sizeof(int));
        offset = sizeof(int) + book_size * book_num;
        ++book_num;
        file.seekp(0);
        file.write(rc(book_num), sizeof(int));
        file.seekp(offset);
        file.write(rc(tmp), book_size);
        std::string after_hash;
        BPT_ISBN.insert(Get_Hash(ISBN),offset);
        file.close();
#ifdef logs
        Arya.add_log(user_id, "ISBN:" + ISBN, "has been created", "Successfully");
        _ISBN = ISBN;
#endif
    } else {
        offset = pos;
#ifdef logs
        Arya.add_log(user_id, "ISBN:" + ISBN, "has been selected", "Successfully");
        _ISBN = ISBN;
#endif
    }
}

void Conner::import(std::stringstream &tokens) {
    int quantity_in, quantity_cur;
    double cost;
    tokens >> quantity_in >> cost;
    if (offset == 0 || tokens.fail()) {
        error();
    }
#ifdef logs
    std::string Cost;
    std::stringstream tok;
    tok.clear();
    tok << cost;
    tok >> Cost;
#endif
    Arya.add_record(user_id, offset, -cost);
    std::fstream file("books.file");
    file.seekg(offset);
    file.read(rc(quantity_cur), sizeof(int));
    quantity_cur += quantity_in;
    file.seekp(offset);
    file.write(rc(quantity_cur), sizeof(int));
    file.close();
#ifdef logs
    std::string con;
    tok.clear();
    tok << "quantity: " << quantity_cur - quantity_in << "->" << quantity_cur;
    getline(tok, con);
    Arya.add_log(user_id, _ISBN, con + " at price " + Cost, "successfully");
#endif
}

void Kara::buy(std::stringstream &tokens) {
    int quantity_buy;
    std::string ISBN;
    tokens >> ISBN >> quantity_buy;
    if (ISBN.length() > len_ISBN) {
        error();
    }
    std::string after_hash;
    int pos=BPT_ISBN.Find(Get_Hash(ISBN));
//    Get_Hash(ISBN, after_hash);
//    std::vector<int> *result = &ULL_ISBN.find(after_hash.c_str());
    if (pos==0) error();
    std::fstream file("books.file");
    file.seekg(pos);
    book tmp;
    file.read(rc(tmp), book_size);
    if (tmp.quantity < quantity_buy) {
        error();
    }
    std::cout << std::setprecision(2) << quantity_buy * tmp.price << '\n';
#ifdef logs
    double price = tmp.price;
    int pre_q = tmp.quantity, cur_q = pre_q - quantity_buy;
#endif
    tmp.quantity -= quantity_buy;
    file.seekp(pos);
    file.write(rc(tmp), book_size);
    file.close();
    Arya.add_record(user_id, pos, quantity_buy * tmp.price);
#ifdef logs
    std::string con;
    std::stringstream tok;
    tok << "Is bought at price " << price << " quantity: " << pre_q << "->" << cur_q;
    std::getline(tok, con);
    Arya.add_log(user_id, "ISBN:" + ISBN, con, "Successfully");
#endif
}

void Kara::show(std::stringstream &tokens) {
    std::string first, second, after_hash;
    tokens >> first;
    std::vector<book> show_list(0);
    std::fstream file("books.file");
    if (first != eol) {
        book tmp;
        tokens >> second;
        std::vector<int> *result;
        if (first == "ISBN") {
            int pos=BPT_ISBN.Find(Get_Hash(second));
//            Get_Hash(second, after_hash);
//            result = &ULL_ISBN.find(after_hash.c_str());
            if (pos) {
                file.seekg(pos);
                file.read(rc(tmp), book_size);
                show_list.push_back(tmp);
            }
        } else if (first == "name") {
            //Get_Hash(second, after_hash);
            //result = &ULL_name.find(after_hash.c_str());
            result=BPT_name.multipleFind(whatDoIKnow(Get_Hash(second),-1));
            int pos;
            for (int i = 1; i < result->size(); ++i) {
                pos = result->operator[](i);
                file.seekg(pos);
                file.read(rc(tmp), book_size);
                show_list.push_back(tmp);
            }
            delete result;
        } else if (first == "author") {
            //Get_Hash(second, after_hash);
            //result = &ULL_author.find(after_hash.c_str());
            result=BPT_author.multipleFind(whatDoIKnow(Get_Hash(second),-1));
            int pos;
            for (int i = 1; i < result->size(); ++i) {
                pos = result->operator[](i);
                file.seekg(pos);
                file.read(rc(tmp), book_size);
                show_list.push_back(tmp);
            }
            delete result;
        } else if (first == "keyword") {
            //Get_Hash(second, after_hash);
            //result = &ULL_key.find(after_hash.c_str());
            result=BPT_key.multipleFind(whatDoIKnow(Get_Hash(second),-1));
            int pos;
            for (int i = 1; i < result->size(); ++i) {
                pos = result->operator[](i);
                file.seekg(pos);
                file.read(rc(tmp), book_size);
                show_list.push_back(tmp);
            }
            delete result;
        } else {
            file.close();
            error();
        }

        if (!show_list.empty()) {
            std::sort(show_list.begin(), show_list.end());
            for (auto i = show_list.begin(); i != show_list.end(); ++i) {
                std::cout << i->ISBN << '\t' << i->name << '\t' << i->author << '\t' << i->keywords << '\t'
                          << std::setprecision(2) << i->price << '\t' << i->quantity << '\n';
            }
        } else std::cout << '\n';
    } else {
        int book_num;
        book tmp;
        file.read(rc(book_num), sizeof(int));
        for (int i = 0; i < book_num; ++i) {
            file.read(rc(tmp), book_size);
            show_list.push_back(tmp);
        }
        if (!show_list.empty()) {
            std::sort(show_list.begin(), show_list.end());
            for (auto i = show_list.begin(); i != show_list.end(); ++i) {
                std::cout << i->ISBN << '\t' << i->name << '\t' << i->author << '\t' << i->keywords << '\t'
                          << std::setprecision(2) << i->price << '\t' << i->quantity << '\n';
            }
        } else std::cout << "\n";
    }
    file.close();
#ifdef logs
    Arya.add_log(user_id, first + ":" + second, "show these books", "Successfully");
#endif
}

void Conner::modify(std::stringstream &tokens) {
    if (!offset) {
        error();
    }
#ifdef logs
    std::string Sec;
    std::string pre;
#endif
    std::fstream file("books.file");
    book tmp;
    file.seekg(offset);
    file.read(rc(tmp), book_size);
    std::string first, second, after_hash;
    tokens >> first;
    while (first != eol) {
        if (first == "ISBN") {
            tokens >> second;
            if (second.length() > len_ISBN) {
                file.close();
                error();
            }
            int pos=BPT_ISBN.Find(Get_Hash(second));
//            Get_Hash(second, after_hash);
//            std::vector<int> *result = &ULL_ISBN.find(after_hash.c_str());
            if (pos) {
                file.close();
                error();
            }
#ifdef logs
            pre = tmp.ISBN;
            Sec = second;
#endif
            BPT_ISBN.Delete(Get_Hash(tmp.ISBN));
//            Get_Hash(tmp.ISBN, after_hash);
//            ULL_ISBN.Delete(after_hash.c_str(), offset);
            BPT_ISBN.insert(Get_Hash(second),offset);
//            Get_Hash(second, after_hash);
//            ULL_ISBN.insert(after_hash.c_str(), offset);
            strcpy(tmp.ISBN, second.c_str());
            file.seekp(offset);
            file.write(rc(tmp), book_size);
        } else if (first == "name") {
            tokens >> second;
            if (second.length() > len_others) {
                file.close();
                error();
            }
#ifdef logs
            pre = tmp.name;
            Sec = second;
#endif
            //Get_Hash(tmp.name, after_hash);
            //ULL_name.Delete(after_hash.c_str(), offset);
            long long hasher=Get_Hash(tmp.name);
            BPT_name.Delete(whatDoIKnow(hasher,offset));
            //Get_Hash(second, after_hash);
            //ULL_name.insert(after_hash.c_str(), offset);
            BPT_name.insert(whatDoIKnow(hasher,-1),-1);
            BPT_name.insert(whatDoIKnow(hasher,offset),offset);
            strcpy(tmp.name, second.c_str());
            file.seekp(offset);
            file.write(rc(tmp), book_size);
        } else if (first == "author") {
            tokens >> second;
            if (second.length() > len_others) {
                file.close();
                error();
            }
#ifdef logs
            pre = tmp.author;
            Sec = second;
#endif
            //Get_Hash(tmp.author, after_hash);
            //ULL_author.Delete(after_hash.c_str(), offset);
            long long hasher=Get_Hash(tmp.author);
            BPT_author.Delete(whatDoIKnow(hasher,offset));
            //Get_Hash(second, after_hash);
            //ULL_author.insert(after_hash.c_str(), offset);
            BPT_author.insert(whatDoIKnow(hasher,-1),-1);
            BPT_author.insert(whatDoIKnow(hasher,offset),offset);
            strcpy(tmp.author, second.c_str());
            file.seekp(offset);
            file.write(rc(tmp), book_size);
        } else if (first == "keyword") {
            tokens >> second;
            if (second.length() > len_others) {
                file.close();
                error();
            }
#ifdef logs
            pre = tmp.keywords;
            Sec = second;
#endif
            std::stringstream keywords;
            //delete old keywords
            if (!DivideKey(tmp.keywords, keywords)) {
                file.close();
                error();
            }
            std::string one_key;
            while (keywords >> one_key) {
                //Get_Hash(one_key, after_hash);
                //ULL_key.Delete(after_hash.c_str(), offset);
                BPT_key.Delete(whatDoIKnow(Get_Hash(one_key),offset));
            }
            //add new keywords
            keywords.clear();
            if (!DivideKey(second, keywords)) {
                file.close();
                error();
            }
            while (keywords >> one_key) {
                //Get_Hash(one_key, after_hash);
                //ULL_key.insert(after_hash.c_str(), offset);
                long long hasher= Get_Hash(one_key);
                BPT_key.insert(whatDoIKnow(hasher,-1),-1);
                BPT_key.insert(whatDoIKnow(hasher,offset),offset);
            }
            strcpy(tmp.keywords, second.c_str());
            file.seekp(offset);
            file.write(rc(tmp), book_size);
        } else if (first == "price") {
            double price_new;
            tokens >> price_new;
#ifdef logs
            std::stringstream tt;
            tt.clear();
            tt << tmp.price << ' ' << price_new << '\n';
            tt >> pre;
            tt >> Sec;
#endif
            tmp.price = price_new;
            file.seekp(offset);
            file.write(rc(tmp), book_size);
        } else {
            file.close();
            error();
        }
#ifdef logs
        std::string con;
        std::stringstream ss;
        ss << first << ": " << pre << "->" << Sec << '\n';
        std::getline(ss, con);
        Arya.add_log(user_id, "ISBN:" + _ISBN, con, "Successfully");
#endif
        tokens >> first;
    }
    file.close();
}

void Markus::show_finance(std::stringstream &tokens) {
    double cost, profit;
    int times = 0;
    tokens >> times;
    Arya.get_finance(cost, profit, times);
    std::cout << std::setprecision(2) << "+ " << profit << " - " << cost << '\n';
}

void Conner::report_myself() {
    int n;
    LOG tmp;
    std::fstream out(user_id + "'s_log.md", std::ios::out);
    out << "## " + user_id + "'s work report\n";
    out << "|user_id|time|object|content|mark|\n";
    out << "|---|---|---|---|---|\n";
    std::fstream file("wall.file");
    file.read(rc(n), sizeof(int));
    for (int i = 0; i < n; ++i) {
        file.seekg(i * log_size + sizeof(int));
        file.read(rc(tmp), log_size);
        if (tmp.id == user_id) {
            out << "|" << tmp.id << "|" << tmp.t << "|" << tmp.object << "|" << tmp.content << "|" << tmp.mark << "|\n";
        }
    }
}

void Markus::report_log() {
    int n;
    LOG tmp;
    std::fstream out("log_report.md", std::ios::out);
    out << "|user_id|time|object|content|mark|\n";
    out << "|---|---|---|---|---|\n";
    std::fstream file("wall.file");
    file.read(rc(n), sizeof(int));
    for (int i = 0; i < n; ++i) {
        file.seekg(i * log_size + sizeof(int));
        file.read(rc(tmp), log_size);
        out << "|" << tmp.id << "|" << tmp.t << "|" << tmp.object << "|" << tmp.content << "|" << tmp.mark << "|\n";
    }
    file.close();
    out.close();
}

struct Cmp {
    bool operator()(const user &a, const user &b) {
        if (a.level != b.level)
            return (a.level > b.level);
        return strcmp(a.id, b.id) > 0;
    }
};

void Markus::report_employee() {
    std::fstream A("user.file");
    std::fstream B("wall.file");
    std::fstream out("report_employee.md", std::ios::out);
    out << "# report employee\n";
    std::map<std::string, user> a;
    std::map<user, std::vector<LOG>, Cmp> b;
    a.clear();
    b.clear();
    int n, m;
    user tmp;
    LOG tmpp;
    A.read(rc(n), sizeof(int));
    for (int i = 0; i < n; ++i) {
        A.read(rc(tmp), user_size);
        a[tmp.id] = tmp;
        b[tmp].clear();
    }
    B.read(rc(m), sizeof(int));
    for (int i = 0; i < m; ++i) {
        B.read(rc(tmpp), log_size);
        b[a[tmpp.id]].push_back(tmpp);
    }
    for (auto it = b.begin(); it != b.end(); ++it) {
        out << "## " << it->first.id << "'s work report\n";
        out << "|user_id|time|object|content|mark|\n";
        out << "|---|---|---|---|---|\n";
        for (auto iter = it->second.begin(); iter != it->second.end(); ++iter) {
            out << "|" << iter->id << "|" << iter->t << "|" << iter->object << "|" << iter->content << "|" << iter->mark
                << "|\n";
        }
    }
}

void Markus::report_finance() {
    std::fstream file1("Nights_watch.file"), out("finance_report.md", std::ios::out);
    out << "# finance report\n";
    int n;
    record pre, now;
    file1.read(rc(n), sizeof(int));
    file1.read(rc(pre), record_size);
    out << "|time|cost|profit|sum_cost|sum_profit|\n";
    out << "|---|---|---|---|---|\n";
    out << "|" << pre.t << "|" << pre.sum_cost << "|" << pre.sum_profit << "|" << pre.sum_cost << "|" << pre.sum_profit
        << "|\n";
    for (int i = 1; i < n; ++i) {
        file1.read(rc(now), record_size);
        out << "|" << now.t << "|" << now.sum_cost - pre.sum_cost << "|" << now.sum_profit - pre.sum_profit << "|"
            << now.sum_cost << "|" << now.sum_profit << "|\n";
        memcpy(&pre, &now, record_size);
    }
}
