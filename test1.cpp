//
// Created by Libro on 2021/1/18.
//

#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include "Unrolled_Linked_List.hpp"

using namespace std;

const int Key_Len = 4;
const int base_len = 100;

struct A {
    bool operator<(const A &other) {
        return strcmp(key, other.key) < 0;
    }

    char key[Key_Len];
    int pos;
};

struct B {
    bool operator<(const A &other) {
        return strcmp(key, other.key) < 0;
    }

    char key[Key_Len];
    int pos, op;
};

vector<B> raw;
vector<A> other;
ULL::Unrolled_Linked_List<Key_Len> l("test1.out", true);

int check_vec(int op, const A &tmp) {
    int sum = 0;
    switch (op) {
        case 0:
            other.push_back(tmp);
            return -1;
        case 1:
            for (int i = 0; i < other.size(); ++i)
                if (strcmp(other[i].key, tmp.key) == 0)
                    sum += other[i].pos;
            return sum;
        case 2:
            for (vector<A>::iterator i = other.begin(); i != other.end(); ++i)
                if (strcmp(i->key, tmp.key) == 0)
                    other.erase(i);
            return -2;
    }
}

int check_ull(int op, const A &tmp) {
    int sum = 0;
    vector<int> *result;
    switch (op) {
        case 0:
            l.insert(tmp.key, tmp.pos);
            return -1;
        case 1:
            result = &l.find(tmp.key);
            for (int i = 0; i < result->size(); ++i)
                sum += result->operator[](i);
            delete result;
            return sum;
        case 2:
            l.Delete(tmp.key);
            return -2;
    }
    return 0;
}

int main() {
    int len;
    A tmp;
    B tmp2;
    bool flag = true;
    char tmp_key[Key_Len];
    while (1) {
        srand(time(NULL));
        len = 10000;
        cout<<len<<endl;
        for (int i = 0; i < len; ++i) {
            for (int j = 0; j < Key_Len; ++j) tmp_key[j] = rand() * 26 / (RAND_MAX + 1) + 65;
            strcpy(tmp.key, tmp_key);
            tmp.pos = i;
            int op = rand() * 3 / (RAND_MAX + 1);
            strcpy(tmp2.key, tmp_key);
            tmp2.pos = i;
            tmp2.op = op;
            raw.push_back(tmp2);
            if (check_vec(op, tmp) == check_ull(op, tmp)) continue;
            flag = false;
            break;
        }
        if (1) {
            if (!flag) cout<<"fail!"<<endl;
            else cout<<"accepted"<<endl;
            fstream file("raw.out", fstream::out);
            file<<raw.size()<<'\n';
            for (int i = 0; i < raw.size(); ++i) {
                file << raw[i].op << ' ';
                for (int j = 0; j < Key_Len; ++j) file << raw[i].key[j];
                file << ' ' << raw[i].pos << '\n';
            }
            if (!flag)break;
        }
    }
    return 0;
}