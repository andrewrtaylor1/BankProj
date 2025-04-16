#pragma once

#include "Exception.h"
#include <string>
#include <memory>

class ExLLOoB : public Exception
{
	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="s">throwing function</param>
		ExLLOoB(std::string s) : Exception(s) {}

		/// <summary>
		/// print the error to cout
		/// </summary>
		void printError()
		{
			std::cout << "Attempted to access out of bounds index in LinkedList, while executing function: " << throwingFunc << "\n";
		}
};

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
		//deletes the node, done by simply removing all references to it
		virtual bool drop(std::shared_ptr<int> i, std::shared_ptr<int> j) = 0;
		//updates the data at a specific index
		virtual bool update(std::shared_ptr<int> i, std::shared_ptr<int> j, std::shared_ptr<T> d) = 0;
		virtual int count(int i) = 0;
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

		/// <summary>
		/// Check if a specific node exists. Only internal nodes count
		/// </summary>
		/// <param name="i">desired index</param>
		/// <param name="j">current index</param>
		/// <returns>whether or not the node exists, bool</returns>
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

		/// <summary>
		/// find a string in a node
		/// </summary>
		/// <param name="s">string to be found</param>
		/// <param name="j">current index</param>
		/// <returns>specific index of string, int</returns>
		int find(std::string s, std::shared_ptr<int> j)
		{
			if (data->compare(s) == 0) //using compare function, must add compare function to search 
			{
				return *j; //dereference the pointer & return it
			}
			
			if (Node<T>::getNext())
			{
				*j = *j + 1; //add one to current index
				return Node<T>::getNext()->find(s, j); //get the next node's find function
			}
			else
			{
				return -1; //return negative one, error/doesn't exist
			}
		}

		/// <summary>
		/// grabs the data from the specific index
		/// </summary>
		/// <param name="i">desired index</param>
		/// <param name="j">current index</param>
		/// <returns>returns a pointer to the data</returns>
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

		/// <summary>
		/// put data at a specific index
		/// </summary>
		/// <param name="i">desired index</param>
		/// <param name="j">current index</param>
		/// <param name="d">data to be placed</param>
		/// <returns>was it successful? bool</returns>
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
				Node<T>::getPrevious()->setNext(n);
				Node<T>::getPrevious() = n;
			}

			return b;
		}

		/// <summary>
		/// finds the node to delete; if it's this one, use the dedicated private delete function
		/// </summary>
		/// <param name="i">desired index</param>
		/// <param name="j">current index</param>
		/// <returns>was it successful? bool</returns>
		bool drop(std::shared_ptr<int> i, std::shared_ptr<int> j)
		{
			bool b = false;
			if (*i > *j)
			{
				*j = *j + 1;
				b = Node<T>::getNext()->drop(i, j);
			}
			if (*i < *j)
			{
				*j = *j - 1;
				b = Node<T>::getPrevious()->drop(i, j);
			}
			if (*i == *j)
			{
				b = true;
				drop();
			}
			return b;
		}

		/// <summary>
		/// update data at a specific index
		/// </summary>
		/// <param name="i">desired index</param>
		/// <param name="j">current index</param>
		/// <param name="d">data to be placed</param>
		/// <returns>was it successful? bool</returns>
		bool update(std::shared_ptr<int> i, std::shared_ptr<int> j, std::shared_ptr<T> d)
		{
			bool b = false;
			if (*i > *j)
			{
				*j = *j + 1;
				b = Node<T>::getNext()->update(i, j, d);
			}
			if (*i < *j)
			{
				*j = *j - 1;
				b = Node<T>::getPrevious()->update(i, j, d);
			}
			if (*i == *j)
			{
				b = true;
				data = d;
			}

			return b;
		}

		/// <summary>
		/// simple function to count each node
		/// </summary>
		/// <param name="i">int of count before</param>
		/// <returns>current count, int</returns>
		int count(int i)
		{
			return Node<T>::getNext()->count(i + 1);
		}

	private:
		/// <summary>
		/// Just simply deletes the current node
		/// </summary>
		/// <returns>was it successful? bool</returns>
		void drop()
		{
			Node<T>::getPrevious()->setNext(Node<T>::getNext());
			Node<T>::getNext()->setPrevious(Node<T>::getPrevious());
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

		/// <summary>
		/// Check if a specific node exists. Only internal nodes count
		/// </summary>
		/// <param name="i">desired index</param>
		/// <param name="j">current index</param>
		/// <returns>whether or not the node exists, bool</returns>
		bool exists(std::shared_ptr<int> i, std::shared_ptr<int> j)
		{
			bool b = false;
			if (*i > *j && Node<T>::getNext())
			{
				return Node<T>::getNext()->exists(i, j);
			}
			if (*i < *j && Node<T>::getPrevious())
			{
				return Node<T>::getPrevious()->exists(i, j);
			}
			if (*i == *j)
			{
				if (!Node<T>::getNext())
				{
					return Node<T>::getPrevious()->exists(i, j);
				}
				if (!Node<T>::getPrevious())
				{
					return Node<T>::getNext()->exists(i, j);
				}
			}
			return b;
		}

		/// <summary>
		/// find a string in a node
		/// </summary>
		/// <param name="s">string to be found</param>
		/// <param name="j">current index</param>
		/// <returns>specific index of string, int</returns>
		int find(std::string s, std::shared_ptr<int> j)
		{
			//out of bounds detection
			if (*j < 0)
			{
				throw ExLLOoB("ExternalNode::find");
				return -1;
			}
			if (Node<T>::getNext())
			{
				return Node<T>::getNext()-> find(s, j);
			}
			else
			{
				return -1;
			}
		}

		/// <summary>
		/// grabs the data from the specific index
		/// </summary>
		/// <param name="i">desired index</param>
		/// <param name="j">current index</param>
		/// <returns>returns a pointer to the data</returns>
		std::shared_ptr<T> get(std::shared_ptr<int> i, std::shared_ptr<int> j)
		{
			//out of bounds detection
			if (*i < 0)
			{
				throw ExLLOoB("ExternalNode::get");
			}
			if (*i > *j && Node<T>::getNext())
			{
				return Node<T>::getNext()->get(i, j);
			}
			if (*i < *j && Node<T>::getPrevious())
			{
				return Node<T>::getPrevious()->get(i, j);
			}
			if (*i == *j)
			{
				if (!Node<T>::getNext())
				{
					return Node<T>::getPrevious()->get(i, j);
				}
				if (!Node<T>::getPrevious())
				{
					return Node<T>::getNext()->get(i, j);
				}
			}
			return std::shared_ptr<T>();
		}

		/// <summary>
		/// put data at a specific index
		/// </summary>
		/// <param name="i">desired index</param>
		/// <param name="j">current index</param>
		/// <param name="d">data to be placed</param>
		/// <returns>was it successful? bool</returns>
		bool put(std::shared_ptr<int> i, std::shared_ptr<int> j, std::shared_ptr<T> d)
		{
			bool b = false;
			//out of bounds detection
			if (*i < 0)
			{
				throw ExLLOoB("ExternalNode::put");
				return b;
			}
			if (*i > *j)
			{
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
			if (*i == *j)
			{
				if (!Node<T>::getNext())
				{
					b = true;
					std::shared_ptr<Node<T>> n(new InternalNode<T>(d, Node<T>::getPrevious()->getNext(), Node<T>::getPrevious()));
					Node<T>::getPrevious()->setNext(n);
					Node<T>::setPrevious(n);
				}
				if (!Node<T>::getPrevious())
				{
					b = true;
					std::shared_ptr<Node<T>> n(new InternalNode<T>(d, Node<T>::getNext(), Node<T>::getNext()->getPrevious()));
					Node<T>::getNext()->setPrevious(n);
					Node<T>::setNext(n);
				}
			}

			return b;
		}

		/// <summary>
		/// deletes data at node; checking is done outside of node. For an external node, this does nothing but traversal
		/// </summary>
		/// <returns>was it successful? bool </returns>
		bool drop(std::shared_ptr<int> i, std::shared_ptr<int> j)
		{
			bool b = false;
			//out of bounds detection
			if (*i < 0)
			{
				throw ExLLOoB("ExternalNode::drop");
				return b;
			}
			if (*i > *j && Node<T>::getNext())
			{
				return Node<T>::getNext()->drop(i, j);
			}
			if (*i < *j && Node<T>::getPrevious())
			{
				return Node<T>::getPrevious()->drop(i, j);
			}
			if (*i == *j)
			{
				if (!Node<T>::getNext())
				{
					return Node<T>::getPrevious()->drop(i, j);
				}
				if (!Node<T>::getPrevious())
				{
					return Node<T>::getNext()->drop(i, j);
				}
			}
			return b;
		}

		/// <summary>
		/// update data at a specific index
		/// </summary>
		/// <param name="i">desired index</param>
		/// <param name="j">current index</param>
		/// <param name="d">data to be placed</param>
		/// <returns>was it successful? bool</returns>
		bool update(std::shared_ptr<int> i, std::shared_ptr<int> j, std::shared_ptr<T> d)
		{
			bool b = false;
			//out of bounds detection
			if (*i < 0)
			{
				throw ExLLOoB("ExternalNode::update");
				return b;
			}
			if (*i > *j && Node<T>::getNext())
			{
				b = Node<T>::getNext()->update(i, j, d);
			}
			if (*i < *j && Node<T>::getPrevious())
			{
				b = Node<T>::getPrevious()->update(i, j, d);
			}
			if (*i == *j)
			{
				if (!Node<T>::getPrevious())
				{
					return Node<T>::getNext()->update(i, j, d);
				}
				if (!Node<T>::getPrevious())
				{
					return Node<T>::getNext()->update(i, j, d);
				}
			}

			return b;
		}

		/// <summary>
		/// simple function to count each node
		/// </summary>
		/// <param name="i">int of count before</param>
		/// <returns>current count, int</returns>
		int count(int i)
		{
			//if we have a next node (head node)
			if (Node<T>::getNext())
			{
				//start going through nodes
				return Node<T>::getNext()->count(i);
			}
			else
			{
				//if we are at the tail node, just return
				return i;
			}
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
			d.reset();
		}

		/// <summary>
		/// checks the existence of an internal node at specific index
		/// </summary>
		/// <param name="i">desired index</param>
		/// <returns>does it exist, bool</returns>
		bool exists(int i)
		{
			bool b = false;
			//we don't need to check for out of bounds because you Should be able to send any number to exists
			//make pointers to values
			std::shared_ptr<int> iPointer = std::shared_ptr<int>(new int(i));
			std::shared_ptr<int> j = std::shared_ptr<int>(new int(0));
			//switching logic; if the index is higher than half the count, we start from the back
			if (i > (count / 2) && i != 0)
			{
				*j = count-1;
				b = tail->exists(iPointer, j);
			}
			else
			{
				b = head->exists(iPointer, j);
			}
			//return no matter what; false if nothing happened
			iPointer.reset(); //clears the pointers
			j.reset();
			return b;
		}
		
		/// <summary>
		/// finds the index for a specific string
		/// </summary>
		/// <param name="s">string to find</param>
		/// <returns>index, -1 if not in list, int</returns>
		int find(std::string s)
		{
			std::shared_ptr<int> j(new int(0));
			return head->find(s, j);
			return -1;
		}

		/// <summary>
		/// gets pointer to data from specific index
		/// </summary>
		/// <param name="i">desired index</param>
		/// <returns>pointer to data</returns>
		std::shared_ptr<T> get(int i)
		{
			//try/catch for exceptions
			try
			{
				//out of bounds detection
				if (i < 0 || i >= count)
				{
					throw ExLLOoB("LinkedList::get");
				}
				std::shared_ptr<int> iPointer = std::shared_ptr<int>(new int(i));
				std::shared_ptr<int> j = std::shared_ptr<int>(new int(0));
				//switching logic; if the index is higher than half the count, we start from the back
				if (i > (count / 2) && i != 0)
				{
					*j = count-1;
					return tail->get(iPointer, j);
				}
				else
				{
					return head->get(iPointer, j);
				}
				iPointer.reset(); //clears the pointers
				j.reset();
			}
			catch (Exception& ex)
			{
				//print error, access by reference means it'll get the right printError
				ex.printError();
			}

			//return null if we didn't get anything
			return std::shared_ptr<T>();
		}

		/// <summary>
		/// get function using the string
		/// </summary>
		/// <param name="s">string to find</param>
		/// <returns>pointer to data, null if it can't be found</returns>
		std::shared_ptr<T> get(std::string s)
		{
			return get(find(s));
		}

		/// <summary>
		/// put at specific index
		/// </summary>
		/// <param name="d">pointer to data</param>
		/// <param name="i">desired index</param>
		/// <returns>was it successful, bool</returns>
		bool put(std::shared_ptr<T> d, int i)
		{
			bool b = false;
			try
			{
				//out of bounds detection; no upper bound for use at the end
				if (i < 0)
				{
					throw ExLLOoB("LinkedList::put");
					return b;
				}
				std::shared_ptr<int> iPointer = std::shared_ptr<int>(new int(i));
				std::shared_ptr<int> j = std::shared_ptr<int>(new int(0));
				//switching logic; if the index is higher than half the count, we start from the back
				if (i > (count / 2) && i != 0)
				{
					*j = count;
					b = tail->put(iPointer, j, d);
				}
				else
				{
					b = head->put(iPointer, j, d);
				}
				if (b) count++;
				iPointer.reset(); //clears the pointers
				j.reset();
			}
			catch (Exception& ex)
			{
				//print error, access by reference means it'll get the right printError
				ex.printError();
			}
			d.reset(); //drop the data pointer cause we don't need it anymore
			//return no matter what; false if nothing happened
			return b;
		}

		/// <summary>
		/// put, unindexed
		/// </summary>
		/// <param name="d">data to put, pointer</param>
		/// <returns>was successful, bool</returns>
		bool put(std::shared_ptr<T> d)
		{
			int i = count; //get count
			return put(d, i); //feed it to numbered put function
			
		}

		/// <summary>
		/// Drop (delete) an entry
		/// </summary>
		/// <param name="i">desired index</param>
		/// <returns>success, bool</returns>
		bool drop(int i)
		{
			bool b = false;
			//try/catch for exceptions
			try
			{
				//out of bounds detection
				if (i < 0 || i >= count)
				{
					throw ExLLOoB("LinkedList::drop");
					return b;
				}
				//just using exist code again; traversing through the list already proves whether or not the index exists
				std::shared_ptr<int> iPointer = std::shared_ptr<int>(new int(i));
				std::shared_ptr<int> j = std::shared_ptr<int>(new int(0));
				//switching logic; if the index is higher than half the count, we start from the back
				if (i > (count / 2) && i != 0)
				{
					*j = count-1;
					b = tail->drop(iPointer, j);
				}
				else
				{
					b = head->drop(iPointer, j);
				}
				//update the count
				updateCount();
				iPointer.reset(); //clears the pointers
				j.reset();
			}
			catch (Exception& ex)
			{
				//print error, access by reference means it'll get the right printError
				ex.printError();
			}
			//return no matter what; false if nothing happened
			return b;
		}

		/// <summary>
		/// Drop (delete) an entry, based on string
		/// </summary>
		/// <param name="i">desired string</param>
		/// <returns>success, bool</returns>
		bool drop(std::string s)
		{
			int i = find(s);
			return drop(i);
		}

		/// <summary>
		/// update data at a given index
		/// </summary>
		/// <param name="i">desired index</param>
		/// <param name="d">data to update</param>
		/// <returns>successful? bool</returns>
		bool update(int i, std::shared_ptr<T> d)
		{
			bool b = false;
			//try/catch for exceptions
			try
			{
				//out of bounds detection
				if (i < 0 || i >= count)
				{
					throw ExLLOoB("LinkedList::update");
					return b;
				}
				//just using exist code again; traversing through the list already proves whether or not the index exists
				std::shared_ptr<int> iPointer = std::shared_ptr<int>(new int(i));
				std::shared_ptr<int> j = std::shared_ptr<int>(new int(0));
				//switching logic; if the index is higher than half the count, we start from the back
				if (i > (count / 2) && i != 0)
				{
					*j = count-1;
					b = tail->update(iPointer, j, d);
				}
				else
				{
					b = head->update(iPointer, j, d);
				}
				iPointer.reset(); //clears the pointers
				j.reset(); 
			}
			catch (Exception& ex)
			{
				//print error, access by reference means it'll get the right printError
				ex.printError();
			}
			d.reset(); //drop the data pointer cause we don't need it anymore
			//return no matter what; false if nothing happened
			return b;
		}

		/// <summary>
		/// update data with a given string value
		/// </summary>
		/// <param name="s">desired string</param>
		/// <param name="d">data to update</param>
		/// <returns>successful? bool</returns>
		bool update(std::string s, std::shared_ptr<T> d)
		{
			int i = find(s);
			return update(i, d);
		}

		/// <summary>
		/// does a quick count of InternalNodes
		/// </summary>
		void updateCount()
		{
			count = head->count(0);
		}

		/// <summary>
		/// get the count
		/// </summary>
		/// <returns>count, int</returns>
		int getCount()
		{
			return count;
		}

};