#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>


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

    Node<T>* getHead() const { return head; }
    Node<T>* getTail() const { return tail; }
    std::size_t getLength() const { return length; }

    void append(const T& data) { appendNode(createNode(data)); }
    void prepend(const T& data) { prependNode(createNode(data)); }
    void remove(const T&);
    void insertAfter(const T&, const T&);
    void printList() const;


private:
    void deepCopy(const LinkedList<T>&);
    void clearList();
    Node<T>* createNode(const T&, Node<T>* = nullptr, Node<T>* = nullptr);
    void appendNode(Node<T>*);
    void prependNode(Node<T>*);
    void removeNode(Node<T>*);
    void insertAfterNode(Node<T>*, Node<T>*);



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
    clearList();
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
    // Don't add node if it is null
    if (node == nullptr) return;

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
    // Don't add node if it is null
    if (node == nullptr) return;

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

template <typename T>
void LinkedList<T>::remove(const T& target_data)
{
    Node<T>* current_node { getHead() };

    // Search for first node with target data
    while (current_node != nullptr && current_node -> data != target_data)
    {
        current_node = current_node -> next;
    }

    // If current_node is null then can't remove it
    if (current_node == nullptr) return;

    removeNode(current_node);

}

template <typename T>
void LinkedList<T>::removeNode(Node<T>* target_node)
{
    // Removing a lone node
    if (target_node == head && target_node == tail)
    {
        head = nullptr;
        tail = nullptr;
        delete target_node;
    }

    // Removing head
    else if (target_node == head)
    {
        head = target_node -> next;
        head -> prev = nullptr;
        delete target_node;
    }

    // Removing tail
    else if (target_node == tail)
    {
        tail = target_node -> prev;
        tail -> next = nullptr;
        delete target_node;
    }

    // Removing internal node

    else 
    {
        target_node -> prev -> next = target_node -> next;
        target_node -> next -> prev = target_node -> prev;
        delete target_node;
    }

    --length;


}

template <typename T>
void LinkedList<T>::insertAfter(const T& target_data, const T& new_data)
{
    Node<T>* current_node { getHead() };

    // Search for first node with target data
    while (current_node != nullptr && current_node -> data != target_data)
    {
        current_node = current_node -> next;
    }

    if (current_node == nullptr) return;

    insertAfterNode(current_node, createNode(new_data));

}

template <typename T>
void LinkedList<T>::insertAfterNode(Node<T>* target_node, Node<T>* new_node)
{

    // Avoid inserting after null node (OR INSERTING A NULL NODE)
    if (target_node == nullptr || new_node == nullptr) 
    {
    }

    // Inserting after lone node
    if (target_node == head && target_node == tail)
    {
        target_node -> next = new_node;
        new_node -> prev = target_node;
        tail = new_node;
    }


    // Inserting after tail
    else if (target_node == tail)
    {
        appendNode(new_node);
        return; // Avoid double incrememnt
    }

    // Inserting after (or at) head
    else
    {
        new_node -> next = target_node -> next;
        new_node -> prev = target_node;

        target_node -> next -> prev = new_node;
        target_node -> next = new_node;
    }

    ++length;


}




/* Function makes a deep copy of another linked list
* @param ll other linked list that will have its contents copied from
*/
template <typename T>
void LinkedList<T>::deepCopy(const LinkedList<T>& ll)
{
    Node<T>* current_node { ll.getHead() };

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
    Node<T>* current_node { getHead() };
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

/* Function that prints linked list */
template <typename T>
void LinkedList<T>::printList() const
{
    Node<T>* current_node { head };
    bool first_go { true };
    const char* symbol { "" };

    while (current_node != nullptr)
    {
        std::cout << symbol << current_node -> data;

        if (first_go) symbol = " -> ";

        current_node = current_node -> next;
    }



}



#endif