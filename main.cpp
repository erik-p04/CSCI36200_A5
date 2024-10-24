#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int t = 5;
class BTreeNode{
	public:
		int keys[t-1];
		BTreeNode* children[t];
		int n;
		bool leaf;
		BTreeNode(bool isLeaf = true) : n(0), leaf(isLeaf){
			for (int i=0; i<t; i++){
				children[i] = nullptr;
			}
		}
		~BTreeNode(){
			if (!leaf){
				for (int i=0; i<=n; i++){
					delete children[i];
				}
			}
		}
};

class BTree{
	private:
		BTreeNode* root;
		void splitChild(BTreeNode* x, int i){
			BTreeNode* y = x->children[i];
			BTreeNode* z = new BTreeNode(y->leaf);
			z->n = t / 2 - 1;
			for (int j=0; j<t/2-1; j++){
				z->keys[j] = y->keys[j+t/2];
			}
			if (!y->leaf){
				for (int j=0; j<t/2; j++){
					z->children[j] = y->children[j+t/2];
				}
			}
			y->n = t/2-1;
			for (int j=x->n; j>=i+1; j--){
				x->children[j+1] = x->children[j];
			}
			x->children[i+1] = z;
			for (int j=x->n-1; j>=i; j--){
				x->keys[j+1] = x->keys[j];
			}
			x->keys[i] = y->keys[t/2-1];
			x->n = x->n + 1;
		}
		void insertNonFull(BTreeNode* x, int k){
			int i = x->n-1;
			if (x->leaf){
				while (i >= 0 && k < x->keys[i]){
					x->keys[i+1] = x->keys[i];
					i--;
				}
				x->keys[i+1] = k;
				x->n = x->n + 1;
			}else{
				while (i >= 0 && k < x->keys[i]){
					i--;
				}
				i++;
				if (x->children[i]->n == t-1){
					splitChild(x, i);
					if (k > x->keys[i]){
						i++;
					}
				}
				insertNonFull(x->children[i], k);
			}
		}
		void traverse(BTreeNode* x){
			int i;
			for(i=0; i< x->n; i++){
				if (!x->leaf){
					traverse(x->children[i]);
				}
				std::cout << " " << x->keys[i];
			}
			if (!x->leaf){
				traverse(x->children[i]);
			}
		}
		BTreeNode* search(BTreeNode* x, int k){
			int i = 0;
			while (i< x->n && k > x->keys[i]){
				i++;
			}
			if (i < x->n && k == x->keys[i]){
				return x;
			}
			if (x->leaf){
				return nullptr;
			}
			return search(x->children[i], k);
		}
	public:
		BTree() { root = new BTreeNode(true);}
		~BTree(){
			deleteRoot(root);
		}
		void deleteRoot(BTreeNode* node){
			if (node == nullptr){
				return;
			}
			if (!node->leaf){
				for (int i=0; i <= node->n; i++){
					deleteRoot(node->children[i]);
				}
			}
			delete node;
		}
		void insert(int k){
			if (root->n == t-1){
				BTreeNode* s = new BTreeNode(false);
				s->children[0] = root;
				root = s;
				splitChild(s, 0);
				insertNonFull(s, k);
			}else{
				insertNonFull(root, k);
			}
		}
		void traverse(){
			if (root != nullptr){
				traverse(root);
			}
		}
		BTreeNode* search(int k){
			return (root == nullptr) ? nullptr : search(root, k);
		}
};

int main() {
	srand(time(0));
	int N;
	bool keepGoing = true;
	while (keepGoing){
		std::cout << "Enter a number >= 400: ";
		std::cin >> N;
		if ( N >= 400 ){
			keepGoing = false;
		}
	}
	std::vector<int> keys;
	for (int i=0; i<N; i++){
		keys.push_back(rand() % (3*N));
	}
	BTree tree;
	for (int key : keys){
		tree.insert(key);
	}
	std::cout << "In-order taversal:" << std::endl;
	tree.traverse();
	std::cout << std::endl;
	std::vector<int> foundKeys;
	for (int key = N; key <= 2 * N; key++){
		if (tree.search(key)){
			foundKeys.push_back(key);
		}
	}
	std::cout << "Keys in the range [" << N << ", " << 2*N << "]:" << std::endl;
	for (int key : foundKeys){
		std::cout << key << " ";
	}
	std::cout << std::endl;
	return 0;
}
