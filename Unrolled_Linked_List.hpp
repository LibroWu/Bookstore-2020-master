//
// Created by Libro on 2021/1/15.
//

#ifndef SRC_UNROLLED_LINKED_LIST_HPP
#define SRC_UNROLLED_LINKED_LIST_HPP

#include <fstream>
#include <string>
#include <cstring>

#define rc(x) reinterpret_cast<char*>(&x)
#define sfc std::fstream::cur

namespace ULL {
    const int MAXN = 300;

    template<int Key_Len>
    class Unrolled_Linked_List {
    private:

        int block_num;
        std::fstream file;

        class block {
            friend Unrolled_Linked_List<Key_Len>;
        private:
            int next;

            class Node {
            public:
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
        }

        ~Unrolled_Linked_List() {
            file.close();
        }

        void insert(char target[], int &pos) {
            block tmp;
            int nxt, nxtt, cur, tmp_num;
            char tmp_key[Key_Len];
            //empty
            if (!block_num) {
                ++block_num;
                tmp.data[tmp.num++] = {target, pos};
                file.seekp(sizeof(int));
                file.write(rc(tmp), block_size);
                return;
            }
            //skip block number
            cur = sizeof(int);
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
                bool have_insert = false;
                int i, j;
                for (j = i = 0; i < MAXN >> 1;) {
                    if (have_insert||cmp(target, tmp.data[i + (MAXN >> 1)].key)) {
                        another.data[j++] = tmp.data[i++ + (MAXN >> 1)];
                    }else {
                        another.data[j++]={target,pos};
                        have_insert=true;
                    }
                }
                another.num=j;
                if (!have_insert) {
                    tmp.num=(MAXN>>1);
                    int k=0;
                    for (;k<tmp.num;++k)
                        if (cmp(target,tmp.data[k].key)) break;
                    ++tmp.num;
                    for (int kk=k+1;kk<tmp.num;++kk) tmp.data[kk]=tmp.data[kk-1];
                    tmp.data[k]={target,pos};
                }else {
                    tmp.num=MAXN>>1;
                }
                file.seekp(cur);
                file.write(tmp);
                file.seekp(sizeof(int)+(block_num-1)*block_size);
                file.write(another);
            }//insert in the block
            else {
                int i;
                for (i = 0; i < tmp.num; ++i)
                    if (cmp(target, tmp.data[i].key)) break;
                ++tmp.num;
                for (int j = i + 1; j < tmp.num; ++j) tmp.data[j] = tmp.data[j - 1];
                tmp.data[i] = {target, pos};
                file.seekp(cur);
                file.write(rc(tmp), block_size);
            }
        }

        int find(char target[]) {

        }

        void Delete(char target[]) {

        }
    };
}
#endif //SRC_UNROLLED_LINKED_LIST_HPP
