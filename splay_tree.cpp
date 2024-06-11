#include <iostream>
#include <memory>

class SplayTree {
    struct Node {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
        std::shared_ptr<Node> parent = nullptr;
        int key;

        Node() {
            key = 0;
        }

        Node(int new_key) {
            key = new_key;
        }
    };
    std::shared_ptr<Node> _root;
public:

    SplayTree() = delete;

    explicit SplayTree(int key): _root(std::make_shared<Node>(key)){}

    SplayTree(const SplayTree& node) {
        _root = node._root;
    }

    void add_node(int new_key) {
        add_node(new_key, _root);
    }

    void remove_node(int removed_key) {
        remove_node(removed_key, _root);
    }

    void print_tree() const {
        print_tree(_root, 0);
    }

    bool find(int key) {
        std::shared_ptr<Node> temp = _root;
        while (temp) {
            if (temp->key == key) {
                splay(temp);
                return true;
            } else if (temp->key > key) {
                temp = temp->left;
            } else {
                temp = temp->right;
            }
        }
        return false;
    }

private:
    void add_node(int new_key, std::shared_ptr<Node> node) {
        if (new_key < node->key) {
            if (!node->left) {
                node->left = std::make_shared<Node>(new_key);
                node->left->parent = node;
                splay(node->left);
            } else {
                add_node(new_key, node->left);
            }
        }
        else {
            if (!node->right) {
                node->right = std::make_shared<Node>(new_key);
                node->right->parent = node;
                splay(node->right);
            } else {
                add_node(new_key, node->right);
            }
        }
    }

    void remove_node(int removed_key, std::shared_ptr<Node> node) {
        if (!node) {
            throw "There is no such element";
        }
        if (node->key == removed_key) {
            splay(node);
            merge(node->left, node->right);
        } else if (node->key > removed_key) {
            remove_node(removed_key, node->left);
        } else {
            remove_node(removed_key, node->right);
        }
    }

    void rotate_left(std::shared_ptr<Node> node) {
        auto parent_node = node->parent; // nullptr
        auto right_son_node = node->right; // _root
        if (parent_node) {
            if (parent_node->left == node) {
                parent_node->left = right_son_node;
            } else {
                parent_node->right = right_son_node;
            }
        }
        auto temp = right_son_node->left;
        right_son_node->left = node;
        node->right = temp;
        node->parent = right_son_node;
        right_son_node->parent = parent_node;
        if (right_son_node->left->right) {
            right_son_node->left->right->parent = right_son_node->left;
        }
    }

    void rotate_right(std::shared_ptr<Node> node) {
        auto parent_node = node->parent;
        auto left_son_node = node->left;
        if (parent_node) {
            if (parent_node->left == node) {
                parent_node->left = left_son_node;
            } else {
                parent_node->right = left_son_node;
            }
        }
        auto temp = left_son_node->right;
        left_son_node->right = node;
        node->left = temp;
        node->parent = left_son_node;
        left_son_node->parent = parent_node;
        if (node->left) {
            node->left->parent = node;
        }
    }

    void splay(std::shared_ptr<Node> node) {
        while (node->parent) {
            if (node == node->parent->left) {
                if (!node->parent->parent) {
                    //std::cout << 1 << std::endl;
                    rotate_right(node->parent);
                } else if (node->parent == node->parent->parent->left) {
                    //std::cout << 2 << std::endl;
                    rotate_right(node->parent->parent);
                    rotate_right(node->parent);
                } else {
                    //std::cout << 3 << std::endl;
                    rotate_right(node->parent->parent);
                    rotate_left(node->parent);
                }
            }
            else {
                if (!node->parent->parent) {
                    //std::cout << 4 << std::endl;
                    rotate_left(node->parent);
                } else if (node->parent == node->parent->parent->right) {
                    //std::cout << 5 << std::endl;
                    rotate_left(node->parent->parent);
                    rotate_left(node->parent);
                } else {
                    //std::cout << 6 << std::endl;
                    rotate_left(node->parent->parent);
                    rotate_right(node->parent);
                }
            }
        }
        _root = node;
        node->parent = nullptr;
    }

    void merge(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
        while (node1->right) {
            node1 = node1->right;
        }
        splay(node1);
        node1->right = node2;
        node2->parent = node1;
        _root = node1;
        _root->parent = nullptr;
    }


    void print_tree(const std::shared_ptr<Node>& node, int level) const {
        if (node) {
            print_tree(node->right, level + 1);
            for (int i = 0; i < level; ++i) {
                std::cout <<'\t';
            }
            std::cout << node->key << '\n';
            print_tree(node->left, level + 1);
        }
    }
};