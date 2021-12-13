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
	Type val; //ֵ
	int height;//�Ե�ǰ���Ϊ�������ĸ߶�
	int size; //�Ըýڵ�Ϊ���������ܽ�����������ظ��ģ�
	int freq; //�������ظ�Ƶ��
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

	inline void Fix(AVLNode<Type>*& ptr); //���������ά��
	inline void Maintain(AVLNode<Type>*& ptr); //ά��ƽ��ṹ

	inline int GetHeight(AVLNode<Type>* ptr); //��ȡ�߶�
	inline int GetNodenum(AVLNode<Type>* ptr); //��ȡ�������Ľڵ���

	void RotateL(AVLNode<Type>*& ptr); //����
	void RotateR(AVLNode<Type>*& ptr); //�ҵ���
	void RotateLR(AVLNode<Type>*& ptr); //����˫��
	void RotateRL(AVLNode<Type>*& ptr); //����˫��

	void inserter_1(AVLNode<Type>*& ptr, Type& el); //�ݹ�ʵ�ֲ���
	void inserter_2(AVLNode<Type>*& ptr, Type& el); //����ʵ�ֲ���
	void deleter_1(AVLNode<Type>*& ptr, Type& el); //�ݹ�ʵ��ɾ��
	void deleter_2(AVLNode<Type>*& ptr, Type& el); //����ʵ��ɾ��

	void preorder(AVLNode<Type>* ptr) const; //ǰ��
	void inorder(AVLNode<Type>* ptr) const; //����

	AVLNode<Type>* findKth(AVLNode<Type>* ptr, int k); //�ҵ���ptrΪ�������ĵ�k��Ԫ��

	int Rank(AVLNode<Type>* ptr, int x); //�õ�x������

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
	int GetRank(Type x); //�õ�x������
	Type Pre(Type x); //Ѱ��x��ǰ��
	Type Suc(Type x); //Ѱ��x�ĺ��
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
		if (po->val < x) { //С���Ҳ����ڵ�ǰ��
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
		if (po->val > x) { //�����Ҳ����ڵĺ��
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
	if (GetHeight(ptr->right) - GetHeight(ptr->left) >= 2 && ptr->right) { //������ת����
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
AVLNode<Type>* AVLTree<Type>::findKth(AVLNode<Type>* ptr, int k) { //����ʵ��
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
	if (!ptr) return 0; //������ôһ��GetHeight������Ϊ�˴���ս��ʱ�����
	else return ptr->height;
}

template <typename Type>
int AVLTree<Type>::Size() {
	return GetNodenum(root); //һ��Ҫ���������ֹ���ֿյ�ʱ�����
}

template <typename Type>
bool AVLTree<Type>::Empty() {
	return GetNodenum(root) == 0; //һ��Ҫ���������ֹ���ֿյ�ʱ�����
}

template <typename Type>
void AVLTree<Type>::RotateL(AVLNode<Type>*& ptr) {
	if (!ptr) return ;
	AVLNode<Type>* subL = ptr;
	ptr = subL->right;
	subL->right = ptr->left;
	ptr->left = subL;
	//ע��Ҫ�������ĸ߶Ⱥͽ�������Լ�˳���ȸ���subL���ٸ���ptr
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
	//���½��
	Fix(subR);
	Fix(ptr);
}

template <typename Type>
void AVLTree<Type>::RotateLR(AVLNode<Type>*& ptr) {
	if (!ptr) return ;
	//ֱ���������е����ҵ���ʵ������˫��
	RotateL(ptr->left);
	RotateR(ptr);
}

template <typename Type>
void AVLTree<Type>::RotateRL(AVLNode<Type>*& ptr) {
	if (!ptr) return ;
	//ֱ���������е����ҵ���ʵ������˫��
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
		(ptr->freq)++; //�������ǵø���Ƶ��
	else if (el < ptr->val)
		inserter_1(ptr->left, el); //�嵽������ȥ
	else if (el > ptr->val)
		inserter_1(ptr->right, el); //�嵽������ȥ
	//���������ߺͽ����
	Fix(ptr); //��Fix��Maintain,Maintain��������˶�ά������Fix
	Maintain(ptr); //���۲嵽��߻����ұ����ͳһά��
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
			} else { //ת��Ϊɾ�����
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
	int connect = 0;//connect���ڱ�ǣ�������棬��Ϊ��ʽ��ջ�洢�����Ա���Ҫ֪�����ӷ���
	stack<AVLNode<Type>*> sk;

	while (p != nullptr) {
		if (el == p->val) break;
		parent = p;
		sk.push(parent);
		if (el < p->val) p = p->left;
		else p = p->right;
	}

	if (p == nullptr) return ; //δ�ҵ�Ҫɾ���Ľ��

	if (p->freq > 1) { //���Ƶ������1
		(p->freq)--;
		Fix(p);
		while (!sk.empty()) {
			Fix(sk.top());
			sk.pop();
		}
		return ;
	}

	if (p->left && p->right) { //��ɾ���Ľ�����������ӣ�����ת����ɾ����̵ķ���
		parent = p;
		sk.push(parent);
		q = p->right; //��p��������Ѱ��p��ֱ�Ӻ�̣�Ҳ�����������������
		while (q->left) {
			parent = q;
			sk.push(parent);
			q = q->left;
		}
		p->val = q->val; //��q�p
		p->freq = q->freq; //������������������Ҫ����freq�ĸ���
		p = q; //��ɾ�����ת��Ϊq
	}

	if (p->left != nullptr) q = p->left; //��ɾ�����pֻ��һ�������q
	else q = p->right; //��ɾ�����pֻ��һ���Ҷ���q����ȫ��

	if (parent == nullptr) ptr = q; //˵��Ҫɾ�����Ǹ����
	else { //��ɾ���Ĳ��Ǹ����
		if (parent->left == p) parent->left = q; //ֱ�ӽ���ȥ
		else parent->right = q;
		while (!sk.empty()) { //����ƽ�⻯
			parent = sk.top();
			sk.pop(); //��ջ���˳������

			Fix(parent); //���������

			if (!sk.empty()) {
				grandparent = sk.top(); //��ջ�еõ��游�����Ϣ
				connect = (grandparent->left == parent) ? -1 : 1; //��ת�����ϲ����ӷ���
			} else connect = 0; //ջ�գ���ת�����ϲ�����

			if (GetHeight(parent->left) - GetHeight(parent->right) == 1 || GetHeight(parent->left) - GetHeight(parent->right) == -1)
				break;//case 1:|bf|=1��˵�������ҵȸ߱��һ��һ�ͣ���������߶Ȳ��䣬������������ת����

			if (GetHeight(parent->left) != GetHeight(parent->right)) { //case 3:|bf|=2����Ҫ���е���
				Maintain(parent);
				if (connect == -1) grandparent->left = parent;
				else if (connect == 1) grandparent->right = parent; //��ת���¸�����һ������
			}
			q = parent; //case 2 |bf|=0 ˵����������߶�-1������Ҫ�������ϵ���������case 3�ĺ�������
		}
		if (sk.empty() == true) ptr = parent; //���������ĸ����
		else { //��δ���������ĸ����
			while (!sk.empty()) {
				Fix(sk.top()); //һ·���ݵ���·���Ͻ���nodenum
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
