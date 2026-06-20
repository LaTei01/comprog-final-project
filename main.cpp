#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

struct Products{
    int itemIndex;
    string itemName;
    string itemBrand; // added
    string itemType; // added
    float itemPrice;
    int itemStock; // corrected

    Products* nextItem;
    Products* prevItem;

};

struct Orders{
    string customerName;
    string itemName;
    int orderAmount;
    float totalPrice; 
    Orders *nextOrder;
    Orders *prevOrder;
};

int MainMenu();

// ########## INVENTORY MANAGEMENT FUNCTION DECLARATION ##########
int ManageInventory(Products** PRODUCTS_LIST);
void searchProduct(Products** PRODUCTS_LIST);
void addProduct(Products** PRODUCTS_LIST);
void updateProduct(Products** PRODUCTS_LIST);
void deleteProduct(Products** PRODUCTS_LIST);
void displayProductsDetails(Products* PRODUCTS_LIST);
void displayInventory(Products* PRODUCTS_LIST);
void checkLowStock(Products* PRODUCTS_LIST);

// ########## ORDERS MANAGEMENT FUNCTIONS DECLARATION ##########
int ManageOrders(Products** PRODUCTS_LIST, Orders** ORDERS_LIST);
void showCustomerOrders(Orders* ORDERS_LIST);
void validateStockAvailability(Products* PRODUCTS_LIST);
void addToCart(Products** PRODUCTS_LIST, Orders** ORDERS_LIST);
void viewAndCheckout(Products** PRODUCTS_LIST, Orders** ORDERS_LIST);
void displayOrderDetails(Orders* ORDERS_LIST);

// ########## FILE INPUT AND OUTPUT FUNCTIONS DECLARATION ##########
void loadData(Products** PRODUCTS_LIST);
void saveData(Products* PRODUCTS_LIST);
void saveReceipt(Orders* ORDERS_LIST, string customerName, float total);

// ########## MAIN FUNCTION ##########
int main(){
    Products* PRODUCTS_LIST = nullptr;
    Orders* ORDERS_LIST = nullptr;
    int choice;

    loadData(&PRODUCTS_LIST);
    choice = MainMenu();

    while (choice != 3) {
        switch (choice) {
            case 1:
                ManageInventory(&PRODUCTS_LIST);
                break;
            case 2:
                ManageOrders(&PRODUCTS_LIST, &ORDERS_LIST);
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        choice = MainMenu();
    while (choice != 3) {
        switch (choice) {
            case 1:
                ManageInventory(&PRODUCTS_LIST);
                break;
            case 2:
                ManageOrders(&PRODUCTS_LIST, &ORDERS_LIST);
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
        choice = MainMenu();
    }

    saveData(PRODUCTS_LIST);
    cout << "Exiting the program. Goodbye!" << endl;
    return 0;
}


int MainMenu(){
    int choice;

    cout << "Welcome to the Beauteq Inventory Management System!" << endl;
    cout << "1. Manage Inventory" << endl;
    cout << "2. Manage Orders" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice: ";

    cin >> choice;
    return choice;
}  

// ########## INVENTORY MANAGEMENT ##########

int ManageInventory(Products** PRODUCTS_LIST){
    int inventoryChoice;

    cout << "\n########## Inventory Management ##########" << endl;
    cout << "1. Search Product" << endl;
    cout << "2. Add Product" << endl;
    cout << "3. Update Product" << endl;
    cout << "4. Delete Product" << endl;
    cout << "5. Display All Product Details" << endl;
    cout << "6. Display In-Store Inventory" << endl;
    cout << "7. Back to Main Menu" << endl;
    cout << "Enter your choice: ";
    cin >> inventoryChoice;

    switch (inventoryChoice) {
        case 1:
            searchProduct(PRODUCTS_LIST);
            break;
        case 2:
            addProduct(PRODUCTS_LIST);
            break;
        case 3:
            updateProduct(PRODUCTS_LIST);
            break;
        case 4:
            deleteProduct(PRODUCTS_LIST);
            break;
        case 5:
            displayProductsDetails(*PRODUCTS_LIST);
            break;
        case 6:
            displayInventory(*PRODUCTS_LIST);
            break;
        case 7:
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }

    ManageInventory(PRODUCTS_LIST);
    return 0;
};


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

int ManageOrders(Products** PRODUCTS_LIST, Orders** ORDERS_LIST) {
    int orderChoice;

    cout << "\n########## Order Management ##########" << endl;
    cout << "1. Show Customer Orders" << endl;
    cout << "2. Validate Stock Availability" << endl;
    cout << "3. Add Item to Cart" << endl;
    cout << "4. View Cart & Checkout" << endl;
    cout << "5. Display Order Details" << endl;
    cout << "6. Back to Main Menu" << endl;
    cout << "Enter your choice: ";
    cin >> orderChoice;

    switch (orderChoice) {
        case 1:
            showCustomerOrders(*ORDERS_LIST);
            break;
        case 2:
            validateStockAvailability(*PRODUCTS_LIST);
            break;
        case 3:
            addToCart(PRODUCTS_LIST, ORDERS_LIST);
            break;
        case 4:
            viewAndCheckout(PRODUCTS_LIST, ORDERS_LIST);
            break;
        case 5:
            displayOrderDetails(*ORDERS_LIST);
            break;
        case 6:
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }

    ManageOrders(PRODUCTS_LIST, ORDERS_LIST);
    return 0;
}

void showCustomerOrders(Orders* ORDERS_LIST) {
    if (ORDERS_LIST == nullptr) {       // Check if there are no orders in the list
        cout << "No orders yet." << endl;
        return;
    }

    Orders* current = ORDERS_LIST;
    do {
        cout << "--------------------------------------------------" << endl;
        cout << "Customer: " << current->customerName << endl;
        cout << "Item: "     << current->itemName     << endl;
        cout << "quantity: "      << current->orderAmount  << endl;
        cout << "Total: P"   << current->totalPrice   << endl;
        current = current->nextOrder;
    } while (current != ORDERS_LIST);
    cout << "--------------------------------------------------" << endl;
}

void validateStockAvailability(Products* PRODUCTS_LIST) {

    if (PRODUCTS_LIST == nullptr) { // Check of there is no products yet in the list
        cout << "No products available." << endl;
        return;
    }

    int targetId, quantity;
    cout << "Enter Product ID: ";
    cin >> targetId;
    cout << "Enter desired quantity: ";
    cin >> quantity;

    Products* current = PRODUCTS_LIST;
    do {
        if (current->itemIndex == targetId) {   
            if (quantity <= current->itemStock) // Stock is sufficient
                cout << "In stock. Available: " << current->itemStock << endl;
            else                                // Stock insufficient
                cout << "Insufficient stock. Available: " << current->itemStock << endl;
            return;
        }
        current = current->nextItem;
    } while (current != PRODUCTS_LIST);

    cout << "Product ID not found." << endl;
}

void addToCart(Products** PRODUCTS_LIST, Orders** ORDERS_LIST) {
    if (*PRODUCTS_LIST == nullptr) {    // Check if there are no procucts in the list
        cout << "No products available." << endl;
        return;
    }

    int targetId, quantity;
    string customerName;

    cout << "Enter Customer Name: ";
    cin.ignore();
    getline(cin, customerName);
    cout << "Enter Product ID: ";
    cin >> targetId;
    cout << "Enter Quantity: ";
    cin >> quantity;

    Products* current = *PRODUCTS_LIST;
    do {
        if (current->itemIndex == targetId) {   // The searched itemIndex is found
            if (quantity > current->itemStock) {
                cout << "Insufficient stock. Available: " << current->itemStock << endl;
                return;
            }

            Orders* newOrder = new Orders();    // Creates new node pointer instance
            newOrder->customerName = customerName;
            newOrder->itemName     = current->itemName;
            newOrder->orderAmount  = quantity;
            newOrder->totalPrice   = current->itemPrice * quantity;
            newOrder->nextOrder    = nullptr;
            newOrder->prevOrder    = nullptr;

            if (*ORDERS_LIST == nullptr) {      // There are no orders in the list yet
                newOrder->nextOrder = newOrder;
                newOrder->prevOrder = newOrder;
                *ORDERS_LIST = newOrder;        // Makes the new order the head of the order list

            } else {                            // There are existing orders
                Orders* tail = (*ORDERS_LIST)->prevOrder;
                tail->nextOrder             = newOrder;
                newOrder->prevOrder         = tail;
                newOrder->nextOrder         = *ORDERS_LIST;
                (*ORDERS_LIST)->prevOrder   = newOrder;     // Appends the order at the last
            }

            cout << "Added to cart: " << current->itemName << " x" << quantity << " — P" << newOrder->totalPrice << endl;
            return;
        }
        current = current->nextItem;
    } while (current != *PRODUCTS_LIST);

    cout << "Product ID not found." << endl;
}

void viewAndCheckout(Products** PRODUCTS_LIST, Orders** ORDERS_LIST) {
    if (*ORDERS_LIST == nullptr) {      // Check if the order list empty
        cout << "Cart is empty." << endl;
        return;
    }

    float grandTotal = 0;
    string customerName;

    cout << "\n########## Cart Summary ##########" << endl;
    Orders* current = *ORDERS_LIST;
    do {
        // Prints: itemName x orderAmount - P(totalPrice)
        cout << current->itemName << " x" << current->orderAmount << " — P" << current->totalPrice << endl;
        grandTotal += current->totalPrice; // Add totalPrices per item to the grandTotal
        customerName = current->customerName;
        current = current->nextOrder;
    } while (current != *ORDERS_LIST);

    cout << "Grand Total: P" << grandTotal << endl;
    cout << "Confirm purchase? (Y/N): ";
    char confirm;
    cin >> confirm;

    if (confirm != 'Y' && confirm != 'y') {     // Cancels checkout
        cout << "Checkout cancelled." << endl;
        return;
    }

    // deduct stock
    current = *ORDERS_LIST;
    do {                                        // Substracts ordered amount if everything goes thru
        Products* prod = *PRODUCTS_LIST;
        do {
            if (prod->itemName == current->itemName) {
                prod->itemStock -= current->orderAmount;
                break;
            }
            prod = prod->nextItem;
        } while (prod != *PRODUCTS_LIST);       // Iterates through the PRODUCTS_LIST to reduce itemStock
        current = current->nextOrder;
    } while (current != *ORDERS_LIST);          // Iterates through the ORDERS_LIST to determine orderAmount to subtract

    saveReceipt(*ORDERS_LIST, customerName, grandTotal);
    saveData(*PRODUCTS_LIST);               

    Orders* toDelete = *ORDERS_LIST;           
    do {
        Orders* next = toDelete->nextOrder;
        delete toDelete;
        toDelete = next;
    } while (toDelete != *ORDERS_LIST);         // Iterates through the ORDERS_LIST and frees the dynamic memory
    *ORDERS_LIST = nullptr;                    

    cout << "Order complete. Receipt saved to receipt.txt" << endl;
}

void displayOrderDetails(Orders* ORDERS_LIST) {

}

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


void saveReceipt(Orders* ORDERS_LIST, string customerName, float total) {
    ofstream file("receipt.txt");
    if (!file.is_open()) return;

    file << "----------- BEAUTEQ TRANSACTION RECEIPT ----------\n";
    file << "Customer: " << customerName << "\n";
    file << "--------------------------------------------------\n";

    Orders* current = ORDERS_LIST;                                  // Contains all the orders list: to be changed later. Just the selected ones should do
    do {
        file << current->itemName << " x" << current->orderAmount
             << " — P" << current->totalPrice << "\n";
        current = current->nextOrder;
    } while (current != ORDERS_LIST);

    file << "--------------------------------------------------\n";
    file << "Total Paid: P" << total << "\n";
    file << "-------- THANK YOU FOR SHOPPING AT BEAUTEQ --------\n";

    file.close();
}   