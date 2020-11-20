#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<memory.h>
#include<iostream>
#include<string>

// -----------------------------------------------------------------------------------------------
// ��������� �������
// -----------------------------------------------------------------------------------------------

class RB{
    struct node{
		node *left;
		node *right;
		node *parent;  
		int value;
		bool color; // true - �������, false - ������ 
		int height;
	};
	node *root;    		// ������
	int nodes_count; 	// ����� ���� ������ 
	
	private:
	    node *New_Node(int value);	        // ��������� ����� �������
	    void Erase_Node(node*);           	// �������� ������ �������
		void Clear(node *)					//���������� ���� ������ ����� �������� 
	    static  node *Rotate_left(node*);  	// ����� �������� � ������ �������
				node *Rotate_right(node*); 	// ������ �������� � ������ ������� 
	    
		bool Insert(int, node**);     		// ������� �������� �� ��������� � ����� ������
	    bool Erase(int, node**);         	// �������� �������� �� ��������� � ����� ������
	    bool GetMin(node**,node**);   		// ���������� � �������� ������������� ���� ���������
		
		void Balance_Insert(node**);      	// ������������ ����� �������
	    bool Balance_Erase_left(node**);    // ������������ ������ ��������
	    bool Balance_Erase_right(node**);   // ������������ ������� ��������
	
	public:
	    RB();				// �������� ������� �����
	    ~RB(); 				// ���� ������		
	    node Find(int);    	// ����� ������� � ������ ��������� - ������ ��������� �� ���
	    void Insert(int);  	// ������� ������� �������� � ������ - ������ �� ������
	    void Erase(int);    // �������� ������� � ������ ��������� - ������ �� ������
	    //void Show();      // ������������ ����� ��� �������
};

// -----------------------------------------------------------------------------------------------
// ���������� ����������: �������� � �������
// -----------------------------------------------------------------------------------------------

// ��������� ����� �������
RB::node *RB::New_Node(int value) { 
    nodes_count++;
    node *node = new node;
    node->value = value;
    node->left = 0;
	node->right = 0;
    node->color = true;
    return node;
}

//�������� ������ �������
void RB::Erase_Node(node *node) { //�������� ������ �������
    nodes_count--;
    delete node;
}

//���������� ���� ������ ����� ��������
void RB::Clear(node *node) { 
    if (!node) {
		return;
	};	 // ���� ����� ��������� �� ������� - ������ �� �����
    Clear(node->left);
    Clear(node->right);
    Erase_Node(node);
}

// ����� �������� � ������ �������
RB::node *RB::Rotate_left(node *node) {
    node *left = node -> left;
    node *local = left -> right;
    right -> left = node;
    node -> right = local;
    return right;
}

// ������ �������� � ������ �������
RB::node *RB::Rotate_right(node *node) {
    node *right = node -> right;
    node *local = right -> left;
    left -> right = node;
    node -> = local;
    return right;
}

// ������� �������� �� ��������� � ����� ������
bool RB::Insert(int value, node **root) {
    node *node = *root;
    if (!node) {
		*root = NewNode(value);
	} else {
        if (value == node->value) {
			return true;
		}
        if (Insert(value, height, value<node->value?&node->p1:&node->p2)){ //!!!
			return true;
		}
        Balance_Insert(root);
    }
    return false;
}

// �������� �������� �� ��������� � ����� ������
bool RB::Erase(int value, node **root) {
    node *t,*node=*root;
    if (!node) {
		return false;
	}
    if (node->value<value) {
        if (Erase(value, &node->right)) {
			return Balance_Erase_right(root);
		}
    } else if(node->value>value) {
        if (Erase(value, &node->left)){
			return Balance_Erase_left(root);
		}
    } else {
        bool res;
        if(!node->right) {
            *root = node->left;
            res =! node->color;
        } else {
            res = GetMin(&node->right,root);
            t = *root;
            t->color = node->color;
            t->left = node->left;
            t->right = node->right;
            if (res) {
				res = Balance_Erase_right(root);
			}
        }
        Erase_Node(node);
        return res;
    }
    return 0;
}

bool RB::GetMin(node **root, node **res) {
    node_st *node=*root;
    if (node->left) {
        if (GetMin(&node->left,res)) {
			return BalanceRemove_left(root);
		}
    } else {
        *root = node->right;
        *res = node;
        return !node->color;
    }
    return false;
}

// -----------------------------------------------------------------------------------------------
// ���������� ����������: ������� � ������������ ������
// -----------------------------------------------------------------------------------------------

void RB::Balance_Insert(node **root) { // ������������ ������ ����� �����
    node *left, *right, *left_color,*right_color;
    node *node = *root;
    if (node->color) {
    	return;	
	};
    left = node->left;
    right = node->right;
    
    // �������� �� ��������-�������: �������-�������; � �����������   
    // ������ ����� �����
    if (left and left->color) {
        right_color = left->right;
        if (right_color and right_color->color) {
        	left = node->left = Rotate_left(left);        	
		}
		right_color = left->left;
        if (left_color and left_color->color) {
            node->color = true;
            left->color = false;
            if (right and right->color) {
				// ���������� ������ 
                left_color->color = true;
                right->color = false;
                return;
            }
            *root = Rotate_right(node);
            return;
        }
    }
    // ������ ������ �����
    if (right and right->color) {
        left_color = right->left;
		if (left_color and left_color->color) {
			right = node->right = Rotate_right(right);
		}
        right_color = right->right;
        if (right_color and right_color->color) {
            node->color = true;
            right->color = false;
            if(left and left->color) {
				// ���������� ������ 
                right_color->color = true;
                left->color = false;
                return;
            }
            *root = Rotate_left(node);
            return;
        }
    }
}

bool RB::Balance_Erase_left(node **root) {
    node *node = *root;
    node *left = node->left;
    node *right = node->right;
    if (left and left->color) {
        left->color = false;
		return false;
    }
    if (right and right->color) { // ������ 1
        node->color=true;
        right->color=false;
        node = *root = Rotate_left(node);
        if (Balance_Erase_left(&node->left)) {
			node->left->color = false;
		}
        return false;
    }
    unsigned int mask = 0;
    node *rl=->p1;
    node *rr=p2->p2;
    if (rl and rl->color) {
		mask|=1;
	}
    if (rr and rr->color) {
    	mask|=2;
    }
    switch(mask) {
        case 0:     // ������ 2 - if ((!rl || !rl->color) and (!rr || !rr->color))
            right->color = true;
            return true;
        case 1:
        
		case 2:     // ������ 3 - if (p22 and p22->red)
            right->color = node->color;
            p22->color = node->color = false;
            *root = Rotate_right(node);	
        case 3:     // ������ 4 - if (p21 and p21->red)
            right->color = true;
            rl->color = false;
            right = node->right = Rotate_left(right);
            rl = right->right;
    }
    return false;
}

bool RB::Balance_Erase_right(node **root) {
    node *node = *root;
    node *left = node->left;
    node *right = node->right;
    if (right and right->color) {
		right->color=false;
		return false;
	}
    if (left and left->color) { // ������ 1
        node->color = true;
        left->color = false;
        node = *root = Rotate_right(node);
        if (BalanceRemove2(&node->right)){
			node->right->color = false;
		}
        return false;
    }
    unsigned int mask=0;
    node *ll = left->left;
    node *lr = left->right;
    if (ll and ll->color){
		mask|=1;
	}
    if (lr and lr->color){
		mask|=2;
	}
    switch(mask) {
        case 0:     // ������ 2 - if((!lr || !lr->color) and (!ll || !ll->color))
            left->color=true;
            return true;
        case 1:     // ������ 4 - if(ll and ll->color)
            left->color = node->color;
            ll->color = node->color = false;
            *root = Rotate_right(node);
    	case 2:
        	
        case 3:     // ������ 3 - if(lr and lr->color)
            left->color = true;
            lr->color = false;
            left = node->left = Rotate_left(p1);
            ll = left->left;
    }
    return false;
}

// -----------------------------------------------------------------------------------------------
// ���������������� �����������
// -----------------------------------------------------------------------------------------------

// �������� ������� �����
RB::RB() { 
    root = 0;
    nodes_count = 0;
}

// ���� ������
RB::~RB() {
    Clear(root); 
}

// ����� ������� � ������ ��������� - ������ ��������� �� ���
int RB::Find(int value) {
    node *node = root;
    while(node) {
        if (node->value == value)
		return true;
        node = node->value>value?node->left:node->right;
    }
    return *node;
}

// ������� ������� �������� � ������ - ������ �� ������
void RB::Insert(int value) {
    Insert(value, &root);
    if (root) {
		root->color=false;
	}
}

// �������� ������� � ������ ��������� - ������ �� ������
void RB::Erase(int value) {
    Erase(&root,value);
}

// ���� ������
void RB::Clear() {
    Clear(root);
    root = 0;
}

