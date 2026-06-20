#include "inventory.h"
#include "filehandling.h"

#include <iostream>
#include <algorithm>
using namespace std;

static const int LOW_STOCK_THRESHOLD = 5;

// ########## INVENTORY MANAGEMENT ##########

int ManageInventory(Products** PRODUCTS_LIST) {
    int inventoryChoice;

    cout << "\n########## Inventory Management ##########" << endl;
    cout << "1. Search Product"              << endl;
    cout << "2. Add Product"                 << endl;
    cout << "3. Update Product"              << endl;
    cout << "4. Delete Product"              << endl;
    cout << "5. Display All Product Details" << endl;
    cout << "6. Display In-Store Inventory"  << endl;
    cout << "7. Check Low Stock"             << endl;
    cout << "8. Back to Main Menu"           << endl;
    cout << "Enter your choice: ";
    cin >> inventoryChoice;

    switch (inventoryChoice) {
        case 1: searchProduct(PRODUCTS_LIST);            break;
        case 2: addProduct(PRODUCTS_LIST);               break;
        case 3: updateProduct(PRODUCTS_LIST);            break;
        case 4: deleteProduct(PRODUCTS_LIST);            break;
        case 5: displayProductsDetails(*PRODUCTS_LIST);  break;
        case 6: displayInventory(*PRODUCTS_LIST);        break;
        case 7: checkLowStock(*PRODUCTS_LIST);           break;
        case 8: return 0;
        default: cout << "Invalid choice. Please try again." << endl; break;
    }

    return ManageInventory(PRODUCTS_LIST);  // Loop until user goes back
}

void searchProduct(Products** PRODUCTS_LIST){
    if (*PRODUCTS_LIST == nullptr) {    // Check if the products list is empty
        cout << "No products found." << endl;
        return;
    }

    string searchKeyword;
    cout << "Search: ";
    cin.ignore();                       // Clear any leftover newline character from previous input operations
    getline(cin, searchKeyword);        // Read the entire search query, including spaces

    
    string lowerKeyword = searchKeyword;    
    // Create a lowercase copy of the search keyword
    transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);

    cout << "\nResults for [" << searchKeyword << "]:" << endl; 
    cout << "--------------------------------------------------" << endl;

    bool found = false;    // Flag to track whether at least one matching product is found
    Products* current = *PRODUCTS_LIST;

    // Traverse the entire products linked list
    do {
        // Create temporary copies of searchable fields
        string tempName  = current->itemName;
        string tempBrand = current->itemBrand;
        string tempType  = current->itemType;

        // Convert all searchable fields to lowercase for case-insensitive comparison.
        transform(tempName.begin(),  tempName.end(),  tempName.begin(),  ::tolower);
        transform(tempBrand.begin(), tempBrand.end(), tempBrand.begin(), ::tolower);
        transform(tempType.begin(),  tempType.end(),  tempType.begin(),  ::tolower);

        // Check if the search keyword exists in the product name, brand, or type using substring matching
        if (tempName.find(lowerKeyword)  != string::npos ||
            tempBrand.find(lowerKeyword) != string::npos ||
            tempType.find(lowerKeyword)  != string::npos) {

            // Display matching product information
            cout << "ID: "    << current->itemIndex
                 << " | "     << current->itemName
                 << " ("      << current->itemBrand << ")"
                 << " | P"    << current->itemPrice
                 << " | Stock: " << current->itemStock << endl;

            // Mark that at least one product has been found
            found = true;
        }
        current = current->nextItem;
    } while (current != *PRODUCTS_LIST);

    // If no matches were found, display a message
    if (!found)
        cout << "Product not found." << endl;
    cout << "--------------------------------------------------" << endl;
}


void addProduct(Products** PRODUCTS_LIST){
    string itemName;

    cout << "Enter Product Name: ";
    cin.ignore();
    getline(cin, itemName);    // Read the full product name, including spaces

    if (*PRODUCTS_LIST != nullptr) {     // Check for duplicate product names if the list is not empty


        Products* current = *PRODUCTS_LIST;

        do {
            // Creates temporary copies for case-insensitive comparison
            string tempName = current->itemName;
            string tempInput = itemName;

            // Converts both strings to lowercase
            transform(tempName.begin(),  tempName.end(),  tempName.begin(),  ::tolower);
            transform(tempInput.begin(), tempInput.end(), tempInput.begin(), ::tolower);

            if (tempName == tempInput) {// Duplicate found

                // Display warning and show available actions
                cout << "WARNING: \"" << itemName << "\" already exists." << endl;
                cout << "1. Enter a different name" << endl;
                cout << "2. Go to Update Product instead" << endl;
                cout << "Enter your choice: ";

                int dupChoice;
                cin >> dupChoice;

                if (dupChoice == 2) {           // Redirect user to update the existing product
                    updateProduct(PRODUCTS_LIST);
                    return;
                } else {
                    addProduct(PRODUCTS_LIST);  // Restart the add product process 
                    return;
                }
            }

            current = current->nextItem;

        } while (current != *PRODUCTS_LIST);
    }

    string itemBrand, itemType;
    float itemPrice;
    int itemStock;

    cout << "Enter Brand: ";
    cin.ignore();
    getline(cin, itemBrand);

    cout << "Enter Type (Lip Product / Eye Product / Skin Product / Others): ";
    getline(cin, itemType);

    cout << "Enter Price: ";
    cin >> itemPrice;

    cout << "Enter Stock Quantity: ";
    cin >> itemStock;

    Products* newProduct = new Products(); // Create new node pointer

    // Assign entered values to the new node
    newProduct->itemName  = itemName;
    newProduct->itemBrand = itemBrand;
    newProduct->itemType  = itemType;
    newProduct->itemPrice = itemPrice;
    newProduct->itemStock = itemStock;

    // Initialize linked list pointers
    newProduct->nextItem  = nullptr;
    newProduct->prevItem  = nullptr;

    // Assign an automatic product index and insert into the circular doubly linked list
    if (*PRODUCTS_LIST == nullptr) { // Products list is empty

        // Assign index 1 and point next/prev to itself
        newProduct->itemIndex = 1;
        newProduct->nextItem  = newProduct;
        newProduct->prevItem  = newProduct;

        *PRODUCTS_LIST = newProduct;         // Make the new node the head of the list


    } else {    // Products list have content
        Products* tail = (*PRODUCTS_LIST)->prevItem;    // Get the current tail node
        newProduct->itemIndex = tail->itemIndex + 1;    // Assign the next available index.

        // Insert the new node after the tail
        tail->nextItem              = newProduct;
        newProduct->prevItem        = tail;
        newProduct->nextItem        = *PRODUCTS_LIST;
        (*PRODUCTS_LIST)->prevItem  = newProduct;
    }

    saveData(*PRODUCTS_LIST); // uncomment later once saveData is complete
    cout << "Product added successfully." << endl;
}


void updateProduct(Products** PRODUCTS_LIST) {
    if (*PRODUCTS_LIST == nullptr) {            // Check if there are no products in the list
        cout << "No products to update." << endl;
        return;
    }

    int targetId;
    cout << "Enter Product ID to update: ";
    cin >> targetId;

    Products* current = *PRODUCTS_LIST;
    do {
        if (current->itemIndex == targetId) {   // Product to be updated is found
            string newName, newBrand, newType;

            // Asks for the fields to be updated and reassign
            cin.ignore();
            cout << "Current Name  [" << current->itemName  << "]: ";
            getline(cin, newName);
            if (!newName.empty()) current->itemName = newName;

            cout << "Current Brand [" << current->itemBrand << "]: ";
            getline(cin, newBrand);
            if (!newBrand.empty()) current->itemBrand = newBrand;

            cout << "Current Type  [" << current->itemType  << "]: ";
            getline(cin, newType);
            if (!newType.empty()) current->itemType = newType;

            cout << "Current Price [" << current->itemPrice << "]: ";
            string priceInput;
            getline(cin, priceInput);
            if (!priceInput.empty()) current->itemPrice = stof(priceInput);

            cout << "Current Stock [" << current->itemStock << "]: ";
            string stockInput;
            getline(cin, stockInput);
            if (!stockInput.empty()) current->itemStock = stoi(stockInput);

            saveData(*PRODUCTS_LIST);   // uncomment when saveData is completed
            cout << "Product updated successfully." << endl;
            return;
        }
        current = current->nextItem;
    } while (current != *PRODUCTS_LIST);

    cout << "Product ID not found." << endl;
}

void deleteProduct(Products** PRODUCTS_LIST) {

    if (*PRODUCTS_LIST == nullptr) {    // Check if there are no products in the list
        cout << "There are no products yet." << endl;
        return;
    }

    int targetId;
    cout << "Enter Product ID to delete: ";
    cin >> targetId;

    Products* current = *PRODUCTS_LIST;
    do {
        if (current->itemIndex == targetId) {       // Searched product found
            cout << "Delete \"" << current->itemName << "\"? (Y/N): ";
            char confirm;
            cin >> confirm;

            if (confirm != 'Y' && confirm != 'y') { // Deletion cancelled
                cout << "Deletion cancelled." << endl;
                return;
            }

            if (current->nextItem == current) {     // Only one node in the list
                *PRODUCTS_LIST = nullptr;           // Deleted only node
            } else {
                // Link the prev of current to next and vice versa
                current->prevItem->nextItem = current->nextItem;
                current->nextItem->prevItem = current->prevItem;
                if (current == *PRODUCTS_LIST)
                    *PRODUCTS_LIST = current->nextItem;
            }

            delete current;             // free to be deleted node
            saveData(*PRODUCTS_LIST);   //  uncomment once saveData is complete
            cout << "Product deleted successfully." << endl;
            return;
        }
        current = current->nextItem;
    } while (current != *PRODUCTS_LIST);

    cout << "Product ID not found." << endl;

}

void displayProductsDetails(Products* PRODUCTS_LIST) {
    if (PRODUCTS_LIST == nullptr) {         // Checks if there are no products in the list
        cout << "No products to display." << endl;
        return;
    }

    cout << "\n########## All Products ##########" << endl;
    cout << "ID | Name | Brand | Type | Price | Stock" << endl;
    cout << "--------------------------------------------------" << endl;

    Products* current = PRODUCTS_LIST;
    do {
        cout << current->itemIndex << " | "
             << current->itemName  << " | "
             << current->itemBrand << " | "
             << current->itemType  << " | P"
             << current->itemPrice << " | "
             << current->itemStock << endl;
        current = current->nextItem;
    } while (current != PRODUCTS_LIST);
    cout << "--------------------------------------------------" << endl;

}

void displayInventory(Products* PRODUCTS_LIST) {
    if (PRODUCTS_LIST == nullptr) {         // Checks if there are no products in the list
        cout << "No products to display." << endl;
        return;
    }

    cout << "\n########## In-Store Inventory by Type ##########" << endl;

    string types[] = {"Lip Product", "Eye Product", "Skin Product", "Others"}; // List of the types of products
    for (const string& type : types) {  // Iterates per type in the list
        cout << "\n" << type << ":" << endl;
        Products* current = PRODUCTS_LIST;
        do {
            if (current->itemType == type)
                cout << "  " << current->itemName << " — Stock: " << current->itemStock << endl;
            current = current->nextItem;
        } while (current != PRODUCTS_LIST);
    }
}
