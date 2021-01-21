//
// Created by Libro on 2021/1/18.
//

#ifndef SRC_WATCHER_H
#define SRC_WATCHER_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include "error.h"

struct record {
    double sum_cost, sum_profit;
    int offset;
    char user_name[31];
};
const int record_size = sizeof(record);

class Watcher {
private:
    std::string file_name;
public:
    Watcher(const std::string &file_name);

    void add_record(const std::string &user_id, int offset, double change);

    void get_finace(double &cost, double &profit, const int &times = -1);
};


#endif //SRC_WATCHER_H
