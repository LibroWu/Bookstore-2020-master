//
// Created by Libro on 2021/1/15.
//

#ifndef SRC_UNROLLED_LINKED_LIST_HPP
#define SRC_UNROLLED_LINKED_LIST_HPP
#include <fstream>
#include <string>
#include <cstring>
#define rc(x) reinterpret_cast<char*>(&x)
#define sfb std::fstream::beg

namespace ULL {
    const int MAXN=300;

    template<int Key_Len>
    class Unrolled_Linked_List {
    private:

        int block_num;
        std::fstream file;
        class block{
            friend Unrolled_Linked_List<Key_Len>;
        private:
            int next,num;
            class Node{
            public:
                char key[Key_Len]={0};
                int pos;
            } data[MAXN];

        public:
            block():next(0),num(0){}
        };
        const int block_size=sizeof(block_num);
        bool cmp(char a[],char b[]){
            for (int i=0;i<Key_Len;++i)
                if (a[i]<b[i]) return 1;
                else if (a[i]>b[i]) return 0;
            return 1;
        }

    public:
        Unrolled_Linked_List(std::string file_name):block_num(0){
            file.open(file_name,std::fstream::binary|std::fstream::out|std::fstream::in);
            file.write(rc(block_num),sizeof(block_num));
        }
        ~Unrolled_Linked_List(){
            file.close();
        }
        void add(char target[],int& pos){
            block tmp;
            int nxt,nxtt,cur,tmp_num;
            char tmp_key[Key_Len];
            if (!block_num){
                ++block_num;
                tmp.data[tmp.num++]={target,pos};
                file.seekp(sizeof(int),sfb);
                file.write(rc(tmp),block_size);
                return;
            }
            cur=sizeof(int);
            file.seekg(cur,sfb);
            file.read(rc(nxt),sizeof(int));
            while (1){
                if (!nxt) {
                    break;
                }
                file.seekg(nxt,sfb);
                file.read(rc(nxtt),sizeof(int));
                file.read(rc(tmp_num),sizeof(int));
                file.read(tmp_key,Key_Len);
                if (cmp(tmp_key,target)) {
                    cur=nxt;
                    nxt=nxtt;
                    continue;
                }
                break;
            }
            file.seekg(cur,sfb);
            file.read(rc(tmp),block_size);
            if (tmp.num==MAXN){

            }else {
                int i;
                for (i=0;i<tmp.num;++i)
                    if (!cmp(tmp.data[i],target)) break;
                ++tmp.num;
                for (int j=i+1;j<tmp.num;++j) tmp.data[j]=tmp.data[j-1];
            }
        }
        int find(char target[]){

        }
        void Delete(char target[]){

        }
    };
}
#endif //SRC_UNROLLED_LINKED_LIST_HPP
