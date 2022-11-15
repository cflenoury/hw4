#ifndef RBBST_H
#define RBBST_H

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
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
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
    void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child);
		void removeFix(AVLNode<Key,Value>* n, int diff);
    void rotateRight(AVLNode<Key,Value>* node);
    void rotateLeft(AVLNode<Key,Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  //Keep comparing with node values until you become a leaf

	if(BinarySearchTree<Key, Value>::root_ == nullptr){//If BST is empty
        BinarySearchTree<Key, Value>::root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
	}

	//AVLNode<Key, Value>* newNode = nullptr;
	bool wasLeftInsert; 

	AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
	//Make a node pointer from &keyValuePair

	while(1){
        if( temp->getKey() > new_item.first ){//If temp's key is less than new node's key
            if(temp->getLeft() != nullptr){//If there is a left child
                temp = temp->getLeft();//Set temp to left child
            }else{//Set it as child				
                AVLNode<Key, Value>* newNode = new  AVLNode<Key, Value>(new_item.first, new_item.second, temp);
                temp->setLeft(newNode);//Set the current node as the parent of the new node
								wasLeftInsert = true;
                break;
            }
        }else if(temp->getKey() < new_item.first ){
            if(temp->getRight() != nullptr){//Look at right child
                temp = temp->getRight();
            }else{//Set it as child						
                AVLNode<Key, Value>* newNode = new  AVLNode<Key, Value>(new_item.first, new_item.second, temp);
                temp->setRight(newNode);
								wasLeftInsert = false;
                break;
            }
        }else{
                temp->setValue(new_item.second);//Change the value of the Node with the same key
                return;//Nothing was inserted
        }
	}

	//BinarySearchTree<Key, Value>::insert(new_item);

	//AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(new_item.first));

	//Balance checking
	// if(wasLeftInsert){
	// 	temp->updateBalance(-1);
	// }else{
	// 	temp->updateBalance(1);
	// }

	if(temp->getBalance() == 1 || temp->getBalance() == -1){//If parent of new node had a non-zero balance, it should now be zero
		temp->setBalance(0);
	}else{//parent's balance before insertion was 0
		if(wasLeftInsert){
			temp->updateBalance(-1);
			insertFix(temp,temp->getLeft());
		}else{
			temp->updateBalance(1);
			insertFix(temp,temp->getRight());
		}
	}

	//AVLNode<Key, Value>* parent = newNode->getParent();


	// //Update parents balance
	// if(newNode == parent->getLeft()){//New node was added to the left
	// 		parent->updateBalance(-1);
	// }else{//New node was added to the right
	// 		parent->updateBalance(1);
	// }

	// if(temp->getBalance()){//If the balance of the parent is not 0 anymore, call insertFix
	// 		//Update parents balance
	// 		if(wasLeftInsert){
	// 			insertFix(temp,temp->getLeft());
	// 		}else{
	// 			insertFix(temp,temp->getRight());
	// 		}			        
	// }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child){

	if(parent == nullptr || parent->getParent() == nullptr){//<-- Might cause a seg fault if parent is null
		//if child is root node or parent is root node
		return;
	}

	AVLNode<Key, Value>* grandparent = parent->getParent();

	if(grandparent->getRight() == parent){//parent is a right child
    grandparent->updateBalance(1);
		if(grandparent->getBalance() == 0){
			return;
		}else if(grandparent->getBalance() == 1){
			insertFix(grandparent, parent);
		}else{//balance of grandparent is 2
			//if zig-zig
			if(parent->getBalance() == 1){
				rotateLeft(grandparent);
				parent->setBalance(0);
				grandparent->setBalance(0);
			}else{//parent's balance is 1 - zig-zag
				//if zig-zag, rotateLeft(parent), rotateRight(grandparent)
				rotateRight(parent);
				rotateLeft(grandparent);
                //Note: Rotation functions don't change balance
				int b = child->getBalance(); 
				if(	b == 1){
					parent->setBalance(0);
					grandparent->setBalance(-1);
				}else if( b == -1){
					parent->setBalance(1);
					grandparent->setBalance(0);
				}else{
					parent->setBalance(0);
					grandparent->setBalance(0);
				}
				child->setBalance(0);
			}
		}
	}else{//parent is a left child
		grandparent->updateBalance(-1);
		if(grandparent->getBalance() == 0){
			return;
		}else if(grandparent->getBalance() == -1){
			insertFix(grandparent, parent);
		}else{//balance of grandparent is -2
			//if zig-zig, rotateRight(grandparent)
			if(parent->getBalance() == -1){
				rotateRight(grandparent);
				parent->setBalance(0);
				grandparent->setBalance(0);
			}else{//parent's balance is 1 - zig-zag
				//if zig-zag, rotateLeft(parent), rotateRight(grandparent)
				rotateLeft(parent);
				rotateRight(grandparent);
				int b = child->getBalance(); 
				if(	b == -1){
					parent->setBalance(0);
					grandparent->setBalance(1);
				}else if( b == 1){
					parent->setBalance(-1);
					grandparent->setBalance(0);
				}else{
					parent->setBalance(0);
					grandparent->setBalance(0);
				}
				child->setBalance(0);
			}
		}
	}

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* node){

	//Get family nodes
	AVLNode<Key, Value>* leftChild = node->getLeft();
	AVLNode<Key, Value>* parent = node->getParent();

	if(parent != nullptr ){
		
		if(parent->getLeft() == node){//node is left child			
			parent->setLeft(leftChild);//Make parent's left child the left child of node			
		}else{//node is right child
			parent->setRight(leftChild);//Make parent's right child the left child of node
		}

	}else{
		BinarySearchTree<Key, Value>::root_ = leftChild;
	}
	//Account for root

	node->setLeft(leftChild->getRight());//Set left child of node to be the right child of its left child
	if(leftChild->getRight() != nullptr){
		leftChild->getRight()->setParent(node);//Set the left child's right child's parent as node
	}
	node->setParent(leftChild);//Set node's parent to be its left child
	leftChild->setRight(node);//Set left child to be parent of node
	leftChild->setParent(parent);//Set the left child's parent to be parent of node
	
	
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* node){

	//Get family nodes
	AVLNode<Key, Value>* rightChild = node->getRight();
	AVLNode<Key, Value>* parent = node->getParent();

	if(parent != nullptr ){
		
		if(parent->getRight() == node){//node is right child			
			parent->setRight(rightChild);//Make parent's left child the left child of node			
		}else{//node is right child
			parent->setLeft(rightChild);//Make parent's left child the left child of node
		}

	}else{
		BinarySearchTree<Key, Value>::root_ = rightChild;
	}

	node->setRight(rightChild->getLeft());//Set left child of node to be the right child of its left child
	if(rightChild->getLeft() != nullptr){
		rightChild->getLeft()->setParent(node);//Set the left child's right child's parent as node	
	}
	node->setParent(rightChild);//Set node's parent to be its left child	
	rightChild->setLeft(node);//Set left child to be parent of node
	rightChild->setParent(parent);//Set the left child's parent to be parent of node

}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{    

	int diff;

	//BC - empty tree
	if(BinarySearchTree<Key, Value>::root_ == nullptr){
		return;
	}

	AVLNode<Key, Value>* nI = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));

	//If node is not in tree
	if(nI == nullptr){
		return;
	}

	//Get node's parent
	AVLNode<Key, Value>* pni = nI->getParent();

	//nI is Leaf node
	if(nI->getRight() == nullptr && nI->getLeft() == nullptr){
		if( nI == BinarySearchTree<Key, Value>::root_){//only one node in tree
			delete BinarySearchTree<Key, Value>::root_;
			BinarySearchTree<Key, Value>::root_ = nullptr;
			return;
		}

		if(pni->getLeft() == nI){//Removing left child so add 1 to balance
			diff = 1;
			pni->setLeft(nullptr);
		}else{
			pni->setRight(nullptr);
			diff = -1;
		}

		delete nI;
		nI = nullptr;

	//nI had only a left Child
	}else if(nI->getLeft() != nullptr && nI->getRight() == nullptr){
		if(nI == BinarySearchTree<Key, Value>::root_){//If left subtree has only one node
			BinarySearchTree<Key, Value>::root_ = nI->getLeft();
			BinarySearchTree<Key, Value>::root_->setParent(nullptr);
			delete nI;
			nI = nullptr;
			return;
		}
		
		if(pni->getLeft() == nI){//Removing left child so update difference by 1
			pni->setLeft(nI->getLeft());
			diff = 1;
		}else{
			pni->setRight(nI->getLeft());
			diff = -1;
		}
		
		nI->getLeft()->setParent(pni);
		delete nI;
		nI = nullptr;

	//n has only a Right Child
	}else if(nI->getLeft() == nullptr && nI->getRight() != nullptr){
		if(nI == BinarySearchTree<Key, Value>::root_){
			BinarySearchTree<Key, Value>::root_ = nI->getRight();
			BinarySearchTree<Key, Value>::root_->setParent(nullptr);
			delete nI;
			nI = nullptr;
			return;
		}
		
		if(pni->getLeft() == nI){//Removing left child so update difference by 1
			diff = 1;
			pni->setLeft(nI->getRight());
		}else{
			pni->setRight(nI->getRight());
			diff = -1;
		}
		nI->getRight()->setParent(pni);
		delete nI;
		nI = nullptr;

	//n has Two children
	}else{
		//Swap with predecessor
		nodeSwap(nI,static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(nI)));

		pni = nI->getParent();

		//If predecessor was leaf node
		if(nI->getRight() == nullptr && nI->getLeft() == nullptr){

			if(pni->getLeft() == nI){//Removing left child so update difference by 1
				diff = 1;
				pni->setLeft(nullptr);
			}else{
			 diff = -1;
				pni->setRight(nullptr);
			}

			delete nI;
			nI = nullptr;

		//If predecessor has a (left) child 
		}else{//Was predecessor a right child or left child?
			if(pni->getLeft() == nI){//Removing left child so update difference by 1
				diff = 1;
				pni->setLeft(nI->getLeft());
			}else{
				pni->setRight(nI->getLeft());
				diff = -1;
			}
			nI->getLeft()->setParent(pni);
			delete nI;
			nI = nullptr;
		}
	}

	removeFix(pni, diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>:: removeFix(AVLNode<Key,Value>* n, int diff){
	if(n == nullptr){
		return;
	}

	int ndiff;

	AVLNode<Key, Value>* parent = n->getParent();
	if(parent != nullptr){
		if(n == parent->getLeft()){
			ndiff = 1;
		}else{
			ndiff = -1;
		}
	}
	
	if(diff == -1){
		if(n->getBalance() + diff == -2){

			AVLNode<Key, Value>* c = n->getLeft();
			
			if(c->getBalance() == -1){
				rotateRight(n);
				n->updateBalance(0);
				c->updateBalance(0);
				removeFix(parent, ndiff);
			}else if(c->getBalance() == 0){
				rotateRight(n);
				n->updateBalance(-1);
				c->updateBalance(+1);
			}else{//child->getBalance() = 1
				AVLNode<Key, Value>* g = c->getRight();
				rotateLeft(c);//c could have balance of 1 but no children 
				rotateRight(n);
				if(g->getBalance() == 1){
					n->updateBalance(0);
					c->updateBalance(-1);
					g->updateBalance(0);
				}else if(g->getBalance() == 0){
					n->updateBalance(0);
					c->updateBalance(0);
					g->updateBalance(0);
				}else{// b(g) == -1
					n->updateBalance(1);
					c->updateBalance(0);
					g->updateBalance(0);
				}
				removeFix(parent, ndiff);
			}

		}else{

			n->updateBalance(-1);

			if(n->getBalance() == 0){
				removeFix(parent, ndiff);
			}

		}
	}else{//diff = 1
		if(n->getBalance() + diff == 2){

			AVLNode<Key, Value>* c = n->getRight();

			if(c->getBalance() == 1){//Zig-zig right
				rotateLeft(n);
				n->updateBalance(0);
				c->updateBalance(0);
				removeFix(parent, ndiff);
			}else if(c->getBalance() == 0){
				rotateLeft(n);
				n->updateBalance(+1);
				c->updateBalance(-1);
			}else{//child->getBalance() = 1
				AVLNode<Key, Value>* g = c->getLeft();
				rotateRight(c);
				rotateLeft(n);
				if(g->getBalance() == -1){
					n->updateBalance(0);
					c->updateBalance(1);
					g->updateBalance(0);
				}else if(g->getBalance() == 0){
					n->updateBalance(0);
					c->updateBalance(0);
					g->updateBalance(0);
				}else{//b(g) == 1
					n->updateBalance(-1);
					c->updateBalance(0);
					g->updateBalance(0);
				}
				removeFix(parent, ndiff);
			}

		// }else if(n->getBalance() + diff == 1){

		// 	n->updateBalance(1);

		// }else{//n->getBalance() + diff = 0

		// 	n->setBalance(0);
		// 	removeFix(parent, ndiff);

		// }
		}else{
			n->updateBalance(1);

			if(n->getBalance()){
				removeFix(parent, ndiff);
			}		

		}
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
