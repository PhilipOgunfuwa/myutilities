#include <iostream>
#include <cassert>
#include <sstream>
#include "linked_list.hpp"

// Dummy assertion macro to ensure tests run even in release mode
#define TEST_ASSERT(expr) \
    do { \
        if (!(expr)) { \
            std::cerr << "Assertion failed: " << #expr << " at line " << __LINE__ << std::endl; \
            std::abort(); \
        } \
    } while (0)

void test_initialization() {
    LinkedList<int> list;
    TEST_ASSERT(list.getHead() == nullptr);
    TEST_ASSERT(list.getTail() == nullptr);
    TEST_ASSERT(list.getLength() == 0);
}

void test_append() {
    LinkedList<int> list;
    
    // Append to empty list
    list.append(10);
    TEST_ASSERT(list.getLength() == 1);
    TEST_ASSERT(list.getHead() != nullptr);
    TEST_ASSERT(list.getTail() != nullptr);
    TEST_ASSERT(list.getHead() == list.getTail());
    TEST_ASSERT(list.getHead()->data == 10);
    TEST_ASSERT(list.getHead()->next == nullptr);
    TEST_ASSERT(list.getHead()->prev == nullptr);

    // Append second element
    list.append(20);
    TEST_ASSERT(list.getLength() == 2);
    TEST_ASSERT(list.getHead()->data == 10);
    TEST_ASSERT(list.getTail()->data == 20);
    TEST_ASSERT(list.getHead()->next == list.getTail());
    TEST_ASSERT(list.getTail()->prev == list.getHead());
    TEST_ASSERT(list.getTail()->next == nullptr);

    // Append third element
    list.append(30);
    TEST_ASSERT(list.getLength() == 3);
    TEST_ASSERT(list.getTail()->data == 30);
    TEST_ASSERT(list.getHead()->next->next == list.getTail());
    TEST_ASSERT(list.getTail()->prev->data == 20);
}

void test_prepend() {
    LinkedList<int> list;

    // Prepend to empty list
    list.prepend(10);
    TEST_ASSERT(list.getLength() == 1);
    TEST_ASSERT(list.getHead() == list.getTail());
    TEST_ASSERT(list.getHead()->data == 10);

    // Prepend second element
    list.prepend(5);
    TEST_ASSERT(list.getLength() == 2);
    TEST_ASSERT(list.getHead()->data == 5);
    TEST_ASSERT(list.getTail()->data == 10);
    TEST_ASSERT(list.getHead()->next == list.getTail());
    TEST_ASSERT(list.getTail()->prev == list.getHead());

    // Prepend third element
    list.prepend(1);
    TEST_ASSERT(list.getLength() == 3);
    TEST_ASSERT(list.getHead()->data == 1);
    TEST_ASSERT(list.getHead()->next->data == 5);
}

void test_copy_constructor_and_assignment() {
    LinkedList<int> original;
    original.append(1);
    original.append(2);
    original.append(3);

    // Test Copy Constructor
    LinkedList<int> copy1(original);
    TEST_ASSERT(copy1.getLength() == 3);
    TEST_ASSERT(copy1.getHead() != original.getHead()); // Ensure deep copy
    TEST_ASSERT(copy1.getHead()->data == 1);
    TEST_ASSERT(copy1.getTail()->data == 3);
    TEST_ASSERT(copy1.getHead()->next->data == 2);

    // Test Copy Assignment
    LinkedList<int> copy2;
    copy2.append(99); // Populate with existing elements to test clearList inside assignment
    copy2 = original;
    TEST_ASSERT(copy2.getLength() == 3);
    TEST_ASSERT(copy2.getHead() != original.getHead());
    TEST_ASSERT(copy2.getHead()->data == 1);
    TEST_ASSERT(copy2.getTail()->data == 3);

    // Test Self Assignment
    copy2 = copy2;
    TEST_ASSERT(copy2.getLength() == 3);
    TEST_ASSERT(copy2.getHead()->data == 1);
}

void test_insert_after() {
    LinkedList<int> list;

    // Target missing from empty list
    list.insertAfter(10, 20);
    TEST_ASSERT(list.getLength() == 0);

    list.append(10);

    // Insert after lone node (Tail)
    list.insertAfter(10, 20);
    TEST_ASSERT(list.getLength() == 2);
    TEST_ASSERT(list.getHead()->data == 10);
    TEST_ASSERT(list.getTail()->data == 20);
    TEST_ASSERT(list.getHead()->next == list.getTail());
    TEST_ASSERT(list.getTail()->prev == list.getHead());

    // Insert after internal node
    list.append(30); // List is now: 10 -> 20 -> 30
    list.insertAfter(20, 25); // Expected: 10 -> 20 -> 25 -> 30
    TEST_ASSERT(list.getLength() == 4);
    
    Node<int>* node20 = list.getHead()->next;
    Node<int>* node25 = node20->next;
    Node<int>* node30 = node25->next;

    TEST_ASSERT(node25->data == 25);
    TEST_ASSERT(node25->prev == node20);
    TEST_ASSERT(node25->next == node30);
    TEST_ASSERT(node30->prev == node25);

    // Target not found in non-empty list
    list.insertAfter(99, 100);
    TEST_ASSERT(list.getLength() == 4);
}

void test_remove() {
    LinkedList<int> list;

    // Remove from empty list
    list.remove(10);
    TEST_ASSERT(list.getLength() == 0);

    // Remove lone node
    list.append(10);
    list.remove(10);
    TEST_ASSERT(list.getLength() == 0);
    TEST_ASSERT(list.getHead() == nullptr);
    TEST_ASSERT(list.getTail() == nullptr);

    // Remove head node from multiple items
    list.append(10);
    list.append(20);
    list.append(30);
    list.remove(10); // Expected: 20 -> 30
    TEST_ASSERT(list.getLength() == 2);
    TEST_ASSERT(list.getHead()->data == 20);
    TEST_ASSERT(list.getHead()->prev == nullptr);

    // Remove tail node from multiple items
    list.remove(30); // Expected: 20
    TEST_ASSERT(list.getLength() == 1);
    TEST_ASSERT(list.getTail()->data == 20);
    TEST_ASSERT(list.getTail()->next == nullptr);

    // Remove internal node
    list.prepend(10);
    list.append(30); // List is: 10 -> 20 -> 30
    list.remove(20); // Expected: 10 -> 30
    TEST_ASSERT(list.getLength() == 2);
    TEST_ASSERT(list.getHead()->next == list.getTail());
    TEST_ASSERT(list.getTail()->prev == list.getHead());

    // Target not found
    list.remove(99);
    TEST_ASSERT(list.getLength() == 2);
}

void test_print_list() {
    LinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);

    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::ostringstream strCout;
    std::cout.rdbuf(strCout.rdbuf());

    list.printList();

    std::cout.rdbuf(oldCoutStreamBuf);

    // Checks current implementation output format
    TEST_ASSERT(strCout.str() == "1 -> 2 -> 3");
}

int main() {
    test_initialization();
    test_append();
    test_prepend();
    test_copy_constructor_and_assignment();
    test_insert_after();
    test_remove();
    test_print_list();

    std::cout << "All clear! All active test suites passed successfully." << std::endl;
    return 0;
}