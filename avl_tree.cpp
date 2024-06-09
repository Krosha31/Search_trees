#include <iostream>
#include <memory>


class AVLTree {
    struct Node {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
        unsigned char height = 1;
        int key;

        Node() {
            left = nullptr;
            right = nullptr;
            height = 0;
            key = 0;
        }

        Node(int new_key) {
            key = new_key;
        }
    };
    std::shared_ptr<Node> _root;
public:

    AVLTree() = delete;

    explicit AVLTree(int key): _root(std::make_shared<Node>(key)){}

    AVLTree(const AVLTree& node) {
        _root = node._root;
    }
    
    void add_node(int new_key) {
        _root = add_node(new_key, _root);
    }

    void print_tree() const {
        print_tree(_root, 0);
    }


private:
    std::shared_ptr<Node> add_node(int new_key, const std::shared_ptr<Node>& node) {
        if (new_key < node->key) {
            if (!node->left) {
                node->left = std::make_shared<Node>(new_key);
            } else {
                node->left = add_node(new_key, node->left);
            }
        }
        else {
            if (!node->right) {
                node->right = std::make_shared<Node>(new_key);
            } else {
                node->right = add_node(new_key, node->right);
            }
        }
        return balance(node);
    }

    [[nodiscard]] static inline int get_balance(const std::shared_ptr<Node>& node) {
        unsigned char left_height = node->left ? node->left->height: 0;
        unsigned char right_height = node->right ? node->right->height: 0;
        return right_height - left_height;
    }


    static inline void fix_height(const std::shared_ptr<Node>& node) {
        unsigned char left_height = node->left ? node->left->height: 0;
        unsigned char right_height = node->right ? node->right->height: 0;
        node->height = std::max(left_height, right_height) + 1;
    }

    std::shared_ptr<Node> right_rotation(const std::shared_ptr<Node>& node) {
        auto new_root = node->left;
        node->left =  node->left->right;
        new_root->right = node;
        fix_height(node);
        fix_height(new_root);
        return new_root;
    }

    std::shared_ptr<Node> left_rotation(std::shared_ptr<Node> node) {
        auto new_root = node->right;
        node->right = node->right->left;
        new_root->left = node;
        fix_height(node);
        fix_height(new_root);
        return new_root;
    }

    std::shared_ptr<Node> balance(std::shared_ptr<Node> node) {
        fix_height(node);
        if (get_balance(node) == 2) {
            if (get_balance(node->right) < 0) {
                node->right = right_rotation(node->right);
            }
            return left_rotation(node);
        }
        if (get_balance(node) == -2) {
            if (get_balance(node->left) > 0) {
                node->left = left_rotation(node->left);
            }
            return right_rotation(node);
        }
        return node;
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


