#include <iostream> 
#include <list>
#include <string>

using namespace std;

struct Products{
    int itemIndex;
    string itemName;
    float itemPrice;
    int productStruct;
    Products* nextItem;
    Products* prevItem;
};

struct Orders{
    string customerName;
    string itemName;
    int orderAmount;
    float totalPrice; // to be edited
    Orders *nextOrder;
    Orders *prevOrder;
};

int MainMenu();

int ManageInventory(Products** PRODUCTS_LIST);
void searchProduct(Products** PRODUCTS_LIST); 
void addProduct(Products** PRODUCTS_LIST);  
void updateProduct(Products** PRODUCTS_LIST);
void deleteProduct(Products** PRODUCTS_LIST);
void displayProductsDetails(Products* PRODUCTS_LIST);
void displayInventory(Products* PRODUCTS_LIST);

int ManageOrders(Products** PRODUCTS_LIST, Orders** ORDERS_LIST);
void showCustomerOrders(Orders* ORDERS_LIST);
void validateStockAvailability(Products* PRODUCTS_LIST, Orders* ORDERS_LIST);
void acceptAndProcessOrders(Products* PRODUCTS_LIST, Orders* ORDERS_LIST); 
/*includes auto deduct and save receipt */
void displayOrderDetails(Orders* ORDERS_LIST);

int main(){
    Products* PRODUCTS_LIST = nullptr;
    Orders* ORDERS_LIST = nullptr;
    int choice;

    choice = MainMenu();

    while(choice != 3){
    switch(choice){
        case 1:
            ManageInventory(&PRODUCTS_LIST);
            break;
        case 2:
            ManageOrders(&PRODUCTS_LIST, &ORDERS_LIST);
            break;
        case 3:
            cout << "Exiting the program. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}
    return 0;
}

int MainMenu(){
    int choice;
    cout << "Welcome to the Inventory Management System!" << endl;
    cout << "Please select an option:" << endl;
    cout << "1. Add Product" << endl;
    cout << "2. Search Product" << endl;
    cout << "3. Update Product" << endl;
    cout << "4. Delete Product" << endl;
    cout << "5. Exit" << endl;
    cin >> choice;
    return choice;
};


int ManageInventory(Products** PRODUCTS_LIST){
    int inventoryChoice;

    cout << "Inventory Management\n";
    cout << "1. Add Product" << endl;
    cout << "2. Update Product" << endl;
    cout << "3. Delete Product" << endl;
    cout << "4. Display All Product Details\n";
    cout << "Enter your choice: ";
    cin >> inventoryChoice;

    switch (inventoryChoice){
        case 1:
        addProduct(PRODUCTS_LIST);
        break;
        case 2:
        updateProduct(PRODUCTS_LIST);
        break;
        case 3:
        deleteProduct(PRODUCTS_LIST);
        break;
        default:
        cout << "Invalid choice. Please try again.\n";
    }
    return 0;
};

void searchProduct(Products** PRODUCTS_LIST){
    string searchName;
    cout << "Enter the product name to search: ";
    cin >> searchName;
    Products* current = *PRODUCTS_LIST;
    while (current != nullptr){
        if (current->itemName==searchName){
            cout << "Index: " << current->itemIndex;
            cout << "Product Name: " << current->itemName;
            cout << "Product Price: "<< current->itemPrice;
            break;
        }else{
            current = current->nextItem;
        }
    }
}

void addProduct(Products** PRODUCTS_LIST){
    Products* newProduct = new Products;
    string itemName;
    float itemPrice;
    cout<< "Enter Product Name: \n";
    cin >> itemName;
    cout<< "Enter Product Price: \n";
    cin >> itemPrice;

    newProduct = new Products();
    newProduct->itemName = itemName;
    newProduct->itemPrice = itemPrice;
    newProduct->nextItem = nullptr;
    newProduct->prevItem = nullptr; 
}

void updateProduct(Products** PRODUCTS_LIST){
    string searchName;
    cout << "Enter product name: \n";
    cin >> searchName;
    Products* current = *PRODUCTS_LIST;
    while (current != nullptr){
        if (current->itemName == searchName){
            cout << "Enter new product name: \n";
            cin >> current->itemName;
            cout << "Enter new product price: \n";
            cin >> current->itemPrice;
            break;
        }else{
            current = current->nextItem;
        }
    }
};

void deleteProduct(Products** PRODUCTS_LIST){

    if(*PRODUCTS_LIST==nullptr){
      cout<<"There are no products yet.";
      return;
    }

    string toBeDeleted;
    cout << "Enter the product name to be deleted: ";
    cin >> toBeDeleted;
    Products* current = *PRODUCTS_LIST;

    while (current != nullptr){
        if (current->itemName==toBeDeleted){
            current->prevItem->nextItem = current->nextItem;
            current->nextItem->prevItem = current->prevItem;
            delete current;
            return;
        }else{
            cout << "Item does not exist";
            current = current->nextItem;
        }
    }
};

void displayProductsDetails(Products* PRODUCTS_LIST);
void displayInventory(Products* PRODUCTS_LIST);

//functions for order management
int ManageOrders(Orders** ORDERS_LIST, Products** PRODUCTS_LIST){
    int orderChoice;
    cout << "Order Management\n";
    cout << "1. Show Customer Orders\n";
    cout << "2. Validate Stock Availability\n";  
    cout << "3. Accept and Process Orders\n";
    cout << "4. Display Order Details\n";
    cout << "Enter your choice: ";
    cin >> orderChoice;
    
    switch (orderChoice){
        case 1:
        showCustomerOrders(*ORDERS_LIST);
        break;
        case 2:
        validateStockAvailability(*PRODUCTS_LIST, *ORDERS_LIST);
        break;
        case 3:
        acceptAndProcessOrders(*PRODUCTS_LIST, *ORDERS_LIST);
        break;
        case 4:
        displayOrderDetails(*ORDERS_LIST);
        break;
        default:
        cout << "Invalid choice. Please try again.\n";

    }
    return 0;
};

void showCustomerOrders(Orders* ORDERS_LIST){
    Orders* current = ORDERS_LIST;
    while (current != nullptr){
        cout << "Customer Name: " << current->customerName << endl;
        cout << "Item Name: " << current->itemName << endl;
        cout << "Order Amount: " << current->orderAmount << endl;
        cout << "Total Price: " << current->totalPrice << endl;
        cout << "-----------------------------" << endl;
        current = current->nextOrder;
    }
}