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
    public:
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
	    node *new_node(int value);	        // ��������� ����� �������
	    void erase_node(node*);           	// �������� ������ �������
		void clear(node *);					// ���������� ���� ������ ����� �������� 
	    static  node *rotate_left(node*);  	// ����� �������� � ������ �������
				node *rotate_right(node*); 	// ������ �������� � ������ �������
		void show(node*);			    // ����������� ����� �� ����� ������
	    
		node* find(node*, int);				// ����� �������� � ����� ��������� � ��������� � ����� ������
		bool insert(int, node**);     		// ������� �������� �� ��������� � ����� ������
	    bool erase(int, node**);         	// �������� �������� �� ��������� � ����� ������
	    bool getmin(node**,node**);   		// ���������� � �������� ������������� ���� ���������
		
		void balance_insert(node**);      	// ������������ ����� �������
	    bool balance_erase_left(node**);    // ������������ ������ ��������
	    bool balance_erase_right(node**);   // ������������ ������� ��������
	
	public:
	    RB();				// �������� ������� �����
	    ~RB(); 				// ���� ������		
	    node* find(int);    // ����� ������� � ������ ��������� - ������ ��������� �� ���
	    void insert(int);  	// ������� ������� �������� � ������ - ������ �� ������
	    void erase(int);    // �������� ������� � ������ ��������� - ������ �� ������
	    void clear();		// ������ ���� ������
	    void show();      	// ������������ ����� ��� �������
};

// -----------------------------------------------------------------------------------------------
// ���������� ����������: �������� � �������
// -----------------------------------------------------------------------------------------------

// ��������� ����� �������
RB::node *RB::new_node(int value) { 
    nodes_count++;
    node* node = new RB::node;
    node->value = value;
    node->left = 0;
	node->right = 0;
    node->color = true;
    return node;
}

//�������� ������ �������
void RB::erase_node(node *node) { //�������� ������ �������
    nodes_count--;
    delete node;
}

//���������� ���� ������ ����� ��������
void RB::clear(node *node) { 
    if (!node) {
		return;
	};	 // ���� ����� ��������� �� ������� - ������ �� �����
    clear(node->left);
    clear(node->right);
    erase_node(node);
}

// ����� �������� � ������ �������
RB::node *RB::rotate_left(node* node) {
    RB::node *right = node -> right;
    RB::node *local = right -> left;
    right -> left = node;
    node -> right = local;
    return right;
}

// ������ �������� � ������ �������
RB::node *RB::rotate_right(node *node) {
    RB::node *left = node -> left;
    RB::node *local = left -> right;
    left -> right = node;
    node -> left = local;
    return left;
}

// ����������� ����� �� ����� ������
void RB::show(node* node)	{
	if (!node) {
		return;
	}
	bool color = node->color;
	if (color){
		std::cout << " - " << node->value << " red " << " - ";
	}
	if (!color){
		std::cout << " - " << node->value << " black " << " - ";
	}
	show(node->left);
	show(node->right);
}

// ����� �������� � ����� ��������� � ��������� � ����� ������
node* RB::find(node* node, int value) {
    while (node) {
        if (node->value == value) {
			return *node;
        	node = node->value>value?node->left:node->right;
    	}
    }
}

// ������� �������� �� ��������� � ����� ������
bool RB::insert(int value, node **root) {
    node *node = *root;
    if (!node) {
		*root = new_node(value);
	} else {
        if (value == node->value) {
			return true;
		}
        if (insert(value, value<node->value? &node->left: &node->right)){ //!!!
			return true;
		}
        balance_insert(root);
    }
    return false;
}

// �������� �������� �� ��������� � ����� ������
bool RB::erase(int value, node **root) {
    node *t,*node=*root;
    if (!node) {
		return false;
	}
    if (node->value<value) {
        if (erase(value, &node->right)) {
			return balance_erase_right(root);
		}
    } else if(node->value>value) {
        if (erase(value, &node->left)){
			return balance_erase_left(root);
		}
    } else {
        bool res;
        if(!node->right) {
            *root = node->left;
            res =! node->color;
        } else {
            res = getmin(&node->right,root);
            t = *root;
            t->color = node->color;
            t->left = node->left;
            t->right = node->right;
            if (res) {
				res = balance_erase_right(root);
			}
        }
        erase_node(node);
        return res;
    }
    return 0;
}

bool RB::getmin(node **root, node **res) {
    node *node = *root;
    if (node->left) {
        if (getmin(&node->left,res)) {
			return balance_erase_left(root);
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

void RB::balance_insert(node **root) { // ������������ ������ ����� �����
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
        	left = node->left = rotate_left(left);        	
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
            *root = rotate_right(node);
            return;
        }
    }
    // ������ ������ �����
    if (right and right->color) {
        left_color = right->left;
		if (left_color and left_color->color) {
			right = node->right = rotate_right(right);
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
            *root = rotate_left(node);
            return;
        }
    }
}

bool RB::balance_erase_left(node **root) {
    RB::node *node = *root;
    RB::node *left = node->left;
    RB::node *right = node->right;
    if (left and left->color) {
        left->color = false;
		return false;
    }
    if (right and right->color) { // ������ 1
        node->color=true;
        right->color=false;
        node = *root = rotate_left(node);
        if (balance_erase_left(&node->left)) {
			node->left->color = false;
		}
        return false;
    }
    unsigned int mask = 0;
    RB::node *rl = right->left;
    RB::node *rr = right->right;
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
            rr->color = node->color = false;
            *root = rotate_right(node);	
        case 3:     // ������ 4 - if (p21 and p21->red)
            right->color = true;
            rl->color = false;
            right = node->right = rotate_left(right);
            rl = right->right;
    }
    return false;
}

bool RB::balance_erase_right(node **root) {
    RB::node *node = *root;
    RB::node *left = node->left;
    RB::node *right = node->right;
    if (right and right->color) {
		right->color=false;
		return false;
	}
    if (left and left->color) { // ������ 1
        node->color = true;
        left->color = false;
        node = *root = rotate_right(node);
        if (balance_erase_right(&node->right)){
			node->right->color = false;
		}
        return false;
    }
    unsigned int mask=0;
    RB::node *ll = left->left;
    RB::node *lr = left->right;
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
            *root = rotate_right(node);
    	case 2:
        	
        case 3:     // ������ 3 - if(lr and lr->color)
            left->color = true;
            lr->color = false;
            left = node->left = rotate_left(left);
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
    clear(root); 
}

// ����� ������� � ������ ��������� - ������ ��������� �� ���
RB::node find(int value) {
    return find(root, value);
}

// ������� ������� �������� � ������ - ������ �� ������
void RB::insert(int value) {
    insert(value, &root);
    if (root) {
		root->color=false;
	}
}

// �������� ������� � ������ ��������� - ������ �� ������
void RB::erase(int value) {
    erase(value, &root);
}

// ���� ������
void RB::clear() {
    clear(root);
    root = 0;
}


// ������������ ����� ��� �������
void RB::show() {
    show(root);
}
