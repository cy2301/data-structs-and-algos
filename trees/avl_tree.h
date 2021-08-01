#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>

template <typename Comparable>
class AVLTree {
    private:
        struct TreeNode {
            Comparable data;
            int height; // height is the max of the heights of the left and right subtrees
            TreeNode* left;
            TreeNode* right;
        };
        TreeNode* root;
    public:
        AVLTree <Comparable>() : root(nullptr) {}
        AVLTree(const AVLTree& other) : root(nullptr) {
            root = copyHelper2(other.root);
        }
        AVLTree& operator=(const AVLTree& other) {
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
            newNode->height = node->height;
            newNode->left = copyHelper2(node->left); // newNode is defined, cool. But its left and right aren't. Thus we define them here
            newNode->right = copyHelper2(node->right);
            return newNode; // once left and right are defined, we return the node. All linked up and nice. If this wasn't the root, it could be some other node's left or right
        }
        ~AVLTree() {
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
                    root->height = 0;
                    return root;
                }
                if (node == nullptr)
                {
                    TreeNode* newNode = new TreeNode;
                    newNode->left = nullptr;
                    newNode->right = nullptr;
                    newNode->data = value;
                    newNode->height = 0;
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
                // NEW STUFF HERE
                node->height = 1 + maxHeightLeftRight(node); // 1 to account for itself + the max, height is always most nodes to null
                
                // ROTATION CONDITIONS
                int balance = getBalance(node); // get the balance, this will tell us which side we're working with
                // Single rotation left
                if (balance < -1 && value > node->right->data) // balance is -2, meaning working with right side tree. (-1) - (1) = -2. Second part tells us the structure
                {
                    TreeNode* temp = singleRotateLeft(node); // rotate on the node
                    if (node == root) // if the node we were rotating on was the root, make sure to set the new parent as the new root
                    {
                        root = temp;
                    }
                    return temp;
                }
                // Double rotation right left
                else if (balance < -1 && value < node->right->data)
                {
                    node->right = singleRotateRight(node->right); // single rotate on the node->right then link it back up
                    TreeNode* tempFinal = singleRotateLeft(node); // single rotate on the current node
                    if (node == root) // if the current node we used to rotate was the root, set the tempFinal (new parent) as the new root
                    {
                        root = tempFinal;
                    }
                    return tempFinal;
                }
                // Single rotation right
                else if (balance > 1 && value < node->left->data) // balance is 2, meaning working with left side tree. (1) - (-1) = 2.
                {
                    TreeNode* temp = singleRotateRight(node);
                    if (node == root)
                    {
                        root = temp;
                    }
                    return temp;
                }
                // Double rotation left right
                else if (balance > 1 && value > node->left->data)
                {
                    node->left = singleRotateLeft(node->left);
                    TreeNode* tempFinal = singleRotateRight(node);
                    if (node == root)
                    {
                        root = tempFinal;
                    }
                    return tempFinal;
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
                        if (root == node) // if we're deleting the root, set new root as the right child
                        {
                            root = toReturn;
                        }
                        delete node;
                        node = toReturn;
                    }
                    else if (node->right == nullptr && node->left != nullptr) // one child (left)
                    {
                        TreeNode* toReturn = node->left;
                        if (root == node)
                        {
                            root = toReturn;
                        }
                        delete node;
                        node = toReturn;
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
                // NEW STUFF HERE
                node->height = 1 + maxHeightLeftRight(node);
                
                // ROTATION CONDITIONS
                int balance = getBalance(node);
                // Single rotation left
                if (balance < -1 && getBalance(node->right) <= 0) 
                // KEY THING: value being deleted DOES NOT MATTER, the structure of the tree does. Use getBalance to find the structure of the tree after removal
                // node->right->left can be 0 or -1, (0) - (0) or (-1) - (0) which leaves us with a structure in need of a single rotation (straight line)
                // if node->right->right was 0, then it's a straight line, but if it was -1, then we get a zig zag which is in need of a double rotation
                {
                    TreeNode* temp = singleRotateLeft(node);
                    if (node == root)
                    {
                        root = temp;
                    }
                    return temp;
                }
                // Double rotation right left
                else if (balance < -1 && getBalance(node->right) > 0)
                {
                    node->right = singleRotateRight(node->right);
                    TreeNode* tempFinal = singleRotateLeft(node);
                    if (node == root)
                    {
                        root = tempFinal;
                    }
                    return tempFinal;
                }
                // Single rotation right
                else if (balance > 1 && getBalance(node->left) >= 0)
                {
                    TreeNode* temp = singleRotateRight(node);
                    if (node == root)
                    {
                        root = temp;
                    }
                    return temp;
                }
                // Double rotation left right
                else if (balance > 1 && getBalance(node->left) < 0)
                {
                    node->left = singleRotateLeft(node->left);
                    TreeNode* tempFinal = singleRotateRight(node);
                    if (node == root)
                    {
                        root = tempFinal;
                    }
                    return tempFinal;
                }
            }
            return node;
        }

        int getBalance(TreeNode* node) {
            if (node->right && node->left == nullptr) // only one exists
            {
                return (-1) - node->right->height; 
            }
            else if (node->left && node->right == nullptr)
            {
                return node->left->height - (-1);
            }
            else if (node->left && node->right) // both exists
            {
                return node->left->height - node->right->height;
            }
            return 0; // if both are null
        }

        int maxHeightLeftRight(TreeNode* node) {
            if (node->left && node->right) // if both exists
            {
                if (node->left->height > node->right->height)
                {
                    return node->left->height;
                }
                else if (node->right->height > node->left->height)
                {
                    return node->right->height;
                }
                else if (node->right->height == node->left->height)
                {
                    return node->left->height;
                }
            }
            if (node->left && node->right == nullptr) // only 1 exists
            {
                return node->left->height;
            }
            if (node->right && node->left == nullptr)
            {
                return node->right->height;
            }
            return -1; // if both are null, return -1
        }

        TreeNode* singleRotateLeft(TreeNode* node) {
            TreeNode* newParent = node->right;
            node->right = newParent->left;
            newParent->left = node;

            node->height = 1 + maxHeightLeftRight(node);
            newParent->height = 1 + maxHeightLeftRight(newParent);
            
            return newParent;
        }
        
        TreeNode* singleRotateRight(TreeNode* node) {
            TreeNode* newParent = node->left;
            node->left = newParent->right;
            newParent->right = node;

            node->height = 1 + maxHeightLeftRight(node);
            newParent->height = 1 + maxHeightLeftRight(newParent);
            
            return newParent;
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
        void print_tree(std::ostream& os = std::cout) const {
            if (root == nullptr)
            {
                os << "<empty>\n";
                return;
            }
            printTreeHelper(os, root, 2);
        }

        void printTreeHelper(std::ostream& os, TreeNode* root, int space) const {
            if (root == nullptr)
            {
                return;
            }
            space += 2;
            printTreeHelper(os, root->right, space);
            for (int i = 4; i < space; i++)
            {
                os << " ";
            }
            os << root->data << "\n";
            printTreeHelper(os, root->left, space);
        }
        
        Comparable& getRoot() {
            return root->data;
        }
        
};

#endif