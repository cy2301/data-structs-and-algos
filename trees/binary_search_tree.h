#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <iostream>
#include <queue>
#include <stack>

using std::queue, std::stack;

template <typename Comparable>
class BinarySearchTree {
    private: 
        struct TreeNode {
            Comparable data;
            TreeNode* left;
            TreeNode* right;
        };
        TreeNode* root;
    public:
        BinarySearchTree <Comparable>() : root(nullptr) {}

        BinarySearchTree(const BinarySearchTree& other) : root(nullptr) {
            root = copyHelper2(other.root);
        }

        BinarySearchTree& operator=(const BinarySearchTree& other) {
            if (this == &other)
            {
                return *this;
            }
            destroyTree(root);
            root = copyHelper2(other.root);
            return *this;
        }
        TreeNode* copyHelper2(TreeNode* node) { // manually construct nodes
            if (node == nullptr)
            {
                return nullptr;
            }
            TreeNode* newNode = new TreeNode;
            newNode->data = node->data;
            newNode->left = copyHelper2(node->left); // newNode is defined, cool. But its left and right aren't. Thus we define them here
            newNode->right = copyHelper2(node->right);
            return newNode; // once left and right are defined, we return the node. All linked up and nice. If this wasn't the root, it could be some other node's left or right
        }

        ~BinarySearchTree() {
            destroyTree(root);
        }

        void destroyTree(TreeNode* node)
        {
            if (node) // if node is not null
            {
                destroyTree(node->left); // delete its left node and all its subtrees
                destroyTree(node->right); // delete its right node and all its subtrees
                delete node; // delete the node itself
            }
        }

        bool contains(const Comparable& value) const {
            return containsHelper(value, root);
        }

        bool containsHelper(const Comparable& value, TreeNode* node) const {
            if (node == nullptr) // base case, gets through entire tree without finding it or empty tree
            {
                return false;
            }
            else if (value > node->data)
            {
                return containsHelper(value, node->right);
            }
            else if (value < node->data)
            {
                return containsHelper(value, node->left);
            }
            else // if equal
            {
                return true;    
            }
        }

        void insert(const Comparable& value) {
            insertHelper(value, root);
        }

        TreeNode* insertHelper (const Comparable& value, TreeNode* node) {
            if (!contains(value))
            {
                if (root == nullptr) // empty tree
                {
                    TreeNode* newNode = new TreeNode;
                    newNode->left = nullptr;
                    newNode->right = nullptr;
                    newNode->data = value;
                    root = newNode;
                    return root;
                }
                if (node == nullptr)
                {
                    TreeNode* newNode = new TreeNode;
                    newNode->left = nullptr;
                    newNode->right = nullptr;
                    newNode->data = value;
                    return newNode;
                }
                else if (value > node->data)
                {
                    node->right = insertHelper(value, node->right);
                }
                else if (value < node->data)
                {
                    node->left = insertHelper(value, node->left);
                }
            }
            return node;
        }

        void remove(const Comparable& value) {
            removeHelper(value, root);
        }

        TreeNode* removeHelper(const Comparable& value, TreeNode* node) {
            if (contains(value))
            {
                if (value > node->data) // keep looking for the node until we find it
                {
                    node->right = removeHelper(value, node->right);
                }
                else if (value < node->data)
                {
                    node->left = removeHelper(value, node->left);
                }
                else // found the node to delete
                {
                    if (node->left == nullptr && node->right != nullptr) // one child (right)
                    {
                        TreeNode* toReturn = node->right; 
                        // save the only child of the parent we are deleting, return that child and it'll link back
                        // Ex: node->right = removeHelper(value,node->right), removeHelper returns the child that is saved
                        // 6
                        //   8 (delete 8)
                        //     10
                        //        12
                        // node6's right = removeHelper(8,node8)
                        // toReturn = node10
                        // delete node8
                        // node6's right = node10
                        if (root == node) // if we're deleting the root, set new root as the right child
                        {
                            root = toReturn;
                        }
                        delete node;
                        return toReturn;
                    }
                    else if (node->right == nullptr && node->left != nullptr) // one child (left)
                    {
                        TreeNode* toReturn = node->left;
                        if (root == node)
                        {
                            root = toReturn;
                        }
                        delete node;
                        return toReturn;
                    }
                    else if (node->left == nullptr && node->right == nullptr) // no children
                    {
                        if (root == node) // if we're deleting the root, set the root to nullptr (DELETING DOES NOT SET TO NULL)
                        {
                            root = nullptr;
                        }
                        delete node;
                        return nullptr;
                    }
                    else // two children 
                    {
                        node->data = find_minNode(node->right)->data; // essentially overwriting the node with the data to replace with
                        node->right = removeHelper(node->data, node->right); // then delete the node with the min data, node takes over identity of the minNode
                    }
                }
            }
            return node;
        }

        const Comparable& find_min() const {
            if (root == nullptr)
            {
                throw std::invalid_argument("");
            }
            TreeNode* marker = root;
            while (marker->left != nullptr)
            {
                marker = marker->left;
            }
            return marker->data;
        }

        TreeNode* find_minNode(TreeNode* node) const {
            TreeNode* marker = node;
            while (marker->left != nullptr)
            {
                marker = marker->left;
            }
            return marker;
        }
        
        const Comparable& find_max() const {
            if (root == nullptr)
            {
                throw std::invalid_argument("");
            }
            TreeNode* marker = root;
            while (marker->right != nullptr)
            {
                marker = marker->right;
            }
            return marker->data;
        }
        /*
        TreeNode* find_maxNode(TreeNode* node) const {
            if (root == nullptr)
            {
                throw std::invalid_argument("");
            }
            TreeNode* marker = node;
            while (marker->right != nullptr)
            {
                marker = marker->right;
            }
            return marker;
        }
        */

        /*
        Comparable& getRoot() {
            return root->data;
        }
        */
        void print_tree(std::ostream& os = std::cout) const {
            if (root == nullptr)
            {
                os << "<empty>\n";
                return;
            }
            printTreeHelper(os,root, 0);
        }

        void printTreeHelper(std::ostream& os, TreeNode* root, int space) const {
            if (root == nullptr)
            {
                return;
            }
            space += 2;
            printTreeHelper(os, root->right, space);
            for (int i = 2; i < space; i++)
            {
                os << " ";
            }
            os << root->data << "\n";
            printTreeHelper(os, root->left, space);
        }

        void inOrderHelper(std::ostream& os, TreeNode* node) const {
            if (node == nullptr)
            {
                return;
            }
            inOrderHelper(os, node->left);
            os << node->data << " ";
            inOrderHelper(os, node->right);
        }

        void inOrderTraversal(std::ostream& os = std::cout) const {
            inOrderHelper(os,root);
            os << "\n";
        }

        void preOrderHelper(std::ostream& os, TreeNode* node) const {
            if (node == nullptr)
            {
                return;
            }
            os << node->data << " ";
            preOrderHelper(os, node->left);
            preOrderHelper(os, node->right);
        }

        void preOrderTraversal(std::ostream& os = std::cout) const {
            preOrderHelper(os,root);
            os << "\n";
        }

        void postOrderHelper(std::ostream& os, TreeNode* node) const {
            if (node == nullptr)
            {
                return;
            }
            postOrderHelper(os, node->left);
            postOrderHelper(os, node->right);
            os << node->data << " ";
        }

        void postOrderTraversal(std::ostream& os = std::cout) const {
            postOrderHelper(os,root);
            os << "\n";
        }

        void breadthFirst(std::ostream& os = std::cout) const {
            queue<TreeNode*> theQ;
            theQ.push(root);
            while (!theQ.empty())
            {
                TreeNode* node = theQ.front();
                os << node->data << " ";
                theQ.pop();
                if (node->left)
                    theQ.push(node->left);
                if (node->right)
                    theQ.push(node->right);
            }
            os << "\n";
        }

        void inOrderIterative(std::ostream& os = std::cout) const {
            stack<TreeNode*> theS;
            TreeNode* node = root;
            while (node != nullptr || !theS.empty())
            {
                while (node != nullptr)
                {
                    theS.push(node);
                    node = node->left;
                }
                node = theS.top();
                os << node->data << " ";
                theS.pop();
                node = node->right;
            }
            os << "\n";
        }
};

#endif