//
// Created by Libro on 2021/1/18.
//

#include "user.h"

Apollo apollo;
Watcher Arya("Nights_watch.file");
ULL::Unrolled_Linked_List<len_hash> ULL_ID("ID.file", "mem_ID.file");
ULL::Unrolled_Linked_List<len_hash> ULL_ISBN("ISBN.file", "mem_ISBN.file");
ULL::Unrolled_Linked_List<len_hash> ULL_name("name.file", "mem_name.file");
ULL::Unrolled_Linked_List<len_hash> ULL_author("author.file", "mem_author.file");
ULL::Unrolled_Linked_List<len_hash> ULL_key("keywords.file", "mem_keywords.file");
std::unordered_map<std::string, int> have_loaded;

void create_file(std::string file_name) {
    std::fstream file(file_name, std::fstream::out);
    file.close();
}

void quit() {
	#ifdef debug
    Base::END();
	#endif
    exit(0);
}

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

#ifdef debug

void print_log() {
    std::cout << "print_log\n";
    std::fstream file("Nights_watch.file");
    int num;
    file.read(rc(num), sizeof(int));
    std::cout << "num=" << num << "\n";
    record tmp;
    for (int i = 0; i < num; ++i) {
        file.seekg(sizeof(int) + i * record_size);
        file.read(rc(tmp), record_size);
        std::cout << tmp.user_name << ' ' << tmp.offset << "\n + " << tmp.sum_profit << " - " << tmp.sum_cost << '\n';
    }
    file.close();
}

void print_book() {
    std::cout << "print_books\n";
    std::fstream file("books.file");
    int num;
    file.read(rc(num), sizeof(int));
    std::cout << "num=" << num << "\n";
    book tmp;
    for (int i = 0; i < num; ++i) {
        file.seekg(sizeof(int) + i * book_size);
        file.read(rc(tmp), book_size);
        std::cout << tmp.ISBN << '\t' << tmp.name << '\t' << tmp.author << '\t' << tmp.keywords << '\n'
                  << std::setprecision(2) << tmp.price << '\t' << tmp.quantity << '\n';
    }
    file.close();
}

void print_user() {
    std::cout << "\nprint_user\n";
    std::fstream file("user.file");
    int num;
    file.read(rc(num), sizeof(int));
    std::cout << "num=" << num << "\n";
    user tmp;
    for (int i = 0; i < num; ++i) {
        file.seekg(sizeof(int) + i * user_size);
        file.read(rc(tmp), user_size);
        std::cout << tmp.id << '\t' << tmp.level << '\t' << tmp.name <<'\t'<<tmp.passwd<< "\n";
    }
    file.close();
}

void Base::END() {
    std::cout << "\n*************************************\n";
    print_log();
    print_book();
    print_user();
    std::freopen("ull_isbn","w",stdout);
    std::cout<<"\nULL_ISBN:\n";
    ULL_ISBN.show_the_list();
    std::fclose(stdout);
    std::freopen("ull_author","w",stdout);
    std::cout<<"\nULL_author:\n";
    ULL_author.show_the_list();
    std::fclose(stdout);
    std::freopen("ull_name","w",stdout);
    std::cout<<"\nULL_name:\n";
    ULL_name.show_the_list();
    std::fclose(stdout);
    std::freopen("ull_key","w",stdout);
    std::cout<<"\nULL_key:\n";
    ULL_key.show_the_list();
    std::fclose(stdout);
    std::freopen("ull_id","w",stdout);
    std::cout<<"\nULL_id:\n";
    ULL_ID.show_the_list();
    std::fclose(stdout);
}

#endif

Conner::Conner() : offset(0) {}

Markus::Markus(const std::string &_user_id, const std::string &_passwd, const std::string &_name) {
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
                    std::cout << "not completed\n";
                    break;//todo report finance
                case 13:
                    std::cout << "not completed\n";
                    break;//todo report employee
                case 14:
                    std::cout << "not completed\n";
                    break;//todo log
                case 15:
                    std::cout << "not completed\n";
                    break;//todo report myself
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
        ULL_ID.initialize(true);
        ULL_ISBN.initialize(true);
        ULL_author.initialize(true);
        ULL_name.initialize(true);
        ULL_key.initialize(true);
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
        Get_Hash(root_name, after_hash);
        ULL_ID.insert(after_hash.c_str(), sizeof(int));
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
#ifdef debug
        std::cout << "error tag1\n";
#endif
        error();
    }
    std::string after_hash;
    int pos;
    Get_Hash(_user_id, after_hash);
    std::vector<int> *result = &ULL_ID.find(after_hash.c_str());
    if (result->size() != 1) {
        delete result;
#ifdef debug
        std::cout << "error tag2\n";
#endif
        error();
    }
    pos = result->operator[](0);
    delete result;
    user tmp;
    std::fstream file("user.file");
    file.seekg(pos);
    file.read(rc(tmp), user_size);
    if (second == eol) {
        strcpy(tmp.passwd, first.c_str());
    }
    else {
        if (strcmp(tmp.passwd, first.c_str()) != 0) {
#ifdef debug
            std::cout << "error tag3\n";
#endif
            error();
        }
        strcpy(tmp.passwd, second.c_str());
    }
    file.seekp(pos);
    file.write(rc(tmp), user_size);
    file.close();
}

Conner::Conner(const std::string &_user_id, const std::string &_passwd, const std::string &_name) : offset(0) {
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
    std::string after_hash;
    Get_Hash(user_id, after_hash);
    std::vector<int> *result = &ULL_ID.find(after_hash.c_str());
    if (result->size()) {
        delete result;
#ifdef debug
        std::cout << "error tag4\n";
#endif
        error();
    }
    delete result;
    user tmp(user_id.c_str(), pswd.c_str(), user_name.c_str(), level);
    file.read(rc(user_num), sizeof(int));
    Get_Hash(user_id, after_hash);
    ULL_ID.insert(after_hash.c_str(), sizeof(int) + user_num * user_size);
    ++user_num;
    file.seekp(0);
    file.write(rc(user_num), sizeof(int));
    file.seekp(sizeof(int) + (user_num-1) * user_size);
    file.write(rc(tmp), user_size);
#ifdef debug
    std::cout << "\n###" << file.fail() << "###\n";
#endif
    file.close();
}

void Base::su(std::stringstream &tokens, int level_cur) {
    std::string user_id, pswd;
    std::fstream file;
    tokens >> user_id >> pswd;
    if (user_id.length() > len_id) error();
    if (pswd.length() > len_pw) error();
    std::string after_hash;
    Get_Hash(user_id, after_hash);
    int offset;
    std::vector<int> *result = &ULL_ID.find(after_hash.c_str());
    int t = result->size();//todo debug
    if (result->size() != 1) {
#ifdef debug
        std::cout<<"\n###"<<result->size()<<'\t'<<user_id<<'\t'<<after_hash<<"###\n";
        std::cout << "error tag5\n";
#endif
        delete result;
        error();
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
#ifdef debug
            std::cout << "error tag6\n";
#endif
            error();
        }
        if (!have_loaded.count(user_id)) {
            have_loaded[user_id] = 1;
        }
        else ++have_loaded[user_id];
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
    }
    else {
        if (strcmp(pswd.c_str(), tmp.passwd) == 0) {
            if (!have_loaded.count(user_id)) {
                have_loaded[user_id] = 1;
            }
            else ++have_loaded[user_id];
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
        }
        else {
#ifdef debug
            std::cout << "error tag7\n";
#endif
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
    Get_Hash(user_id, after_hash);
    std::vector<int> *result = &ULL_ID.find(after_hash.c_str());
    if (result->size()) {
        delete result;
#ifdef debug
        std::cout << "error tag8\n";
#endif
        error();
    }
    delete result;
    user tmp(user_id.c_str(), pswd.c_str(), user_name.c_str(), 1);
    file.read(rc(user_num), sizeof(int));
    Get_Hash(user_id, after_hash);
    ULL_ID.insert(after_hash.c_str(), sizeof(int) + user_num * user_size);
    ++user_num;
    file.seekp(0);
    file.write(rc(user_num), sizeof(int));
    file.seekp(sizeof(int) + (user_num-1) * user_size);
    file.write(rc(tmp), user_size);
#ifdef debug
    std::cout << "\n###" << file.fail() << "###\n";
#endif
    file.close();
}

void Markus::Delete(std::stringstream &tokens) {
    std::string user_id, after_hash;
    tokens >> user_id;
    if (user_id.length() > len_id) error();
    if (!have_loaded.count(user_id) || have_loaded[user_id] == 0) {
        Get_Hash(user_id, after_hash);
        std::vector<int> *result = &ULL_ID.find(after_hash.c_str());
        if (result->size() != 1) {
            delete result;
#ifdef debug
            std::cout << "error tag9\n";
#endif
            error();
        }
        int pos = result->operator[](0);
        delete result;
        ULL_ID.Delete(after_hash.c_str(), pos);
    }
    else {
#ifdef debug
        std::cout << "error tag10\n";
#endif
        error();
    }
}

void Conner::select(std::stringstream &tokens) {
    std::string ISBN, after_hash;
    std::fstream file;
    tokens >> ISBN;
    Get_Hash(ISBN, after_hash);
    std::vector<int> *result = &ULL_ISBN.find(after_hash.c_str());
    if (result->size() == 0) {
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
        Get_Hash(ISBN, after_hash);
        ULL_ISBN.insert(after_hash.c_str(), offset);
        file.close();
    }
    else if (result->size() == 1) {
        offset = result->operator[](0);
    }
    else {
        delete result;
#ifdef debug
        std::cout << "error tag11\n";
#endif
        error();
    }
#ifdef debug
        std::cout<<offset<<"\n";
#endif
    delete result;
}

void Conner::import(std::stringstream &tokens) {
    int quantity_in, quantity_cur;
    double cost;
    tokens >> quantity_in >> cost;
    if (offset == 0 || tokens.fail()) {
#ifdef debug
        std::cout << "error tag12\n";
#endif
        error();
    }
    Arya.add_record(user_id, offset, -cost);
    std::fstream file("books.file");
    file.seekg(offset);
    file.read(rc(quantity_cur), sizeof(int));
    quantity_cur += quantity_in;
    file.seekp(offset);
    file.write(rc(quantity_cur), sizeof(int));
    file.close();
}

void Kara::buy(std::stringstream &tokens) {
    int quantity_buy;
    std::string ISBN;
    tokens >> ISBN >> quantity_buy;
    if (ISBN.length() > len_ISBN) {
#ifdef debug
        std::cout << "error tag13\n";
#endif
        error();
    }
    std::string after_hash;
    Get_Hash(ISBN, after_hash);
    std::vector<int> *result = &ULL_ISBN.find(after_hash.c_str());
    if (result->size() != 1) {
        delete result;
#ifdef debug
        std::cout << "error tag14\n";
#endif
        error();
    }
    int pos = result->operator[](0);
    delete result;
    std::fstream file("books.file");
    file.seekg(pos);
    book tmp;
    file.read(rc(tmp), book_size);
    if (tmp.quantity < quantity_buy) {
#ifdef debug
        std::cout << "error tag15\n";
#endif
        error();
    }
    std::cout << std::setprecision(2) << quantity_buy * tmp.price << '\n';
    tmp.quantity -= quantity_buy;
    file.seekp(pos);
    file.write(rc(tmp), book_size);
    file.close();
    Arya.add_record(user_id, pos, quantity_buy * tmp.price);
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
            Get_Hash(second, after_hash);
            result = &ULL_ISBN.find(after_hash.c_str());
            int pos;
            for (int i = 0; i < result->size(); ++i) {
                pos = result->operator[](i);
                file.seekg(pos);
                file.read(rc(tmp), book_size);
                show_list.push_back(tmp);
            }
            delete result;
        }
        else if (first == "name") {
            Get_Hash(second, after_hash);
            result = &ULL_name.find(after_hash.c_str());
            int pos;
            for (int i = 0; i < result->size(); ++i) {
                pos = result->operator[](i);
                file.seekg(pos);
                file.read(rc(tmp), book_size);
                show_list.push_back(tmp);
            }
            delete result;
        }
        else if (first == "author") {
            Get_Hash(second, after_hash);
            result = &ULL_author.find(after_hash.c_str());
            int pos;
            for (int i = 0; i < result->size(); ++i) {
                pos = result->operator[](i);
                file.seekg(pos);
                file.read(rc(tmp), book_size);
                show_list.push_back(tmp);
            }
            delete result;
        }
        else if (first == "keyword") {
            Get_Hash(second, after_hash);
            result = &ULL_key.find(after_hash.c_str());
            int pos;
            for (int i = 0; i < result->size(); ++i) {
                pos = result->operator[](i);
                file.seekg(pos);
                file.read(rc(tmp), book_size);
                show_list.push_back(tmp);
            }
            delete result;
        }
        else {
            file.close();
#ifdef debug
            std::cout << "error tag16\n";
#endif
            error();
        }
        if (!show_list.empty()) {
            std::sort(show_list.begin(), show_list.end());
            for (auto i = show_list.begin(); i != show_list.end(); ++i) {
                std::cout << i->ISBN << '\t' << i->name << '\t' << i->author << '\t' << i->keywords << '\t'
                          << std::setprecision(2) << i->price << '\t' << i->quantity << '\n';
            }
        }
        else std::cout << '\n';
    }
    else {
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
        }
        else std::cout << "\n";
    }
    file.close();
}

void Conner::modify(std::stringstream &tokens) {
    if (!offset) {
#ifdef debug
        std::cout << "error tag17\n";
#endif
        error();
    }
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
#ifdef debug
                std::cout << "error tag18\n";
#endif
                error();
            }
            Get_Hash(second, after_hash);
            std::vector<int> *result = &ULL_ISBN.find(after_hash.c_str());
            if (!result->empty()) {
                file.close();
                delete result;
#ifdef debug
                std::cout << "error tag19\n";
#endif
                error();
            }
            delete result;
            Get_Hash(tmp.ISBN, after_hash);
            ULL_ISBN.Delete(after_hash.c_str(), offset);
            Get_Hash(second, after_hash);
            ULL_ISBN.insert(after_hash.c_str(), offset);
            strcpy(tmp.ISBN, second.c_str());
            file.seekp(offset);
            file.write(rc(tmp), book_size);
        }
        else if (first == "name") {
            tokens >> second;
            if (second.length() > len_others) {
                file.close();
#ifdef debug
                std::cout << "error tag20\n";
#endif
                error();
            }
            Get_Hash(tmp.name, after_hash);
            ULL_name.Delete(after_hash.c_str(), offset);
            Get_Hash(second, after_hash);
            ULL_name.insert(after_hash.c_str(), offset);
            strcpy(tmp.name, second.c_str());
            file.seekp(offset);
            file.write(rc(tmp), book_size);
        }
        else if (first == "author") {
            tokens >> second;
            if (second.length() > len_others) {
                file.close();
#ifdef debug
                std::cout << "error tag21\n";
#endif
                error();
            }
            Get_Hash(tmp.author, after_hash);
            ULL_author.Delete(after_hash.c_str(), offset);
            Get_Hash(second, after_hash);
            ULL_author.insert(after_hash.c_str(), offset);
            strcpy(tmp.author, second.c_str());
            file.seekp(offset);
            file.write(rc(tmp), book_size);
        }
        else if (first == "keyword") {
            tokens >> second;
            if (second.length() > len_others) {
                file.close();
#ifdef debug
                std::cout << "error tag22\n";
#endif
                error();
            }
            std::stringstream keywords;
            //delete old keywords
            if (!DivideKey(tmp.keywords, keywords)) {
                file.close();
#ifdef debug
                std::cout << "error tag23\n";
#endif
                error();
            }
            std::string one_key;
            while (keywords >> one_key) {
                Get_Hash(one_key, after_hash);
                ULL_key.Delete(after_hash.c_str(), offset);
            }
            //add new keywords
            keywords.clear();
            if (!DivideKey(second, keywords)) {
                file.close();
#ifdef debug
                std::cout << "error tag24\n";
#endif
                error();
            }
            while (keywords >> one_key) {
                Get_Hash(one_key, after_hash);
                ULL_key.insert(after_hash.c_str(), offset);
            }
            strcpy(tmp.keywords, second.c_str());
            file.seekp(offset);
            file.write(rc(tmp), book_size);
        }
        else if (first == "price") {
            double price_new;
            tokens >> price_new;
            tmp.price = price_new;
            file.seekp(offset);
            file.write(rc(tmp), book_size);
        }
        else {
            file.close();
#ifdef debug
            std::cout << "error tag25\n";
#endif
            error();
        }
        tokens >> first;
    }
    file.close();
}

void Markus::show_finance(std::stringstream &tokens) {
    double cost, profit;
    int times;
    tokens >> times;
    Arya.get_finance(cost, profit, times);
    std::cout << std::setprecision(2) << "+ " << profit << " - " << cost << '\n';
}