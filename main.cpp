#include <iostream>
#include <vector>
#include "Unrolled_Linked_List.hpp"
using namespace std;

const int key_len=1;

int main() {
    ULL::Unrolled_Linked_List<key_len> l("text.out");
    vector<int>* result;
    int n,op,pos;
    char key[key_len];
    cin>>n;
    for (int i=0;i<n;++i){
        cin>>op;
        switch (op) {
            case 0:l.show_the_list();break;
            case 1:cin>>key>>pos;l.insert(key,pos);break;
            case 2:
                cin>>key;
                result=&l.find(key);
                for (int j=0;j<result->size();++j) cout<<"---"<<result->operator[](j)<<"---\n";
                break;
            default:
                cin>>key;
                l.Delete(key);
        }
    }
    return 0;
}
