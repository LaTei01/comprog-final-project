# comprog-final-project
the final boss for first level

CODE SPECIFICATIONS
--------------------DATA STRUCTURES-----------------------

struct Products (Circular Doubly Linked List) //nodes that represent one product keeping unit in the makeup catalog

FIELD         |TYPE           |DESCRIPTION
itemIndex      int             Auto-incremented unique product ID
itemName       string          Full produc name (e.g.,Maybelline Tattoo 
                               Brow)
itemBrand      string          Brand Name (e.g.,Maybelline)
itemType       string          Category: Lip Product | Eye Product | 
                               Skin Product | Others
itemPrice      float           Retail price in Philippine Peso (₱)
itemStock      int             Current stock quantity
*nextItem      Products*       Pointer to next node in list
*prevItem      Products*       Pointer to previous node in list

struct Products{
    int itemIndex;
    string itemName;
    string itemBrand;
    string itemType;
    float itemPrice;
    int itemStock;
    Products* nextItem;
    Products* prevItem;
};


struct Orders (Circular Doubly Linked List) //nodes that represent one item line in a customer's order session

FIELD         |TYPE           |DESCRIPTION
customerName   string          Name of the customer placing the order
itemName       string          Name of the ordered product
orderAmount    int             Quantity requested by the customer
totalPrice     float           Computed: itemPrice * orderAmount
*nextOrder     Orders*         Pointer to next order node
*prevOrder     Orders*         Pointer to previous order node

struct Orders{
    string customerName;
    string itemName;
    int orderAmount;
    float totalPrice;
    Orders* nextOrder;
    Orders* prevOrder;
}

--------------------FUNCTION DEFINITIONS-----------------------

1. NAVIGATION
    1.1. int MainMenu();
    1.2. int ManageInventory(Products** PRODUCTS_LIST);
    1.3. int ManageOrders(Products** PRODUCT_LIST, Orders** ORDERS_LIST);
2. INVENTORY FUNCTIONS
    2.1. void searchProduct(Products** PRODUCTS_LIST);
    2.2. void addProduct(Products** PRODUCTS_LIST);
    2.3. void updateProduct(Products** PRODUCTS_LIST);
    2.4. void deleteProduct(Products** PRODUCTS_LIST);
    2.5. void displayProductsDetails(Products* PRODUCTS_LIST);
    2.6. void displayInventory(Products* PRODUCTS_LIST);
    2.7. void checkLowStock(Products* PRODUCTS_LIST);
3. ORDER FUNCTIONS
    3.1. void showCustomerOrders(Orders* ORDERS_LIST)
    3.2. void showCustomerOrders(Orders* ORDERS_LIST)
    3.3. void addToCart(Products** PRODUCTS_LIST, Orders**
           ORDERS_LIST)
    3.4. void viewAndCheckout(Products** PRODUCTS_LIST, Orders**
           ORDERS_LIST)
    3.5. void displayOrderDetails(Orders* ORDERS_LIST)
4. FILE INPUT AND OUTPUT FUNCTIONS
    4.1. void displayOrderDetails(Orders* ORDERS_LIST)
    4.2. void saveData(Products* PRODUCTS_LIST)
    4.3. void saveReceipt(Orders* ORDERS_LIST, string 
           customerName, float total)

--------------MENU SYSTEM AND USER FLOWS----------------

1. Main Menu
    1.1. Manage Inventory
        Runs manageInventory()
    1.2. Manage Orders
        Runs manageOrders()
    1.3. Exit
        Saves the data and terminates the program

2. Manage Inventory
    2.1. Search Product
    2.2. Add Product
    2.3. Update Product
    2.4. Delete Product
    2.5. Display All Products Details
    2.6. Display In-Store Inventory 
    2.7. Back to Main Menu

3. Manage Orders
    3.1. Show Customer Ordes
    3.2. Validate Stock Availability
    3.3. Accept and Process Orders
    3.4. Checkout and Auto-Deduct
    3.5. Save Receipt
    3.6. Back to Main Menu

4. Helper Functions
    4.1. Load Data
    4.2. Save Data