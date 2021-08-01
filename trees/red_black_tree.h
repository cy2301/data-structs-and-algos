#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <iostream>

template <typename Comparable>
class RedBlackTree {
    private: 
        struct Node {
            Comparable value;
            Node* left;
            Node* right;
            Node* parent;
            int color;
        };
        Node* root;
    public:
        enum Color {RED, BLACK};
        
        RedBlackTree <Comparable>() : root(nullptr) {}

        RedBlackTree(const RedBlackTree& other) : root(nullptr) {
            root = copyHelper(other.root);
            if (root)
            {
                root->parent = nullptr;
            }
        }

        RedBlackTree& operator=(const RedBlackTree& other) {
            if (this == &other)
            {
                return *this;
            }
            destroyTree(root);
            root = copyHelper(other.root);
            if (root) 
            {
                root->parent = nullptr;
            }
            return *this;
        }

        Node* copyHelper(Node* node) { // manually construct nodes
            if (node == nullptr)
            {
                return nullptr;
            }
            Node* newNode = new Node;
            newNode->value = node->value;
            newNode->color = node->color;
            newNode->left = copyHelper(node->left); // newNode is defined, cool. But its left and right aren't. Thus we define them here
            newNode->right = copyHelper(node->right);
            if (newNode->left) { // if the left turns out to actually exist, we set its parent to this node. we also set its siblings to the right of the this node
                newNode->left->parent = newNode;
            }
            if (newNode->right) {
                newNode->right->parent = newNode;
            }
            /* -- doesn't work, parent hasn't been set yet and when it enters its own function call
            if (newNode->parent && (newNode == newNode->parent->right))
                newNode->sibling = parent->left;
            else if (newNode->parent && (newNode == newNode->parent->left))
                newNode->sibling = parent->right;
            */
            return newNode; // once left and right are defined, we return the node. All linked up and nice. If this wasn't the root, it could be some other node's left or right
        }

        ~RedBlackTree() {
            destroyTree(root);
        }

        void destroyTree(Node* node)
        {
            if (node) // if node is not null
            {
                destroyTree(node->left); // delete its left node and all its subtrees
                destroyTree(node->right); // delete its right node and all its subtrees
                delete node; // delete the node itself
            }
        }

        void insert(const Comparable& data) {
            if (!contains(data)) {
                Node* toInsert = new Node;
                toInsert->left = nullptr;
                toInsert->right = nullptr;
                toInsert->parent = nullptr;
                toInsert->value = data;
                toInsert->color = RED;
                root = insertHelper(toInsert, root);

                fixInsert(toInsert);
            }
        }

        Node* insertHelper(Node* toInsert, Node* node) {
            if (root == nullptr)
            {
                toInsert->color = BLACK;
                root = toInsert;
                return root;
            }
            else if (node == nullptr)
            {
                return toInsert;
            }
            else if (toInsert->value > node->value)
            {
                node->right = insertHelper(toInsert, node->right);
                node->right->parent = node;
            }
            else if (toInsert->value < node->value)
            {
                node->left = insertHelper(toInsert, node->left);
                node->left->parent = node;
            }
            return node;
        }

        void fixInsert(Node* node) {
            while ((node != root) && (node->color == RED) && (node->parent->color == RED)) 
            {
                if (node->parent == node->parent->parent->right) // right side case
                {
                    if (node->parent->parent->left && node->parent->parent->left->color == RED) // uncle exists and is red, recolor
                    {
                        node->parent->parent->color = RED; // grandparent to red
                        node->parent->parent->left->color = BLACK; // uncle to black
                        node->parent->color = BLACK; // parent to black
                        node = node->parent->parent; // set node to grandparent to keep trying
                    }
                    else // uncle doesn't exist or is black
                    {
                        if (node == node->parent->right) // RR case, single rotate left
                        {
                            singleRotateLeft(node->parent->parent); // single rotate on the highest node of the 3
                            node = node->parent; // set node to the new root of the subtree
                        }
                        else if (node == node->parent->left) // RL case, double rotate right left
                        {
                            singleRotateRight(node->parent);
                            singleRotateLeft(node->parent);
                        }
                    }
                }
                else if (node->parent == node->parent->parent->left) // left side case
                {
                    if (node->parent->parent->right && node->parent->parent->right->color == RED)
                    {
                        node->parent->parent->color = RED; // grandparent to red
                        node->parent->parent->right->color = BLACK; // uncle to black
                        node->parent->color = BLACK; // parent to black
                        node = node->parent->parent; // set node to grandparent to keep trying
                    }
                    else 
                    {
                        if (node == node->parent->left) // LL case, single rotate right
                        {
                            singleRotateRight(node->parent->parent);
                            node = node->parent;
                        }
                        else if (node == node->parent->right) // LR case, double rotate left right
                        {
                            singleRotateLeft(node->parent);
                            singleRotateRight(node->parent);
                        }
                    }
                }
            }
            root->color = BLACK;
        }

        void remove(const Comparable& data) {
            if (contains(data)) {
                removeHelper(data);
            }
        }

        void removeHelper(const Comparable& data) {
            bool replaceValueNode = false;
            bool replaceValueRoot = false;
            Comparable temp;
            Node* marker = root;
            Node* sibling = nullptr;
            Node* toFind = find(data, root);
            if ((toFind == root) && (root == nullptr)) // root is only node or doesn't exist
            {
                root = nullptr;
                return;
            }
            if (toFind == root && !root->right && !root->left)
            {
                delete root;
                root = nullptr;
                return;
            }
            if (toFind == root) 
            /*
            if we're deleting the root, we find the smallest node on the right subtree to delete instead.
            The actual value replacement will be done at the very end to avoid duplicates when performing the
            actual remove
            */
            {
                replaceValueRoot = true;
                temp = find_minNode(root->right)->value;
                toFind = find(temp,root->right);
            }
            else if (toFind->right && toFind->left) // same as above
            {
                replaceValueNode = true;
                temp = find_minNode(toFind->right)->value;
                toFind = find(temp,toFind->right);
            }
            if (!toFind->right && !toFind->left && toFind->color == RED)
            // if the node we're deleting is a leaf (no children) and it's color is red, then we simply delete it
            {
                if (toFind->parent->right == toFind)
                {
                    toFind->parent->right = nullptr;
                }
                else if (toFind->parent->left == toFind)
                {
                    toFind->parent->left = nullptr;
                }
                delete toFind;
                toFind = nullptr;
                marker = toFind;
            }
            while (marker != toFind)
            {
                // STEP 1
                if (marker == root) // if it's the root, check if its children are black. if they are advance them
                {
                    if ((!marker->right || marker->right->color == BLACK) && (!marker->left || marker->left->color == BLACK))
                    {
                        marker->color = RED;
                        if (toFind->value > marker->value)
                        {
                            marker = marker->right;
                        }
                        else if (toFind->value < marker->value)
                        {
                            marker = marker->left;
                        }
                        if (marker->parent->right == marker)
                            sibling = marker->parent->left;
                        else if (marker->parent->left == marker)
                            sibling = marker->parent->right;
                    }
                }
                if ((marker->right && marker->right->color == RED) || (marker->left && marker->left->color == RED) || (marker->color == RED))
                {
                    if (toFind->value > marker->value) // which way to traverse solving unique root case
                    {
                        marker = marker->right;
                    }
                    else if (toFind->value < marker->value)
                    {
                        marker = marker->left;
                    }
                    if (marker->parent->right == marker)
                        sibling = marker->parent->left;
                    else if (marker->parent->left == marker)
                        sibling = marker->parent->right;

                    if (marker->color == BLACK && marker->parent->color == BLACK && (sibling || sibling->color == RED))
                    {
                        if (marker->parent->left == marker)
                        {
                            singleRotateLeftRemove(marker->parent);
                            marker->parent->color = RED;
                            marker->parent->parent->color = BLACK;
                            if (marker->parent->right == marker)
                                sibling = marker->parent->left;
                            else if (marker->parent->left == marker)
                                sibling = marker->parent->right;
                        }
                        else if (marker->parent->right == marker)
                        {
                            singleRotateRightRemove(marker->parent);
                            marker->parent->color = RED;
                            marker->parent->parent->color = BLACK;
                            if (marker->parent->right == marker)
                                sibling = marker->parent->left;
                            else if (marker->parent->left == marker)
                                sibling = marker->parent->right;
                        }
                    }
                }
                // STEP 2
                if ((marker->color == BLACK) && (!marker->right || marker->right->color == BLACK) && (!marker->left || marker->left->color == BLACK)) // x has 2 black nodes
                {
                    // CASE 2A1
                    if ((!sibling->right || sibling->right->color == BLACK) && (!sibling->left || sibling->left->color == BLACK)) // if sibling also has black children
                    {
                        marker->parent->color = BLACK;
                        marker->color = RED;
                        sibling->color = RED;
                    }
                    // CASE 2A2
                    else if (marker == marker->parent->left && sibling->left && sibling->left->color == RED) // double rotate right left
                    {
                        singleRotateRightRemove(sibling);
                        singleRotateLeftRemove(sibling->parent->parent);
                        sibling->color = BLACK;
                        marker->parent->color = BLACK;
                        marker->color = RED;
                    }
                    else if (marker == marker->parent->right && sibling->right && sibling->right->color == RED) // double rotate left right
                    {
                        singleRotateLeftRemove(sibling);
                        singleRotateRightRemove(sibling->parent->parent);
                        sibling->color = BLACK;
                        marker->parent->color = BLACK;
                        marker->color = RED;
                    }
                    // CASE 2A3
                    else if (marker == marker->parent->left && sibling->right && sibling->right->color == RED) // single rotate left
                    {
                        singleRotateLeftRemove(sibling->parent);
                        sibling->color = RED;
                        sibling->right->color = BLACK;
                        marker->parent->color = BLACK;
                        marker->color = RED;
                    }
                    else if (marker == marker->parent->right && sibling->left && sibling->left->color == RED) // single rotate right
                    {
                        singleRotateRightRemove(sibling->parent);
                        sibling->color = RED;
                        sibling->left->color = BLACK;
                        marker->parent->color = BLACK;
                        marker->color = RED;
                    }
                }
            }
            if (marker != nullptr)
            {
                if (marker->right && !marker->left)
                {
                    marker->value = marker->right->value;
                    delete marker->right;
                    marker->right = nullptr;
                }
                else if (marker->left && !marker->right)
                {
                    marker->value = marker->left->value;
                    delete marker->left;
                    marker->left = nullptr;
                }
                else if (!marker->left && !marker->right)
                {
                    if (marker->parent->right == marker)
                        marker->parent->right = nullptr;
                    else if (marker->parent->left == marker)
                        marker->parent->left = nullptr;
                    delete marker;
                }
            }
            if (replaceValueRoot)
            {
                root->value = temp;
            }
            if (replaceValueNode)
            {
                toFind = find(data, root);
                toFind->value = temp;
            }
            root->color = BLACK;
        }
        
        void singleRotateLeft(Node* node) {
            Node* newParent = node->right;
            node->right = newParent->left;
            newParent->left = node;

            int tempColor = newParent->color;
            newParent->color = node->color;
            node->color = tempColor;

            if (node != root) // if the node was not the root pointer
            {
                newParent->parent = node->parent; // sets newParent's parent to the old node's parent
                if (node->parent->right && node->parent->right == node) // determines which side to link back
                    newParent->parent->right = newParent; // links newParent back up to the tree
                else if (node->parent->left && node->parent->left == node)
                    newParent->parent->left = newParent; // links newParent back up to the tree
            }
            else 
            {
                root = newParent; // if the node was the root, we set the new root to be the newParent
            }
            node->parent = newParent; // the old node's parent is set to newParent
            if (node->right)
                node->right->parent = node;
            
        }
        
        void singleRotateRight(Node* node) { // fix siblings and parents
            Node* newParent = node->left;
            node->left = newParent->right;
            newParent->right = node;
            
            int tempColor = newParent->color;
            newParent->color = node->color;
            node->color = tempColor;

            if (node != root) // if the node was not the root pointer
            {
                newParent->parent = node->parent; // sets newParent's parent to the old node's parent
                if (node->parent->right && node->parent->right == node) // determines which side to link back
                    newParent->parent->right = newParent; // links newParent back up to the tree
                else if (node->parent->left && node->parent->left == node)
                    newParent->parent->left = newParent; // links newParent back up to the tree
            }
            else 
            {
                root = newParent;
            }
            node->parent = newParent; // the old node's parent is set to newParent
            if (node->left)
                node->left->parent = node;
        }

        void singleRotateLeftRemove(Node* node) {
            Node* newParent = node->right;
            node->right = newParent->left;
            newParent->left = node;

            if (node != root) // if the node was not the root pointer
            {
                newParent->parent = node->parent; // sets newParent's parent to the old node's parent
                if (node->parent->right && node->parent->right == node) // determines which side to link back
                    newParent->parent->right = newParent; // links newParent back up to the tree
                else if (node->parent->left && node->parent->left == node)
                    newParent->parent->left = newParent; // links newParent back up to the tree
            }
            else 
            {
                root = newParent; // if the node was the root, we set the new root to be the newParent
            }
            node->parent = newParent; // the old node's parent is set to newParent
            if (node->right)
                node->right->parent = node;
            
        }

        void singleRotateRightRemove(Node* node) { // fix siblings and parents
            Node* newParent = node->left;
            node->left = newParent->right;
            newParent->right = node;

            if (node != root) // if the node was not the root pointer
            {
                newParent->parent = node->parent; // sets newParent's parent to the old node's parent
                if (node->parent->right && node->parent->right == node) // determines which side to link back
                    newParent->parent->right = newParent; // links newParent back up to the tree
                else if (node->parent->left && node->parent->left == node)
                    newParent->parent->left = newParent; // links newParent back up to the tree
            }
            else 
            {
                root = newParent;
            }
            node->parent = newParent; // the old node's parent is set to newParent
            if (node->left)
                node->left->parent = node;
        }

        bool contains(const Comparable& data) const {
            return containsHelper(data, root);
        }

        bool containsHelper(const Comparable& data, Node* node) const {
            if (node == nullptr)
            {
                return false;
            }
            else if (data > node->value)
            {
                return containsHelper(data, node->right);
            }
            else if (data < node->value)
            {
                return containsHelper(data, node->left);
            }
            else
            {
                return true;
            }
        }

        Node* find(const Comparable& data, Node* node) const {
            if (node == nullptr)
            {
                return nullptr;
            }
            else if (data > node->value)
            {
                return find(data, node->right);
            }
            else if (data < node->value)
            {
                return find(data, node->left);
            }
            else
            {
                return node;
            }
        }

        const Comparable& find_min() const {
            if (root == nullptr)
            {
                throw std::invalid_argument("");
            }
            Node* _marker = root;
            while (_marker->left) // while _marker->next exists
            {
                _marker = _marker->left;
            }
            return _marker->value;
        }

        const Comparable& find_max() const {
            if (root == nullptr)
            {
                throw std::invalid_argument("");
            }
            Node* _marker = root;
            while (_marker->right) // while _marker->next exists
            {
                _marker = _marker->right;
            }
            return _marker->value;
        }

        Node* find_minNode(Node* node) const {
            Node* marker = node;
            while (marker->left != nullptr)
            {
                marker = marker->left;
            }
            return marker;
        }

        int color(const Node* node) const {
            if (node == nullptr)
            {
                return BLACK;
            }
            return node->color;
        }

        const Node* get_root() const {
            if (root == nullptr)
            {
                return nullptr;
            }
            return root;
        }

        void print_tree(std::ostream& os = std::cout) const {
            if (root == nullptr)
            {
                os << "<empty>\n";
                return;
            }
            printTreeHelper(os,root, 0);
        }

        void printTreeHelper(std::ostream& os, Node* node, int space) const {
            if (node == nullptr)
            {
                return;
            }
            space += 4;
            printTreeHelper(os, node->right, space);
            for (int i = 2; i < space; i++)
            {
                os << " ";
            }
            if (node->color == RED)
                os << "R:" << node->value << "\n";
            else
                os << node->value << "\n";
            printTreeHelper(os, node->left, space);
        }

};

#endif  // RED_BLACK_TREE_H

