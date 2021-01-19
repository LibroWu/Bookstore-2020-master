//
// Created by Libro on 2021/1/18.
//

#ifndef SRC_APOLLO_H
#define SRC_APOLLO_H

#include <string>
#include <sstream>
#include <iostream>

const std::string eol = "#@$%";

void GetLine(std::string &s, const char &mark);
bool Divide(const std::string& input,std::string& first,std::string& second);
bool DivideKey(const std::string& input,std::stringstream & output);

class PaperCup {
public:
    //command_type
    //0 for su, tokens has one or two tokens (user_id and passwd(optional))
    //1 for logout, tokens is empty
    //2 for useradd, tokens has four tokens (user_id,passwd,level and name)
    //3 for register, tokens has three tokens (user_id,passwd and name)
    //4 for delete, tokens has one token (user_id)
    //5 for passwd, tokens has two or three tokens (user_id,new_pw,old_pw(optional))
    //6 for select, tokens has one token ISBN
    //7 for modify, tokens has even tokens. eg: "ISBN" will be followed by [ISBN]
    //8 for import, tokens has two tokens. (quantity and cost_price)
    //9 for show, tokens has zero or two tokens, the same as modify.
    //10 for show finance, tokens has zero or one token (time)
    //11 for buy, tokens has two tokens (ISBN and quantity)
    //12 for report finance
    //13 for report employee
    //14 for log
    //15 for report myself
    //-1 for invalid command
    //-2 for quit
    //besides,each tokens will end up with an eol
    int command_type;
    std::stringstream tokens;
};

class Apollo {
    //std::string file_name;
public:
    //Apollo(std::string file_name="empty");
    PaperCup &listen();
};


#endif //SRC_APOLLO_H
