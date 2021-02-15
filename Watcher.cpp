//
// Created by Libro on 2021/1/18.
//

#include "Watcher.h"

Watcher::Watcher(const std::string &file_name) : file_name(file_name) {}

void Watcher::init() {
    std::fstream file(file_name);
    double tmp=0;
    int num = 1;
    file.write(reinterpret_cast<char *>(&num), sizeof(int));
    file.seekp(sizeof(int));
    file.write(reinterpret_cast<char *>(&tmp), sizeof(double));
    file.write(reinterpret_cast<char *>(&tmp), sizeof(double));
    file.close();
}

void Watcher::add_record(const std::string &user_id, int offset, double change) {
    std::fstream file(file_name);
    int num;
    record tmp;
    strcpy(tmp.user_name, user_id.c_str());
    tmp.offset = offset;
    file.read(reinterpret_cast<char *>(&num), sizeof(int));
    if (num) {
        file.seekg(sizeof(int) + (num - 1) * record_size);
        file.read(reinterpret_cast<char *>(&tmp.sum_cost), sizeof(double));
        file.read(reinterpret_cast<char *>(&tmp.sum_profit), sizeof(double));
        if (change >= 0) {
            tmp.sum_profit += change;
        }
        else tmp.sum_cost -= change;
    }
    else {
        tmp.sum_cost = tmp.sum_profit = 0;
        if (change >= 0) {
            tmp.sum_profit += change;
        }
        else tmp.sum_cost -= change;
    }
    ++num;
    file.seekp(0);
    file.write(reinterpret_cast<char *>(&num), sizeof(int));
    file.seekp(sizeof(int) + (num - 1) * record_size);
    file.write(reinterpret_cast<char *>(&tmp), record_size);
    file.close();
}

void Watcher::get_finance(double &cost, double &profit, const int &times) {
    std::fstream file(file_name);
    int num;
    file.read(reinterpret_cast<char *>(&num), sizeof(int));
    file.seekg(sizeof(int) + (num - 1) * record_size);
    file.read(reinterpret_cast<char *>(&cost), sizeof(double));
    file.read(reinterpret_cast<char *>(&profit), sizeof(double));
    if (times == -1 || num == times) {
        file.close();
        return;
    }
    if (times > num) {
        file.close();
        error();
    }
    double prefix_cost, prefix_profit;
    file.seekg(sizeof(int) + (num - 1 - times) * record_size);
    file.read(reinterpret_cast<char *>(&prefix_cost), sizeof(double));
    file.read(reinterpret_cast<char *>(&prefix_profit), sizeof(double));
    cost -= prefix_cost;
    profit -= prefix_profit;
    file.close();
}