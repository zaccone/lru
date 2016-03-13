#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>

using namespace std;

struct Node {
    int key;
    int val;

    Node* next;
    Node* prev;

    Node(int key, int val) {
        this->key = key;
        this->val = val;
        next = prev = NULL;
    }
};

class LRU {
    private:
        int capacity;
        int size;
        unordered_map<int, Node*> _map;
        Node* head;
        Node* tail;

        void evict() {
            while(size > capacity) {
                if(!tail) {
                    break;
                }
                Node* prev = tail->prev;
                if(tail->prev)
                    tail->prev->next = NULL;
                auto it = _map.find(tail->key);
                _map.erase(it);
                delete tail;
                tail = prev;
                size--;
            }
        }

        void moveToFront(Node* node) {
            if (node == head || head == tail)
                return;
            if (node == tail) {
                tail = node->prev;
            }

            Node* prev = node->prev;
            if(prev) {
                prev->next = node->next;
            }
            if(node->next) {
                node->next->prev = prev;
            }
            node->prev = NULL;
            head->prev = node;
            node->next = head;
            head = node;
            tail->next = NULL;
        }

    public:
        LRU(int cap) {
            size = 0;
            capacity = cap;
            head = NULL;
            tail = NULL;
        }

        int get(int k) {
            Node* node = _map[k];
            if(node) {
                moveToFront(node);
                return node->val;
            } else {
                return -1;
            }
        }

        void set(int k, int v) {
                Node* n = _map[k];
                if(n) {
                    n->val = v;
                } else {
                    Node *n = new Node(k, v);
                    _map[k] = n;
                    if(head) {
                        head->prev = n;
                        n->next = head;
                        head = n;
                    } else {
                        head = n;
                        tail = n;
                    }
                    size++;
                    if(capacity >0 && size > capacity)
                        evict();
                }
        }
};

int main() {
    LRU lru(5);

    for(int i = 1; i < 10; i++) {
        lru.set(i,i);
    }

    for(int i = 0 ; i < 10; i++) {
        cout << "Testing " << i << " got: " << lru.get(i) << endl;
    }

    lru.set(1,1);
    lru.set(2,2);


    cout << endl << endl;

    for(int i = 0 ; i < 10; i++) {
        cout << "Testing " << i << " got: " << lru.get(i) << endl;
    }

    lru.get(7);
    lru.get(1);
    lru.get(2);
    lru.set(3,3);

    cout << endl << endl;

    for(int i = 0 ; i < 10; i++) {
        cout << "Testing " << i << " got: " << lru.get(i) << endl;
    }

    return 0;
}
