#include <iostream>
#include <vector>
#include "Unrolled_Linked_List.hpp"

using namespace std;

const int key_len = 4;

int main() {
    ofstream o("test.out");
    o.close();
    ULL::Unrolled_Linked_List<key_len> l("text.out", true);
    vector<int> *result;
    int n, op, pos;
    char key[key_len];
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> op;
        switch (op) {
            case 3:
                l.show_the_list();
                break;
            case 0:
                cin >> key >> pos;
                l.insert(key, pos);
                break;
            case 1:
                cin >> key>>pos;
                result = &l.find(key);
                for (int j = 0; j < result->size(); ++j) cout << "---" << result->operator[](j) << "---\n";
                delete result;
                break;
            default:
                cin >> key>>pos;
                l.Delete(key);
        }
    }
    return 0;
}
