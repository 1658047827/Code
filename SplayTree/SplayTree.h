//Splay
#include <iostream>
#include <climits>
using std::cin, std::cout;

template <typename Type>
class SplayTree {
private:
	struct Node {
		Type val;
		Node* child[2];
		Node* father;
		int size, freq;
		Node(): size(1), freq(1), father(nullptr) { child[0] = child[1] = nullptr; }
		Node(Type _val): val(_val), size(1), freq(1), father(nullptr) { child[0] = child[1] = nullptr;}
	};
	Node* root;

	inline unsigned int GetSize(Node* x) { return x ? x->size : 0; } //防止RE
	inline void update(Node* x) { if (x) x->size = GetSize(x->child[0]) + GetSize(x->child[1]) + x->freq; }
	inline int ident(Node* x, Node* f); //返回0代表x是左儿子，1代表是右儿子
	inline void connect(Node* x, Node* fa, int how);
	inline void rotate(Node* x);
	inline void splay(Node* x, Node* y); //将x splay为y的儿子
	void inorder(Node* ptr);
public:
	SplayTree(): root(nullptr) {
//		Insert(INT_MAX), Insert(INT_MIN); //有时要事先放两个极值防止RE	
	}
	void Insert(Type val);
	void Remove(Type val);
	bool Find(Type val);
	Type Kth_Element(int k);
	Type Pre_une(Type val);
	Type Suc_une(Type val);
	int Rank(Type val);
	void Inorder();
};

template <typename Type>
inline void SplayTree<Type>::splay(Node* x, Node* y) {
	if (!x) return ;
	while (x->father != y) { //直到x的父亲是y
		Node* p = x->father, * g = p->father;
		if (g != y) ident(x, p)^ident(p, g) ? rotate(x) : rotate(p);
		rotate(x);
	}
	update(x); //因为每一次rotate都将x向上移，所以size一直在改变，在rotate里面update意义不大，只需要在最后update即可
	if (y == nullptr) root = x; //如果y是nullptr，就要改变根指针
}

template <typename Type>
inline void SplayTree<Type>::rotate(Node* x) {
	Node* p = x->father, * g = p->father;
	int r = ident(x, p);
	connect(x, g, ident(p, g));
	connect(x->child[!r], p, r);
	connect(p, x, !r);
	update(p);
}

template <typename Type>
inline void SplayTree<Type>::connect(Node* x, Node* fa, int how) {
	if (x) x->father = fa;
	if (fa) fa->child[how] = x;
}

template <typename Type>
inline int SplayTree<Type>::ident(Node* x, Node* f) {
	if (!f) return 0; //父结点为空，防止RE
	return f->child[1] == x;
}

template <typename Type>
void SplayTree<Type>::inorder(Node* ptr) {
	if (ptr) {
		inorder(ptr->child[0]);
		cout << ptr->val << ' ';
		inorder(ptr->child[1]);
	}
}

template <typename Type>
void SplayTree<Type>::Inorder() {
	inorder(root);
}

template <typename Type>
Type SplayTree<Type>::Pre_une(Type val) { //先插入，然后找前驱，最后删除
	Insert(val); //插入后val是在根结点
	Node* x = root->child[0];
	while (x->child[1]) x = x->child[1];
	Remove(val);
	return x->val;
}

template <typename Type>
Type SplayTree<Type>::Suc_une(Type val) { //先插入，然后找后继，最后删除
	Insert(val); //插入后val是在根结点
	Node* x = root->child[1];
	while (x->child[0]) x = x->child[0];
	Remove(val);
	return x->val;
}

template <typename Type>
int SplayTree<Type>::Rank(Type val) {
	Find(val); //Find后，val在根结点
	return GetSize(root->child[0]) + 1;
}

template <typename Type>
Type SplayTree<Type>::Kth_Element(int k) {
	Node* x = root;
	while (1) {
		if (GetSize(x->child[0]) + x->freq >= k && GetSize(x->child[0]) < k) break;
		if (GetSize(x->child[0]) >= k) x = x->child[0];
		else {
			k -= (GetSize(x->child[0]) + x->freq);
			x = x->child[1];
		}
	}
	return x->val;
}

template <typename Type>
bool SplayTree<Type>::Find(Type val) { //将最小的大于等于val的值，或者最大的小于等于val的值splay到根
	Node* x = root;
	while ( x/*注意root为空的情况*/ && x->child[val > x->val] && val != x->val) x = x->child[val > x->val];
	if (!x) return false;
	if (x) splay(x, nullptr);
	return (x->val == val) ? true : false;
}

template <typename Type>
void SplayTree<Type>::Insert(Type val) {
	if (!root) { //特判根为空的情况
		root = new Node(val);
		return ;
	}
	for (Node* x = root; x; x = x->child[val > x->val]) {
		if (x->val == val) {
			x->freq++; //增加频数
			splay(x, nullptr); //注意splay
			return ;
		}
		if (!x->child[val > x->val]) { //遇到了空
			x->child[ val > x->val] = new Node(val);
			x->child[val > x->val]->father = x;
			splay(x->child[val > x->val], nullptr);
			return ;
		}
	}
}

template <typename Type>
void SplayTree<Type>::Remove(Type val) {
	//不选择利用前驱和后继删除的方式，因为这样必须插入INF和-INF，否则容易RE
	Find(val); //将val splay到根
	if (root->val != val) return ; //找不到
	Node* x = root;
	if (x->freq > 1) x->freq--;
	else {
		if (!x->child[0]) { //有一个子树是空
			root = x->child[1]; //转移root
			if (root) root->father = nullptr; //防止RE
			delete x; //回收
		} else {
			Node* k = x->child[0];
			while (k->child[1]) k = k->child[1]; //寻找前驱
			splay(k, x);
			root = k, root->father = nullptr; //此时左子树的右子树肯定是空
			connect(x->child[1], root, 1); //再将右子树转移到左子树
			delete x;
		}
	}
}
