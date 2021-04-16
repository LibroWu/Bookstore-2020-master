//
// Created by Libro on 2021/1/18.
//

#include "Apollo.h"

union int_char4{
    unsigned int is_int;
    char is_char[sizeof(int)];
};

void Get_Hash(const std::string& input,std::string& output){
    output.clear();
    int_char4 sub_hash1={0},sub_hash2={0};
    unsigned int xor_num[4]={1073758344,268960770,16779332,268468481};
    int len=input.length();
    //sub_hash1 197
    for (int i=0;i<len;++i) {
        sub_hash1.is_int=(sub_hash1.is_int<<7)+(sub_hash1.is_int<<6)+(sub_hash1.is_int<<2)+sub_hash1.is_int+input[i];
        sub_hash1.is_int^=xor_num[input[i]%4];
        if (input[i]%7==0) sub_hash1.is_int<<=1;
        if (input[i]%13==0) sub_hash1.is_int>>=1;
    }
    for (int i=0;i<sizeof(int);++i){
        output+=((sub_hash1.is_char[i])?sub_hash1.is_char[i]:input[0]);
    }
    //sub_hash2 157
    for (int i=0;i<len;++i) {
        sub_hash2.is_int=(sub_hash2.is_int<<7)+(sub_hash2.is_int<<4)+(sub_hash2.is_int<<3)+(sub_hash2.is_int<<2)+sub_hash2.is_int+input[i];
        sub_hash2.is_int^=xor_num[3-input[i]%4];
    }
    for (int i=0;i<sizeof(int)-1;++i)
        sub_hash2.is_char[i]^=sub_hash2.is_char[sizeof(int)-1];
    for (int i=0;i<sizeof(int)-1;++i){
        output+=((sub_hash2.is_char[i])?sub_hash2.is_char[i]:input[input.length()-1]);
    }
}
long long Get_Hash(const std::string& input){
    unsigned int xor_num[4]={1073758344,268960770,16779332,268468481};
    int len=input.length();
    long long tmp=0;
    //sub_hash1 197
    for (int i=0;i<len;++i) {
        tmp=(tmp<<7)+(tmp<<6)+(tmp<<2)+tmp+input[i];
        tmp^=xor_num[input[i]%4];
        if (input[i]%7==0) tmp<<=1;
        if (input[i]%13==0) tmp>>=1;
    }
    tmp<<=32;
    long long tmp2=0;
    //sub_hash2 157
    for (int i=0;i<len;++i) {
        tmp2=(tmp2<<7)+(tmp2<<4)+(tmp2<<3)+(tmp2<<2)+tmp2+input[i];
        tmp2^=xor_num[3-input[i]%4];
    }
    tmp+=tmp2;
    return tmp;
}
void GetLine(std::string &s, const char &mark) {
	if (std::cin.fail()||std::cin.eof()) exit(0);
    char c = getchar();
	if (c==EOF) exit(0);
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
    if (first=="name"||first=="author"||first=="keyword"){
        second=second.substr(1,second.size()-2);
    }
    return 1;
}

bool DivideKey(const std::string& input, std::stringstream &output) {
    output.clear();
    int i=0;
    std::string tmp;
    int len=input.length();
    while (i<len){
        if (input[i]=='|'){
            output<<tmp<<' ';
            tmp.clear();
        }else tmp+=input[i];
        ++i;
    }
    output<<tmp;
    return (i==input.length());
}

PaperCup &Apollo::listen() {
    std::string s, token;
    PaperCup *tmp = new PaperCup;
    tmp->tokens.clear();
	while(1) {
    GetLine(s, '\n');
	if (s.empty()) continue;
	break;
	}
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
                if (first=="keyword") {
                    std::stringstream keys;
                    DivideKey(second,keys);
                    std::string one_key;
                    std::vector<std::string> key_vec(0);
                    while (keys>>one_key){
                        for (int i=0;i<key_vec.size();++i)
                            if (key_vec[i]==one_key) {
                                tmp->command_type=-1;
                                tmp->tokens<<eol;
                                return *tmp;
                            }
                        key_vec.push_back(one_key);
                    }
                }
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
            tmp->command_type = 9;
            tmp->tokens << eol;
            return *tmp;
        }
        if (token == "finance") {
            tmp->command_type = 10;
            S_in >> token;
            if (token == eol) {
                tmp->tokens<<-1<<' '<<eol;
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
    else if (token=="quit"||token=="exit"){
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