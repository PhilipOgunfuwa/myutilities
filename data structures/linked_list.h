#ifndef LINKED_LIST_H
#define LINKED_LIST_H

template <typename T>
struct Node
{
    T data {};
    Node<T>* next {};
    Node<T>* prev {};
};

// A doubly linked list
template <typename T>
class LinkedList 
{
private:
    Node<T>* head { nullptr };
    Node<T>* tail { nullptr };
    std::size_t length { 0 };

public:
    // No move semantics supported (for now)
    LinkedList(const LinkedList<T>&&) = delete;
    LinkedList<T>& operator=(const LinkedList<T>&&) = delete;

    // Support Default and Copy initialization, and copy assignment
    LinkedList() = default;
    LinkedList(const LinkedList<T>&);
    LinkedList<T>& operator=(const LinkedList<T>&);
    ~LinkedList();

    Node<T>* getHead() { return head; }
    Node<T>* getTail() { return tail; }
    std::size_t getLength() { return length; }

    void append(const T& data) { appendNode(createNode(data)); }
    void prepend(const T& data) { prependNode(createNode(data)); };


private:
    void deepCopy(const LinkedList<T>&);
    void clearList();
    Node<T>* createNode(const T&, Node<T>* = nullptr, Node<T>* = nullptr);
    void appendNode(Node<T>* node);
    void prependNode(Node<T>* node);



};


template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& ll)
{
    deepCopy(ll);
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& ll)
{
    // Avoid self copying
    if (this == &ll)
        return *this;
    
    clearList();
    deepCopy(ll);

    return *this;

}

template <typename T>
LinkedList<T>::~LinkedList()
{
    Node<T>* current_node { head };
    Node<T>* next_node { nullptr };
    std::size_t nodes_seen { 0 };

    // Free all nodes
    while (current_node != nullptr && nodes_seen < length)
    {
        next_node = current_node -> next;
        ++nodes_seen;
        delete current_node;
        current_node = next_node;
    }

}

/* Function creates a node struct
* @param data nodes new data
* @param next optional pointer to next node
* @param prev optional pointer to previous node
* @return pointer to dynamically allocated Node struct
*/
template <typename T>
Node<T>* LinkedList<T>::createNode(const T& data, Node<T>* next, Node<T>* prev)
{
    return new Node<T>{ data, next, prev };
}

/* Function appends a node object to linked list
 * @param node is the node to be appended to linked list
*/
template <typename T>
void LinkedList<T>::appendNode(Node<T>* node)
{
    // Don't add node if it is null, head or tail
    if (node == nullptr || node == head || node == tail)
        return;

    if (head == nullptr)
    {
        head = node;
        tail = node;
    }

    else
    {
        tail -> next = node;
        node -> prev = tail;
        tail = node;
    }
    
    ++length;
}

/* Function appends a node object to linked list
 * @param node is the node to be prepended to linked list
*/
template <typename T>
void LinkedList<T>::prependNode(Node<T>* node)
{
    // Don't add node if it is null, head or tail
    if (node == nullptr || node == head || node == tail)
        return;

    if (head == nullptr)
    {
        head = node;
        tail = node;
    }

    else
    {
        head -> prev = node;
        node -> next = head;
        head = node;
    }
    
    ++length;
}



/* Function makes a deep copy of another linked list
* @param ll other linked list that will have its contents copied from
*/
template <typename T>
void LinkedList<T>::deepCopy(const LinkedList<T>& ll)
{
    Node<T>* current_node { ll.head };

    // Take current nodes data and then create new node
    // while appending it to linked list
    while (current_node != nullptr)
    {
        append(current_node -> data);
        current_node = current_node -> next;
    }
}

/* Function clears list of any nodes */
template <typename T>
void LinkedList<T>::clearList()
{
    length = 0;
    Node<T>* current_node { head };
    Node<T>* next_node { nullptr };

    while (current_node != nullptr)
    {
        next_node = current_node -> next;
        delete current_node;
        current_node = next_node;
    }

    head = nullptr;
    tail = nullptr;


}



#endif