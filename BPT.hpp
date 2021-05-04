//
// Created by Libro on 2021/4/9.
//

#ifndef BPT_BPT_HPP
#define BPT_BPT_HPP
#define debugs

#ifdef debugs

#include <iostream>

using std::cin;
using std::cout;
#endif

#include "MemoryRiver.hpp"
#include <cstring>
#include <vector>
#include <algorithm>

using std::lower_bound;
using std::upper_bound;
using std::vector;

//support duplicate key
//by using some tricks
template<class T, class U, int M, int L, class Compare=std::less<T>>
class BPT {
private:
#define halfM (M-M/2)
#define halfL (L-L/2)

    //have branches
    //like light penetrates the crystal
    class crystalNode {
    public:
        bool is_leaf;
        int number;
        T Fence[M + 1];
        int child[M + 2];

        crystalNode() : number(0), is_leaf(0) {}

#ifdef debugs

        void print() {
            cout << "\n----" << number << "----\n";
            for (int i = 0; i < number - 1; ++i) {
                cout << Fence[i] << ' ';
            }
            cout << "\n|||||||||||||||||||||||\n";
            for (int i = 0; i < number; ++i) {
                cout << child[i] << ' ';
            }
            cout << "\n-----------------\n";
        }

#endif
    };

    class indexNode {
    public:
        int next, pre, number;
        T v[L + 1];
        U index[L + 1];

        indexNode() : number(0), next(0), pre(0) {}

        //not nullptr if split
        indexNode *insert(const T &t, const U &ind) {
            int pos = lower_bound(v, v + number, t) - v;
            if (t==v[pos]) return nullptr;
            for (int i = number; i > pos; --i) {
                v[i] = v[i - 1];
                index[i] = index[i - 1];
            }
            v[pos] = t;
            index[pos] = ind;
            ++number;
            indexNode *newInd = nullptr;
            if (number > L) {
                //todo have a bug here, reach L
                //todo if reserve an empty storage in the end, then the coding would be much more enjoyable
                //todo I choose to enjoy the coding
                newInd = new indexNode;
                for (int i = 0; i < halfL; ++i) {
                    newInd->index[i] = index[i + L / 2 + 1];
                    newInd->v[i] = v[i + L / 2 + 1];
                }
                newInd->next = next;
                newInd->number = halfL;
                number = L / 2 + 1;
            }
            return newInd;
        }

        bool Delete(const T &t) {
            int pos = lower_bound(v, v + number, t) - v;
            if (pos < number && v[pos] == t) {
                for (int i = pos; i < number; ++i) {
                    v[i] = v[i + 1];
                    index[i] = index[i + 1];
                }
                --number;
                return number < halfL;
            }
            return false;
        }

#ifdef debugs

        void print() {
            cout << "\n***" << number << "***\n";
            for (int i = 0; i < number; ++i) {
                cout << "---" << v[i] << ' ' << index[i] << "---\n";
            }
            cout << "\n******\n";
        }

#endif
    };


    //the third info is for the position of the root
    MemoryRiver<crystalNode, 3> crystalMemory;
    MemoryRiver<indexNode, 2> indexMemory;

    Compare cmp;

    struct Pair {
        T t;
        int pos;
    };

    Pair *sub_insert(const T &t, const U &index, const int &pos) {
        Pair *ptr = nullptr;
        crystalNode sub_root;
        crystalMemory.read(sub_root, pos);
        int num = lower_bound(sub_root.Fence, sub_root.Fence + sub_root.number - 1, t) - sub_root.Fence;
        if (sub_root.is_leaf) {
            indexNode ind;
            indexMemory.read(ind, sub_root.child[num]);
            indexNode *tmp = ind.insert(t, index);
            if (tmp == nullptr)
                indexMemory.update(ind, sub_root.child[num]);
            else {
                ind.next = indexMemory.write(*tmp);
                indexMemory.update(ind, sub_root.child[num]);
                for (int i = sub_root.number; i > num + 1; --i) {
                    sub_root.child[i] = sub_root.child[i - 1];
                    sub_root.Fence[i - 1] = sub_root.Fence[i - 2];
                }
                sub_root.child[num + 1] = ind.next;
                sub_root.Fence[num] = tmp->v[0];
                ++sub_root.number;
                //the sub_root splits
                if (sub_root.number > M + 1) {
                    crystalNode another, n;
                    for (int i = 0; i < halfM + 1; ++i)
                        another.child[i] = sub_root.child[i + M / 2 + 1];
                    for (int i = 0; i < halfM; ++i)
                        another.Fence[i] = sub_root.Fence[i + M / 2 + 1];
                    ptr = new Pair;
                    ptr->t = sub_root.Fence[M / 2];
                    another.number = halfM + 1;
                    sub_root.number = M / 2 + 1;
                    sub_root.is_leaf = true;
                    another.is_leaf = true;
                    crystalMemory.update(sub_root, pos);
                    ptr->pos = crystalMemory.write(another);
                } else crystalMemory.update(sub_root, pos);
                delete tmp;
            }
        } else {
            Pair *tmp = sub_insert(t, index, sub_root.child[num]);
            if (tmp) {
                for (int i = sub_root.number; i > num + 1; --i) {
                    sub_root.child[i] = sub_root.child[i - 1];
                    sub_root.Fence[i - 1] = sub_root.Fence[i - 2];
                }
                sub_root.Fence[num] = tmp->t;
                sub_root.child[num + 1] = tmp->pos;
                sub_root.number++;
                if (sub_root.number > M + 1) {
                    crystalNode another, n;
                    for (int i = 0; i < halfM + 1; ++i)
                        another.child[i] = sub_root.child[i + M / 2 + 1];
                    for (int i = 0; i < halfM; ++i)
                        another.Fence[i] = sub_root.Fence[i + M / 2 + 1];
                    ptr = new Pair;
                    ptr->t = sub_root.Fence[M / 2];
                    another.number = halfM + 1;
                    sub_root.number = M / 2 + 1;
                    sub_root.is_leaf = false;
                    another.is_leaf = false;
                    crystalMemory.update(sub_root, pos);
                    ptr->pos = crystalMemory.write(another);
                } else crystalMemory.update(sub_root, pos);
                delete tmp;
            }
        }
        return ptr;
    }

    crystalNode *sub_Delete(const T &t, int pos) {
        crystalNode *ptr = new crystalNode;
        crystalMemory.read(*ptr, pos);
        int num = upper_bound(ptr->Fence, ptr->Fence + ptr->number - 1, t) - ptr->Fence;
        if (ptr->is_leaf) {
            indexNode ind;
            indexMemory.read(ind, ptr->child[num]);
            if (ind.Delete(t)) {
                bool flag = true, flag2 = false;
                if (num) {
                    indexNode lind;
                    indexMemory.read(lind, ptr->child[num - 1]);
                    if (lind.number > halfL) {
                        for (int i = ind.number; i > 0; --i) {
                            ind.v[i] = ind.v[i - 1];
                            ind.index[i] = ind.index[i - 1];
                        }
                        ++ind.number;
                        ind.v[0] = lind.v[--lind.number];
                        ind.index[0] = lind.index[lind.number];
                        ptr->Fence[num - 1] = ind.v[0];
                        flag2 = true;
                        indexMemory.update(ind, ptr->child[num]);
                        indexMemory.update(lind, ptr->child[num - 1]);
                    }//merge with left index node
                    else if (ptr->number > halfM + 1 || num == ptr->number - 1) {
                        for (int i = 0; i < ind.number; ++i) {
                            lind.v[i + lind.number] = ind.v[i];
                            lind.index[i + lind.number] = ind.index[i];
                        }
                        lind.next = ind.next;
                        lind.number += ind.number;
                        indexMemory.Delete(ptr->child[num]);
                        for (int i = num; i < ptr->number; ++i) {
                            ptr->child[i] = ptr->child[i + 1];
                            ptr->Fence[i - 1] = ptr->Fence[i];
                        }
                        --ptr->number;
                        flag2 = true;
                        indexMemory.update(lind, ptr->child[num - 1]);
                    } //problem has not been solved
                    else flag = false;
                }
                if (num == 0 && ptr->number > 1 || !flag) {
                    indexNode rind;
                    indexMemory.read(rind, ptr->child[num + 1]);
                    if (rind.number > halfL) {
                        ind.v[ind.number] = rind.v[0];
                        ind.index[ind.number++] = rind.index[0];
                        --rind.number;
                        for (int i = 0; i < rind.number; ++i) {
                            rind.v[i] = rind.v[i + 1];
                            rind.index[i] = rind.index[i + 1];
                        }
                        ptr->Fence[num] = rind.v[0];
                        flag2 = true;
                        indexMemory.update(ind, ptr->child[num]);
                        indexMemory.update(rind, ptr->child[num + 1]);
                    }//merge with right index node
                    else {
                        for (int i = 0; i < rind.number; ++i) {
                            ind.v[i + ind.number] = rind.v[i];
                            ind.index[i + ind.number] = rind.index[i];
                        }
                        ind.next = rind.next;
                        ind.number += rind.number;
                        indexMemory.Delete(ptr->child[num + 1]);
                        for (int i = num + 1; i < ptr->number; ++i) {
                            ptr->child[i] = ptr->child[i + 1];
                            ptr->Fence[i - 1] = ptr->Fence[i];
                        }
                        --ptr->number;
                        flag2 = true;
                        indexMemory.update(ind, ptr->child[num]);
                    }
                }
                if (!flag2) indexMemory.update(ind, ptr->child[num]);
                if (ind.number == 0) {
                    indexMemory.Delete(ptr->child[num]);
                    --ptr->number;
                }
                if (ptr->number - 1 < halfM) return ptr;
                crystalMemory.update(*ptr, pos);
            } else indexMemory.update(ind, ptr->child[num]);
        } else {
            crystalNode *sub_ptr = sub_Delete(t, ptr->child[num]);
            if (sub_ptr) {
                bool flag = true, flag2 = false;
                if (num) {
                    crystalNode l_crystal;
                    crystalMemory.read(l_crystal, ptr->child[num - 1]);
                    if (l_crystal.number - 1 > halfM) {
                        for (int i = sub_ptr->number; i > 1; --i) {
                            sub_ptr->child[i] = sub_ptr->child[i - 1];
                            sub_ptr->Fence[i - 1] = sub_ptr->Fence[i - 2];
                        }
                        sub_ptr->child[1] = sub_ptr->child[0];
                        ++sub_ptr->number;
                        sub_ptr->child[0] = l_crystal.child[--l_crystal.number];
                        sub_ptr->Fence[0] = ptr->Fence[num - 1];
                        ptr->Fence[num - 1] = l_crystal.Fence[l_crystal.number - 1];
                        flag2 = true;
                        crystalMemory.update(*sub_ptr, ptr->child[num]);
                        crystalMemory.update(l_crystal, ptr->child[num - 1]);
                    } //merge with left crystal node
                    else if (ptr->number > halfM + 1 || num == ptr->number - 1) {
                        for (int i = 0; i < sub_ptr->number; ++i) {
                            l_crystal.child[i + l_crystal.number] = sub_ptr->child[i];
                            l_crystal.Fence[i + l_crystal.number] = sub_ptr->Fence[i];
                        }
                        l_crystal.Fence[l_crystal.number - 1] = ptr->Fence[num - 1];
                        l_crystal.number += sub_ptr->number;
                        crystalMemory.Delete(ptr->child[num]);
                        for (int i = num; i < ptr->number; ++i) {
                            ptr->child[i] = ptr->child[i + 1];
                            ptr->Fence[i - 1] = ptr->Fence[i];
                        }
                        --ptr->number;
                        flag2 = true;
                        crystalMemory.update(l_crystal, ptr->child[num - 1]);
                    }//problem has not been solved
                    else flag = false;
                }
                if (num == 0 && ptr->number > 1 || !flag) {
                    crystalNode r_crystal;
                    crystalMemory.read(r_crystal, ptr->child[num + 1]);
                    if (r_crystal.number > halfM + 1) {
                        sub_ptr->Fence[sub_ptr->number - 1] = ptr->Fence[num];
                        ptr->Fence[num] = r_crystal.Fence[0];
                        sub_ptr->child[sub_ptr->number++] = r_crystal.child[0];
                        --r_crystal.number;
                        for (int i = 0; i < r_crystal.number; ++i) {
                            r_crystal.Fence[i] = r_crystal.Fence[i + 1];
                            r_crystal.child[i] = r_crystal.child[i + 1];
                        }
                        flag2 = true;
                        crystalMemory.update(*sub_ptr, ptr->child[num]);
                        crystalMemory.update(r_crystal, ptr->child[num + 1]);
                    } //merge with right crystal node
                    else {
                        for (int i = 0; i < r_crystal.number; ++i) {
                            sub_ptr->child[i + sub_ptr->number] = r_crystal.child[i];
                            sub_ptr->Fence[i + sub_ptr->number] = r_crystal.Fence[i];
                        }
                        sub_ptr->Fence[sub_ptr->number - 1] = ptr->Fence[num];
                        sub_ptr->number += r_crystal.number;
                        crystalMemory.Delete(ptr->child[num + 1]);
                        for (int i = num + 1; i < ptr->number; ++i) {
                            ptr->child[i] = ptr->child[i + 1];
                            ptr->Fence[i - 1] = ptr->Fence[i];
                        }
                        --ptr->number;
                        flag2 = true;
                        crystalMemory.update(*sub_ptr, ptr->child[num]);
                    }
                }
                if (!flag2) crystalMemory.update(*sub_ptr, ptr->child[num]);
                if (ptr->number - 1 < halfM) return ptr;
                crystalMemory.update(*ptr, pos);
            }
        }
        delete ptr;
        return nullptr;
    }

public:
    BPT() = default;

    BPT(const string &crystalFN, const string &indexFN) : crystalMemory(crystalFN), indexMemory(indexFN) {}

    void initialise(const string &crystalFN, const string &indexFN) {
        crystalMemory.initialise(crystalFN);
        indexMemory.initialise(indexFN);
    }

    void initialise() {
        crystalMemory.initialise();
        indexMemory.initialise();
    }

    void insert(const T &t, const U &index) {
        int root_pos;
        crystalMemory.get_info(root_pos, 3);
        crystalNode root;
        if (root_pos == 0) {
            indexNode Beg;
            Beg.index[0] = index;
            Beg.v[Beg.number++] = t;
            root.child[root.number++] = indexMemory.write(Beg);
            root.is_leaf = true;
            crystalMemory.write_info(crystalMemory.write(root), 3);
        } else {
            Pair *tmp = sub_insert(t, index, root_pos);
            if (tmp) {
                crystalNode newRoot;
                newRoot.child[0] = root_pos;
                newRoot.child[1] = tmp->pos;
                newRoot.Fence[0] = tmp->t;
                newRoot.number = 2;
                crystalMemory.write_info(crystalMemory.write(newRoot), 3);
                delete tmp;
            }
        }
    }

    void Delete(const T &t) {
        int root_pos;
        crystalMemory.get_info(root_pos, 3);
        if (root_pos) {
            crystalNode *tmp = sub_Delete(t, root_pos);
            if (tmp) {
                if (tmp->number == 0 || !tmp->is_leaf && tmp->number == 1) {
                    crystalMemory.Delete(root_pos);
                    if (!tmp->is_leaf)
                        crystalMemory.write_info(tmp->child[0], 3);
                    else {
                        crystalMemory.Delete(root_pos);
                        crystalMemory.write_info(0, 3);
                    }
                } else crystalMemory.update(*tmp, root_pos);
            }
            delete tmp;
        }
    }

    bool empty() {
        int root_pos;
        crystalMemory.get_info(root_pos, 3);
        return root_pos == 0;
    }

    void modify(const T &t, U &u) {
        int pos, num;
        crystalMemory.get_info(pos, 3);
        crystalNode tmp;
        while (pos > 0) {
            crystalMemory.read(tmp, pos);
            num = upper_bound(tmp.Fence, tmp.Fence + tmp.number - 1, t) - tmp.Fence;
            if (tmp.is_leaf) {
                indexNode ind;
                indexMemory.read(ind, tmp.child[num]);
                int N = lower_bound(ind.v, ind.v + ind.number, t) - ind.v;
                if (ind.v[N] == t) {
                    ind.index[N]=u;
                    indexMemory.update(ind,tmp.child[num]);
                }
                //not found
                return;
            }
            pos = tmp.child[num];
        }
    }

    U Find(const T &t) {
        int pos, num;
        crystalMemory.get_info(pos, 3);
        crystalNode tmp;
        while (pos > 0) {
            crystalMemory.read(tmp, pos);
            num = upper_bound(tmp.Fence, tmp.Fence + tmp.number - 1, t) - tmp.Fence;
            if (tmp.is_leaf) {
                indexNode ind;
                indexMemory.read(ind, tmp.child[num]);
                int N = lower_bound(ind.v, ind.v + ind.number, t) - ind.v;
                if (N<ind.number && ind.v[N] == t) return ind.index[N];
                //not found
                return U();
            }
            pos = tmp.child[num];
        }
        return U();
    }

    vector<U>* multipleFind(const T& t) {
        int pos, num;
        crystalMemory.get_info(pos, 3);
        crystalNode tmp;
        while (pos > 0) {
            crystalMemory.read(tmp, pos);
            num = upper_bound(tmp.Fence, tmp.Fence + tmp.number - 1, t) - tmp.Fence;
            if (tmp.is_leaf) {
                indexNode ind;
                indexMemory.read(ind, tmp.child[num]);
                int N = lower_bound(ind.v, ind.v + ind.number, t) - ind.v;
                if (N<ind.number && ind.v[N] == t) {
                    vector<U>* tmp=new vector<U>;
                    while (ind.v[N].key==t.key) {
                        tmp->push_back(ind.index[N]);
                        ++N;
                        if (N==ind.number) {
                            if (ind.next==0) break;
                            indexMemory.read(ind,ind.next);
                            N=0;
                        }
                    }
                    return tmp;
                }
                //not found
                return nullptr;
            }
            pos = tmp.child[num];
        }
        return nullptr;
    }

    void Clear() {
        crystalMemory.initialise();
        indexMemory.initialise();
    }

#ifdef debugs

    void print() {
        int l = 0, r = 0, que[10000];
        bool flag[10000] = {0};
        crystalMemory.get_info(que[r++], 3);
        if (que[0] == 0) return;
        int pos;
        while (l < r) {
            pos = que[l++];
            if (flag[l - 1]) {
                indexNode t;
                indexMemory.read(t, pos);
                cout << "----" << pos << "----";
                t.print();
                cout << '\n';
            } else {
                crystalNode t;
                cout << "----" << pos << "----";
                crystalMemory.read(t, pos);
                t.print();
                cout << '\n';
                for (int i = 0; i < t.number; ++i) {
                    flag[r] = t.is_leaf;
                    que[r++] = t.child[i];
                }
            }
        }
    }

#endif
};


#endif //BPT_BPT_HPP
