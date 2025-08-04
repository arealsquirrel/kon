#ifndef KN_SET_HPP
#define KN_SET_HPP

#include "kon/container/arraylist.hpp"
#include "kon/core/allocator.hpp"
#include "kon/core/util.hpp"
#include <functional>
#include <kon/core/core.hpp>

namespace kon {

template<class Key, class Value>
struct SetTreeNode {
	SetTreeNode() = default;
    SetTreeNode(const Key key, const Value &value, bool color,
			SetTreeNode<Key, Value> *left=nullptr, SetTreeNode<Key, Value> *right=nullptr, SetTreeNode<Key, Value> *parent=nullptr)
        : m_pair(key, value), m_color(color), m_left(left), m_right(right), m_parent(parent) {}

	

	Pair<const Key, Value> m_pair;
	
    bool m_color {false}; // false is black, true is red
    SetTreeNode<Key, Value> *m_left {nullptr};
    SetTreeNode<Key, Value> *m_right {nullptr};
    SetTreeNode<Key, Value> *m_parent {nullptr};
};

/*
 * ok so this tree thing is horribly cache bad
 * its great for fragmentation, and i will fix this later
 * because i dont want to do that right now
 */
template<class Key, class Value>
class Set  {
public:
    using TreeNode = SetTreeNode<Key, Value>;

    Set(Allocator *allocator)
        : m_allocator(allocator) {
		
		m_nil = reinterpret_cast<TreeNode*>(
				m_allocator->allocate_mem(sizeof(TreeNode)));
		m_nil->m_left = m_nil;
		m_nil->m_right = m_nil;
		m_nil->m_parent = nullptr;
		m_nil->m_color = false;
		m_rootNode = m_nil;
    }

    ~Set() {
        reset(m_rootNode);

		m_allocator->free_mem(KN_MEM_POINTER(m_nil), sizeof(TreeNode));
    }

public:
	// ----------- MODIFICATION ----------- //
    Pair<const Key, Value> &add(const Pair<const Key, Value> &pair) {
        u64 hash = m_hash(pair.first);
        TreeNode *parent = nullptr;
        TreeNode *curr = m_rootNode;
        while (curr != m_nil) {
            parent = curr;
            if(m_hash(curr->m_pair.first) > hash) {
                curr = curr->m_left;
            } else {
                curr = curr->m_right;
            }
        }

		TreeNode *node = m_allocator->allocate<TreeNode>(1, pair.first, pair.second,
		 		true, m_nil, m_nil, parent);

        if(parent == nullptr) {
            m_rootNode = node;
        } else if(m_hash(parent->m_pair.first) > hash) {
            parent->m_left = node;
        } else {
            parent->m_right = node;
        }

        if(node->m_parent == nullptr) {
            node->m_color = 0;
            return node->m_pair;
        }

        if(node->m_parent->m_parent == nullptr) return node->m_pair;
        recolor(node);
        return node->m_pair;
    }

	// resets the tree
    void reset(TreeNode *from) {
        while (from != m_nil) {
            if(from->m_left == m_nil && from->m_right == m_nil) {
				m_allocator->free(from);
                return;
            }

            reset(from->m_left);
            from->m_left = m_nil;
            reset(from->m_right);
            from->m_right = m_nil;
        }
    }

	// find the minimum hash in the tree
    inline TreeNode *minimum(TreeNode *node) {
        while (node->m_left != m_nil) {
            node = node->m_left;
        }
        return node;
    }

	// find the maximum hash in the tree
    inline TreeNode *maximum(TreeNode *node) {
        while (node->m_right != m_nil) {
            node = node->m_right;
        }
        return node;
    }

	// erases a key from the array
	inline void erase(const Key key) {
		delete_node(find_node(key));
	}

	inline TreeNode *find_node(const Key key) const {
		u64 hash = m_hash(key);
        TreeNode *curr = m_rootNode;
        while(curr != m_nil) {
            if(m_hash(curr->m_pair.first) == hash) return curr;
            if(m_hash(curr->m_pair.first) > hash) {
                curr = curr->m_left;
            } else {
                curr = curr->m_right;
            }
        }
        return curr;
    }

	inline bool has_key(const Key key) {
		return (find_node(key) != m_nil);
	}

    inline Pair<const Key, Value> &find(const Key key) { 
		return find_node(key)->m_pair; 
	}

    inline TreeNode *get_root() const { return m_rootNode; }

	void delete_node(TreeNode *node) {
        TreeNode *y = node;
        TreeNode *x = nullptr;
        int y_original_color = y->m_color;
        if (node->m_left == m_nil) {
            x = node->m_right;
            rb_transplant(node, node->m_right);
        } else if (node->m_right == m_nil) {
            x = node->m_left;
            rb_transplant(node, node->m_left);
        } else {
            y = minimum(node->m_right);
            y_original_color = y->m_color;
            x = y->m_right;
            if (y->m_parent == node) {
                x->m_parent = y;
            } else {
                rb_transplant(y, y->m_right);
                y->m_right = node->m_right;
                y->m_right->m_parent = y;
            }

            rb_transplant(node, y);
            y->m_left = node->m_left;
            y->m_left->m_parent = y;
            y->m_color = node->m_color;
        }

		m_allocator->free(node);
        // delete node;
        if (y_original_color == 0) {
            delete_fix(x);
        }
    }

private:
    void rb_transplant(TreeNode *u, TreeNode *v) {
        if (u->m_parent == nullptr) {
            m_rootNode = v;
        } else if (u == u->m_parent->m_left) {
            u->m_parent->m_left = v;
        } else {
            u->m_parent->m_right = v;
        }

        v->m_parent = u->m_parent;
    }

    void delete_fix(TreeNode *x) {
        TreeNode *s;
        while (x != m_rootNode && x->m_color == 0) {
            if (x == x->m_parent->m_left) {
                s = x->m_parent->m_right;
                if (s->m_color == 1) {
                    s->m_color = 0;
                    x->m_parent->m_color = 1;
                    rotate_left(x->m_parent);
                    s = x->m_parent->m_right;
                }

                if (s->m_left->m_color == 0 && s->m_right->m_color == 0) {
                    s->m_color = 1;
                    x = x->m_parent;
                } else {
                    if (s->m_right->m_color == 0) {
                        s->m_left->m_color = 0;
                        s->m_color = 1;
                        rotate_right(s);
                        s = x->m_parent->m_right;
                    }

                    s->m_color = x->m_parent->m_color;
                    x->m_parent->m_color = 0;
                    s->m_right->m_color = 0;
                    rotate_left(x->m_parent);
                    x = m_rootNode;
                }
            } else {
                s = x->m_parent->m_left;
                if (s->m_color == 1) {
                    s->m_color = 0;
                    x->m_parent->m_color = 1;
                    rotate_right(x->m_parent);
                    s = x->m_parent->m_left;
                }

                if (s->m_right->m_color == 0 && s->m_right->m_color == 0) {
                    s->m_color = 1;
                    x = x->m_parent;
                } else {
                    if (s->m_left->m_color == 0) {
                        s->m_right->m_color = 0;
                        s->m_color = 1;
                        rotate_left(s);
                        s = x->m_parent->m_left;
                    }

                    s->m_color = x->m_parent->m_color;
                    x->m_parent->m_color = 0;
                    s->m_left->m_color = 0;
                    rotate_right(x->m_parent);
                    x = m_rootNode;
                }
            }
        }
        x->m_color = 0;
    }

private:
    void rotate_left(TreeNode *x) {
        // set y to be the right child of x
        TreeNode *y = x->m_right;
        x->m_right = y->m_left;
        if(y->m_left != m_nil) {
            y->m_left->m_parent = x;
        } 
        
        y->m_parent = x->m_parent;
        if(x->m_parent == nullptr) {
            m_rootNode = y;
            x->m_parent = m_nil;
        } else if(x == x->m_parent->m_left) {
            x->m_parent->m_left = y;
        } else {
            x->m_parent->m_right = y;
        }
        y->m_left = x;
        x->m_parent = y;
    }

    void rotate_right(TreeNode *x) {
        TreeNode *y = x->m_left;
        x->m_left = y->m_right;
        if(y->m_right != m_nil) {
            y->m_right->m_parent = x;
        }

        y->m_parent = x->m_parent;
        if(x->m_parent == nullptr) {
            m_rootNode = y;
            y->m_parent = m_nil;
        }else if(x == x->m_parent->m_right) {
            x->m_parent->m_right = y;
        } else {
            x->m_parent->m_left = y;
        }

        y->m_right = x;
        x->m_parent = y;
    }

    void recolor(TreeNode *from) {
        while(from != m_rootNode && from->m_parent->m_color == 1) {
            if(from->m_parent == from->m_parent->m_parent->m_left) {
                // Right uncle
                TreeNode *uncle = from->m_parent->m_parent->m_right;
                if(uncle->m_color == 1) {
                    from->m_parent->m_color = 0;
                    uncle->m_color = 0;
                    from->m_parent->m_parent->m_color = 1;
                    from = from->m_parent->m_parent;
                } else {
                    if(from == from->m_parent->m_right) {
                        from = from->m_parent;
                        rotate_left(from);
                    }
                    from->m_parent->m_color = 0;
                    from->m_parent->m_parent->m_color = 1;
                    rotate_right(from->m_parent->m_parent);
                }
            } else {
                TreeNode *uncle = from->m_parent->m_parent->m_left;
                if(uncle->m_color == 1) {
                    from->m_parent->m_color = 0;
                    uncle->m_color = 0;
                    from->m_parent->m_parent->m_color = 1;
                    from = from->m_parent->m_parent;
                } else {
                    if(from == from->m_parent->m_left) {
                        from = from->m_parent;
                        rotate_right(from);
                    }
                    from->m_parent->m_color = 0;
                    from->m_parent->m_parent->m_color = 1;
                    rotate_left(from->m_parent->m_parent);
                }
            }
        }

        m_rootNode->m_color = 0;
    }

private:
    size_t m_nodeCount {0};
    TreeNode *m_rootNode {nullptr};
    TreeNode *m_nil=nullptr;
	std::hash<Key> m_hash;
	Allocator *m_allocator;
};

}

#endif
