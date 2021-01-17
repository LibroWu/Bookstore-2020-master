//
// Created by Libro on 2021/1/15.
//

#ifndef SRC_UNROLLED_LINKED_LIST_HPP
#define SRC_UNROLLED_LINKED_LIST_HPP

#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#define rc(x) reinterpret_cast<char*>(&x)
#define sfc std::fstream::cur
#define debug

#ifdef debug

#include <iostream>

#endif

namespace ULL {
    //should be even
    const int MAXN = 300;

    template<int Key_Len>
    class Unrolled_Linked_List {
    private:

        int block_num, init;
        std::fstream file;

        class block {
            friend Unrolled_Linked_List<Key_Len>;
            friend std::ostream & operator<<(std::ostream &out,const block& obj){
                out<<"number of the data="<<obj.num<<"\n";
                for (int i=0;i<obj.num;++i)
                    out<<"***"<<obj.data[i].key<<"  "<<obj.data[i].pos<<"***\n";
                return out;
            }
        private:
            int next;

            struct Node {
                char key[Key_Len] = {0};
                int pos;
            } data[MAXN];

            int num;
        public:
            block() : next(0), num(0) {}
        };

        const int block_size = sizeof(block_num);

        bool cmp(char a[], char b[]) {
            for (int i = 0; i < Key_Len; ++i)
                if (a[i] < b[i]) return 1;
                else if (a[i] > b[i]) return 0;
            return 1;
        }

    public:
        Unrolled_Linked_List(std::string file_name) : block_num(0) {
            file.open(file_name, std::fstream::binary | std::fstream::out | std::fstream::in);
            file.write(rc(block_num), sizeof(block_num));
            init = sizeof(int);
        }

        ~Unrolled_Linked_List() {
            file.close();
        }

        void insert(char target[Key_Len], int &pos) {
            block tmp;
            int nxt, nxtt, cur, tmp_num;
            char tmp_key[Key_Len];
            //empty
            if (!block_num) {
                ++block_num;
                strcpy(tmp.data[tmp.num++].key,target);
                tmp.data[tmp.num++].pos=pos;
                file.seekp(sizeof(int));
                file.write(rc(tmp), block_size);
                return;
            }
            //skip block number
            cur = init;
            file.seekg(cur);
            file.read(rc(nxt), sizeof(int));
            //find the position to insert
            while (1) {
                if (!nxt) {
                    break;
                }
                file.seekg(nxt);
                file.read(rc(nxtt), sizeof(int));
                file.read(tmp_key, Key_Len);
                if (cmp(tmp_key, target)) {
                    cur = nxt;
                    nxt = nxtt;
                    continue;
                }
                break;
            }
            //insert
            file.seekg(cur);
            file.read(rc(tmp), block_size);
            //add new block
            if (tmp.num == MAXN) {
                block another;
                //change the pointer
                another.next = tmp.next;
                tmp.next = sizeof(int) + block_num * block_size;
                ++block_num;
                int to_insert;
                bool have_inserted = false;
                for (to_insert = 0; to_insert < tmp.num; ++to_insert)
                    if (cmp(target, tmp.data[to_insert].key)) break;
                if (to_insert < MAXN >> 1) {
                    another.num = MAXN >> 1;
                    for (int i = 0; i < another.num; ++i) another.data[i] = tmp.data[i + (MAXN >> 1)];
                    tmp.num = 1 + (MAXN >> 1);
                    for (int i = to_insert + 1; i < tmp.num; ++i) tmp.data[i] = tmp.data[i - 1];
                    strcpy(tmp.data[to_insert].key,target);
                    tmp.data[to_insert].pos=pos;
                }
                else {
                    tmp.num = MAXN >> 1;
                    int j = MAXN >> 1;
                    another.num = (MAXN >> 1) + 1;
                    for (int i = 0; i < another.num; ++i)
                        if (j == to_insert) {
                            strcpy(another.data[i].key,target);
                            another.data[i].pos=pos;
                        }
                        else another.data[i] = tmp.data[j++];
                }
                file.seekp(cur);
                file.write(rc(tmp),block_size);
                file.seekp(sizeof(int) + (block_num - 1) * block_size);
                file.write(rc(another),block_size);

            }//insert in the block
            else {
                int i;
                for (i = 0; i < tmp.num; ++i)
                    if (cmp(target, tmp.data[i].key)) break;
                ++tmp.num;
                for (int j = i + 1; j < tmp.num; ++j) tmp.data[j] = tmp.data[j - 1];
                strcpy(tmp.data[i].key,target);
                tmp.data[i].pos=pos;
                file.seekp(cur);
                file.write(rc(tmp), block_size);
            }
        }

        std::vector<int> &find(char target[]) {
            std::vector<int> *result = new std::vector<int>(0);
            if (!block_num) {
                //todo throw something
                return *result;
            }
            char tmp_key[Key_Len];
            block tmp;
            int cur, nxt, nxtt, pre = 0;
            //skip block number
            cur = init;
            file.seekg(cur);
            file.read(rc(nxt), sizeof(int));
            //find
            while (1) {
                if (!nxt) {
                    break;
                }
                file.seekg(nxt);
                file.read(rc(nxtt), sizeof(int));
                file.read(tmp_key, Key_Len);
                if (!cmp(target, tmp_key)) {
                    pre = cur;
                    cur = nxt;
                    nxt = nxtt;
                    continue;
                }
                break;
            }
            if (pre) {
                file.seekg(pre);
                file.read(rc(tmp), block_size);
                int i = tmp.num - 1;
                for (; i >= 0; --i) {
                    if (tmp.data[i].key != target) break;
                }
                ++i;
                for (int j = i; j < tmp.num; ++j) result->push_back(tmp.data[j].pos);
            }
            while (1) {
                file.seekg(cur);
                file.read(rc(tmp), block_size);
                int i;
                for (i = 0; i < tmp.num; ++i) if (tmp.data[i].key == target) break;
                for (; i < tmp.num; ++i)
                    if (tmp.data[i].key == target) result->push_back(tmp.data[i].pos);
                    else break;
                if (i == tmp.num && nxt) {
                    file.seekg(nxt);
                    file.read(rc(nxtt), sizeof(int));
                    file.read(tmp_key, Key_Len);
                    if (tmp_key == target) {
                        cur = nxt;
                        nxt = nxtt;
                        continue;
                    }
                }
                break;
            }
            return *result;
        }

        void Delete(char target[]) {
            if (!block_num) {
                //todo throw something
                return;
            }
            char tmp_key[Key_Len];
            block tmp;
            int cur, nxt, nxtt, pre = 0;
            //skip block number
            cur = init;
            file.seekg(cur);
            file.read(rc(nxt), sizeof(int));
            //find
            while (1) {
                if (!nxt) {
                    break;
                }
                file.seekg(nxt);
                file.read(rc(nxtt), sizeof(int));
                file.read(tmp_key, Key_Len);
                if (!cmp(target, tmp_key)) {
                    pre = cur;
                    cur = nxt;
                    nxt = nxtt;
                    continue;
                }
                break;
            }
            if (pre) {
                file.seekg(pre);
                file.read(rc(tmp), block_size);
                int i = tmp.num - 1;
                for (; i >= 0; --i) {
                    if (tmp.data[i].key != target) break;
                }
                tmp.num = i + 1;
                file.seekp(pre);
                file.write(rc(tmp), block_size);
            }
            while (1) {
                file.seekg(cur);
                file.read(rc(tmp), block_size);
                int i, j;
                for (i = 0; i < tmp.num; ++i)
                    if (tmp.data[i].key == target) break;
                for (j = i + 1; j < tmp.num; ++j)
                    if (tmp.data[j].key != target) break;
                int k = i;
                for (; j < tmp.num; ++j)
                    tmp.data[k++] = tmp.data[j];
                tmp.num = k;
                //block empty
                if (tmp.num == 0) {
                    if (!pre) { init = nxt; }
                    else {
                        file.seekg(pre);
                        file.write(rc(nxt),sizeof(int));
                    }
                }// not empty
                if (j == tmp.num && nxt) {
                    file.seekg(nxt);
                    file.read(rc(nxtt), sizeof(int));
                    file.read(tmp_key, Key_Len);
                    if (tmp_key == target) {
                        cur = nxt;
                        nxt = nxtt;
                        continue;
                    }
                }
                break;
            }
        }

#ifdef debug

        void show_the_list() {
            if (!block_num) return;
            block tmp;
            int cur=init,cnt=0;
            while (1){
                file.seekg(cur);
                file.read(rc(tmp),block_size);
                std::cout<<"block"<<cnt<<":"<<std::endl;
                std::cout<<tmp<<std::endl;
                if (!tmp.next) break;
                cur=tmp.next;
            }
        }
#endif
    };
}
#endif //SRC_UNROLLED_LINKED_LIST_HPP
