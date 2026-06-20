#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include "structs.h"
#include <string>
using namespace std;

// ########## FILE I/O FUNCTION DECLARATIONS ##########

// Loads product records from inventory.txt into the circular doubly linked list
// Creates the list from scratch; safe to call on an empty PRODUCTS_LIST
void loadData(Products** PRODUCTS_LIST);

// Persists the entire PRODUCTS_LIST to inventory.txt, overwriting the file
// Each product is written as a pipe-delimited line: itemIndex|itemName|itemBrand|itemType|itemPrice|itemStock
void saveData(Products* PRODUCTS_LIST);

// Writes a receipt to receipt.txt for one completed order (one customer, one product, one quantity)
// Overwritten each time, like "last receipt printed" and not a history log
void saveReceipt(string customerName, string itemName, int quantity, float total);

// Appends a single completed order to transactions.txt, the running order history log. 
// Unlike saveReceipt, this file is never overwritten as every order adds a new dated entry
// Used by showTransactionHistory() in orders.cpp
void appendTransaction(string customerName, string itemName, int quantity, float total);

#endif // FILEHANDLING_H