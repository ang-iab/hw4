#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    int findH(AVLNode<Key, Value>* n);
    void rotateRight(Node<Key, Value>* node);
    void rotateLeft(Node<Key, Value>* node);
    void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    AVLNode<Key, Value>* find(const Key& key);
    AVLNode<Key, Value>* findPredecessor(AVLNode<Key, Value>* current);
    void removeFix(AVLNode<Key, Value>* n, int8_t diff);

};

// My Helper Function
template <class Key, class Value>
int AVLTree<Key, Value>::findH(AVLNode<Key, Value>* n)
{
    if (n == nullptr) return 0;

    int lenLeft = 0;
    int lenRight = 0;

    if (n->getLeft() != nullptr)
    {
        lenLeft = 1 + findH(n->getLeft());
    }
    
    if (n->getRight() != nullptr)
    {
        lenRight =  1 + findH(n->getRight());
    }

    if (lenLeft > lenRight) return lenLeft;
    return lenRight;
}

// My Helper Function
template <class Key, class Value>
void AVLTree<Key, Value>::rotateRight(Node<Key, Value>* node)
{
    AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*>(node);

    if (n == nullptr || n->getLeft() == nullptr) return;

    AVLNode<Key, Value>* p = static_cast<AVLNode<Key, Value>*>(n->getParent());
    AVLNode<Key, Value>* c = static_cast<AVLNode<Key, Value>*>(n->getLeft());
    AVLNode<Key, Value>* crc = static_cast<AVLNode<Key, Value>*>(c->getRight());

    // Right Rotation
    c->setRight(n);
    n->setLeft(crc);

    // Update Parents
    n->setParent(c);

    if (crc != nullptr) crc->setParent(n);

    c->setParent(p);
    if (p == nullptr)
    {
        this->root_ = c;
        c->setParent(nullptr);
    }
    else if (p->getLeft() == n)
    {
        p->setLeft(c);
    }
    else if (p->getRight() == n)
    {
        p->setRight(c);
    }
    
    n->setBalance(findH(n->getRight()) - findH(n->getLeft()));
    c->setBalance(findH(c->getRight()) - findH(c->getLeft()));
}

// My Helper Function
template <class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(Node<Key, Value>* node)
{
    AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*>(node);

    if (n == nullptr || n->getRight() == nullptr) return;

    AVLNode<Key, Value>* p = static_cast<AVLNode<Key, Value>*>(n->getParent());
    AVLNode<Key, Value>* c = static_cast<AVLNode<Key, Value>*>(n->getRight());
    AVLNode<Key, Value>* crc = static_cast<AVLNode<Key, Value>*>(c->getLeft());

    // Left Rotation
    c->setLeft(n);
    n->setRight(crc);

    // Update Parents
    n->setParent(c);

    if (crc != nullptr) crc->setParent(n);

    c->setParent(p);
    if (p == nullptr)
    {
        this->root_ = c;
        c->setParent(nullptr);
    }
    else if (p->getRight() == n)
    {
        p->setRight(c);
    }
    else if (p->getLeft() == n)
    {
        p->setLeft(c);
    }
    
    n->setBalance(findH(n->getRight()) - findH(n->getLeft()));
    c->setBalance(findH(c->getRight()) - findH(c->getLeft()));
}

// My Helper Function
template <class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n)
{
    if (p == nullptr || p->getParent() == nullptr) return;

    AVLNode<Key, Value>* g = p->getParent();
    bool leftC = (g->getLeft() == p);

    if (leftC)
    {
        g->updateBalance(-1);

        if (g->getBalance() == 0) return;
        else if (g->getBalance() == -1) insertFix(g, p);
        else if (g->getBalance() == -2)
        {
            // Zig-Zig Case
            if (p->getLeft() == n)
            {
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            // Zig-Zag Case
            else
            {
                int8_t bn = n->getBalance();

                rotateLeft(p);
                rotateRight(g);

                if (bn == -1)
                {
                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0);
                }
                else if (bn == 0)
                {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if (bn == 1)
                {
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }
    else
    {
        g->updateBalance(1);

        if (g->getBalance() == 0) return;
        else if (g->getBalance() == 1) insertFix(g, p);
        else if (g->getBalance() == 2)
        {
            // Zig-Zig Case
            if (p->getRight() == n)
            {
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            // Zig-Zag Case
            else
            {
                int8_t bn = n->getBalance();

                rotateRight(p);
                rotateLeft(g);

                if (bn == 1)
                {
                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0);
                }
                else if (bn == 0)
                {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if (bn == -1)
                {
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if (this->empty())
    {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }

    Node<Key, Value>* curr = this->root_;
    Node<Key, Value>* parent = nullptr;

    while (curr != nullptr)
    {
        parent = curr;
        if (new_item.first < curr->getKey())
        {
            curr = curr->getLeft();
        }
        else if (new_item.first > curr->getKey())
        {
            curr = curr->getRight();
        }
        else
        {
            static_cast<AVLNode<Key, Value>*>(curr)->setValue(new_item.second);
            return;
        }
    }
    
    AVLNode<Key, Value>* parentAVL = static_cast<AVLNode<Key, Value>*>(parent);
    AVLNode<Key, Value>* n = new AVLNode<Key, Value>(new_item.first, new_item.second, parentAVL);
    bool leftC = true;
    
    if (new_item.first < parentAVL->getKey())
    {
        parentAVL->setLeft(n);
    }
    else if (new_item.first > parentAVL->getKey())
    {
        parentAVL->setRight(n);
        leftC = false;
    }
    
    int8_t oldB = parentAVL->getBalance();
    
    if (leftC) parentAVL->updateBalance(-1);
    else parentAVL->updateBalance(1);

    if (oldB == 0)
    {
        insertFix(parentAVL, n);
    }
}

// My Helper Function
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::find(const Key& key)
{
    Node<Key, Value>* curr = this->root_;

    while (curr != nullptr)
    {
        if (curr->getKey() == key) return static_cast<AVLNode<Key, Value>*>(curr);
        else if (key < curr->getKey()) curr = curr->getLeft();
        else curr = curr->getRight();
    }

    return nullptr;
}

// My Helper Function
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::findPredecessor(AVLNode<Key, Value>* current)
{
    // If the left child exists, the predecessor is the right most child of the left subtree
    if (current == nullptr) return nullptr;

    if (current->getLeft() != nullptr)
    {
        current = current->getLeft();

        while (current->getRight() != nullptr)
        {
            current = current->getRight();
        }
        
        return current;
    }
    // Walk up the ancestor chain until traversing the first right child
    AVLNode<Key, Value>* n = current->getParent();

    while (n != nullptr && current == n->getLeft())
    {
        current = n;
        n = n->getParent();
    }
    
    return n;
}

// My Helper Function
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff)
{
    if (n == nullptr) return;

    AVLNode<Key, Value>* p = n->getParent();
    int8_t ndiff = 0;

    if (p != nullptr)
    {
        if (n == p->getLeft()) ++ndiff;
        else --ndiff;
    }

    if (diff == -1)
    {
        if (n->getBalance() + diff == -2)
        {
            AVLNode<Key, Value>* c = n->getLeft();

            // Zig-Zig Case 1
            if (c->getBalance() == -1)
            {
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            }
            // Zig-Zig Case 2
            else if (c->getBalance() == 0)
            {
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
                return;
            }
            // Zig-Zag Case
            else
            {
                AVLNode<Key, Value>* g = c->getRight();
                int8_t bg = g->getBalance();

                rotateLeft(c);
                rotateRight(n);

                if (bg == 1)
                {
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                else if (bg == 0)
                {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else
                {
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }

                removeFix(p, ndiff);
            }
        }
        else if (n->getBalance() + diff == -1)
        {
            n->setBalance(-1);
            return;
        }
        else
        {
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }
    else
    {
        if (n->getBalance() + diff == 2)
        {
            AVLNode<Key, Value>* c = n->getRight();

            // Zig-Zig Case 1
            if (c->getBalance() == 1)
            {
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            }
            // Zig-Zig Case 2
            else if (c->getBalance() == 0)
            {
                rotateLeft(n);
                n->setBalance(1);
                c->setBalance(-1);
                return;
            }
            // Zig-Zag Case
            else
            {
                AVLNode<Key, Value>* g = c->getLeft();
                int8_t bg = g->getBalance();

                rotateRight(c);
                rotateLeft(n);

                if (bg == -1)
                {
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                }
                else if (bg == 0)
                {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else
                {
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }

                removeFix(p, ndiff);
            }
        }
        else if (n->getBalance() + diff == 1)
        {
            n->setBalance(1);
            return;
        }
        else
        {
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* n = find(key);

    if (n == nullptr) return;

    // Two Children Case
    if(n->getLeft() != nullptr && n->getRight() != nullptr)
    {
        nodeSwap(n, findPredecessor(n));
    }

    AVLNode<Key, Value>* parent = n->getParent();
    int8_t diff = 0;

    // At Most One Child
    if (parent != nullptr)
    {
        if (n == parent->getLeft()) ++diff;
        else --diff;
    }

    AVLNode<Key, Value>* c;
    if (n->getLeft() != nullptr) c = n->getLeft();
    else c = n->getRight();

    if (c != nullptr) c->setParent(parent);

    if (parent != nullptr)
    {
        if (n == parent->getLeft()) parent->setLeft(c);
        else parent->setRight(c);
    }
    else
    {
        this->root_ = c;
        if (c != nullptr) c->setParent(nullptr);
    }
    
    delete n;
    
    if (parent != nullptr)
    {
        removeFix(parent, diff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
