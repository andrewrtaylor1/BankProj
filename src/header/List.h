#pragma once

#include <string>
#include <memory>

/// <summary>
/// Abstract Node class for Linked List.
/// </summary>
template <typename T>
class Node
{
	private:
		//these are made private because it forces the Node::set/get functions which work better
		std::shared_ptr<Node<T>> next; //next Node
		std::shared_ptr<Node<T>> previous; //previous Node
	public:
		Node(std::shared_ptr<Node<T>> n, std::shared_ptr<Node<T>> p)
			//constructor; as this is abstract & never constructed directly, no need for a default. that'll be in the derived classes
		{
			next = n;
			previous = p;
		}

		void setNext(std::shared_ptr<Node<T>> n) //function to set Next
		{
			next = n;
		}

		void setPrevious(std::shared_ptr<Node<T>> p) //function to set Previous
		{
			previous = p;
		}

		std::shared_ptr<Node<T>> getNext() //gets Next
		{
			return next;
		}

		std::shared_ptr<Node<T>> getPrevious() //gets Previous
		{
			return previous;
		}

		virtual ~Node() {}

		//these functions have to be virtual, as only InternalNode will have the data pointer.

		//Inserts the data at a specific spot. i = desired index, j = current index
		virtual bool put(std::shared_ptr<int> i, std::shared_ptr<int> j, std::shared_ptr<T> d) = 0;

		//Grabs a pointer to the data at a given index, if it exists. Returns nullptr if it doesn't. i = desired index, j = current index
		virtual std::shared_ptr<T> get(std::shared_ptr<int> i, std::shared_ptr<int> j) = 0;

		//moves through the list in an indexed way, returns bool if we get to the desired index & Internal Node exists. i = desired index, j = current index
		virtual bool exists(std::shared_ptr<int> i, std::shared_ptr<int> j) = 0;
		
		//finds a string, has to be exact. I'm not going to use C++23 just to get a Contains function I don't need, & it'd break other things
		virtual int find(std::string s, std::shared_ptr<int> j) = 0;
};

/// <summary>
/// Internal node, actually points to data
/// </summary>
template <typename T>
class InternalNode : public Node<T>
{
	public:
		std::shared_ptr<T> data; //pointer to the data

		InternalNode(std::shared_ptr<T> d, std::shared_ptr<Node<T>> n = std::shared_ptr<Node<T>>(), std::shared_ptr<Node<T>> p = std::shared_ptr<Node<T>>()) : Node<T>(n,p)
		//constructor will always have data pointer
		{
			data = d; //set data pointer
		}

		~InternalNode() {}


		bool exists(std::shared_ptr<int> i, std::shared_ptr<int> j)
		{
			bool b = false;
			if (*i > *j)
			{
				*j = *j + 1;
				b = Node<T>::getNext()->exists(i, j);
			}
			if (*i < *j)
			{
				*j = *j - 1;
				b = Node<T>::getPrevious()->exists(i, j);
			}
			if (*i == *j)
			{
				b = true;
			}
			return b;
		}

		int find(std::string s, std::shared_ptr<int> j)
		{
			if (data->compare(s) == 0)
			{
				return (*j);
			}
			
			if (Node<T>::getNext())
			{
				*j = *j + 1;
				return Node<T>::getNext()->find(s, j);
			}
			else
			{
				return -1;
			}
		}

		std::shared_ptr<T> get(std::shared_ptr<int> i, std::shared_ptr<int> j)
		{
			if (*i > *j)
			{
				*j = *j + 1;
				return Node<T>::getNext()->get(i, j);
			}
			if (*i < *j)
			{
				*j = *j - 1;
				return Node<T>::getPrevious()->get(i, j);
			}
			if (*i == *j)
			{
				return data;
			}
			return std::shared_ptr<T>();
		}

		bool put(std::shared_ptr<int> i, std::shared_ptr<int> j, std::shared_ptr<T> d)
		{
			bool b = false;
			if (*i > *j)
			{
				*j = *j + 1;
				b = Node<T>::getNext()->put(i, j, d);
			}
			if (*i < *j)
			{
				*j = *j - 1;
				b = Node<T>::getPrevious()->put(i, j, d);
			}
			if (*i == *j)
			{
				std::shared_ptr<Node<T>> n(new InternalNode<T>(d));
				Node<T>::getPrevious()->getNext() = n;
				Node<T>::getPrevious() = n;
			}

			return b;
		}
};

/// <summary>
/// external nodes; works as tail or head depending on direction
/// </summary>
template <typename T>
class ExternalNode : public Node<T>
{
	public:
		ExternalNode(std::shared_ptr<Node<T>> n = std::shared_ptr<Node<T>>(), std::shared_ptr<Node<T>> p = std::shared_ptr<Node<T>>()) : Node<T>(n, p) {}
		
		~ExternalNode() {}

		bool exists(std::shared_ptr<int> i, std::shared_ptr<int> j)
		{
			bool b = false;
			if (*i > *j)
			{
				*j = *j + 1;
				return Node<T>::getNext()->exists(i, j);
			}
			if (*i < *j)
			{
				*j = *j - 1;
				return Node<T>::getPrevious()->exists(i, j);
			}
			return b;
		}

		int find(std::string s, std::shared_ptr<int> j)
		{
			if (Node<T>::getNext())
			{
				(*j) = (*j) + 1;
				return Node<T>::getNext()-> find(s, j);
			}
			else
			{
				return -1;
			}
		}

		std::shared_ptr<T> get(std::shared_ptr<int> i, std::shared_ptr<int> j)
		{
			if (*i > *j)
			{
				*j = *j + 1;
				return Node<T>::getNext()->get(i, j);
			}
			if (*i < *j)
			{
				*j= *j -1;
				return Node<T>::getPrevious()->get(i, j);
			}
			return std::shared_ptr<T>();
		}

		bool put(std::shared_ptr<int> i, std::shared_ptr<int> j, std::shared_ptr<T> d)
		{
			bool b = false;

			if (*i > *j)
			{
				*j = *j + 1;
				if (!Node<T>::getNext())
				{
					b = true;
					std::shared_ptr<Node<T>> n(new InternalNode<T>(d, Node<T>::getPrevious()->getNext(), Node<T>::getPrevious()));
					Node<T>::getPrevious()->setNext(n);
					Node<T>::setPrevious(n);
				}
				else
				{
					b = Node<T>::getNext()->put(i, j, d);
				}
			}
			if (*i < *j)
			{
				*j = *j - 1;
				if (!Node<T>::getPrevious())
				{
					b = true;
					std::shared_ptr<Node<T>> n(new InternalNode<T>(d, Node<T>::getNext()->getPrevious(), Node<T>::getNext()));
					Node<T>::getNext()->setPrevious(n);
					Node<T>::setNext(n);
				}
				else
				{
					b = Node<T>::getPrevious()->put(i, j, d);
				}
			}

			return b;
		}
};

/// <summary>
/// Doubly linked list, utilizing smart pointers & templates
/// </summary>
template <typename T>
class LinkedList
{
	private:
		std::shared_ptr<Node<T>> head; //head pointer
		std::shared_ptr<Node<T>> tail; //tail pointer
		int count = 0; //last count of internal nodes; updated via operations
	public:
		LinkedList() //constructor
		{
			head = std::shared_ptr<Node<T>>(new ExternalNode<T>(std::shared_ptr<Node<T>>(new ExternalNode<T>()))); //initialize head pointer, create tail pointer
			tail = head->getNext(); //grab tail pointer
			tail->setPrevious(head);

		}
		LinkedList(std::shared_ptr<T> d) //constructor
		{
			head = std::shared_ptr<Node<T>>(new ExternalNode<T>(std::shared_ptr<Node<T>>(new ExternalNode<T>()))); //initialize head pointer, create tail pointer
			tail = head->getNext(); //grab tail pointer
			tail->setPrevious(head);
			put(d);
		}

		bool exists(int i)
		{
			bool b = false;
			std::shared_ptr<int> iPointer = std::shared_ptr<int>(new int(i));
			std::shared_ptr<int> j = std::shared_ptr<int>(new int(0));
			//switching logic; if the index is higher than half the count, we start from the back
			if (i > (count / 2))
			{
				*j = count;
				b = tail->exists(iPointer, j);
			}
			else
			{
				b = head->exists(iPointer, j);
			}
			return b;
		}
		
		int find(std::string s)
		{
			std::shared_ptr<int> j(new int(0));
			return head->find(s, j);
			return -1;
		}

		std::shared_ptr<T> get(int i)
		{
			std::shared_ptr<int> iPointer = std::shared_ptr<int>(new int(i));
			std::shared_ptr<int> j = std::shared_ptr<int>(new int(0));
			//switching logic; if the index is higher than half the count, we start from the back
			if (i > (count / 2))
			{
				*j = count+1;
				return tail->get(iPointer, j);
			}
			else
			{
				return head->get(iPointer, j);
			}


			return std::shared_ptr<T>();
		}

		std::shared_ptr<T> get(std::string s)
		{
			return get(find(s));
		}

		bool put(std::shared_ptr<T> d, int i)
		{
			bool b = false;

			std::shared_ptr<int> iPointer = std::shared_ptr<int>(new int(i));
			std::shared_ptr<int> j = std::shared_ptr<int>(new int(0));
			//switching logic; if the index is higher than half the count, we start from the back
			if (i > (count / 2))
			{
				*j = count;
				b = tail->put(iPointer, j, d);
			}
			else
			{
				b = head->put(iPointer, j, d);
			}
			if (b) count++;
			return b;
		}

		bool put(std::shared_ptr<T> d)
		{
			bool b = false;
			int i = count+1;
			b = put(d, i);
			return b;
		}

		int getCount()
		{
			return count;
		}

};