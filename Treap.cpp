#include <iostream>
using namespace std;

template <class T>
class Node {
public:
    T key;
    int priority, size;
    Node *left, *right;
    
    Node(const T& _key) : key(_key), priority(rand()), size(1), left(NULL), right(NULL) { }
    void setLeft(Node* newLeft) { left = newLeft; calcSize(); }
    void setRight(Node* newRight) { right = newRight; calcSize(); }
    void calcSize() {
        size = 1;
        if(left) size += left->size;
        if(right) size += right->size;
    }
};

template <class T>
class TreeMap {
public:
    TreeMap() : _root(NULL) { }
    typedef pair<Node<T>*, Node<T>* > Nodepair;
    void insert(const T& key) {
        _root = _insert(_root, new Node<T>(key));
    }
    void erase(const T& key) {
        _root = _erase(_root, key);
    }
    T& getKth(int k) {
        Node<T>* knode = kth(_root, k);
        return knode->key;
    }
    int countLessThan(T k) {
        return _countLessThan(_root, k);
    }
    
private:
    int _countLessThan(Node<T>* root, T& k) {
        if(root==NULL) return 0;
        if(root->key >= k) {
            return _countLessThan(root->left, k);
        }
        int ls = (root->left ? root->left->size : 0);
        return ls + 1 + _countLessThan(root->right, k);
    }
    
    Node<T>* kth(Node<T>* root, int k) {
        int leftSize = 0;
        if(root->left != NULL)
            leftSize = root->left->size;
        if(k <= leftSize) return kth(root->left, k);
        if(k == leftSize + 1) return root;
        return kth(root->right, k - leftSize - 1);
    }
    Node<T>* merge(Node<T>* a, Node<T>* b) {
        if(a==NULL) return b;
        if(b==NULL) return a;
        if(a->priority < b->priority) {
            b->setLeft(merge(a, b->left));
            return b;
        }
        a->setRight(merge(a->right, b));
        return a;
    }
    Node<T>* _erase(Node<T>* root, T& key) {
        if(root == NULL) return root;
        if(root->key == key) {
            Node<T>* ret = merge(root->left, root->right);
            delete root;
            return ret;
        }
        if(key < root->key) {
            root->setLeft(_erase(root->left, key));
        } else {
            root->serRight(_erase(root->right, key));
        }
        return root;
    }
    Nodepair _split(Node<T>* root, T& key) {
        if(root == NULL) return Nodepair(NULL, NULL);
        if(root->key < key) {
            Nodepair rs = _split(root->right, key);
            root->setRight(rs.first);
            return Nodepair(root, rs.second);
        }
        Nodepair ls = _split(root->left, key);
        root->setLeft(ls.second);
        return Nodepair(ls.first, root);
    }
    Node<T>* _insert(Node<T>* root, Node<T>* node) {
        if(root == NULL) return node;
        if(root->priority < node->priority) {
            Nodepair s = _split(root, node->key);
            node->setLeft(s.first);
            node->setRight(s.second);
            return node;
        } else if(node->key < root->key) {
            root->setLeft(_insert(root->left, node));
        } else {
            root->setRight(_insert(root->right, node));
        }
        return root;
    }
    Node<T>* _root;
};

int main(int argc, const char * argv[]) {
    int arr[5] = {10, 2, 3, 5, 12};
    
    TreeMap<int> map;
    for(int i=0; i<5; i++) {
        map.insert(arr[i]);
    }
    
    cout << "kth(3): " << map.getKth(3) << endl;
    cout << "11 less: " << map.countLessThan(11) << endl;
    return 0;
}
