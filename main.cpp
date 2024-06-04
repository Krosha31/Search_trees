#include <iostream>
#include <memory>
#include "avl_tree.cpp"
#include <queue>
#include <vector>
#include <algorithm>

//
//template <typename Tree>
//void print_tree(std::shared_ptr<Tree> tree) {
//    int tabs = static_cast<int>(tree->height);
//    std::vector<std::shared_ptr<Tree>> print_queue {tree};
//    // std::any_of(print_queue.begin(), print_queue.end(), [](std::shared_ptr<Tree> node) {return node;})
//    while (tabs != 0) {
//        for (int i = 0; i < tabs; ++i) {
//            std::cout << '\t';
//        }
//        std::vector<std::shared_ptr<Tree>> new_print_queue;
//        for (const auto& node: print_queue) {
//            if (node) {
//                std::cout << node->key << "\t\t";
//                new_print_queue.push_back(node->left);
//                new_print_queue.push_back(node->right);
//            }
//            else {
//                std::cout << " " << "\t\t";
//            }
//        }
//        print_queue = new_print_queue;
//        --tabs;
//        std::cout << '\n';
//    }
//}

template <typename Tree>
void print_tree(std::shared_ptr<Tree> tree, int level) {
    if (tree) {
        print_tree(tree->right, level + 1);
        for (int i = 0; i < level; ++i) {
            std::cout <<'\t';
        }
        std::cout << tree->key << "(" << static_cast<int>(tree->height) << ")" << '\n';
        print_tree(tree->left, level + 1);
    }
}

int main() {
    std::shared_ptr<Node> root = std::make_shared<Node>(1);
    for (int i = 2; i < 4; i++) {
        auto predroot = root;
        root = root->add_node(i);
        //root->print_tree();
    }
    std::cout << static_cast<int>(root->height) << " " << root->get_balance();
    std::cout << '\n';
    print_tree(root, 0);
}
