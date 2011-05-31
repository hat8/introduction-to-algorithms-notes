//////////////////////////////////////////////////////////////////////////  
///    COPYRIGHT NOTICE  
///    Copyright (c) 2009
///    All rights reserved.  
///  
/// @file		binary_search_tree.cpp
/// @brief		binary_search_tree.cpp�ļ������ 
///  			
///  			binary_search_tree.cpp����ϸ����
/// 
/// @author		̷����	chuanqi.tan@gmail.com 
/// @date		2011/05/30
/// @version 	1.0
///  
///  
///    �޶�˵��������汾  
//////////////////////////////////////////////////////////////////////////  

#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <iomanip>
#include <limits>
using namespace std;

namespace chapter12
{
	/// ���������
	class BinarySearchTree
	{
	private:
		/// ����������еĽ��
		struct _Node
		{
			int Value;
			_Node *Parent;
			_Node *Left;
			_Node *Right;
		};

	public:
		BinarySearchTree() : _root(NULL)
		{}

		~BinarySearchTree()
		{
			//�Ӹ���㿪ʼ�ݹ������
			_RecursiveReleaseNode(_root);
		}

		/// ����һ�����
		/// 
		/// �����������в���һ��ֵ
		/// @param	new_value	Ҫ�����ֵ
		/// @return				�Ƿ����ɹ���ʧ����ζ�������Ѿ����ڸ�ֵ
		bool Insert(int const new_value)
		{
			if (Search(new_value))
			{//�Ѿ�����
				return false;
			}

			if (!_root)
			{//������ǵ�1���ڵ�
				_root = new _Node();
				_root->Value = new_value;
				return true;
			}

			//�ǵ�1���ڵ�
			_Node *current_node = _root;

			while (current_node)
			{
				_Node *&next_node_pointer = (new_value > current_node->Value ? current_node->Right : current_node->Left);
				if (next_node_pointer)
				{
					current_node = next_node_pointer;
				}
				else
				{
					next_node_pointer = new _Node();
					next_node_pointer->Value = new_value;
					next_node_pointer->Parent = current_node;
					break;
				}
			}

			return true;
		}

		/// ɾ�����
		/// 
		/// �ڶ����������ɾ��һ��ֵ
		/// @param	delete_value	Ҫɾ����ֵ
		/// @return					�Ƿ�ɾ���ɹ���ɾ��ʧ����ζ������û�����ֵ�Ľ��
		bool Delete(int const delete_value)
		{
			_Node *delete_node = _Search(_root, delete_value);
			if (!delete_node)
			{//δ�ҵ��õ�
				return false;
			}
			else
			{
				_DeleteNode(delete_node);
				return true;
			}
		}

		/// ����Ԫ��
		/// 
		/// �ڵ�ǰ����������в���ĳһֵ
		/// @param	search_value	Ҫ���ҵ�ֵ
		/// @return					�Ƿ��ڶ��������ҵ�ֵΪsearch_value�Ľ��
		/// @retval		true		���ҵ��˸�Ԫ��
		/// @retval		false		�Ҳ�����Ԫ��
		bool Search(int const search_value) const
		{
			return _Search(_root, search_value) != NULL;
		}

		/// ��ʾ��ǰ�����������״̬
		/// 
		/// ʹ��dot������ǰ���������
		void Display() const
		{
			cout << "digraph graphname" << (rand() % 1000) << "{" << endl
				 << "    node [shape = record,height = .1];" << endl;
			_Display(_root);
			cout << "}" << endl;
		}
	
	private:
		void _DeleteNode( _Node * delete_node ) 
		{
			if (delete_node->Left && delete_node->Right)
			{//Ҫɾ���Ľ��ͬʱ������������������
				//ǰ����㣺ǰ��һ�����ڣ���Ϊ�ý��ͬʱ������������
				_Node *previous_node = _GetPreviousNode(delete_node);

				delete_node->Value = previous_node->Value;

				//previous_ndeһ��û���������������ٵݹ����һ���������if��else��֧
				_DeleteNode(previous_node);	
			}
			else
			{//Ҫɾ���Ľ��������һ���ӽ��Ϊ��
				//sub_nodeΪdelete_node������
				//sub_nodeҪôΪdelete_node����������ҪôΪdelete_node����������������delete_node���ӽ��ʱΪ��
				_Node *sub_node = (delete_node->Left ? delete_node->Left : delete_node->Right);

				if (delete_node->Parent == NULL)
				{//�ǵ�1�����
					_root = sub_node;
				}
				else
				{
					(delete_node->Parent->Left == delete_node ? delete_node->Parent->Left : delete_node->Parent->Right) = sub_node;

					if (sub_node)
					{//��delete_node���ӽ��ʱ��Ҫ�����ӽ���Parentָ��
						sub_node->Parent = delete_node->Parent;
					}
				}

				delete delete_node;
			}
		}

		/// �õ�һ��ͬʱ�������������Ľڵ��ǰ��
		/// 
		/// @notes		node��ǰ��һ�����ڣ���Ϊnodeͬʱ������������������
		_Node * _GetPreviousNode( _Node * node )
		{
			if (!node->Left || !node->Right)
			{//�Ⱦ������������㣬�����������ƵĽ���ǰ���㷨����������
				throw std::exception("node����ͬʱ������������������");
			}

			_Node *previous_node = node; 
			//�����ǰ��������һ�£����ҵ�ͷ
			previous_node = previous_node->Left;
			while (previous_node->Right)
			{
				previous_node = previous_node->Right;
			}

			return previous_node;
		}

		void _RecursiveReleaseNode(_Node *node)
		{
			if (node)
			{			
				_RecursiveReleaseNode(node->Left);
				_RecursiveReleaseNode(node->Right);
				delete node;
			}
		}

		/// �ǵݹ����һ�����
		_Node * _Search(_Node *node, int const search_value) const
		{
			while (node && node->Value != search_value)
			{
				if (search_value < node->Value)
				{
					node = node->Left;
				}
				else
				{
					node = node->Right;
				}
			}

			//���������nodeΪ�վ���δ�ҵ�
			return node;
		}

		void _Display(_Node *node) const
		{
			if (node)
			{
				cout << "    node" << node->Value << "[label = \"<f0>|<f1>" << node->Value << "|<f2>\"];" << endl;

				if (node->Left)
				{
					cout << "    \"node" << node->Value << "\":f0 -> \"node" << node->Left->Value << "\":f1;" << endl;
					_Display(node->Left);
				}

				if (node->Right)
				{
					cout << "    \"node" << node->Value << "\":f2 -> \"node" << node->Right->Value << "\":f1;" << endl;
					_Display(node->Right);
				}
			}
		}

		_Node *_root;			///< ����������ĸ����
	};



	int test()
	{
		BinarySearchTree bst;
		//�����ֵ����һ�ö��������
		for (int i = 0; i < 10; ++i)
		{
			bst.Insert(rand() % 100);						
		}
		bst.Display();

		//ɾ�����е�����ֵ���
		for (int i = 1; i < 100; i += 2)
		{
			if(bst.Delete(i))
			{
				cout << "### Deleted [" << i << "] ###" << endl;
			}
		}
		bst.Display();

		//����100���ڵ���������ڶ���������У�����ʾ
		cout << endl;
		for (int i = 0; i < 100; i += 1)
		{
			if (bst.Search(i))
			{
				cout << "����[" << i << "]Ԫ�أ�\t�ɹ�" << endl;
			}			
		}

		return 0;
	}
}