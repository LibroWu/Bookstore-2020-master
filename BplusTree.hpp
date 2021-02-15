//
// Created by Libro on 2021/1/30.
//

#ifndef CODE_BPLUSTREE_HPP
#define CODE_BPLUSTREE_HPP

#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <algorithm>

template<int Key_Len, int MAXN>
class BpulsTree {
private:
    int root, init, Leaf_Num, nonLeaf_Num, free_Leaf_Num, free_nunLeaf_Num;
    std::string file_name_Leaf, file_name_nonLeaf, memfile_name_Leaf, memfile_name_nonLeaf;
    std::fstream file_Leaf, file_nonLeaf, memfile_Leaf, memfile_nonLeaf;

    template<class T>
    void file_write(std::fstream &file, const T &t) {
        file.write(reinterpret_cast<char *>(&t), sizeof(t));
    }

    template<class T>
    void file_read(std::fstream &file, T &t) {
        file.read(reinterpret_cast<char *>(&t), sizeof(t));
    }

    void Leaf_allocate(int &free_num, int &offset) {
        memfile_Leaf.open(memfile_name_Leaf);
        file_write(memfile_Leaf, --free_num);
        memfile_Leaf.seekg((1 + free_num) * sizeof(int));
        file_read(memfile_Leaf, offset);
        memfile_Leaf.close();
    }

    void nonLeaf_allocate(int &free_num, int &offset) {
        memfile_nonLeaf.open(memfile_name_nonLeaf);
        file_write(memfile_nonLeaf, --free_num);
        memfile_nonLeaf.seekg((1 + free_num) * sizeof(int));
        file_read(memfile_nonLeaf, offset);
        memfile_nonLeaf.close();
    }

    void Leaf_free(int &free_num, const int &offset) {
        memfile_Leaf.open(memfile_name_Leaf);
        file_write(memfile_Leaf, ++free_num);
        memfile_Leaf.seekp(free_num);
        file_write(memfile_Leaf, offset);
        memfile_Leaf.close();
    }

    void nonLear_free(int &free_num, const int &offset) {
        memfile_nonLeaf.open(memfile_name_nonLeaf);
        file_write(memfile_nonLeaf, ++free_num);
        memfile_nonLeaf.seekp(free_num);
        file_write(memfile_nonLeaf, offset);
        memfile_nonLeaf.close();
    }

    class nonLeafNode {
        friend BpulsTree<Key_Len, MAXN>;
    private:
        int rightBrother, father, leftBrother, num;

        class Element {
            friend BpulsTree<Key_Len, MAXN>;
            friend nonLeafNode;
        private:
            char key[Key_Len];
        public:
            Element(const char *target) {
                strcpy(key, target);
            }

            Element(const Element& other){
                strcpy(key,other.key);
            }

            Element(Element &&other) : key(other.key) {
                other.key = nullptr;
            }

            bool operator<(const Element &other) {
                return strcmp(key, other.key) < 0;
            }
        } data[MAXN - 1];

        int child_link[MAXN];
    public:

        nonLeafNode() : num(0), leftBrother(0), rightBrother(0), father(0) {}
    };

    class LeafNode {
        friend BpulsTree<Key_Len, MAXN>;
    private:
        int rightBrother, father, leftBrother, num;

        class Element {
            friend BpulsTree<Key_Len, MAXN>;
            friend LeafNode;
        private:
            char key[Key_Len];
            int offset;
        public:
            Element(const char *target, const int &offset) : offset(offset) {
                strcpy(key, target);
            }

            Element(Element &&other) : offset(other.offset), key(other.key) {
                other.key = nullptr;
            }
            Element(const Element& other):offset(other.offset){
                strcpy(key,other.key);
            }
            bool operator<(const Element &other) {
                return (strcmp(key, other.key) < 0);
            }
        } data[MAXN];

    public:
        LeafNode() : num(0), leftBrother(0), rightBrother(0), father(0) {}
    };

public:
    BpulsTree(const std::string &file1, const std::string &file2) : file_name_Leaf("runData/"+file1), file_name_nonLeaf("runData/"+file2),
                                                                    memfile_name_Leaf("runData/mem" + file1),
                                                                    memfile_name_nonLeaf("runData/mem" + file2) {}

    void CreatFile() {
        root = 2 * sizeof(int);
        free_nunLeaf_Num = free_Leaf_Num = Leaf_Num = nonLeaf_Num = 0;
        std::ofstream out;
        int x(0), y(root);
        out.open(file_name_Leaf);
        file_write(out, x);
        file_write(out, y);
        out.close();
        out.open(file_name_nonLeaf);
        file_write(out, x);
        file_write(out, y);
        out.close();
        out.open(memfile_name_Leaf);
        file_write(out, x);
        out.close();
        out.open(memfile_name_nonLeaf);
        file_write(out, x);
        out.close();
    }

    void Initialize() {
        std::ifstream in;
        in.open(file_name_nonLeaf);
        file_read(in, nonLeaf_Num);
        file_read(in, root);
        in.close();
        in.open(file_name_Leaf);
        file_read(in, Leaf_Num);
        file_read(in, init);
        in.close();
        in.open(memfile_name_Leaf);
        file_read(in, free_Leaf_Num);
        in.close();
        in.open(memfile_name_nonLeaf);
        file_read(in, free_nunLeaf_Num);
        in.close();
    }

    void insert(const char *target, const int &offset) {
        if (!nonLeaf_Num) {
            if (!Leaf_Num) {
                if (free_Leaf_Num) {
                    ++Leaf_Num;
                    file_Leaf.open(file_name_Leaf);
                    file_write(file_Leaf, Leaf_Num);
                    LeafNode tmp;
                    tmp.num = 1;
                    tmp.data[0] = LeafNode::Element(target, offset);
                    Leaf_allocate(free_Leaf_Num, init);
                    file_Leaf.seekp(init);
                    file_write(file_Leaf, tmp);
                    file_Leaf.close();
                }
                else {
                    ++Leaf_Num;
                    file_Leaf.open(file_name_Leaf);
                    file_write(file_Leaf, Leaf_Num);
                    LeafNode tmp;
                    tmp.num = 1;
                    tmp.data[0] = LeafNode::Element(target, offset);
                    file_Leaf.seekp(init);
                    file_write(file_Leaf, tmp);
                    file_Leaf.close();
                }
            }
            else {
                file_Leaf.open(file_name_Leaf);
                file_Leaf.seekg(init);
                LeafNode tmp;
                file_read(file_Leaf, tmp);
                if (tmp.num != MAXN) {
                    typename LeafNode::Element tmpElement(target,offset);
                    int to_insert=std::upper_bound(tmp.data,tmp.data+tmp.num,tmpElement);
                    for (int i=tmp.num;i>=to_insert+1;--i) tmp.data[i]=tmp.data[i-1];
                    ++tmp.num;
                    tmp.data[to_insert]=std::move(tmpElement);
                    file_Leaf.seekp(init);
                    file_write(file_Leaf,tmp);
                    file_Leaf.close();
                }//Divide the Node
                else {

                }
            }
            return;
        }


    }

    void Delete(const char *target, const int &offset);

    std::vector<int> &find(const char *target);
};

#endif //CODE_BPLUSTREE_HPP