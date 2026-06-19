#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>



using namespace std;

struct Products{ //structure for managing products
    int itemIndex;
    string itemName;
    string itemBrand; // added
    string itemType; // added
    float itemPrice;
    int itemStock;
    Products* nextItem;
    Products* prevItem;

};

struct Orders{ //structure for managing orders
    string customerName;
    string itemName;
    int orderAmount;
    float totalPrice; 
    Orders *nextOrder;
    Orders *prevOrder;
};

//function declarations 
int mainMenu();

int manageInventory(Products** PRODUCTS_LIST);
void searchProduct(Products** PRODUCTS_LIST); 
void addProduct(Products** PRODUCTS_LIST);  
void updateProduct(Products** PRODUCTS_LIST);
void deleteProduct(Products** PRODUCTS_LIST);
void displayProductsDetails(Products* PRODUCTS_LIST);
void displayInventory(Products* PRODUCTS_LIST);
void checkLowStock(Products* PRODUCTS_LIST);

// ########## ORDERS MANAGEMENT FUNCTIONS DECLARATION ##########

int manageOrders(Products** PRODUCTS_LIST, Orders** ORDERS_LIST);
void showCustomerOrders(Orders* ORDERS_LIST);
void validateStockAvailability(Products* PRODUCTS_LIST, Orders* ORDERS_LIST);
void acceptAndProcessOrders(Products* PRODUCTS_LIST, Orders* ORDERS_LIST); //includes auto deduct and save receipt
void displayOrderDetails(Orders* ORDERS_LIST);

int main(){ //main function
    Products* PRODUCTS_LIST = nullptr;
    Orders* ORDERS_LIST = nullptr;
    int choice;

    choice = mainMenu();

    while(choice != 3){
    switch(choice){
        case 1:
            manageInventory(&PRODUCTS_LIST);
            break;
        case 2:
            manageOrders(&PRODUCTS_LIST, &ORDERS_LIST);
            break;
        case 3:
            cout << "Exiting the program. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }

    // saveData(PRODUCTS_LIST);
    cout << "Exiting the program. Goodbye!" << endl;
    return 0;
}

int mainMenu(){ //main menu function
    int choice;
    cout << "Welcome to the Inventory Management System!\n";
    cout << "1. Manage Inventory\n";
    cout << "2. Manage Orders\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: \n";
    cin >> choice;
    switch (choice){
        case 1:
        cout << "Managing inventory...\n";
        break;
        case 2:
        cout << "Managing orders...\n";
        break;
        case 3:
        cout << "Exiting the program. Have a good day!\n";
        break;
        default:
        cout << "Invalid choice. Please try again.\n";
    }
    return choice;
}  

// ########## INVENTORY MANAGEMENT ##########

int manageInventory(Products** PRODUCTS_LIST){ //inventory management function
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
        deleteProduct(PRODUCTS_LIST);
        break;
        case 4:
        displayProductsDetails(*PRODUCTS_LIST);
        break;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }

    ManageInventory(PRODUCTS_LIST);
    return 0;
};

void searchProduct(Products** PRODUCTS_LIST){ //search product function
    string searchName;
    cout << "Enter the product name to search: ";
    cin >> searchName;
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

void addProduct(Products** PRODUCTS_LIST){ //add product function
    Products* newProduct = new Products();
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
    Products* current = *PRODUCTS_LIST; //to assign index to new product
    
    cout<< "Enter Product Name: \n";
    cin >> itemName;
    cout << "Enter Product Price: \n";
    cin >> itemPrice;
    cout << "Enter Product Stock: \n";
    cin >> itemStock;


    newProduct->itemName = itemName;
    newProduct->itemPrice = itemPrice;
    newProduct->itemStock = itemStock;
    newProduct->nextItem = nullptr;
    newProduct->prevItem = nullptr;
    
    if (*PRODUCTS_LIST == nullptr){
        newProduct->itemIndex = 1;
        *PRODUCTS_LIST = newProduct;
    }else{
        int count=1;
        Products* current = *PRODUCTS_LIST;
        while (current->nextItem != nullptr){
            current = current->nextItem;
            count++;
        }
        newProduct->itemIndex = count + 1;
        current->nextItem = newProduct;
        newProduct->prevItem = current;
    }
}

void updateProduct(Products** PRODUCTS_LIST){ //update product function
    string searchName;
    cout << "Enter product name: \n";
    cin >> searchName;
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

            // saveData(*PRODUCTS_LIST);   // uncomment when saveData is completed
            cout << "Product updated successfully." << endl;
            return;
        }
        current = current->nextItem;
    } while (current != *PRODUCTS_LIST);

void deleteProduct(Products** PRODUCTS_LIST){ //delete product function

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
            // saveData(*PRODUCTS_LIST);   //  uncomment once saveData is complete
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

void displayProductsDetails(Products* PRODUCTS_LIST){
    Products* current = PRODUCTS_LIST;
    while (current != nullptr){
        cout << "Index: " << current->itemIndex << endl;
        cout << "Product Name: " << current->itemName << endl;
        cout << "Product Price: "<< current->itemPrice << endl;
        cout << "Product Stock: " << current->itemStock << endl;
        cout << "-----------------------------" << endl;
        current = current->nextItem;
    }
    return;
}
void displayInventory(Products* PRODUCTS_LIST){
    return;
}

//functions for order management
int manageOrders(Products** PRODUCTS_LIST, Orders** ORDERS_LIST){ //order management function
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

void showCustomerOrders(Orders* ORDERS_LIST){ //show customer orders function
    Orders* current = ORDERS_LIST;
    do {
        cout << "--------------------------------------------------" << endl;
        cout << "Customer: " << current->customerName << endl;
        cout << "Item: "     << current->itemName     << endl;
        cout << "Qty: "      << current->orderAmount  << endl;
        cout << "Total: P"   << current->totalPrice   << endl;
        current = current->nextOrder;
    } while (current != ORDERS_LIST);
    cout << "--------------------------------------------------" << endl;
}

void validateStockAvailability(Products* PRODUCTS_LIST) {

}

void addToCart(Products** PRODUCTS_LIST, Orders** ORDERS_LIST) {
    if (*PRODUCTS_LIST == nullptr) {    // Check if there are no procucts in the list
        cout << "No products available." << endl;
        return;
    }
}

void validateStockAvailability(Products* PRODUCTS_LIST, Orders* ORDERS_LIST){
    return;
}
void acceptAndProcessOrders(Products* PRODUCTS_LIST, Orders* ORDERS_LIST){
    return;
} //includes auto deduct and save receipt
void displayOrderDetails(Orders* ORDERS_LIST){
    return;
}
