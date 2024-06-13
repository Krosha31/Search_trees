#include <iostream>
#include <memory>

class SearchTree {
    struct Node {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
        int key;

        Node() {
            left = nullptr;
            right = nullptr;
            key = 0;
        }

        Node(int new_key) {
            key = new_key;
        }
    };
    std::shared_ptr<Node> _root;
public:
    const std::string name = "stock";
    SearchTree() = delete;

    explicit SearchTree(int key): _root(std::make_shared<Node>(key)){}

    SearchTree(const SearchTree& node) {
        _root = node._root;
    }

    void add_node(int new_key) {
        add_node(new_key, _root);
    }

    void print_tree() const {
        print_tree(_root, 0);
    }

    bool find(int key) {
        std::shared_ptr<Node> temp = _root;
        while (temp) {
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

    void remove_node(int removed_key) {
        _root = remove_node(removed_key, _root);
    }

    int get_height() const {
        return get_height(_root);
    }

private:
    void add_node(int new_key, std::shared_ptr<Node> node) {
        if (new_key < node->key) {
            if (!node->left) {
                node->left = std::make_shared<Node>(new_key);
            } else {
                add_node(new_key, node->left);
            }
        }
        else {
            if (!node->right) {
                node->right = std::make_shared<Node>(new_key);
            } else {
                add_node(new_key, node->right);
            }
        }
    }

    [[nodiscard]] std::shared_ptr<Node> get_min(std::shared_ptr<Node> node) const {
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    std::shared_ptr<Node> remove_node(int removed_key, std::shared_ptr<Node> node) {
        if (!node) {
            return node;
        }
        if (removed_key < node->key) {
            node->left = remove_node(removed_key, node->left);
        } else if (removed_key > node->key) {
            node->right = remove_node(removed_key, node->right);
        } else if (node->left && node->right) {
            node->key = get_min(node->right)->key;
            node->right = remove_node(node->key, node->right);
        } else {
            if (node->left) {
                node = node->left;
            } else if (node->right) {
                node = node->right;
            } else {
                node = nullptr;
            }
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

    int get_height(std::shared_ptr<Node> node) const {
        if (node) {
            return 1 + std::max(get_height(node->left), get_height(node->right));
        }
        return 0;
    }
};