#pragma once
#ifndef _HDSDATABASE_AVLTREE_H
#define _HDSDATABASE_AVLTREE_H

#include <iostream>
#include <queue>

typedef int KeyType;
typedef int ElemType;
typedef struct BSTNode {
	KeyType key;
	int height;
	ElemType data;
	BSTNode* left, * right;
	BSTNode() = default;
	BSTNode(KeyType k, ElemType d) :key(k), height(1), data(d), left(NULL), right(NULL) {}
}BSTree;

int GetHeight(BSTree* rt) {	//得到高度 
	if (rt == NULL)	return 0;
	return rt->height;
}

void UpdateHeight(BSTree* rt) {	//更新高度 
	if (rt == NULL)	return;
	rt->height = std::max(GetHeight(rt->left), GetHeight(rt->right)) + 1;
}

//左左调整(bf=2)，右旋，左子节点变成父节点，其多余的右子节点变成降级节点的左子节点 
void UpdateLL(BSTree*& rt) {
	BSTree* pl = rt->left;
	rt->left = pl->right;
	pl->right = rt;
	rt = pl;
	UpdateHeight(rt->left);
	UpdateHeight(rt->right);
	UpdateHeight(rt);
}

//右右调整(bf=-2),左旋，右子节点变成父节点，其多余的左子节点变成降级节点的右子节点 
void UpdateRR(BSTree*& rt) {
	BSTree* pr = rt->right;
	rt->right = pr->left;
	pr->left = rt;
	rt = pr;
	UpdateHeight(rt->left);
	UpdateHeight(rt->right);
	UpdateHeight(rt);
}

//左右调整(bf=2),先对左子节点左旋调整为左左型，再进行左左调整 
void UpdateLR(BSTree*& rt) {
	UpdateRR(rt->left);
	UpdateHeight(rt->left->left);
	UpdateHeight(rt->left->right);
	UpdateHeight(rt->left);

	UpdateLL(rt);
	UpdateHeight(rt->left);
	UpdateHeight(rt->right);
	UpdateHeight(rt);
}

// 右左调整(bf = -2), 先对右子节点右旋调整为右右型，再进行右右调整
void UpdateRL(BSTree*& rt) {
	UpdateLL(rt->right);
	UpdateHeight(rt->right->left);
	UpdateHeight(rt->right->right);
	UpdateHeight(rt->right);

	UpdateRR(rt);
	UpdateHeight(rt->left);
	UpdateHeight(rt->right);
	UpdateHeight(rt);
}

BSTree* SearchBST(BSTree* rt, KeyType k) {		//查找
	if (rt == NULL || k == rt->key)	
		return rt;
	if (k < rt->key)	
		return SearchBST(rt->left, k);
	else	
		return SearchBST(rt->right, k);
}

// 查询主键对应的行数，如果找到相等的主键则直接返回
// 否则应找到和要查询主键最接近的但小于要查询主键的节点位置
BSTree* SearchNode(BSTree* rt, KeyType k) {
	if (rt == NULL)
		return NULL;
	else {
		BSTree *p = rt;
		// 用于查询过程中指向最后一个比要查询主键小的节点
		BSTree* pri = rt;  
		while (p) {
			if (p->key == k)
				return p;
			else if (p->key < k) {
				pri = p;
				p = p->right;
			}
			else if (p->key > k) {
				p = p->left;
			}
		}
		return pri;
	}
}

/*
插入节点操作，先插入节点，再对其影响的祖先节点进行平衡调整，同时更新其节点高度。
节点A的平衡调整可分为四种情况：
一、LL型：在A节点的左孩子的左子树上插入节点，导致A节点的平衡因子变为2。
可对其进行右旋，将A的左孩子代替A成为根节点，而原A的左孩子的右子树成为A的左子树。
二、RR型：与LL型相反，是在A节点的右孩子的右子树上插入节点，导致A节点的平衡因子变为-2。
可对其进行左旋，将A的右孩子代替A成为根节点，而原A的右孩子的左子树成为A的右子树。
三、LR型：在A节点的左孩子的右子树上插入节点，导致A节点的平衡因子变为2。
可先对A的左孩子进行左旋操作（RR型），再对A节点进行右旋操作（LL型）。
四、RL型：在A节点的右孩子的左子树上插入节点，导致A节点的平衡因子变为-2。
可先对A的左孩子进行右旋操作（LL型），再对A节点进行左旋操作（RR型）。
*/
bool InsertBST(BSTree*& rt, KeyType k, ElemType d) {		//插入 
	if (rt == NULL) {
		rt = new BSTNode(k, d);
		return true;
	}
	//if (k == rt->key)	return false;
	bool res = true;
	if (k <= rt->key) {
		res = InsertBST(rt->left, k, d);
		if (res && GetHeight(rt->left) - GetHeight(rt->right) > 1) {
			if (k < rt->left->key)	UpdateLL(rt);	//左左
			else	UpdateLR(rt);				//左右 
		}
	}
	else {
		res = InsertBST(rt->right, k, d);
		if (res && GetHeight(rt->left) - GetHeight(rt->right) < -1) {
			if (k > rt->right->key)	UpdateRR(rt);	//右右 
			else	UpdateRL(rt);					//右左 
		}
	}
	if (res) UpdateHeight(rt);
	return res;
}

void DeleteBST_(BSTree*& rt, BSTree* pt) {		//删除节点有左右子树时处理 
	if (rt->right == NULL) {
		BSTree* p = rt;
		pt->key = rt->key;
		rt = rt->left;
		delete p;
	}
	else {
		DeleteBST_(rt->right, pt);
		if (GetHeight(rt->left) - GetHeight(rt->right) > 1) {
			UpdateLL(rt);					//左左 
		}
	}
	UpdateHeight(rt);
}

/*
删除节点操作。可先删除节点，再对其影响的祖先节点进行平衡调整，同时更新其节点的高度。
同二叉排序树相同，删除节点分三种情况
一：被删除节点没有孩子节点，则直接删除即可
二：被删除节点只有一个孩子节点，则将其孩子节点代替删除节点的位置，随后删除节点即可
三：被删除节点有左右孩子，则可移花接木，即取左子树的最大值（也可取右子树的最小值）存放在被删除节点中，随后删除左子树的最大值的节点即可
对于情况一，可同情况二处理
而对于平衡调整，需先更新其节点的高度，对于情况三处理时也需更新其节点高度，
再对其左右子树高度判断其为哪种平衡调整，调整时同时更新其节点高度
*/
bool DeleteBST(BSTree*& rt, KeyType k) {		//删除
	if (rt == NULL)	return false;
	bool res = true;
	if (rt->key == k) {
		if (rt->left == NULL) {
			rt = rt->right;
		}
		else if (rt->right == NULL) {
			rt = rt->left;
		}
		else {
			DeleteBST_(rt->left, rt);
		}
	}
	else if (k < rt->key) {
		res = DeleteBST(rt->left, k);
		if (res && GetHeight(rt->left) - GetHeight(rt->right) > 1) {
			if (k < rt->left->key)	UpdateLL(rt);	//左左
			else	UpdateLR(rt);				//左右
		}
		else if (res && GetHeight(rt->left) - GetHeight(rt->right) < -1) {
			if (k > rt->right->key)	UpdateRR(rt);	//右右 
			else	UpdateRL(rt);					//右左 
		}
	}
	else {
		res = DeleteBST(rt->right, k);
		if (res && GetHeight(rt->left) - GetHeight(rt->right) > 1) {
			if (k < rt->left->key)	UpdateLL(rt);	//左左
			else	UpdateLR(rt);				//左右 
		}
		else if (res && GetHeight(rt->left) - GetHeight(rt->right) < -1) {
			if (k > rt->right->key)	UpdateRR(rt);	//右右 
			else	UpdateRL(rt);					//右左 
		}
	}
	if (res)	UpdateHeight(rt);
	return res;
}

void InorderTraversal(BSTree* rt) {	//中序遍历 
	if (rt == NULL)	return;
	InorderTraversal(rt->left);
	//cout << rt->key << " ";
	InorderTraversal(rt->right);
}

bool Judge(BSTree* rt) {		//判断是否为AVL
	if (rt == NULL)	return true;
	if (Judge(rt->left) && Judge(rt->right) && abs(GetHeight(rt->left) - GetHeight(rt->right)) <= 1)	return true;
	return false;
}

void LevelOrder(BSTree* root) {	//层序遍历 
	if (root == NULL)  return;
	std::queue<BSTree*> que;
	que.push(root);
	int n;
	BSTree* rt;
	//std::cout << "层序遍历：当前节点 (高度) = 左节点 右节点" << std::endl;
	while (!que.empty()) {
		n = que.size();
		while (n--) {
			rt = que.front();	que.pop();
			std::cout << rt->key << " (" << rt->height << ")\t=\t";
			if (rt->left)	std::cout << rt->left->key << "\t";
			else std::cout << "#\t";
			if (rt->right)	std::cout << rt->right->key << "\t";
			else std::cout << "#\t";
			std::cout << std::endl;
			if (rt->left)	que.push(rt->left);
			if (rt->right)	que.push(rt->right);
		}
	}
}


#endif // !_HDSDATABASE_AVLTREE_H

