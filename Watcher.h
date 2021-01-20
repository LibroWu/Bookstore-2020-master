//
// Created by Libro on 2021/1/18.
//

#ifndef SRC_WATCHER_H
#define SRC_WATCHER_H
#include <string>
#include <fstream>
class Watcher {
private:
    std::string file_name;
public:
    Watcher(const std::string& file_name);
    
};


#endif //SRC_WATCHER_H
