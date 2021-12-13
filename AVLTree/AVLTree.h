//AVLTree ( height version )
#include <iostream>
#include <stack>
#include <climits>
using std::cin, std::cout;
using std::stack;

template <typename Type> class AVLTree;

template <typename Type>
class AVLNode {
private:
	Type val; //值
	int height;//以当前结点为根的树的高度
	int size; //以该节点为根的树的总结点数（包括重复的）
	int freq; //该数据重复频次
	AVLNode* left, * right;
	friend class AVLTree<Type>;
public:
	AVLNode(): height(1), size(1), freq(1), left(nullptr), right(nullptr) {}
	AVLNode(Type& v): val(v), height(1), size(1), freq(1), left(nullptr), right(nullptr) {}
};

template <typename Type>
class AVLTree {
private:
	AVLNode<Type>* root;

	inline void Fix(AVLNode<Type>*& ptr); //对数据域的维护
	inline void Maintain(AVLNode<Type>*& ptr); //维护平衡结构

	inline int GetHeight(AVLNode<Type>* ptr); //获取高度
	inline int GetNodenum(AVLNode<Type>* ptr); //获取该子树的节点数

	void RotateL(AVLNode<Type>*& ptr); //左单旋
	void RotateR(AVLNode<Type>*& ptr); //右单旋
	void RotateLR(AVLNode<Type>*& ptr); //左右双旋
	void RotateRL(AVLNode<Type>*& ptr); //右左双旋

	void inserter_1(AVLNode<Type>*& ptr, Type& el); //递归实现插入
	void inserter_2(AVLNode<Type>*& ptr, Type& el); //迭代实现插入
	void deleter_1(AVLNode<Type>*& ptr, Type& el); //递归实现删除
	void deleter_2(AVLNode<Type>*& ptr, Type& el); //迭代实现删除

	void preorder(AVLNode<Type>* ptr) const; //前序
	void inorder(AVLNode<Type>* ptr) const; //中序

	AVLNode<Type>* findKth(AVLNode<Type>* ptr, int k); //找到以ptr为根的树的第k个元素

	int Rank(AVLNode<Type>* ptr, int x); //得到x的排名

	AVLNode<Type>* find(AVLNode<Type>* ptr, Type x);
public:
	AVLTree(): root(nullptr) {}
	void Insert(Type el);
	void Remove(Type el);
	int Size();
	bool Empty();
	void PreorderTraversal() const;
	void InorderTraversal() const;
	Type Kth_Element(int k);
	int GetRank(Type x); //得到x的排名
	Type Pre(Type x); //寻找x的前驱
	Type Suc(Type x); //寻找x的后继
	Type FindMin();
	Type FindMax();
	AVLNode<Type>* Find(Type x);
};

template <typename Type>
AVLNode<Type>* AVLTree<Type>::find(AVLNode<Type>* ptr, Type x) {
	if (!ptr) return nullptr;
	if (ptr->val == x) return ptr;
	if (ptr->val > x) return find(ptr->left, x);
	return find(ptr->right, x);
}

template <typename Type>
AVLNode<Type>* AVLTree<Type>::Find(Type x) {
	return find(root, x);
}

template <typename Type>
int AVLTree<Type>::Rank(AVLNode<Type>* ptr, int x) {
	if (!ptr) return 0;
	if (ptr->val == x) return GetNodenum(ptr->left) + 1;
	if (ptr->val > x) return Rank(ptr->left, x);
	return Rank(ptr->right, x) + GetNodenum(ptr->left) + ptr->freq;
}

template <typename Type>
int AVLTree<Type>::GetRank(Type x) {
	return Rank(root, x);
}

template <typename Type>
Type AVLTree<Type>::FindMin() {
	if (!root) return Type();
	AVLNode<Type>* p = root;
	while (p->left) p = p->left;
	return p->val;
}

template <typename Type>
Type AVLTree<Type>::FindMax() {
	if (!root) return Type();
	AVLNode<Type>* p = root;
	while (p->right) p = p->right;
	return p->val;
}

template <typename Type>
Type AVLTree<Type>::Pre(Type x) {
	AVLNode<Type>* po = root;
	Type ret = FindMin();
	while (po) {
		if (po->val < x) { //小于且不等于的前驱
			ret = std::max(ret, po->val);
			po = po->right;
		} else
			po = po->left;
	}
	return ret;
}

template <typename Type>
Type AVLTree<Type>::Suc(Type x) {
	AVLNode<Type>* po = root;
	Type ret = FindMax();
	while (po) {
		if (po->val > x) { //大于且不等于的后继
			ret = std::min(ret, po->val);
			po = po->left;
		} else
			po = po->right;
	}
	return ret;
}

template <typename Type>
inline void AVLTree<Type>::Fix(AVLNode<Type>*& ptr) {
	if (!ptr) return ;
	ptr->size = GetNodenum(ptr->left) + GetNodenum(ptr->right) + ptr->freq;
	ptr->height = std::max(GetHeight(ptr->left), GetHeight(ptr->right)) + 1;
}

template <typename Type>
inline void AVLTree<Type>::Maintain(AVLNode<Type>*& ptr) {
	if (!ptr) return ;
	if (GetHeight(ptr->right) - GetHeight(ptr->left) >= 2 && ptr->right) { //进行旋转调整
		if (GetHeight(ptr->right->right) >= GetHeight(ptr->right->left))
			RotateL(ptr);
		else
			RotateRL(ptr);
	} else if (GetHeight(ptr->left) - GetHeight(ptr->right) >= 2 && ptr->left) {
		if (GetHeight(ptr->left->left) >= GetHeight(ptr->left->right))
			RotateR(ptr);
		else
			RotateLR(ptr);
	}
}

template <typename Type>
inline int AVLTree<Type>::GetNodenum(AVLNode<Type>* ptr) {
	if (!ptr) return 0;
	else return ptr->size;
}

template <typename Type>
AVLNode<Type>* AVLTree<Type>::findKth(AVLNode<Type>* ptr, int k) { //迭代实现
	if (!ptr) return nullptr;
	if (GetNodenum(ptr->left) >= k) return findKth(ptr->left, k);
	if (GetNodenum(ptr->left) + ptr->freq >= k) return ptr;
	return findKth(ptr->right, k - GetNodenum(ptr->left) - ptr->freq);
}

template <typename Type>
Type AVLTree<Type>::Kth_Element(int k) {
	if (!root) return Type();
	if (k <= 0 || k > root->size) return Type();
	AVLNode<Type>* ret = findKth(root, k);
	if (ret != nullptr) {
		return ret->val;
	} else
		return Type();
}

template <typename Type>
void AVLTree<Type>::preorder(AVLNode<Type>* ptr) const {
	if (ptr) {
		cout << ptr->val << ' ';
		preorder(ptr->left);
		preorder(ptr->right);
	}
}

template <typename Type>
void AVLTree<Type>::inorder(AVLNode<Type>* ptr) const {
	if (ptr) {
		inorder(ptr->left);
		cout << ptr->val << ' ';
		inorder(ptr->right);
	}
}

template <typename Type>
void AVLTree<Type>::PreorderTraversal() const {
	preorder(root);
}

template <typename Type>
void AVLTree<Type>::InorderTraversal() const {
	inorder(root);
}

template <typename Type>
inline int AVLTree<Type>::GetHeight(AVLNode<Type>* ptr) {
	if (!ptr) return 0; //设置这么一个GetHeight，就是为了处理空结点时的情况
	else return ptr->height;
}

template <typename Type>
int AVLTree<Type>::Size() {
	return GetNodenum(root); //一定要用这个，防止出现空的时候调用
}

template <typename Type>
bool AVLTree<Type>::Empty() {
	return GetNodenum(root) == 0; //一定要用这个，防止出现空的时候调用
}

template <typename Type>
void AVLTree<Type>::RotateL(AVLNode<Type>*& ptr) {
	if (!ptr) return ;
	AVLNode<Type>* subL = ptr;
	ptr = subL->right;
	subL->right = ptr->left;
	ptr->left = subL;
	//注意要更新树的高度和结点数，以及顺序先更新subL，再更新ptr
	Fix(subL);
	Fix(ptr);
}

template <typename Type>
void AVLTree<Type>::RotateR(AVLNode<Type>*& ptr) {
	if (!ptr) return ;
	AVLNode<Type>* subR = ptr;
	ptr = subR->left;
	subR->left = ptr->right;
	ptr->right = subR;
	//更新结点
	Fix(subR);
	Fix(ptr);
}

template <typename Type>
void AVLTree<Type>::RotateLR(AVLNode<Type>*& ptr) {
	if (!ptr) return ;
	//直接利用已有的左、右单旋实现左右双旋
	RotateL(ptr->left);
	RotateR(ptr);
}

template <typename Type>
void AVLTree<Type>::RotateRL(AVLNode<Type>*& ptr) {
	if (!ptr) return ;
	//直接利用已有的左、右单旋实现左右双旋
	RotateR(ptr->right);
	RotateL(ptr);
}

template <typename Type>
void AVLTree<Type>::inserter_1(AVLNode<Type>*& ptr, Type& el) {
	if (!ptr) {
		ptr = new AVLNode(el);
		return ;
	}
	if (el == ptr->val)
		(ptr->freq)++; //！！！记得更新频数
	else if (el < ptr->val)
		inserter_1(ptr->left, el); //插到左子树去
	else if (el > ptr->val)
		inserter_1(ptr->right, el); //插到右子树去
	//最后更新树高和结点数
	Fix(ptr); //先Fix后Maintain,Maintain里面包含了对维护结点的Fix
	Maintain(ptr); //无论插到左边还是右边最后统一维护
}

template <typename Type>
void AVLTree<Type>::inserter_2(AVLNode<Type>*& ptr, Type& el) {
	//TODO
}

template <typename Type>
void AVLTree<Type>::Insert(Type el) {
	inserter_1(root, el);
}

template <typename Type>
void AVLTree<Type>::deleter_1(AVLNode<Type>*& ptr, Type& v) {
	if (!ptr) return ;
	if (ptr->val == v) {
		if (ptr->freq > 1) {
			ptr->freq--;
		} else {
			if (!ptr->right) {
				AVLNode<Type>* temp = ptr;
				ptr = ptr->left;
				delete (temp);
			} else { //转化为删除后继
				AVLNode<Type>* po = ptr->right;
				while (po->left) po = po->left;
				ptr->val = po->val;
				ptr->freq = po->freq;
				po->freq = 1;
				Fix(ptr);
				deleter_2(ptr->right, po->val);
			}
		}
	} else if (ptr->val < v)
		deleter_1(ptr->right, v);
	else
		deleter_1(ptr->left, v);
	Fix(ptr);
	Maintain(ptr);
}

template <typename Type>
void AVLTree<Type>::deleter_2(AVLNode<Type>*& ptr, Type& el) {
	AVLNode<Type>* parent = nullptr, * p = ptr, *q = nullptr, *grandparent = nullptr;
	int connect = 0;//connect用于标记，详见后面，因为显式用栈存储，所以必须要知道连接方向
	stack<AVLNode<Type>*> sk;

	while (p != nullptr) {
		if (el == p->val) break;
		parent = p;
		sk.push(parent);
		if (el < p->val) p = p->left;
		else p = p->right;
	}

	if (p == nullptr) return ; //未找到要删除的结点

	if (p->freq > 1) { //如果频数大于1
		(p->freq)--;
		Fix(p);
		while (!sk.empty()) {
			Fix(sk.top());
			sk.pop();
		}
		return ;
	}

	if (p->left && p->right) { //被删除的结点有两个儿子，采用转化成删除后继的方法
		parent = p;
		sk.push(parent);
		q = p->right; //在p的右子树寻找p的直接后继，也就是右子树的最左边
		while (q->left) {
			parent = q;
			sk.push(parent);
			q = q->left;
		}
		p->val = q->val; //用q填补p
		p->freq = q->freq; //！！！！！！！！不要忘记freq的覆盖
		p = q; //被删除结点转化为q
	}

	if (p->left != nullptr) q = p->left; //被删除结点p只有一个左儿子q
	else q = p->right; //被删除结点p只有一个右儿子q或者全空

	if (parent == nullptr) ptr = q; //说明要删除的是根结点
	else { //被删除的不是根结点
		if (parent->left == p) parent->left = q; //直接接上去
		else parent->right = q;
		while (!sk.empty()) { //重新平衡化
			parent = sk.top();
			sk.pop(); //从栈中退出父结点

			Fix(parent); //调整父结点

			if (!sk.empty()) {
				grandparent = sk.top(); //从栈中得到祖父结点信息
				connect = (grandparent->left == parent) ? -1 : 1; //旋转后与上层连接方向
			} else connect = 0; //栈空，旋转后不与上层连接

			if (GetHeight(parent->left) - GetHeight(parent->right) == 1 || GetHeight(parent->left) - GetHeight(parent->right) == -1)
				break;//case 1:|bf|=1，说明由左右等高变成一高一低，但是整体高度不变，不必再向上旋转调整

			if (GetHeight(parent->left) != GetHeight(parent->right)) { //case 3:|bf|=2，需要进行调整
				Maintain(parent);
				if (connect == -1) grandparent->left = parent;
				else if (connect == 1) grandparent->right = parent; //旋转后新根与上一层连接
			}
			q = parent; //case 2 |bf|=0 说明树的整体高度-1，还需要继续向上调整（包括case 3的后续处理）
		}
		if (sk.empty() == true) ptr = parent; //调整到树的根结点
		else { //还未调整到树的根结点
			while (!sk.empty()) {
				Fix(sk.top()); //一路回溯调整路径上结点的nodenum
				sk.pop();
			}
		}
	}
	delete p;
}

template <typename Type>
void AVLTree<Type>::Remove(Type el) {
	deleter_1(root, el);
}
