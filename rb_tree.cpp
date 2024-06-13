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
    const std::string name = "red_black";
    RBTree() = delete;

    explicit RBTree(int key){
        _null = std::make_shared<Node>(1);
        _null->color = Color::BLACK;
        _root = std::make_shared<Node>(key);
        _root->color = Color::BLACK;
        _root->left = _root->right = _null;
    }

    RBTree(const RBTree& tree) {
        _root = tree._root;
        _null = tree._root;
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
        while (temp != _null) {
            if (temp->key == key) {
                return true;
            } else if (temp->key > key) {
                temp = temp->left;
            } else {
                temp = temp->right;
            }
        }
        return false;
    }

    [[nodiscard]] int get_height() const {
        return get_height(_root);
    }

private:
    void add_node(int new_key, std::shared_ptr<Node> node) {
        if (new_key < node->key) {
            if (node->left == _null) {
                node->left = std::make_shared<Node>(new_key);
                node->left->left = node->left->right = _null;
                node->left->parent = node;
                balance_after_add(node->left);
            } else {
                add_node(new_key, node->left);
            }
        } else {
            if (node->right == _null) {
                node->right = std::make_shared<Node>(new_key);
                node->right->left = node->right->right = _null;
                node->right->parent = node;
                balance_after_add(node->right);
            } else {
                add_node(new_key, node->right);
            }
        }
    }

    void balance_after_add(std::shared_ptr<Node> node) {
        std::shared_ptr<Node> some_name;
        while (node->parent && node->parent->color == Color::RED) {
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
                        rotate_right(node);
                    }
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    rotate_left(node->parent->parent);
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
                        rotate_left(node);
                    }
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    rotate_right(node->parent->parent);
                }
            }
        }
        _root->color = Color::BLACK;
    }

    void rotate_left(std::shared_ptr<Node> x_node) {
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


    void rotate_right(std::shared_ptr<Node> x_node) {
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

    void transplant_tree(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
        if (!node1->parent) {
            _root = node2;
        } else if (node1 == node1->parent->left) {
            node1->parent->left = node2;
        } else {
            node1->parent->right = node2;
        }
        node2->parent = node1->parent;
    }
    
    std::shared_ptr<Node> get_min(std::shared_ptr<Node> node) {
        while (node->left != _null) {
            node = node->left;
        }
        return node;
    }

    void remove_node(int removed_key, std::shared_ptr<Node> node) {
        std::shared_ptr<Node> z = _null;
        std::shared_ptr<Node> x, y;
        while (node != _null) {
            if (node->key == removed_key) {
                z = node;
            }

            if (node->key <= removed_key) {
                node = node->right;
            } else {
                node = node->left;
            }
        }

        if (z == _null) {
            std::cout << "Key not found in the tree" << std::endl;
            return;
        }

        y = z;
        Color y_original_color = y->color;
        if (z->left == _null) {
            x = z->right;
            transplant_tree(z, z->right);
        } else if (z->right == _null) {
            x = z->left;
            transplant_tree(z, z->left);
        } else {
            y = get_min(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant_tree(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant_tree(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        if (y_original_color == Color::BLACK) {
            balance_after_remove(x);
        }
    }
    
    void balance_after_remove(std::shared_ptr<Node> x) {
        std::shared_ptr<Node> s;
        while (x != _null &&x != _root && x->color == Color::BLACK) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == Color::RED) {
                    s->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rotate_left(x->parent);
                    s = x->parent->right;
                }
                if (s->left->color == Color::BLACK && s->right->color == Color::BLACK) {
                    s->color = Color::RED;
                    x = x->parent;
                } else {
                    if (s->right->color == Color::BLACK) {
                        s->left->color = Color::BLACK;
                        s->color = Color::RED;
                        rotate_right(s);
                        s = x->parent->right;
                    }

                    s->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    s->right->color = Color::BLACK;
                    rotate_left(x->parent);
                    x = _root;
                }
            } else {
                s = x->parent->left;
                if (s->color == Color::RED) {
                    s->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rotate_right(x->parent);
                    s = x->parent->left;
                }
                if (s == _null) {
                    std::cout << x->right << " " << x->left;
                    print_tree();
                }
                if (s->right->color == Color::BLACK && s->right->color == Color::BLACK) {
                    s->color = Color::RED;
                    x = x->parent;
                } else {
                    if (s->left->color == Color::BLACK) {
                        s->right->color = Color::BLACK;
                        s->color = Color::RED;
                        rotate_left(s);
                        s = x->parent->left;
                    }

                    s->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    s->left->color = Color::BLACK;
                    rotate_right(x->parent);
                    x = _root;
                }
            }
        }
        x->color = Color::BLACK;
    }

    void print_tree(const std::shared_ptr<Node>& node, int level) const {
        if (node != _null) {
            print_tree(node->right, level + 1);
            for (int i = 0; i < level; ++i) {
                std::cout <<"\t\t\t\t";
            }
            std::cout << node->key << '(' << static_cast<int>(node->color) << ")" << '\n';
            print_tree(node->left, level + 1);
        }
    }

    int get_height(std::shared_ptr<Node> node) const {
        if (node) {
            return 1 + std::max(get_height(node->left), get_height(node->right));
        }
        return 0;
    }
};