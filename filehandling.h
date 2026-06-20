#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include "structs.h"
#include <string>
using namespace std;

// ########## FILE I/O FUNCTION DECLARATIONS ##########

// Loads product records from inventory.txt into the circular doubly linked list.
// Creates the list from scratch; safe to call on an empty PRODUCTS_LIST.
void loadData(Products** PRODUCTS_LIST);

// Persists the entire PRODUCTS_LIST to inventory.txt, overwriting the file.
// Each product is written as a pipe-delimited line:
//   itemIndex|itemName|itemBrand|itemType|itemPrice|itemStock
void saveData(Products* PRODUCTS_LIST);

// Writes a formatted transaction receipt to receipt.txt.
// Includes every order in ORDERS_LIST, the customer name, and the grand total.
void saveReceipt(Orders* ORDERS_LIST, string customerName, float total);

#endif // FILEHANDLING_H