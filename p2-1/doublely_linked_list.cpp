// A C implementation of a doublely-linked list
// Isaac Webb				06NOV17

#include <stdlib.h>
#include <stdio.h>
#include "doublely_linked_list.h"

DLinkedList* create_dlinkedlist(void) {
	// Create space for the new linked list
	DLinkedList* newList = (DLinkedList *) malloc(sizeof(DLinkedList));

	// Initialize all parameters to 0/NULL
	newList->head = NULL;
	newList->tail = NULL;
	newList->current = NULL;
	newList->size = 0;

	// Return the new list
	return newList;
}

LLNode* create_llnode(void* data) {
	// Create a pointer to the new node
	LLNode *node = (LLNode *) malloc(sizeof(LLNode));

	// Initialize the values to NULL and the input
	node->next = NULL;
	node->previous = NULL;
	node->data = data;

	// Return the new node
	return node;
}

void insertHead(DLinkedList* dLinkedList, void* data) {
	// Create a new node
	LLNode* newNode = create_llnode(data);

	// If there were no nodes to begin with, correct the pointers
	dLinkedList->size++;
	if (dLinkedList->head == NULL){
		dLinkedList->head = newNode;
		dLinkedList->tail = newNode;
	} else {
		newNode->next = dLinkedList->head;
		(dLinkedList->head)->previous = newNode;
		dLinkedList->head = newNode;
	}
}

void insertTail(DLinkedList* dLinkedList, void* data) {
	// Create a new node
	LLNode *node = create_llnode(data);

	// If there were no nodes to begin with, correct the pointers
	dLinkedList->size++;
	if (dLinkedList->tail == NULL) {
		dLinkedList->head = node;
		dLinkedList->tail = node;
	} else {
		node->previous = dLinkedList->tail;
		(dLinkedList->tail)->next = node;
		dLinkedList->tail = node;
	}

}

int insertAfter(DLinkedList* dLinkedList, void* newData) {
	// Only update if the current pointer is not NULL
	if (dLinkedList->current != NULL) {
		// Create the new node
		LLNode *node = create_llnode(newData);

		// Insert the new node into the list
		dLinkedList->size++;
		node->next = (dLinkedList->current)->next;
		node->previous = dLinkedList->current;
		if (node->next != NULL) (node->next)->previous = node;
		if (node->previous != NULL) (node->previous)->next = node;

		// Check to make sure we set the tail pointer if we are the new tail
		if (node->previous == dLinkedList->tail) dLinkedList->tail = node;

		// Return the success code
		return 1;
	}

	return 0;
}

int insertBefore(DLinkedList* dLinkedList, void* newData) {
	// Only update if the current pointer is not NULL
	if (dLinkedList->current != NULL) {
		// Create the new node
		LLNode *node = create_llnode(newData);

		// Insert the new node into the list
		dLinkedList->size++;
		node->next = dLinkedList->current;
		node->previous = (dLinkedList->current)->previous;
		if (node->next != NULL) (node->next)->previous = node;
		if (node->previous != NULL) (node->previous)->next = node;

		// Check to make sure we set the head pointer if we are the new head
		if (node->next == dLinkedList->head) dLinkedList->head = node;

		// Return the success code
		return 1;
	}

	return 0;
}

void* deleteBackward(DLinkedList* dLinkedList) {
	// Remove the node and go backward
	free(removeBackward(dLinkedList));

	// Only return the current node's data if it is non-null
	if (dLinkedList->current != NULL) return (dLinkedList->current)->data;

	// Return NULL otherwise
	return NULL;
}

void* deleteForward(DLinkedList* dLinkedList) {
	// Remove the node and go forward
	free(removeForward(dLinkedList));

	// Only return the current node's data if it is non-null
	if (dLinkedList->current != NULL) return (dLinkedList->current)->data;
	
	// Return NULL otherwise
	return NULL;
}

void* removeBackward(DLinkedList* dLinkedList) {
	// Only delete the current node if it is non-null
	if (dLinkedList->current != NULL) {
		// Check to see if the current node is the tail
		if ((dLinkedList->current)->next == NULL) {
			dLinkedList->tail = (dLinkedList->current)->previous;
		}
		
		// Check to see if the current node is the head
		if ((dLinkedList->current)->previous == NULL) {
			dLinkedList->head = (dLinkedList->current)->next;
		}

		// Move the current pointer and set the next and previous pointers
		if ((dLinkedList->current)->previous != NULL)
			((dLinkedList->current)->previous)->next = (dLinkedList->current)->next;
		if ((dLinkedList->current)->next != NULL)
			((dLinkedList->current)->next)->previous = (dLinkedList->current)->previous;
		LLNode *deletedNode = dLinkedList->current;
		void *data = deletedNode->data;
		dLinkedList->current = (dLinkedList->current)->previous;
		free(deletedNode);
		dLinkedList->size--;

		// Return the current value only if the pointer is non-null
		return data;
	}

	// Return NULL otherwise
	return NULL;
}

void* removeForward(DLinkedList* dLinkedList) {
	// Only delete the current node if it is non-null
	if (dLinkedList->current != NULL) {
		// Check to see if the current node is the tail
		if ((dLinkedList->current)->next == NULL) {
			dLinkedList->tail = (dLinkedList->current)->previous;
		}
		
		// Check to see if the current node is the head
		if ((dLinkedList->current)->previous == NULL) {
			dLinkedList->head = (dLinkedList->current)->next;
		}

		// Move the current pointer and set the next and previous pointers
		if ((dLinkedList->current)->previous != NULL)
			((dLinkedList->current)->previous)->next = (dLinkedList->current)->next;
		if ((dLinkedList->current)->next != NULL)
			((dLinkedList->current)->next)->previous = (dLinkedList->current)->previous;
		LLNode *deletedNode = dLinkedList->current;
		void *data = deletedNode->data;
		dLinkedList->current = (dLinkedList->current)->next;
		free(deletedNode);
		dLinkedList->size--;

		// Return the current value only if the pointer is non-null
		return data;
	}

	// Return NULL otherwise
	return NULL; 
}

void destroyList(DLinkedList* dLinkedList) {
	// If the head isn't NULL, remove all of the following nodes
	if(dLinkedList->head != NULL){
		getHead(dLinkedList);
		while(deleteForward(dLinkedList)){};
	}

	// Free up the list's memory
	free(dLinkedList);
}

void* getHead(DLinkedList* dLinkedList) {
	// Update the current pointer
	dLinkedList->current = dLinkedList->head;

	// Only return data if the head is non-null
	if (dLinkedList->current != NULL) return (dLinkedList->current)->data;

	// Return NULL otherwise
	return NULL;
}

void* getTail(DLinkedList* dLinkedList) {
	// Update the current pointer
	dLinkedList->current = dLinkedList->tail;

	// Only return data if the tail is non-null
	if (dLinkedList->current != NULL) return (dLinkedList->current)->data;

	// Return NULL otherwise
	return NULL;
}

void* getCurrent(DLinkedList* dLinkedList) {
	// Only return data if the current node is non-null
	if (dLinkedList->current != NULL) return (dLinkedList->current)->data;

	// Return NULL otherwise
	return NULL;
}

void* getNext(DLinkedList* dLinkedList) {
	// Only return data if the current and next pointers are non-null
	if (dLinkedList->current != NULL) {
		dLinkedList->current = (dLinkedList->current)->next;
		if (dLinkedList->current != NULL) return (dLinkedList->current)->data;
	}
	
	// Return NULL otherwise
	return NULL;
}

void* getPrevious(DLinkedList* dLinkedList) {
	// Only return data if the current and previous pointers are non-null
	if (dLinkedList->current != NULL) {
		dLinkedList->current = (dLinkedList->current)->previous;
		if (dLinkedList->current != NULL) return (dLinkedList->current)->data;
	}

	// Return NULL otherwise
	return NULL;
}

int getSize(DLinkedList* dLinkedList) {
	return dLinkedList->size;
}
