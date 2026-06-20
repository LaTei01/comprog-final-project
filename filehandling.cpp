#include "filehandling.h"

#include <fstream>
#include <iostream>
#include <ctime>
using namespace std;

// ########## FILE I/O ##########

void loadData(Products** PRODUCTS_LIST) {
    ifstream file("inventory.txt");  
    if (!file.is_open()) return;    // Check if existing file is open

    string line;
    while (getline(file, line)) {   // Get data per line
        if (line.empty()) continue;

        Products* newProduct = new Products();
        string token;
        int field = 0;

        for (int i = 0; i <= (int)line.size(); i++) {   // Iterate thru the data line by line
            if (i == (int)line.size() || line[i] == '|') {
                switch (field) {                        // Classify data line per line index per batch
                    case 0: newProduct->itemIndex = stoi(token); break;
                    case 1: newProduct->itemName  = token;       break;
                    case 2: newProduct->itemBrand = token;       break;
                    case 3: newProduct->itemType  = token;       break;
                    case 4: newProduct->itemPrice = stof(token); break;
                    case 5: newProduct->itemStock = stoi(token); break;
                }
                field++;
                token.clear();
            } else {
                token += line[i];                       // Count how many likes
            }
        }

        newProduct->nextItem = nullptr;
        newProduct->prevItem = nullptr;

        if (*PRODUCTS_LIST == nullptr) {
            newProduct->nextItem = newProduct;
            newProduct->prevItem = newProduct;
            *PRODUCTS_LIST = newProduct;
        } else {
            Products* tail = (*PRODUCTS_LIST)->prevItem;
            tail->nextItem             = newProduct;
            newProduct->prevItem       = tail;
            newProduct->nextItem       = *PRODUCTS_LIST;
            (*PRODUCTS_LIST)->prevItem = newProduct;
        }
    }

    file.close();                                         // Close file for safety
}

void saveData(Products* PRODUCTS_LIST) {
    ofstream file("inventory.txt");
    if (!file.is_open() || PRODUCTS_LIST == nullptr) return;    

    Products* current = PRODUCTS_LIST;
    do {                                                // Saves file in a line by line format for loading
        file << current->itemIndex << "|"
             << current->itemName  << "|"
             << current->itemBrand << "|"
             << current->itemType  << "|"
             << current->itemPrice << "|"
             << current->itemStock << "\n";
        current = current->nextItem;
    } while (current != PRODUCTS_LIST);                 // Iterates thru the whole file

    file.close();
}


void saveReceipt(string customerName, string itemName, int quantity, float total) {
    ofstream file("receipt.txt");
    if (!file.is_open()) return;

    file << "----------- BEAUTEQ TRANSACTION RECEIPT ----------\n";
    file << "Customer: " << customerName << "\n";
    file << "--------------------------------------------------\n";
    file << itemName << " x" << quantity << " — P" << total << "\n";
    file << "--------------------------------------------------\n";
    file << "Total Paid: P" << total << "\n";
    file << "-------- THANK YOU FOR SHOPPING AT BEAUTEQ --------\n";

    file.close();
}

void appendTransaction(string customerName, string itemName, int quantity, float total) {
    ofstream file("transactions.txt", ios::app);   // ios::app: always appends, never overwrites previous history
    if (!file.is_open()) return;

    time_t now = time(nullptr);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M", localtime(&now));

    file << "==================================================\n";
    file << "Date: "     << timestamp     << "\n";
    file << "Customer: " << customerName  << "\n";
    file << "Item: "     << itemName << " x" << quantity << "\n";
    file << "Total Paid: P" << total << "\n";

    file.close();
}