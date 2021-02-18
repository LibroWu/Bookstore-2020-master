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
//#define debug


namespace ULL {
    //should be even
    const int MAXN = 400;
    constexpr int HALF_MAXN = MAXN >> 1;
    //if the sum of two adjacent blocks' size
    //is less than the MERGE_THRESHOLD,the two
    //blocks will be merged
    const int MERGE_THRESHOLD = 300;

    template<int Key_Len>
    class Unrolled_Linked_List {
    private:

        int block_num, init;
        std::fstream file, memfile;
        std::string file_name, memfile_name;
        //attention:
        //the storage allocation's minimal size is 4 byte
        //that is,if the length of key is 7 bytes,then sizeof
        //Node will be 12 bytes(8+4);
        struct Node {
            char key[Key_Len + 1] = {0};
            int pos;
        };

        class block {
            friend Unrolled_Linked_List<Key_Len>;
        private:
            int next, num;

        public:
            Node data[MAXN];

            block() : next(0), num(0) {}
        };

        const int block_size = sizeof(block);
        const int node_size = sizeof(Node);

        bool cmp(const char a[], const char b[]) {
            return (strcmp(a, b) < 0);
        }

        bool is_same(const char a[], const char b[]) {
            return (strcmp(a, b) == 0);
        }
        void mem_free(int &cur) {
            int free_num;
            memfile.seekg(0);
            memfile.read(rc(free_num), sizeof(int));
            ++free_num;
            memfile.seekp(free_num * sizeof(int));
            memfile.write(rc(cur), sizeof(int));
            memfile.seekp(0);
            memfile.write(rc(free_num), sizeof(int));
        }

        void mem_allocate(int &free_num, int &address) {
            memfile.seekg(sizeof(int) * free_num);
            memfile.read(rc(address), sizeof(int));
            --free_num;
            memfile.seekp(0);
            memfile.write(rc(free_num), sizeof(int));
        }

        void merge_blocks(const int &cur, int &nxt) {
            block tmp, another;
            mem_free(nxt);
            //merge the two blocks
            file.seekg(cur);
            file.read(rc(tmp), block_size);
            file.read(rc(another), block_size);
            for (int i = tmp.num; i < tmp.num + another.num; ++i)
                tmp.data[i] = another.data[i - tmp.num];
            tmp.num += another.num;
            tmp.next = another.next;
            file.seekp(cur);
            file.write(rc(tmp), block_size);
        }
    public:
        Unrolled_Linked_List(std::string file_name, std::string memory_pool) : block_num(0),
                                                                               file_name(file_name),
                                                                               memfile_name(
                                                                                       memory_pool) {
        }

        //option true for first_open
        void initialize(bool op = false) {
            file.open(file_name, std::fstream::binary | std::fstream::out | std::fstream::in);
            memfile.open(memfile_name, std::fstream::binary | std::fstream::out | std::fstream::in);
            if (op) {
                file.write(rc(block_num), sizeof(int));
                init = 2 * sizeof(int);
                file.write(rc(init), sizeof(int));
                memfile.write(rc(block_num), sizeof(int));
                file.seekg(0);
                file.read(rc(block_num), sizeof(int));
                file.read(rc(init), sizeof(int));
            }
            else {
                file.read(rc(block_num), sizeof(int));
                file.read(rc(init), sizeof(int));
            }

            file.close();
            memfile.close();
        };

        ~Unrolled_Linked_List() {
            if (file.is_open())
                file.close();
            if (memfile.is_open())
                memfile.close();
        }

        void insert(const char *target, const int &pos) {
            file.open(file_name);
            memfile.open(memfile_name);
            block tmp, another;
            Node element;
            int nxt, nxtt, cur, cur_num, nxt_num, free_num;
            //empty
            if (!block_num) {
                memfile.seekg(0);
                memfile.read(rc(free_num), sizeof(int));
                ++block_num;
                file.seekp(0);
                file.write(rc(block_num), sizeof(int));
                strcpy(tmp.data[tmp.num].key, target);
                tmp.data[tmp.num].pos = pos;
                ++tmp.num;
                if (!free_num) {
                    init = sizeof(int) * 2;
                    file.seekp(init);
                    file.write(rc(tmp), block_size);
                }
                else {
                    mem_allocate(free_num, init);
                    file.seekp(init);
                    file.write(rc(tmp), block_size);
                }
                file.close();
                memfile.close();
                return;
            }
            //skip block number
            cur = init;
            file.seekg(cur);
            file.read(rc(nxt), sizeof(int));
            file.read(rc(cur_num), sizeof(int));
            //find the position to insert
            while (1) {
                if (!nxt) {
                    break;
                }
                file.seekg(nxt);
                file.read(rc(nxtt), sizeof(int));
                file.read(rc(nxt_num), sizeof(int));
                file.read(rc(element), sizeof(element));
                if (cmp(element.key, target) || is_same(element.key, target) && element.pos <= pos) {
                    //merge
                    if (cur_num + nxt_num < MERGE_THRESHOLD) {
                        //reuse the storage
                        merge_blocks(cur, nxt);
                        nxt = nxtt;
                    }
                    else {
                        cur = nxt;
                        nxt = nxtt;
                    }
                    continue;
                }
                break;
            }
            //insert
            file.seekg(cur);
            file.read(rc(tmp), block_size);
            //add a new block
            if (tmp.num == MAXN) {
                block another;
                int to_insert;
                bool have_inserted = false;
                for (to_insert = 0; to_insert < tmp.num; ++to_insert)
                    if (cmp(target, tmp.data[to_insert].key) ||
                        is_same(target, tmp.data[to_insert].key) && (pos <= tmp.data[to_insert].pos))
                        break;
                if (to_insert < HALF_MAXN) {
                    another.num = HALF_MAXN;
                    for (int i = 0; i < another.num; ++i) another.data[i] = tmp.data[i + (HALF_MAXN)];
                    tmp.num = 1 + (HALF_MAXN);
                    for (int i = tmp.num; i >= to_insert + 1; --i) tmp.data[i] = tmp.data[i - 1];
                    strcpy(tmp.data[to_insert].key, target);
                    tmp.data[to_insert].pos = pos;
                }
                else {
                    have_inserted = false;
                    tmp.num = HALF_MAXN;
                    int j = HALF_MAXN;
                    another.num = (HALF_MAXN) + 1;
                    for (int i = 0; i < another.num; ++i)
                        if (!have_inserted && j == to_insert) {
                            strcpy(another.data[i].key, target);
                            another.data[i].pos = pos;
                            have_inserted = true;
                        }
                        else another.data[i] = tmp.data[j++];
                }
                //change the pointer
                memfile.seekg(0);
                memfile.read(rc(free_num), sizeof(int));
                if (!free_num) {
                    another.next = tmp.next;
                    tmp.next = sizeof(int) * 2 + block_num * block_size;
                }
                else {
                    another.next = tmp.next;
                    mem_allocate(free_num, tmp.next);
                }
                ++block_num;
                file.seekp(0);
                file.write(rc(block_num), sizeof(int));
                file.seekp(cur);
                file.write(rc(tmp), block_size);
                file.seekp(tmp.next);
                file.write(rc(another), block_size);
            }//insert in the block
            else {
                int i;
                for (i = 0; i < tmp.num; ++i)
                    if (cmp(target, tmp.data[i].key) ||
                        is_same(target, tmp.data[i].key) && (pos <= tmp.data[i].pos))
                        break;
                ++tmp.num;
                for (int j = tmp.num - 1; j > i; --j)
                    tmp.data[j] = tmp.data[j - 1];
                strcpy(tmp.data[i].key, target);
                tmp.data[i].pos = pos;
                file.seekp(cur);
                file.write(rc(tmp), block_size);
            }
            file.close();
            memfile.close();
        }

        std::vector<int> &find(const char target[]) {
            file.open(file_name);
            memfile.open(memfile_name);
            std::vector<int> *result = new std::vector<int>(0);
            if (!block_num) {
                //todo throw something
                file.close();
                memfile.close();
                return *result;
            }
            char tmp_key[Key_Len + 1];
            block tmp, another;
            int cur, nxt, nxtt, pre = 0, cur_num, nxt_num, free_num;
            //skip block number
            cur = init;
            file.seekg(cur);
            file.read(rc(nxt), sizeof(int));
            file.read(rc(cur_num), sizeof(int));
            //find
            while (1) {
                if (!nxt) {
                    break;
                }
                file.seekg(nxt);
                file.read(rc(nxtt), sizeof(int));
                file.read(rc(nxt_num), sizeof(int));
                file.read(tmp_key, Key_Len + 1);
                if (!cmp(target, tmp_key)) {
                    //merge
                    if (cur_num + nxt_num < MERGE_THRESHOLD) {
                        //reuse the storage
                        merge_blocks(cur, nxt);
                        nxt = nxtt;
                    }
                    else {
                        pre = cur;
                        cur = nxt;
                        nxt = nxtt;
                    }
                    continue;
                }
                break;
            }

            bool flag1 = false, flag2 = false;
            while (1) {
                file.seekg(cur);
                file.read(rc(tmp), block_size);
                int i;
                for (i = 0; i < tmp.num; ++i) if (is_same(tmp.data[i].key, target)) break;
                if (!flag1) {
                    flag1 = true;
                    flag2 = (i == 0);
                }
                for (; i < tmp.num; ++i)
                    if (is_same(tmp.data[i].key, target)) result->push_back(tmp.data[i].pos);
                    else break;
                if (i == tmp.num && nxt) {
                    file.seekg(nxt);
                    file.read(rc(nxtt), sizeof(int));
                    file.read(rc(nxt_num), sizeof(int));
                    file.read(tmp_key, Key_Len + 1);
                    if (is_same(tmp_key, target)) {
                        cur = nxt;
                        nxt = nxtt;
                        continue;
                    }
                }
                break;
            }
            if (flag2 && pre) {
                file.seekg(pre);
                file.read(rc(tmp), block_size);
                int i = tmp.num - 1;
                for (; i >= 0; --i) {
                    if (!is_same(tmp.data[i].key, target)) break;
                }
                ++i;
                for (int j = i; j < tmp.num; ++j) result->push_back(tmp.data[j].pos);
            }
            file.close();
            memfile.close();
            return *result;
        }


        void Delete(const char target[], const int &pos) {
            file.open(file_name);
            memfile.open(memfile_name);
            if (!block_num) {
                file.close();
                memfile.close();
                return;
            }
            block tmp, another;
            int cur, nxt, nxtt, pre = 0, cur_num, nxt_num, free_num;
            Node element;
            //skip block number
            cur = init;
            file.seekg(cur);
            file.read(rc(nxt), sizeof(int));
            file.read(rc(cur_num), sizeof(int));
            //find
            while (1) {
                if (!nxt) {
                    break;
                }
                file.seekg(nxt);
                file.read(rc(nxtt), sizeof(int));
                file.read(rc(nxt_num), sizeof(int));
                file.read(rc(element), sizeof(element));
                if (cmp(element.key, target) || is_same(element.key, target) && element.pos <= pos) {
                    //merge
                    if (cur_num + nxt_num < MERGE_THRESHOLD) {
                        //reuse the storage
                        merge_blocks(cur, nxt);
                        nxt = nxtt;
                    }
                    else {
                        pre = cur;
                        cur = nxt;
                        nxt = nxtt;
                    }
                    continue;
                }
                break;
            }
            bool flag1 = false, flag2 = false;
            while (1) {
                file.seekg(cur);
                file.read(rc(tmp), block_size);
                if (!flag1) {
                    flag1 = true;
                    flag2 = (is_same(tmp.data[0].key, target) && (tmp.data[0].pos == pos));
                }
                int i, j, j1, tmp_num = tmp.num;
                for (i = 0; i < tmp.num; ++i)
                    if (is_same(tmp.data[i].key, target) && (tmp.data[i].pos == pos)) break;
                for (j = i + 1; j < tmp.num; ++j)
                    if (!(is_same(tmp.data[j].key, target) && (tmp.data[j].pos == pos))) break;
                j1 = j;
                int k = i;
                for (; j < tmp.num; ++j)
                    tmp.data[k++] = tmp.data[j];
                tmp.num = k;
                //block empty
                if (tmp.num == 0) {
                    if (!pre) { init = nxt; }
                    else {
                        file.seekg(pre);
                        file.write(rc(nxt), sizeof(int));
                    }
                    //reuse
                    mem_free(cur);
                    --block_num;
                }// not empty
                else {
                    file.seekp(cur);
                    file.write(rc(tmp), block_size);
                }
                if (j1 == tmp_num && nxt) {
                    file.seekg(nxt);
                    file.read(rc(nxtt), sizeof(int));
                    file.read(rc(element), sizeof(element));
                    if (is_same(element.key, target) && (element.pos == pos)) {
                        pre = cur;
                        cur = nxt;
                        nxt = nxtt;
                        continue;
                    }
                }
                break;
            }
            if (flag2 && pre) {
                file.seekg(pre);
                file.read(rc(tmp), block_size);
                int i = tmp.num - 1;
                for (; i >= 0; --i) {
                    if (!(is_same(tmp.data[i].key, target) && (tmp.data[i].pos == pos))) break;
                }
                tmp.num = i + 1;
                file.seekp(pre);
                file.write(rc(tmp), block_size);
            }
            file.close();
            memfile.close();
        }


    };
}
#endif //SRC_UNROLLED_LINKED_LIST_HPP
