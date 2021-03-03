//
// Created by Libro on 2021/1/18.
//

#ifndef SRC_WATCHER_H
#define SRC_WATCHER_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <ctime>
#include <sstream>
#include "error.h"

#define logs
#ifdef logs
class LOG {
public:
    char id[35],t[25], object[30], content[200], mark[30];
    LOG(const std::string& id, const std::string &T, const std::string &Object, const std::string &Content,
        const std::string &Mark);
    LOG()=default;
};

const int log_size = sizeof(LOG);
#endif
struct record {
    double sum_cost, sum_profit;
    int offset;
    char user_name[31],t[31];
};
const int record_size = sizeof(record);

class Watcher {
private:
    std::string file_name,log_name;
public:
    Watcher(const std::string &file_name,const std::string &log_name);
    void init();
    void add_record(const std::string &user_id, int offset, double change);

    void get_finance(double &cost, double &profit, const int &times );

    void add_log(const std::string &_id,const std::string &Obj, const std::string &Con, const std::string &m);
};


#endif //SRC_WATCHER_H
