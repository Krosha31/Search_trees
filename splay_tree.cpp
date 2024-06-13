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
    const std::string name = "splay";
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

    int get_height() const {
        return get_height(_root);
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
            std::cout << "There is no such element" << std::endl;
            return;
        }
        if (node->key == removed_key) {
            if (node->right && node->left) {
                splay(node);
                node->left->parent = nullptr;
                node->right->parent = nullptr;
                merge(node->left, node->right);
            } else {
                std::shared_ptr<Node> new_node;
                if (!node->left && !node->right) {
                    new_node = nullptr;
                } else if (!node->left) {
                    new_node = node->right;
                } else {
                    new_node = node->left;
                }
                if (!node->parent) {
                    _root = new_node;
                    _root->parent = nullptr;
                } else if (node == node->parent->left) {
                    node->parent->left = new_node;
                    if (new_node) {
                        new_node->parent = node->parent;
                    }
                } else {
                    node->parent->right = new_node;
                    if (new_node) {
                        new_node->parent = node->parent;
                    }
                }
            }
        } else if (node->key > removed_key) {
            remove_node(removed_key, node->left);
        } else {
            remove_node(removed_key, node->right);
        }
    }

    void rotate_left(std::shared_ptr<Node> node) {
        auto parent_node = node->parent;
        auto right_son_node = node->right;
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
                    rotate_right(node->parent);
                } else if (node->parent == node->parent->parent->left) {
                    rotate_right(node->parent->parent);
                    rotate_right(node->parent);
                } else {
                    rotate_right(node->parent);
                    rotate_left(node->parent);
                }
            }
            else {
                if (!node->parent->parent) {
                    rotate_left(node->parent);
                } else if (node->parent == node->parent->parent->right) {
                    rotate_left(node->parent->parent);
                    rotate_left(node->parent);
                } else {
                    //std::cout << 1 << std::endl;
                    rotate_left(node->parent);
                    //std::cout << 2 << std::endl;
                    rotate_right(node->parent);
                    //std::cout << 3 << std::endl;
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

    int get_height(std::shared_ptr<Node> node) const {
        if (node) {
            return 1 + std::max(get_height(node->left), get_height(node->right));
        }
        return 0;
    }
};