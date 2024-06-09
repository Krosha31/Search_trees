#include <iostream>
#include <memory>


class RBTree {
    enum class Color {BLACK, RED};
    struct Node {
        std::shared_ptr<Node> parent = nullptr;
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
        int key;
        Color color = Color::RED;

        Node() {
            key = 0;
        }

        Node(int new_key) {
            key = new_key;
        }
    };
    std::shared_ptr<Node> _root;
    std::shared_ptr<Node> _null;
public:
    RBTree() = delete;

    explicit RBTree(int key){
        _root = std::make_shared<Node>(key);
        _root->color = Color::BLACK;
        _root->left = _root->right = _null;
    }

    RBTree(const RBTree& node) {
        _root = node._root;
    }

    void add_node(int new_key) {
        add_node(new_key, _root);
    }

    void print_tree() const {
        print_tree(_root, 0);
    }
private:
    void add_node(int new_key, const std::shared_ptr<Node>& node) {
        if (new_key < node->key) {
            if (!node->left) {
                node->left = std::make_shared<Node>(new_key);
                node->left->left = node->left->right = _null;
                node->left->parent = node;
                balance_after_adding(node->left);
            } else {
                add_node(new_key, node->left);
            }
        } else {
            if (!node->right) {
                node->right = std::make_shared<Node>(new_key);
                node->right->left = node->right->right = _null;
                node->right->parent = node;
                balance_after_adding(node->right);
            } else {
                add_node(new_key, node->right);
            }
        }
    }

    std::shared_ptr<Node> balance_after_adding(std::shared_ptr<Node> node) {
        std::shared_ptr<Node> some_name;
        while (node->parent->color == Color::RED) {
            if (node->parent == node->parent->parent->right) {
                some_name = node->parent->parent->left;
                if (some_name->color == Color::RED) {
                    some_name->color = Color::BLACK;
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        right_rotation(node);
                    }
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    left_rotation(node->parent->parent);
                }
            } else {
                some_name = node->parent->parent->right;
                if (some_name->color == Color::RED) {
                    some_name->color = Color::BLACK;
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        left_rotation(node);
                    }
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    right_rotation(node->parent->parent);
                }
            }
        }
        _root->color = Color::BLACK;
    }

    void left_rotation(std::shared_ptr<Node> x_node) {
        auto y_node = x_node->right;
        x_node->right = y_node->left;
        if (y_node->left != _null) {
            y_node->left->parent = x_node;
        }
        y_node->parent = x_node->parent;
        if (x_node->parent == nullptr) {
            _root = y_node;
        } else if (x_node == x_node->parent->left) {
            x_node->parent->left = y_node;
        } else {
            x_node->parent->right = y_node;
        }
        y_node->left = x_node;
        x_node->parent = y_node;
    }


    void right_rotation(std::shared_ptr<Node> x_node) {
        auto y_node = x_node->left;
        x_node->left = y_node->right;
        if (y_node->right != _null) {
            y_node->right->parent = x_node;
        }
        y_node->parent = x_node->parent;
        if (x_node->parent == nullptr) {
            _root = y_node;
        } else if (x_node == x_node->parent->right) {
            x_node->parent->right = y_node;
        } else {
            x_node->parent->left = y_node;
        }
        y_node->right = x_node;
        x_node->parent = y_node;
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