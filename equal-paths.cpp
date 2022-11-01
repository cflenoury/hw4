#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int calculateHeightIfBalanced(Node* root) {
	// Base case: an empty tree has a height of 0
	if (root == nullptr) return 0;

	//Leaf node
	if(root->left == nullptr && root->right == nullptr){
		//Add one to the height
		return 1;

	//Two subtress
	}else if(root->left != nullptr && root->right != nullptr){

		if(!equalPaths(root)) return -1;//If the subtrees aren't equal, paths aren't of equal length so 

	//One subtree
	}else{
		//Add to the height
		if(root->left != nullptr){
			return (1+calculateHeightIfBalanced(root->left));
		}else{
			return (1+calculateHeightIfBalanced(root->right));
		}
	}
	return 0;//Never gets here but supresses warnings
}

bool equalPaths(Node * root)
{
		int f1 = calculateHeightIfBalanced(root->left);
		int f2 = calculateHeightIfBalanced(root->right);

		if( f1 < 0 || f2 < 0){
			return false;
		}

		if( f1 != f2 ){
			return false;
		}

		return true;

}

