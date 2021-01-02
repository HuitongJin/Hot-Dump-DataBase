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

int GetHeight(BSTree* rt) {	//�õ��߶� 
	if (rt == NULL)	return 0;
	return rt->height;
}

void UpdateHeight(BSTree* rt) {	//���¸߶� 
	if (rt == NULL)	return;
	rt->height = std::max(GetHeight(rt->left), GetHeight(rt->right)) + 1;
}

//�������(bf=2)�����������ӽڵ��ɸ��ڵ㣬���������ӽڵ��ɽ����ڵ�����ӽڵ� 
void UpdateLL(BSTree*& rt) {
	BSTree* pl = rt->left;
	rt->left = pl->right;
	pl->right = rt;
	rt = pl;
	UpdateHeight(rt->left);
	UpdateHeight(rt->right);
	UpdateHeight(rt);
}

//���ҵ���(bf=-2),���������ӽڵ��ɸ��ڵ㣬���������ӽڵ��ɽ����ڵ�����ӽڵ� 
void UpdateRR(BSTree*& rt) {
	BSTree* pr = rt->right;
	rt->right = pr->left;
	pr->left = rt;
	rt = pr;
	UpdateHeight(rt->left);
	UpdateHeight(rt->right);
	UpdateHeight(rt);
}

//���ҵ���(bf=2),�ȶ����ӽڵ���������Ϊ�����ͣ��ٽ���������� 
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

// �������(bf = -2), �ȶ����ӽڵ���������Ϊ�����ͣ��ٽ������ҵ���
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

BSTree* SearchBST(BSTree* rt, KeyType k) {		//����
	if (rt == NULL || k == rt->key)	
		return rt;
	if (k < rt->key)	
		return SearchBST(rt->left, k);
	else	
		return SearchBST(rt->right, k);
}

// ��ѯ������Ӧ������������ҵ���ȵ�������ֱ�ӷ���
// ����Ӧ�ҵ���Ҫ��ѯ������ӽ��ĵ�С��Ҫ��ѯ�����Ľڵ�λ��
BSTree* SearchNode(BSTree* rt, KeyType k) {
	if (rt == NULL)
		return NULL;
	else {
		BSTree *p = rt;
		// ���ڲ�ѯ������ָ�����һ����Ҫ��ѯ����С�Ľڵ�
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
����ڵ�������Ȳ���ڵ㣬�ٶ���Ӱ������Ƚڵ����ƽ�������ͬʱ������ڵ�߶ȡ�
�ڵ�A��ƽ������ɷ�Ϊ���������
һ��LL�ͣ���A�ڵ�����ӵ��������ϲ���ڵ㣬����A�ڵ��ƽ�����ӱ�Ϊ2��
�ɶ��������������A�����Ӵ���A��Ϊ���ڵ㣬��ԭA�����ӵ���������ΪA����������
����RR�ͣ���LL���෴������A�ڵ���Һ��ӵ��������ϲ���ڵ㣬����A�ڵ��ƽ�����ӱ�Ϊ-2��
�ɶ��������������A���Һ��Ӵ���A��Ϊ���ڵ㣬��ԭA���Һ��ӵ���������ΪA����������
����LR�ͣ���A�ڵ�����ӵ��������ϲ���ڵ㣬����A�ڵ��ƽ�����ӱ�Ϊ2��
���ȶ�A�����ӽ�������������RR�ͣ����ٶ�A�ڵ��������������LL�ͣ���
�ġ�RL�ͣ���A�ڵ���Һ��ӵ��������ϲ���ڵ㣬����A�ڵ��ƽ�����ӱ�Ϊ-2��
���ȶ�A�����ӽ�������������LL�ͣ����ٶ�A�ڵ��������������RR�ͣ���
*/
bool InsertBST(BSTree*& rt, KeyType k, ElemType d) {		//���� 
	if (rt == NULL) {
		rt = new BSTNode(k, d);
		return true;
	}
	//if (k == rt->key)	return false;
	bool res = true;
	if (k <= rt->key) {
		res = InsertBST(rt->left, k, d);
		if (res && GetHeight(rt->left) - GetHeight(rt->right) > 1) {
			if (k < rt->left->key)	UpdateLL(rt);	//����
			else	UpdateLR(rt);				//���� 
		}
	}
	else {
		res = InsertBST(rt->right, k, d);
		if (res && GetHeight(rt->left) - GetHeight(rt->right) < -1) {
			if (k > rt->right->key)	UpdateRR(rt);	//���� 
			else	UpdateRL(rt);					//���� 
		}
	}
	if (res) UpdateHeight(rt);
	return res;
}

void DeleteBST_(BSTree*& rt, BSTree* pt) {		//ɾ���ڵ�����������ʱ���� 
	if (rt->right == NULL) {
		BSTree* p = rt;
		pt->key = rt->key;
		rt = rt->left;
		delete p;
	}
	else {
		DeleteBST_(rt->right, pt);
		if (GetHeight(rt->left) - GetHeight(rt->right) > 1) {
			UpdateLL(rt);					//���� 
		}
	}
	UpdateHeight(rt);
}

/*
ɾ���ڵ����������ɾ���ڵ㣬�ٶ���Ӱ������Ƚڵ����ƽ�������ͬʱ������ڵ�ĸ߶ȡ�
ͬ������������ͬ��ɾ���ڵ���������
һ����ɾ���ڵ�û�к��ӽڵ㣬��ֱ��ɾ������
������ɾ���ڵ�ֻ��һ�����ӽڵ㣬���亢�ӽڵ����ɾ���ڵ��λ�ã����ɾ���ڵ㼴��
������ɾ���ڵ������Һ��ӣ�����ƻ���ľ����ȡ�����������ֵ��Ҳ��ȡ����������Сֵ������ڱ�ɾ���ڵ��У����ɾ�������������ֵ�Ľڵ㼴��
�������һ����ͬ���������
������ƽ����������ȸ�����ڵ�ĸ߶ȣ��������������ʱҲ�������ڵ�߶ȣ�
�ٶ������������߶��ж���Ϊ����ƽ�����������ʱͬʱ������ڵ�߶�
*/
bool DeleteBST(BSTree*& rt, KeyType k) {		//ɾ��
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
			if (k < rt->left->key)	UpdateLL(rt);	//����
			else	UpdateLR(rt);				//����
		}
		else if (res && GetHeight(rt->left) - GetHeight(rt->right) < -1) {
			if (k > rt->right->key)	UpdateRR(rt);	//���� 
			else	UpdateRL(rt);					//���� 
		}
	}
	else {
		res = DeleteBST(rt->right, k);
		if (res && GetHeight(rt->left) - GetHeight(rt->right) > 1) {
			if (k < rt->left->key)	UpdateLL(rt);	//����
			else	UpdateLR(rt);				//���� 
		}
		else if (res && GetHeight(rt->left) - GetHeight(rt->right) < -1) {
			if (k > rt->right->key)	UpdateRR(rt);	//���� 
			else	UpdateRL(rt);					//���� 
		}
	}
	if (res)	UpdateHeight(rt);
	return res;
}

void InorderTraversal(BSTree* rt) {	//������� 
	if (rt == NULL)	return;
	InorderTraversal(rt->left);
	//cout << rt->key << " ";
	InorderTraversal(rt->right);
}

bool Judge(BSTree* rt) {		//�ж��Ƿ�ΪAVL
	if (rt == NULL)	return true;
	if (Judge(rt->left) && Judge(rt->right) && abs(GetHeight(rt->left) - GetHeight(rt->right)) <= 1)	return true;
	return false;
}

void LevelOrder(BSTree* root) {	//������� 
	if (root == NULL)  return;
	std::queue<BSTree*> que;
	que.push(root);
	int n;
	BSTree* rt;
	//std::cout << "�����������ǰ�ڵ� (�߶�) = ��ڵ� �ҽڵ�" << std::endl;
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

