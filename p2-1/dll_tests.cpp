#include "doublely_linked_list.h"
#include "gtest/gtest.h"


// Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>


struct ListItem {};

// Don't create spares! It will cause memory leaks
void make_items(ListItem* result[], unsigned n)
{
	while(n--)
	{
		result[n] = (ListItem*) malloc(sizeof(ListItem));
	}
}


TEST(InitializationTests, CreateDestroy)
{
	DLinkedList* list = create_dlinkedlist();
	destroyList(list);
}

TEST(Access, getHead_Empty)
{
	DLinkedList* list = create_dlinkedlist();
	EXPECT_EQ(NULL, getHead(list));
	destroyList(list);
}


TEST(Insert, Head_Single)
{
	// Create the list items for the test
	size_t num_items = 1;
	ListItem* m[num_items]; make_items(m, num_items);

	// Create the list and insert at the head
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);

	// Check that the behavior was correct
	EXPECT_EQ(m[0], getHead(list));
	EXPECT_EQ(m[0], getTail(list));
	EXPECT_EQ(m[0], getCurrent(list));

	// Delete the list
	destroyList(list);
}

TEST(Insert, Head_Multiple)
{
	// Create list items for test
	size_t num_items = 3;
	ListItem* m[num_items]; make_items(m, num_items);

	// Insert 3 items at the head (list is now [2, 1, 0])
	DLinkedList* list = create_dlinkedlist();
	for (int i = 0; i < 3; i++)
		insertHead(list, m[i]);

	// Make sure the current pointer wasn't updated more than once
	EXPECT_EQ(NULL, getCurrent(list));

	// Check forward links
	ASSERT_EQ(m[2], getHead(list));
	for (int i = 1; i >= 0; i--)
		ASSERT_EQ(m[i], getNext(list));
	ASSERT_EQ(NULL, getNext(list));

	// Check backward links
	ASSERT_EQ(m[0], getTail(list));
	for (int i = 1; i < 3; i++)
		ASSERT_EQ(m[i], getPrevious(list));
	ASSERT_EQ(NULL, getPrevious(list));

	// Delete the list
	destroyList(list);
}

TEST(Insert, Tail_Single)
{
	// Initialize list items for test 
	size_t num_items = 1;
	ListItem* m[num_items]; make_items(m, num_items);

	// Use the items in your list
	DLinkedList* list = create_dlinkedlist();
	insertTail(list, m[0]);

	// Check that the behavior was correct
	EXPECT_EQ(m[0], getHead(list));
	EXPECT_EQ(m[0], getTail(list));
	EXPECT_EQ(m[0], getCurrent(list));

	// Delete the list
	destroyList(list);
}

TEST(Insert, Tail_Multiple)
{
	// Create list items for test
	size_t num_items = 3;
	ListItem* m[num_items]; make_items(m, num_items);

	// Insert 3 items at the tail (list is now [0, 1, 2])
	DLinkedList* list = create_dlinkedlist();
	for (int i = 0; i < 3; i++)
		insertTail(list, m[i]);

	// Make sure current isn't updated more than once
	EXPECT_EQ(NULL, getCurrent(list));

	// Check backward links
	ASSERT_EQ(m[2], getTail(list));
	for (int i = 1; i >= 0; i--)
		ASSERT_EQ(m[i], getPrevious(list));
	ASSERT_EQ(NULL, getPrevious(list));

	// Check forward links
	ASSERT_EQ(m[0], getHead(list));
	for (int i = 1; i < 3; i++)
		ASSERT_EQ(m[i], getNext(list));
	ASSERT_EQ(NULL, getNext(list));

	// Delete the list
	destroyList(list);
}

TEST(Insert, Before_NULL)
{
	// Attempt to add an item to an empty list
	DLinkedList *list = create_dlinkedlist();
	EXPECT_EQ(0, insertBefore(list, NULL));

	// Delete the list
	destroyList(list);
}

TEST(Insert, Before_Head)
{
	// Create list items for test
	size_t num_items = 2;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert 1 item
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	ASSERT_EQ(m[0], getHead(list));
	ASSERT_EQ(m[0], getTail(list));

	// Invoke insertBefore and assert a success
	ASSERT_EQ(1, insertBefore(list, m[1]));
	EXPECT_EQ(2, getSize(list));

	// Make sure the current pointer is in the correct position
	EXPECT_EQ(m[0], getCurrent(list));

	// Check backward links
	ASSERT_EQ(m[0], getTail(list));
	EXPECT_EQ(m[1], getPrevious(list));
	EXPECT_EQ(NULL, getPrevious(list));

	// Check forward links
	ASSERT_EQ(m[1], getHead(list));
	EXPECT_EQ(m[0], getNext(list));
	EXPECT_EQ(NULL, getNext(list));

	// Delete the list
	destroyList(list);
}

TEST(Insert, Before_Multiple)
{
	// Create list items for test
	size_t num_items = 3;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert 2 items at the tail (list is now [0, 1])
	DLinkedList* list = create_dlinkedlist();
	insertTail(list, m[0]);
	insertTail(list, m[1]);
	ASSERT_EQ(m[0], getHead(list));
	ASSERT_EQ(m[1], getTail(list));

	// Insert the third item in between the two
	ASSERT_EQ(1, insertBefore(list, m[2]));
	EXPECT_EQ(3, getSize(list));

	// Make sure the current pointer hasn't moved
	EXPECT_EQ(m[1], getCurrent(list));

	// Check backward links
	ASSERT_EQ(m[1], getTail(list));
	EXPECT_EQ(m[2], getPrevious(list));
	EXPECT_EQ(m[0], getPrevious(list));
	EXPECT_EQ(NULL, getPrevious(list));

	// Check forward links
	ASSERT_EQ(m[0], getHead(list));
	EXPECT_EQ(m[2], getNext(list));
	EXPECT_EQ(m[1], getNext(list));
	EXPECT_EQ(NULL, getNext(list));

	// Delete the list
	destroyList(list);
}

TEST(Insert, After_NULL)
{
	// Attempt to add an item to an empty list
	DLinkedList *list = create_dlinkedlist();
	EXPECT_EQ(0, insertAfter(list, NULL));

	// Delete the list
	destroyList(list);
}

TEST(Insert, After_Tail)
{
	// Create list items for test
	size_t num_items = 2;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert 1 item
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	ASSERT_EQ(m[0], getHead(list));
	ASSERT_EQ(m[0], getTail(list));

	// Invoke insertAfter and assert a success
	ASSERT_EQ(1, insertAfter(list, m[1]));
	EXPECT_EQ(2, getSize(list));
	
	// Make sure the current pointer is in the correct position
	EXPECT_EQ(m[0], getCurrent(list));
	
	// Check backward links
	ASSERT_EQ(m[1], getTail(list));
	EXPECT_EQ(m[0], getPrevious(list));
	EXPECT_EQ(NULL, getPrevious(list));

	// Check forward links
	ASSERT_EQ(m[0], getHead(list));
	EXPECT_EQ(m[1], getNext(list));
	EXPECT_EQ(NULL, getNext(list));

	// Delete the list
	destroyList(list);
}

TEST(Insert, After_Multiple)
{
	// Create list items for test
	size_t num_items = 3;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert 2 items at the head (list is now [0, 1])
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	insertHead(list, m[1]);
	ASSERT_EQ(m[0], getTail(list));
	ASSERT_EQ(m[1], getHead(list));

	// Insert the third item in between the two
	ASSERT_EQ(1, insertAfter(list, m[2]));
	EXPECT_EQ(3, getSize(list));

	// Make sure the current pointer hasn't moved
	EXPECT_EQ(m[1], getCurrent(list));

	// Check backward links
	ASSERT_EQ(m[0], getTail(list));
	EXPECT_EQ(m[2], getPrevious(list));
	EXPECT_EQ(m[1], getPrevious(list));
	EXPECT_EQ(NULL, getPrevious(list));

	// Check forward links
	ASSERT_EQ(m[1], getHead(list));
	EXPECT_EQ(m[2], getNext(list));
	EXPECT_EQ(m[0], getNext(list));
	EXPECT_EQ(NULL, getNext(list));

	// Delete the list
	destroyList(list);
}

TEST(Delete, Backward_NULL)
{
	// Make an empty list
	DLinkedList *list = create_dlinkedlist();

	// Attempt to delete a NULL current pointer
	EXPECT_EQ(NULL, deleteBackward(list));
}

TEST(Delete, Backward_Single)
{
	// Create list items for test
	size_t num_items = 1;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Create the list
	DLinkedList *list = create_dlinkedlist();
	insertHead(list, m[0]);
	ASSERT_EQ(m[0], getHead(list));

	// Delete the item
	EXPECT_EQ(NULL, deleteBackward(list));
	EXPECT_EQ(0, getSize(list));

	// Make sure all of the pointers have changed accordingly
	EXPECT_EQ(NULL, getHead(list));
	EXPECT_EQ(NULL, getTail(list));
	EXPECT_EQ(NULL, getCurrent(list));
}

TEST(Delete, Backward_Two)
{
	// Create list items for test
	size_t num_items = 2;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert both items
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	insertTail(list, m[1]);
	ASSERT_EQ(m[0], getHead(list));
	EXPECT_EQ(m[1], getTail(list));

	// Invoke deleteBackward and assert m[0] to be the return
	ASSERT_EQ(m[0], deleteBackward(list));
	EXPECT_EQ(1, getSize(list));
	
	// Make sure the pointers are in the correct positions
	EXPECT_EQ(m[0], getCurrent(list));
	EXPECT_EQ(m[0], getHead(list));
	EXPECT_EQ(NULL, getNext(list));
	EXPECT_EQ(m[0], getTail(list));
	EXPECT_EQ(NULL, getPrevious(list));
	
	// Delete the list
	destroyList(list);
}

TEST(Delete, Backward_Two_NULL)
{
	// Create list items for test
	size_t num_items = 2;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert both items
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	insertTail(list, m[1]);
	ASSERT_EQ(m[1], getTail(list));
	ASSERT_EQ(m[0], getHead(list));

	// Invoke deleteBackward and assert NULL to be the return
	ASSERT_EQ(NULL, deleteBackward(list));
	EXPECT_EQ(1, getSize(list));
	
	// Make sure the pointers are in the correct positions
	EXPECT_EQ(NULL, getCurrent(list));
	EXPECT_EQ(m[1], getHead(list));
	EXPECT_EQ(NULL, getNext(list));
	EXPECT_EQ(m[1], getTail(list));
	EXPECT_EQ(NULL, getPrevious(list));
	
	// Delete the list
	destroyList(list);
}

TEST(Delete, Backward_Multiple)
{
	// Create list items for test
	size_t num_items = 3;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert both items
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	insertTail(list, m[1]);
	ASSERT_EQ(m[1], getTail(list));
	ASSERT_EQ(m[0], getHead(list));
	insertAfter(list, m[2]);
	ASSERT_EQ(m[2], getNext(list));

	// Invoke deleteBackward and assert m[0] to be the return
	ASSERT_EQ(m[0], deleteBackward(list));
	EXPECT_EQ(2, getSize(list));
	
	// Make sure the pointers are in the correct positions
	EXPECT_EQ(m[0], getCurrent(list));
	EXPECT_EQ(m[0], getHead(list));
	EXPECT_EQ(m[1], getNext(list));
	EXPECT_EQ(m[1], getTail(list));
	EXPECT_EQ(m[0], getPrevious(list));
	
	// Delete the list
	destroyList(list);
}

TEST(Delete, Forward_NULL)
{
	// Make an empty list
	DLinkedList *list = create_dlinkedlist();

	// Attempt to delete a NULL current pointer
	EXPECT_EQ(NULL, deleteForward(list));
}

TEST(Delete, Forward_Single)
{
	// Create list items for test
	size_t num_items = 1;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Create the list
	DLinkedList *list = create_dlinkedlist();
	insertHead(list, m[0]);
	ASSERT_EQ(m[0], getHead(list));

	// Delete the item
	EXPECT_EQ(NULL, deleteForward(list));
	EXPECT_EQ(0, getSize(list));

	// Make sure all of the pointers have changed accordingly
	EXPECT_EQ(NULL, getHead(list));
	EXPECT_EQ(NULL, getTail(list));
	EXPECT_EQ(NULL, getCurrent(list));
}

TEST(Delete, Forward_Two)
{
	// Create list items for test
	size_t num_items = 2;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert both items
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	insertTail(list, m[1]);
	ASSERT_EQ(m[1], getTail(list));
	ASSERT_EQ(m[0], getHead(list));

	// Invoke deleteForward and assert m[1] to be the return
	ASSERT_EQ(m[1], deleteForward(list));
	EXPECT_EQ(1, getSize(list));
	
	// Make sure the pointers are in the correct positions
	EXPECT_EQ(m[1], getCurrent(list));
	EXPECT_EQ(m[1], getHead(list));
	EXPECT_EQ(NULL, getNext(list));
	EXPECT_EQ(m[1], getTail(list));
	EXPECT_EQ(NULL, getPrevious(list));
	
	// Delete the list
	destroyList(list);
}

TEST(Delete, Forward_Two_NULL)
{
	// Create list items for test
	size_t num_items = 2;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert both items
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	insertTail(list, m[1]);
	ASSERT_EQ(m[0], getHead(list));
	ASSERT_EQ(m[1], getTail(list));

	// Invoke deleteForward and assert NULL to be the return
	ASSERT_EQ(NULL, deleteForward(list));
	EXPECT_EQ(1, getSize(list));
	
	// Make sure the pointers are in the correct positions
	EXPECT_EQ(NULL, getCurrent(list));
	EXPECT_EQ(m[0], getHead(list));
	EXPECT_EQ(NULL, getNext(list));
	EXPECT_EQ(m[0], getTail(list));
	EXPECT_EQ(NULL, getPrevious(list));
	
	// Delete the list
	destroyList(list);
}

TEST(Delete, Forward_Multiple)
{
	// Create list items for test
	size_t num_items = 3;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert both items
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	insertTail(list, m[1]);
	ASSERT_EQ(m[1], getTail(list));
	ASSERT_EQ(m[0], getHead(list));
	insertAfter(list, m[2]);
	ASSERT_EQ(m[2], getNext(list));

	// Invoke deleteForward and assert m[1] to be the return
	ASSERT_EQ(m[1], deleteForward(list));
	EXPECT_EQ(2, getSize(list));
	
	// Make sure the pointers are in the correct positions
	EXPECT_EQ(m[1], getCurrent(list));
	EXPECT_EQ(m[0], getHead(list));
	EXPECT_EQ(m[1], getNext(list));
	EXPECT_EQ(m[1], getTail(list));
	EXPECT_EQ(m[0], getPrevious(list));
	
	// Delete the list
	destroyList(list);
}

TEST(Remove, Backward_NULL)
{
	// Make an empty list
	DLinkedList *list = create_dlinkedlist();

	// Attempt to remove a NULL current pointer
	EXPECT_EQ(NULL, removeBackward(list));
}

TEST(Remove, Backward_Single)
{
	// Create list items for test
	size_t num_items = 1;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Create the list
	DLinkedList *list = create_dlinkedlist();
	insertHead(list, m[0]);
	ASSERT_EQ(m[0], getHead(list));

	// Remove the item
	EXPECT_EQ(m[0], removeBackward(list));
	EXPECT_EQ(0, getSize(list));

	// Make sure all of the pointers have changed accordingly
	EXPECT_EQ(NULL, getHead(list));
	EXPECT_EQ(NULL, getTail(list));
	EXPECT_EQ(NULL, getCurrent(list));
}

TEST(Remove, Backward_Two)
{
	// Create list items for test
	size_t num_items = 2;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert both items
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	insertTail(list, m[1]);
	ASSERT_EQ(m[0], getHead(list));
	EXPECT_EQ(m[1], getTail(list));

	// Invoke removeBackward and assert m[1] to be the return
	ASSERT_EQ(m[1], removeBackward(list));
	EXPECT_EQ(1, getSize(list));
	
	// Make sure the pointers are in the correct positions
	EXPECT_EQ(m[0], getCurrent(list));
	EXPECT_EQ(m[0], getHead(list));
	EXPECT_EQ(NULL, getNext(list));
	EXPECT_EQ(m[0], getTail(list));
	EXPECT_EQ(NULL, getPrevious(list));
	
	// Delete the list
	destroyList(list);
}

TEST(Remove, Backward_Two_NULL)
{
	// Create list items for test
	size_t num_items = 2;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert both items
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	insertTail(list, m[1]);
	ASSERT_EQ(m[1], getTail(list));
	ASSERT_EQ(m[0], getHead(list));

	// Invoke removeBackward and assert m[0] to be the return
	ASSERT_EQ(m[0], removeBackward(list));
	EXPECT_EQ(1, getSize(list));
	
	// Make sure the pointers are in the correct positions
	EXPECT_EQ(NULL, getCurrent(list));
	EXPECT_EQ(m[1], getHead(list));
	EXPECT_EQ(NULL, getNext(list));
	EXPECT_EQ(m[1], getTail(list));
	EXPECT_EQ(NULL, getPrevious(list));
	
	// Delete the list
	destroyList(list);
}

TEST(Remove, Backward_Multiple)
{
	// Create list items for test
	size_t num_items = 3;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert both items
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	insertTail(list, m[1]);
	ASSERT_EQ(m[1], getTail(list));
	ASSERT_EQ(m[0], getHead(list));
	insertAfter(list, m[2]);
	ASSERT_EQ(m[2], getNext(list));

	// Invoke removeBackward and assert m[2] to be the return
	ASSERT_EQ(m[2], removeBackward(list));
	EXPECT_EQ(2, getSize(list));
	
	// Make sure the pointers are in the correct positions
	EXPECT_EQ(m[0], getCurrent(list));
	EXPECT_EQ(m[0], getHead(list));
	EXPECT_EQ(m[1], getNext(list));
	EXPECT_EQ(m[1], getTail(list));
	EXPECT_EQ(m[0], getPrevious(list));
	
	// Delete the list
	destroyList(list);
}

TEST(Remove, Forward_NULL)
{
	// Make an empty list
	DLinkedList *list = create_dlinkedlist();

	// Attempt to remove a NULL current pointer
	EXPECT_EQ(NULL, removeForward(list));
}

TEST(Remove, Forward_Single)
{
	// Create list items for test
	size_t num_items = 1;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Create the list
	DLinkedList *list = create_dlinkedlist();
	insertHead(list, m[0]);
	ASSERT_EQ(m[0], getHead(list));

	// Remove the item
	EXPECT_EQ(m[0], removeForward(list));
	EXPECT_EQ(0, getSize(list));

	// Make sure all of the pointers have changed accordingly
	EXPECT_EQ(NULL, getHead(list));
	EXPECT_EQ(NULL, getTail(list));
	EXPECT_EQ(NULL, getCurrent(list));
}

TEST(Remove, Forward_Two)
{
	// Create list items for test
	size_t num_items = 2;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert both items
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	insertTail(list, m[1]);
	ASSERT_EQ(m[1], getTail(list));
	ASSERT_EQ(m[0], getHead(list));

	// Invoke removeForward and assert m[0] to be the return
	ASSERT_EQ(m[0], removeForward(list));
	EXPECT_EQ(1, getSize(list));
	
	// Make sure the pointers are in the correct positions
	EXPECT_EQ(m[1], getCurrent(list));
	EXPECT_EQ(m[1], getHead(list));
	EXPECT_EQ(NULL, getNext(list));
	EXPECT_EQ(m[1], getTail(list));
	EXPECT_EQ(NULL, getPrevious(list));
	
	// Delete the list
	destroyList(list);
}

TEST(Remove, Forward_Two_NULL)
{
	// Create list items for test
	size_t num_items = 2;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert both items
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	insertTail(list, m[1]);
	ASSERT_EQ(m[0], getHead(list));
	ASSERT_EQ(m[1], getTail(list));

	// Invoke removeForward and assert m[1] to be the return
	ASSERT_EQ(m[1], removeForward(list));
	EXPECT_EQ(1, getSize(list));
	
	// Make sure the pointers are in the correct positions
	EXPECT_EQ(NULL, getCurrent(list));
	EXPECT_EQ(m[0], getHead(list));
	EXPECT_EQ(NULL, getNext(list));
	EXPECT_EQ(m[0], getTail(list));
	EXPECT_EQ(NULL, getPrevious(list));
	
	// Delete the list
	destroyList(list);
}

TEST(Remove, Forward_Multiple)
{
	// Create list items for test
	size_t num_items = 3;
	ListItem* m[num_items];
	make_items(m, num_items);

	// Insert both items
	DLinkedList* list = create_dlinkedlist();
	insertHead(list, m[0]);
	insertTail(list, m[1]);
	ASSERT_EQ(m[1], getTail(list));
	ASSERT_EQ(m[0], getHead(list));
	insertAfter(list, m[2]);
	ASSERT_EQ(m[2], getNext(list));

	// Invoke removeForward and assert m[2] to be the return
	ASSERT_EQ(m[2], removeForward(list));
	EXPECT_EQ(2, getSize(list));
	
	// Make sure the pointers are in the correct positions
	EXPECT_EQ(m[1], getCurrent(list));
	EXPECT_EQ(m[0], getHead(list));
	EXPECT_EQ(m[1], getNext(list));
	EXPECT_EQ(m[1], getTail(list));
	EXPECT_EQ(m[0], getPrevious(list));
	
	// Delete the list
	destroyList(list);
}
