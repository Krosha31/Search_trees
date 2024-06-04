#include <iostream>
#include <memory>

class Node {
public:
    int key;

    Node() = delete;

    explicit Node(int key): key(key){}

    Node(const Node& node) {
        key = node.key;
        left = node.left;
        right = node.right;
        height = node.height;
    }

    [[nodiscard]] inline int get_balance() const{
        unsigned char left_height = left ? left->height: 0;
        unsigned char right_height = right ? right->height: 0;
        return right_height - left_height;
    }

    std::shared_ptr<Node> add_node(int new_key) {
        if (new_key < key) {
            if (!left) {
                left = std::make_shared<Node>(new_key);
            }
            else {
                left = left->add_node(new_key);
            }
        }
        else {
            if (!right) {
                right = std::make_shared<Node>(new_key);
            }
            else {
                right = right->add_node(new_key);
            }
        }
        return balance();
    }

    void print_tree() const {
        if (left) {
            left->print_tree();
        }
        std::cout << key << " ";
        if (right) {
            right->print_tree();
        }
    }


//private:
    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> right = nullptr;
    unsigned char height = 1;


    inline void fix_height() {
        unsigned char left_height = left ? left->height: 0;
        unsigned char right_height = right ? right->height: 0;
        height = std::max(left_height, right_height) + 1;
    }

    std::shared_ptr<Node> right_rotation() {
        auto new_root = left;
        left = left->right;
        new_root->right = std::make_shared<Node>(*this);
        this->fix_height();
        new_root->fix_height();
        return new_root;
    }

    std::shared_ptr<Node> left_rotation() {
        auto new_root = right;
        right = right->left;
        new_root->left = std::shared_ptr<Node>(this);
        this->fix_height();
        new_root->fix_height();
        return new_root;
    }

    std::shared_ptr<Node> balance() {
        this->fix_height();
        if (this->get_balance() == 2) {
            if (right->get_balance() < 0) {
                right = right->right_rotation();
            }
            return this->left_rotation();
        }
        if (this->get_balance() == -2) {
            if (left->get_balance() > 0) {
                left = left->left_rotation();
            }
            return this->right_rotation();
        }
        return std::make_shared<Node>(*this);
    }
};


