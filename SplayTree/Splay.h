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

	inline unsigned int GetSize(Node* x) { return x ? x->size : 0; } //��ֹRE
	inline void update(Node* x) { if (x) x->size = GetSize(x->child[0]) + GetSize(x->child[1]) + x->freq; }
	inline int ident(Node* x, Node* f); //����0����x������ӣ�1�������Ҷ���
	inline void connect(Node* x, Node* fa, int how);
	inline void rotate(Node* x);
	inline void splay(Node* x, Node* y); //��x splayΪy�Ķ���
	void inorder(Node* ptr);
public:
	SplayTree(): root(nullptr) {}
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
	while (x->father != y) { //ֱ��x�ĸ�����y
		Node* p = x->father, * g = p->father;
		if (g != y) ident(x, p)^ident(p, g) ? rotate(x) : rotate(p);
		rotate(x);
	}
	update(x); //��Ϊÿһ��rotate����x�����ƣ�����sizeһֱ�ڸı䣬��rotate����update���岻��ֻ��Ҫ�����update����
	if (y == nullptr) root = x; //���y��nullptr����Ҫ�ı��ָ��
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
	if (!f) return 0; //�����Ϊ�գ���ֹRE
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
Type SplayTree<Type>::Pre_une(Type val) { //�Ȳ��룬Ȼ����ǰ�������ɾ��
	Insert(val); //�����val���ڸ����
	Node* x = root->child[0];
	while (x->child[1]) x = x->child[1];
	Remove(val);
	return x->val;
}

template <typename Type>
Type SplayTree<Type>::Suc_une(Type val) { //�Ȳ��룬Ȼ���Һ�̣����ɾ��
	Insert(val); //�����val���ڸ����
	Node* x = root->child[1];
	while (x->child[0]) x = x->child[0];
	Remove(val);
	return x->val;
}

template <typename Type>
int SplayTree<Type>::Rank(Type val) {
	Find(val); //Find��val�ڸ����
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
bool SplayTree<Type>::Find(Type val) { //����С�Ĵ��ڵ���val��ֵ����������С�ڵ���val��ֵsplay����
	Node* x = root;
	while ( x/*ע��rootΪ�յ����*/ && x->child[val > x->val] && val != x->val) x = x->child[val > x->val];
	if (x) splay(x, nullptr);
	return (x->val == val) ? true : false;
}

template <typename Type>
void SplayTree<Type>::Insert(Type val) {
	if (!root) { //���и�Ϊ�յ����
		root = new Node(val);
		return ;
	}
	for (Node* x = root; x; x = x->child[val > x->val]) {
		if (x->val == val) {
			x->freq++; //����Ƶ��
			splay(x, nullptr); //ע��splay
			return ;
		}
		if (!x->child[val > x->val]) { //�����˿�
			x->child[ val > x->val] = new Node(val);
			x->child[val > x->val]->father = x;
			splay(x->child[val > x->val], nullptr);
			return ;
		}
	}
}

template <typename Type>
void SplayTree<Type>::Remove(Type val) {
	//��ѡ������ǰ���ͺ��ɾ���ķ�ʽ����Ϊ�����������INF��-INF����������RE
	Find(val); //��val splay����
	if (root->val != val) return ; //�Ҳ���
	Node* x = root;
	if (x->freq > 1) x->freq--;
	else {
		if (!x->child[0]) { //��һ�������ǿ�
			root = x->child[1]; //ת��root
			if (root) root->father = nullptr; //��ֹRE
			delete x; //����
		} else {
			Node* k = x->child[0];
			while (k->child[1]) k = k->child[1]; //Ѱ��ǰ��
			splay(k, x);
			root = k, root->father = nullptr; //��ʱ���������������϶��ǿ�
			connect(x->child[1], root, 1); //�ٽ�������ת�Ƶ�������
			delete x;
		}
	}
}
