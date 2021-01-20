//
// Created by Libro on 2021/1/18.
//

#include "Apollo.h"


void GetLine(std::string &s, const char &mark) {
    char c = getchar();
    while (c != mark && c != EOF) {
        s += c;
        c = getchar();
    }
}

bool Divide(const std::string &input, std::string &first, std::string &second) {
    if (input[0] != '-') return 0;
    if (input.find('=') == std::string::npos) return 0;
    unsigned long long pos = input.find('=');
    first = input.substr(1, pos - 1);
    second = input.substr(pos + 1);
    if (first=="name"||first=="author"){
        second=second.substr(1,second.size()-2);
    }
    return 1;
}

bool DivideKey(const std::string& input, std::stringstream &output) {
    int i=1;
    std::string tmp;
    while (input[i]!='\"'){
        if (input[i]=='|'){
            output<<tmp<<' ';
            tmp.clear();
        }else tmp+=input[i];
        ++i;
    }
    output<<tmp;
    return (i+1==input.length());
}

PaperCup &Apollo::listen() {
    std::string s, token;
    PaperCup *tmp = new PaperCup;
    tmp->tokens.clear();
    GetLine(s, '\n');
    std::stringstream S_in(s + ' ' + eol);
    S_in >> token;
    if (token == "su") {
        tmp->command_type = 0;
        S_in >> token;
        tmp->tokens << token << ' ';
        S_in >> token;
        if (token != eol) {
            tmp->tokens << token << ' ';
            S_in >> token;
            if (token != eol) {
                tmp->command_type = -1;
                tmp->tokens << eol;
                return *tmp;
            }
        }
        tmp->tokens << eol;
        return *tmp;
    }
    else if (token == "logout") {
        tmp->command_type = 1;
        S_in >> token;
        if (token != eol)
            tmp->command_type = -1;
        tmp->tokens << eol;
        return *tmp;
    }
    else if (token == "useradd") {
        tmp->command_type = 2;
        for (int i = 0; i < 4; ++i) {
            S_in >> token;
            if (token == eol) {
                tmp->command_type = -1;
                tmp->tokens << eol;
                return *tmp;
            }
            tmp->tokens << token << ' ';
        }
        S_in >> token;
        if (token != eol) {
            tmp->command_type = -1;
            return *tmp;
        }
        tmp->tokens << eol;
        return *tmp;
    }
    else if (token == "register") {
        tmp->command_type = 3;
        for (int i = 0; i < 3; ++i) {
            S_in >> token;
            if (token == eol) {
                tmp->command_type = -1;
                tmp->tokens << eol;
                return *tmp;
            }
            tmp->tokens << token << ' ';
        }
        S_in >> token;
        if (token != eol) {
            tmp->command_type = -1;
            tmp->tokens << eol;
            return *tmp;
        }
        tmp->tokens << eol;
        return *tmp;
    }
    else if (token == "delete") {
        tmp->command_type = 4;
        S_in >> token;
        if (token == eol) {
            tmp->command_type = -1;
            tmp->tokens << eol;
            return *tmp;
        }
        tmp->tokens << token << ' ';
        S_in >> token;
        if (token != eol) {
            tmp->command_type = -1;
            tmp->tokens << eol;
            return *tmp;
        }
        tmp->tokens << eol;
        return *tmp;
    }
    else if (token == "passwd") {
        tmp->command_type = 5;
        int cnt = 0;
        S_in >> token;
        while (token != eol) {
            ++cnt;
            tmp->tokens << token << ' ';
            S_in >> token;
        }
        if (cnt == 2 || cnt == 3) {
            tmp->tokens << eol;
            return *tmp;
        }
        tmp->command_type = -1;
        tmp->tokens << eol;
        return *tmp;
    }
    else if (token == "select") {
        tmp->command_type = 6;
        int cnt = 0;
        S_in >> token;
        while (token != eol) {
            ++cnt;
            tmp->tokens << token << ' ';
            S_in >> token;
        }
        if (cnt == 1) {
            tmp->tokens << eol;
            return *tmp;
        }
        tmp->command_type = -1;
        tmp->tokens << eol;
        return *tmp;
    }
    else if (token == "modify") {
        tmp->command_type = 7;
        S_in >> token;
        int cnt = 0;
        std::string first, second;
        while (token != eol) {
            if (Divide(token, first, second)) {
                ++cnt;
                tmp->tokens << first << ' ' << second << ' ';
            }
            else {
                tmp->command_type = -1;
                tmp->tokens << eol;
                return *tmp;
            }
            S_in >> token;
        }
        if (cnt > 5) tmp->command_type = -1;
        tmp->tokens << eol;
        return *tmp;
    }
    else if (token == "import") {
        tmp->command_type = 8;
        int cnt = 0;
        S_in >> token;
        while (token != eol) {
            ++cnt;
            tmp->tokens << token << ' ';
            S_in >> token;
        }
        if (cnt == 2) {
            tmp->tokens << eol;
            return *tmp;
        }
        tmp->command_type = -1;
        tmp->tokens << eol;
        return *tmp;
    }
        //show and show finance
    else if (token == "show") {
        S_in >> token;
        if (token == eol) {
            tmp->tokens << eol;
            tmp->command_type = 9;
            tmp->tokens << eol;
            return *tmp;
        }
        if (token == "finance") {
            tmp->command_type = 10;
            S_in >> token;
            if (token == eol) {
                tmp->tokens << eol;
                return *tmp;
            }
            tmp->tokens << token << ' ' << eol;
            S_in >> token;
            if (token != eol) tmp->command_type = -1;
            return *tmp;
        }
        std::string first, second;
        if (Divide(token, first, second)) {
            tmp->command_type = 9;
            tmp->tokens << first << ' ' << second << ' ';
        }
        else {
            tmp->command_type = -1;
            tmp->tokens << eol;
            return *tmp;
        }
        S_in >> token;
        if (token != eol) tmp->command_type = -1;
        tmp->tokens << eol;
        return *tmp;
    }
    else if (token == "buy") {
        tmp->command_type = 11;
        int cnt = 0;
        S_in >> token;
        while (token != eol) {
            ++cnt;
            tmp->tokens << token << ' ';
            S_in >> token;
        }
        if (cnt == 2) {
            tmp->tokens << eol;
            return *tmp;
        }
        tmp->command_type = -1;
        tmp->tokens << eol;
        return *tmp;
    }
        //report finance and report employee and report myself
    else if (token == "report") {
        S_in >> token;
        if (token == "finance") tmp->command_type = 12;
        else if (token == "employee") tmp->command_type = 13;
        else if (token == "myself") tmp->command_type = 15;
        else tmp->command_type = -1;
        S_in >> token;
        if (token != eol) tmp->command_type = -1;
        tmp->tokens << eol;
        return *tmp;
    }
    else if (token == "log") {
        S_in >> token;
        tmp->command_type = 14;
        if (token != eol)
            tmp->command_type = -1;
        tmp->tokens << eol;
        return *tmp;
    }
    else if (token=="quit"){
        tmp->command_type=-2;
        tmp->tokens<<eol;
        return *tmp;
    }
    else{
        tmp->command_type = -1;
        tmp->tokens << eol;
        return *tmp;
    }
}