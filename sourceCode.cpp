#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// -------------------- Structures --------------------

// Product details
struct Product {
    string id;
    string name;
    string brand;
    string type;
    double price;
    int stockQuantity;
    string dateModified;
};

// Items added to the cart
struct CartItem {
    string productId;
    string productName;
    double price;
    int quantity;
};

// Voucher details
struct Voucher {
    string code;
    double discount;      
    int usesRemaining;    // 0 usesRemaining means it can no longer be used
};

// -------------------- Globals --------------------
string CURRENT_DATE; 

// -------------------- Constants --------------------
// Text files for saving data
const string INVENTORY_FILE = "inventory.txt";
const string VOUCHERS_FILE  = "vouchers.txt";
const string RECEIPT_FILE   = "receipt.txt";

// -------------------- Function Prototypes --------------------
// File handling
void loadInventoryFromFile(vector<Product> &inventory, const string &filename = INVENTORY_FILE);
void saveInventoryToFile(const vector<Product> &inventory, const string &filename = INVENTORY_FILE);
void loadVouchersFromFile(vector<Voucher> &vouchers, const string &filename = VOUCHERS_FILE);
void saveVouchersToFile(const vector<Voucher> &vouchers, const string &filename = VOUCHERS_FILE);

// String helpers
string toUpper(const string &s);
string toLower(const string &s);
void splitLine(const string &line, char delim, vector<string> &parts);
void parseMultipleIDs(const string &input, vector<string> &outIds);

// Display and inventory actions
void displayDashboard(const vector<Product> &inventory);
void checkAndDisplayLowStock(const vector<Product> &inventory);
void displayAllProducts(const vector<Product> &inventory);
void searchProducts(vector<Product> &inventory, vector<Voucher> &vouchers);
bool addNewProductInteractive(vector<Product> &inventory);
void updateProductByIndex(vector<Product> &inventory, size_t idx);
void deleteProductsInteractive(vector<Product> &inventory);

// Voucher menu
void voucherManagementMenu(vector<Voucher> &vouchers);

// Order processing
void generateReceiptFile(const string &customer_name, const string &order_date, const string &customer_address,
                         const vector<CartItem> &cart, double subtotal, const string &voucherCode,
                         double discountAmount, double finalTotal, const string &paymentMethod,
                         const string &filename = RECEIPT_FILE);
void orderMenu(vector<Product> &inventory, vector<Voucher> &activeVouchers);

// Main menu for inventory
void inventoryMenu(vector<Product> &inventory, vector<Voucher> &vouchers);

// -------------------- main --------------------
int main() {
    vector<Product> inventory;
    vector<Voucher> activeVouchers;

    // Get date at startup
    cout << "Enter current date (YYYY-MM-DD): ";
    getline(cin, CURRENT_DATE);
    
    if (CURRENT_DATE.empty())
 
    // Load saved data
    loadInventoryFromFile(inventory);
    loadVouchersFromFile(activeVouchers);

    while (true) {
        displayDashboard(inventory);

        cout << "================================================\n";
        cout << "        RETAIL ENTERPRISE SYSTEM MAIN MENU       \n";
        cout << "================================================\n";
        cout << "1. Inventory Management System\n";
        cout << "2. Customer Order Processing\n";
        cout << "3. Exit Program\n";
        cout << "Choice: ";

        int main_choice;
        // Check if input is a valid number
        if (!(cin >> main_choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: Invalid selection choice!\n";
            continue;
        }
        cin.ignore();

        if (main_choice == 1) {
            inventoryMenu(inventory, activeVouchers);
        } else if (main_choice == 2) {
            orderMenu(inventory, activeVouchers);
        } else if (main_choice == 3) {
            cout << "Terminating process session. Goodbye!\n";
            break;
        } else {
            cout << "Error: Invalid selection choice!\n";
        }
    }

    return 0;
}

// -------------------- Implementations --------------------

// -------------------- Utilities --------------------

// Convert string to uppercase
string toUpper(const string &s) {
    string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c >= 'a' && c <= 'z') c = char(c - ('a' - 'A'));
        out.push_back(c);
    }
    return out;
}

// Convert string to lowercase
string toLower(const string &s) {
    string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c >= 'A' && c <= 'Z') c = char(c + ('a' - 'A'));
        out.push_back(c);
    }
    return out;
}

// Split a line from the text file into parts
void splitLine(const string &line, char delim, vector<string> &parts) {
    parts.clear();
    size_t start = 0;
    while (start <= line.size()) {
        size_t pos = line.find(delim, start);
        if (pos == string::npos) {
            parts.push_back(line.substr(start));
            break;
        } else {
            parts.push_back(line.substr(start, pos - start));
            start = pos + 1;
        }
    }
}

// Extract product IDs from a comma-separated input
void parseMultipleIDs(const string &input, vector<string> &outIds) {
    outIds.clear();
    size_t start = 0;
    while (start < input.size()) {
        // Skip extra spaces and commas
        while (start < input.size() && (input[start] == ' ' || input[start] == ',')) ++start;
        if (start >= input.size()) break;
        
        size_t pos = start;
        while (pos < input.size() && input[pos] != ',') ++pos;
        string id = input.substr(start, pos - start);
        
        // Remove spaces around the ID
        size_t a = 0;
        while (a < id.size() && id[a] == ' ') ++a;
        
        size_t b = id.size();
        while (b > a && id[b-1] == ' ') --b;
        
        if (b > a) outIds.push_back(id.substr(a, b - a));
        start = pos + 1;
    }
}

// -------------------- File I/O --------------------

// Save inventory vector to text file using '|' as separator
void saveInventoryToFile(const vector<Product> &inventory, const string &filename) {
    ofstream ofs(filename.c_str());
    if (!ofs) return;
    for (size_t i = 0; i < inventory.size(); ++i) {
        const Product &p = inventory[i];
        ofs << p.id << '|' << p.type << '|' << p.brand << '|' << p.name << '|'
            << fixed << setprecision(2) << p.price << '|' << p.stockQuantity << '|' << p.dateModified << '\n';
    }
}

// Read inventory text file and add items to vector
void loadInventoryFromFile(vector<Product> &inventory, const string &filename) {
    inventory.clear();
    ifstream ifs(filename.c_str());
    if (!ifs) return;
    string line;
    vector<string> parts;
    
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        splitLine(line, '|', parts);
        if (parts.size() < 7) continue; 
        
        Product p;
        p.id            = parts[0];
        p.type          = parts[1];
        p.brand         = parts[2];
        p.name          = parts[3];
        // Convert strings to numbers safely
        try { p.price         = stod(parts[4]); } catch (...) { p.price = 0.0; }
        try { p.stockQuantity = stoi(parts[5]); } catch (...) { p.stockQuantity = 0; }
        p.dateModified  = parts[6];
        inventory.push_back(p);
    }
}

// Save vouchers to file
void saveVouchersToFile(const vector<Voucher> &vouchers, const string &filename) {
    ofstream ofs(filename.c_str());
    if (!ofs) return;
    for (size_t i = 0; i < vouchers.size(); ++i) {
        ofs << vouchers[i].code << '|'
            << vouchers[i].discount << '|'
            << vouchers[i].usesRemaining << '\n';
    }
}

// Read vouchers from file
void loadVouchersFromFile(vector<Voucher> &vouchers, const string &filename) {
    vouchers.clear();
    ifstream ifs(filename.c_str());
    if (!ifs) return;
    string line;
    vector<string> parts;
    
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        splitLine(line, '|', parts);
        if (parts.size() < 3) continue;
        
        Voucher v;
        v.code = parts[0];
        try { v.discount      = stod(parts[1]); } catch (...) { v.discount = 0.0; }
        try { v.usesRemaining = stoi(parts[2]); } catch (...) { v.usesRemaining = 0; }
        vouchers.push_back(v);
    }
}

// -------------------- Display / Inventory ops --------------------

// Show quick stats about the current inventory
void displayDashboard(const vector<Product> &inventory) {
    cout << "\n================================================\n";
    cout << "               INVENTORY DASHBOARD              \n";
    cout << "================================================\n";
    
    int total_unique_items      = static_cast<int>(inventory.size());
    int total_stock_quantity    = 0;
    double total_inventory_value = 0.0;
    int low_stock_alerts_count  = 0;
    
    for (size_t i = 0; i < inventory.size(); ++i) {
        total_stock_quantity   += inventory[i].stockQuantity;
        total_inventory_value  += inventory[i].price * inventory[i].stockQuantity;
        if (inventory[i].stockQuantity < 5) ++low_stock_alerts_count;
    }
    
    cout << "📦 Total Unique Products: " << total_unique_items << '\n';
    cout << "🧮 Total Stock Volume   : " << total_stock_quantity << " units\n";
    cout << "💰 Total Asset Valuation : P" << fixed << setprecision(2) << total_inventory_value << '\n';
    cout << "⚠️ Low Stock Flagged Items: " << low_stock_alerts_count << '\n';
    cout << "------------------------------------------------\n";
    
    if (low_stock_alerts_count > 0) checkAndDisplayLowStock(inventory);
}

// Print items that have less than 5 stock
void checkAndDisplayLowStock(const vector<Product> &inventory) {
    cout << "[LOW STOCK ALERTS]\n";
    int low_stock_count = 0;
    for (size_t i = 0; i < inventory.size(); ++i) {
        if (inventory[i].stockQuantity < 5) {
            cout << "⚠️ WARNING: " << inventory[i].name <<" is low on stock! (" << inventory[i].stockQuantity << " left)\n";
            ++low_stock_count;
        }
    }
    if (low_stock_count == 0) cout << "All stocks are good.\n";
    cout << "------------------------------------------------\n";
}

// Print a list of all products
void displayAllProducts(const vector<Product> &inventory) {
    cout << "\n--- COMPLETE INVENTORY TABLE ---\n";
    for (size_t i = 0; i < inventory.size(); ++i) {
        const Product &p = inventory[i];
        cout << "ID: " << p.id << " | " << p.type << " | [" << p.brand << "] " << p.name
             << " | Price: P" << fixed << setprecision(2) << p.price
             << " | Stock: " << p.stockQuantity << '\n';
    }
}

// Search for a product and give options to modify or delete it
void searchProducts(vector<Product> &inventory, vector<Voucher> &vouchers) {
    cout << "Search: ";
    string keyword;
    getline(cin, keyword);
    string keyLower = toLower(keyword);
    
    cout << "\nResults for [" << keyword << "]:\n";
    cout << "------------------------------------------------\n";
    vector<size_t> foundIndexes;
    
    // Check ID, name, brand, and type
    for (size_t i = 0; i < inventory.size(); ++i) {
        const Product &p = inventory[i];
        if (toLower(p.id).find(keyLower)    != string::npos ||
            toLower(p.name).find(keyLower)  != string::npos ||
            toLower(p.brand).find(keyLower) != string::npos ||
            toLower(p.type).find(keyLower)  != string::npos) {
            
            cout << foundIndexes.size()+1 << ". ID: " << p.id << " | " << p.name << " (" << p.brand << ") | P"
                 << fixed << setprecision(2) << p.price << " | Stock: " << p.stockQuantity << '\n';
            foundIndexes.push_back(i);
        }
    }
    
    if (foundIndexes.empty()) {
        cout << "No matching products found.\n";
        cout << "------------------------------------------------\n";
        return;
    }
    cout << "------------------------------------------------\n";

    int opt = 0;
    while (true) {
        cout << "Options: 1. Modify  2. Delete  3. Back to Menu\n";
        cout << "Choice: ";
        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter 1, 2, or 3.\n";
            continue;
        }
        cin.ignore();
        if (opt >= 1 && opt <= 3) break;
        cout << "Invalid selection. Please enter 1, 2, or 3.\n";
    }

    if (opt == 1) {
        // Edit search result
        int sel = 0;
        while (true) {
            cout << "Enter result number to modify: ";
            if (!(cin >> sel)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Enter a valid result number.\n";
                continue;
            }
            cin.ignore();
            if (sel >= 1 && static_cast<size_t>(sel) <= foundIndexes.size()) break;
            cout << "Selection out of range. Try again.\n";
        }
        size_t idx = foundIndexes[sel - 1];
        updateProductByIndex(inventory, idx);
        saveInventoryToFile(inventory);
        
    } else if (opt == 2) {
        // Delete search result
        cout << "Enter product ID(s) to delete (comma separated) or enter result numbers (prefix '#') separated by commas.\n";
        cout << "Examples: P002,P003  or  #1,#3\n";
        cout << "Input: ";
        string delInput;
        getline(cin, delInput);
        if (delInput.empty()) { cout << "No input provided.\n"; return; }
        
        bool usedNumbers = false;
        for (size_t i = 0; i < delInput.size(); ++i) if (delInput[i] == '#') { usedNumbers = true; break; }
        
        vector<string> tokens;
        parseMultipleIDs(delInput, tokens);
        
        if (usedNumbers) {
            vector<size_t> toRemoveIdx;
            for (size_t t = 0; t < tokens.size(); ++t) {
                string tok = tokens[t];
                if (!tok.empty() && tok[0] == '#') tok = tok.substr(1);
                try {
                    int num = stoi(tok);
                    if (num >= 1 && static_cast<size_t>(num) <= foundIndexes.size())
                        toRemoveIdx.push_back(foundIndexes[num - 1]);
                } catch (...) { }
            }
            
            // Remove duplicates
            for (size_t i = 0; i < toRemoveIdx.size(); ++i)
                for (size_t j = i + 1; j < toRemoveIdx.size(); ++j)
                    if (toRemoveIdx[i] == toRemoveIdx[j]) { toRemoveIdx.erase(toRemoveIdx.begin() + j); --j; }
                    
            // Sort descending so deletion doesn't mess up indexes
            for (size_t i = 0; i < toRemoveIdx.size(); ++i)
                for (size_t j = i + 1; j < toRemoveIdx.size(); ++j)
                    if (toRemoveIdx[i] < toRemoveIdx[j]) swap(toRemoveIdx[i], toRemoveIdx[j]);
                    
            for (size_t k = 0; k < toRemoveIdx.size(); ++k) {
                size_t ridx = toRemoveIdx[k];
                if (ridx < inventory.size()) {
                    cout << "Confirm delete " << inventory[ridx].id << " - " << inventory[ridx].name << " (Y/N): ";
                    string conf; getline(cin, conf);
                    conf = toUpper(conf);
                    if (conf == "Y" || conf == "YES") {
                        cout << "Deleting: " << inventory[ridx].id << " - " << inventory[ridx].name << '\n';
                        inventory.erase(inventory.begin() + ridx);
                    } else {
                        cout << "Skipped: " << inventory[ridx].id << '\n';
                    }
                }
            }
            saveInventoryToFile(inventory);

        } else {
            // Delete using exact IDs
            vector<string> ids;
            for (size_t t = 0; t < tokens.size(); ++t) if (!tokens[t].empty()) ids.push_back(tokens[t]);
            if (ids.empty()) { cout << "No valid IDs found.\n"; return; }
            
            for (size_t i = 0; i < ids.size(); ++i) {
                bool removed = false;
                for (size_t j = 0; j < inventory.size(); ++j) {
                    if (inventory[j].id == ids[i]) {
                        cout << "Confirm delete " << inventory[j].id << " - " << inventory[j].name << " (Y/N): ";
                        string conf; getline(cin, conf);
                        conf = toUpper(conf);
                        if (conf == "Y" || conf == "YES") {
                            cout << "Deleting: " << inventory[j].id << " - " << inventory[j].name << '\n';
                            inventory.erase(inventory.begin() + j);
                        } else {
                            cout << "Skipped: " << inventory[j].id << '\n';
                        }
                        removed = true;
                        break;
                    }
                }
                if (!removed) cout << "ID not found: " << ids[i] << '\n';
            }
            saveInventoryToFile(inventory);
        }
    }
}

// Edit fields of a specific product
void updateProductByIndex(vector<Product> &inventory, size_t idx) {
    if (idx >= inventory.size()) return;
    Product &p = inventory[idx];
    
    while (true) {
        cout << "\nEditing: " << p.id << " | " << p.name << '\n';
        cout << "Field to modify:\n";
        cout << "1. Product ID\n";
        cout << "2. Type\n";
        cout << "3. Brand\n";
        cout << "4. Name\n";
        cout << "5. Price\n";
        cout << "6. Stock Quantity\n";
        cout << "7. Done\n";
        cout << "Choice: ";
        
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        cin.ignore();
        
        if      (choice == 1) { cout << "New Product ID: ";      getline(cin, p.id);    cout << "Updated.\n"; }
        else if (choice == 2) { cout << "New Type: ";    getline(cin, p.type);  cout << "Updated.\n"; }
        else if (choice == 3) { cout << "New Brand: ";   getline(cin, p.brand); cout << "Updated.\n"; }
        else if (choice == 4) { cout << "New Name: ";    getline(cin, p.name);  cout << "Updated.\n"; }
        else if (choice == 5) {
            cout << "New Price: ";
            if (!(cin >> p.price)) { cin.clear(); cin.ignore(10000,'\n'); cout << "Invalid price.\n"; }
            else { cin.ignore(); cout << "Updated.\n"; }
        } else if (choice == 6) {
            cout << "New Stock Quantity: ";
            if (!(cin >> p.stockQuantity)) { cin.clear(); cin.ignore(10000,'\n'); cout << "Invalid quantity.\n"; }
            else { cin.ignore(); cout << "Updated.\n"; }
        } else if (choice == 7) {
            p.dateModified = CURRENT_DATE;
            cout << "Done editing. Date modified updated to " << CURRENT_DATE << '\n';
            break;
        } else {
            cout << "Invalid selection.\n";
        }
    }
}

// Delete product directly without searching
void deleteProductsInteractive(vector<Product> &inventory) {
    cout << "Delete options:\n";
    cout << "1. Delete single product by ID\n";
    cout << "2. Delete multiple products by IDs (comma separated)\n";
    cout << "3. Delete by search results (use Search first)\n";
    cout << "Choice: ";
    int c;
    if (!(cin >> c)) { cin.clear(); cin.ignore(10000,'\n'); cout << "Invalid input.\n"; return; }
    cin.ignore();
    
    if (c == 1) {
        cout << "Enter ID to delete: ";
        string id; getline(cin, id);
        for (size_t i = 0; i < inventory.size(); ++i) {
            if (inventory[i].id == id) {
                cout << "Confirm delete " << inventory[i].id << " - " << inventory[i].name << " (Y/N): ";
                string conf; getline(cin, conf);
                conf = toUpper(conf);
                if (conf == "Y" || conf == "YES") {
                    inventory.erase(inventory.begin() + i);
                    cout << "Product deleted.\n";
                    saveInventoryToFile(inventory);
                } else {
                    cout << "Deletion cancelled.\n";
                }
                return;
            }
        }
        cout << "Product ID not found.\n";
        
    } else if (c == 2) {
        cout << "Enter IDs separated by comma: ";
        string line; getline(cin, line);
        vector<string> ids; parseMultipleIDs(line, ids);
        for (size_t k = 0; k < ids.size(); ++k) {
            bool removed = false;
            for (size_t j = 0; j < inventory.size(); ++j) {
                if (inventory[j].id == ids[k]) {
                    cout << "Confirm delete " << inventory[j].id << " - " << inventory[j].name << " (Y/N): ";
                    string conf; getline(cin, conf);
                    conf = toUpper(conf);
                    if (conf == "Y" || conf == "YES") {
                        inventory.erase(inventory.begin() + j);
                        cout << "Deleted: " << ids[k] << '\n';
                    } else {
                        cout << "Skipped: " << ids[k] << '\n';
                    }
                    removed = true;
                    break;
                }
            }
            if (!removed) cout << "ID not found: " << ids[k] << '\n';
        }
        saveInventoryToFile(inventory);
        
    } else {
        cout << "Please go back and use the Search option.\n";
    }
}

// -------------------- Voucher management --------------------
// Menu for managing discount vouchers
void voucherManagementMenu(vector<Voucher> &vouchers) {
    while (true) {
        cout << "\n--- Voucher Management ---\n";
        cout << "1. View Vouchers\n";
        cout << "2. Add Voucher\n";
        cout << "3. Edit Voucher\n";
        cout << "4. Delete Voucher\n";
        cout << "5. Back to Inventory Menu\n";
        cout << "Choice: ";
        int ch;
        if (!(cin >> ch)) { cin.clear(); cin.ignore(10000,'\n'); cout << "Invalid input.\n"; continue; }
        cin.ignore();

         if (ch == 1) {
            // Counts how many uses are left (0 = expired) 
            cout << "\nVouchers:\n";
            if (vouchers.empty()) { cout << "No vouchers on record.\n"; continue; }
            for (size_t i = 0; i < vouchers.size(); ++i) {
                cout << i+1 << ". " << vouchers[i].code
                     << " (" << static_cast<int>(vouchers[i].discount * 100) << "% off)"
                     << " | Uses remaining: " << vouchers[i].usesRemaining;
                if (vouchers[i].usesRemaining == 0) cout << " [EXHAUSTED]";
                cout << '\n';
            }

        } else if (ch == 2) {
            Voucher v;
            cout << "Enter voucher code: "; getline(cin, v.code);
            cout << "Discount percent (e.g., 20): ";
            double pct;
            if (!(cin >> pct)) { cin.clear(); cin.ignore(10000,'\n'); cout << "Invalid percent.\n"; continue; }
            cin.ignore();
            v.discount = pct / 100.0;

            cout << "Enter number of uses allowed: ";
            if (!(cin >> v.usesRemaining) || v.usesRemaining < 1) {
                cin.clear(); cin.ignore(10000,'\n');
                cout << "Invalid input. Defaulting to 1 use.\n";
                v.usesRemaining = 1;
            } else {
                cin.ignore();
            }

            vouchers.push_back(v);
            saveVouchersToFile(vouchers);
            cout << "Voucher [" << v.code << "] added with " << v.usesRemaining << " use(s).\n";

        } else if (ch == 3) {
            cout << "Voucher code to edit: ";
            string code; getline(cin, code);
            bool found = false;
            for (size_t i = 0; i < vouchers.size(); ++i) {
                if (toUpper(vouchers[i].code) == toUpper(code)) {
                    found = true;
                     cout << "Editing: " << vouchers[i].code
                         << " | " << static_cast<int>(vouchers[i].discount * 100) << "% off"
                         << " | Remaining Uses: " << vouchers[i].usesRemaining << '\n';
                    cout << "1. Change code\n";
                    cout << "2. Change discount\n";
                    cout << "3. Change remaining uses\n";
                    cout << "Choice: ";
                    int e;
                    if (!(cin >> e)) { cin.clear(); cin.ignore(10000,'\n'); cout << "Invalid.\n"; break; }
                    cin.ignore();
                    if (e == 1) {
                        cout << "New code: "; getline(cin, vouchers[i].code);
                        cout << "Code updated.\n";
                    } else if (e == 2) {
                        cout << "New percent: ";
                        double pct;
                        if (!(cin >> pct)) { cin.clear(); cin.ignore(10000,'\n'); cout << "Invalid.\n"; }
                        else { vouchers[i].discount = pct / 100.0; cin.ignore(); cout << "Discount updated.\n"; }
                    } else if (e == 3) {
                        cout << "New remaining uses: ";
                        int newUses;
                        if (!(cin >> newUses) || newUses < 0) {
                            cin.clear(); cin.ignore(10000,'\n');
                            cout << "Invalid input. No changes made.\n"; 
                        } else {
                            vouchers[i].usesRemaining = newUses;
                            cin.ignore();
                            cout << "Uses remaining updated to " << newUses << ".\n";
                        }
                    } else {
                        cout << "Invalid choice.\n";
                    }
                    saveVouchersToFile(vouchers);
                    break;
                }
            }
            if (!found) cout << "Voucher not found.\n";

        } else if (ch == 4) {
            cout << "Voucher code to delete: ";
            string code; getline(cin, code);
            bool removed = false;
            for (size_t i = 0; i < vouchers.size(); ++i) {
                if (toUpper(vouchers[i].code) == toUpper(code)) {
                      cout << "Confirm delete voucher " << vouchers[i].code << " (Y/N): ";
                    string conf; getline(cin, conf);
                    conf = toUpper(conf);
                    if (conf == "Y" || conf == "YES") {
                        vouchers.erase(vouchers.begin() + i);
                        removed = true;
                        saveVouchersToFile(vouchers);
                        cout << "Voucher deleted.\n";
                    } else {
                        cout << "Deletion Cancelled.\n";
                    }
                    break;
                }
            }
            if (!removed) cout << "Voucher not found.\n";

        } else if (ch == 5) {
            return;
        } else {
            cout << "Invalid selection.\n";
        }
    }
}

// -------------------- Order processing --------------------

// Write the receipt details to a text file
void generateReceiptFile(const string &customer_name, const string &order_date, const string &customer_address,
                         const vector<CartItem> &cart, double subtotal, const string &voucherCode,
                         double discountAmount, double finalTotal, const string &paymentMethod,
                         const string &filename) {
    ofstream ofs(filename.c_str());
    if (!ofs) { cout << "Error: Unable to write receipt file.\n"; return; }
    
    ofs << "========================================\n";
    ofs << "         OFFICIAL TRANSACTION RECEIPT   \n";
    ofs << "========================================\n";
    ofs << "Customer : " << customer_name << '\n';
    ofs << "Date     : " << order_date    << '\n';
    ofs << "Address  : " << customer_address << '\n';
    ofs << "----------------------------------------\n";
    
    for (size_t i = 0; i < cart.size(); ++i) {
        ofs << cart[i].productName << " (x" << cart[i].quantity << ") - P"
            << fixed << setprecision(2) << (cart[i].price * cart[i].quantity) << '\n';
    }
    
    ofs << "----------------------------------------\n";
    ofs << "Subtotal Cash    : P" << fixed << setprecision(2) << subtotal << '\n';
    ofs << "Voucher Used : " << voucherCode << " (-P" << fixed << setprecision(2) << discountAmount << ")\n";
    ofs << "Final Payment : P" << fixed << setprecision(2) << finalTotal << '\n';
    ofs << "Payment Mode : " << paymentMethod << '\n';
    ofs << "========================================\n";
}

// Handle adding to cart and checkout
void orderMenu(vector<Product> &inventory, vector<Voucher> &activeVouchers) {
    vector<CartItem> temporary_cart;
    double applied_discount      = 0.0;
    string applied_voucher_code  = "NONE";

    string customer_name, order_date, customer_address;
    cout << "\nEnter Customer Name: "; getline(cin, customer_name);
    cout << "Enter Order Date   : "; getline(cin, order_date);
    if (order_date.empty()) order_date = CURRENT_DATE;
    cout << "Enter Delivery Address      : "; getline(cin, customer_address);

    while (true) {
        cout <<  "\nActions: 1. Add Product to Cart | 2. Edit Cart | 3. Apply Voucher | 4. Checkout | 5. Terminate Order\n";
        cout << "Choice: ";
        int order_action;
        if (!(cin >> order_action)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Returning to order menu.\n";
            continue;
        }
        cin.ignore();

        if (order_action == 1) {
            string search_id;
            cout << "Product ID: ";
            getline(cin, search_id);
            bool item_found = false;
            size_t foundIndex = 0;
            
            for (size_t i = 0; i < inventory.size(); ++i) {
                if (inventory[i].id == search_id) {
                    item_found = true;
                    foundIndex = i;
                    const Product &p = inventory[i];
                    cout << "\nItem Found:\n";
                    cout << "Name : " << p.name  << '\n';
                    cout << "Brand: " << p.brand << '\n';
                    cout << "Stock: " << p.stockQuantity << '\n';
                    cout << "----------------------------------------\n";
                    break;
                }
            }
            if (!item_found) {
                cout << "Transaction Error. Product ID not found.\n";
                continue;
            }
            
            int purchase_qty;
            while (true) {
                cout << "Quantity to purchase: ";
                if (!(cin >> purchase_qty)) {
                    cin.clear(); cin.ignore(10000, '\n');
                    cout << "Invalid quantity input. Enter a numeric value.\n";
                    continue;
                }
                cin.ignore();
                if (purchase_qty <= 0) { cout << "Quantity must be at least 1. Try again.\n"; continue; }
                if (purchase_qty > inventory[foundIndex].stockQuantity) {
                    cout << "Insufficient stock. Available: " << inventory[foundIndex].stockQuantity << ". Enter a smaller quantity.\n";
                    continue;
                }
                break;
            }
            
            // Add or merge to cart
            bool merged = false;
            for (size_t i = 0; i < temporary_cart.size(); ++i) {
                if (temporary_cart[i].productId == inventory[foundIndex].id) {
                    temporary_cart[i].quantity += purchase_qty;
                    merged = true;
                    break;
                }
            }
            if (!merged)
                temporary_cart.push_back({inventory[foundIndex].id, inventory[foundIndex].name, inventory[foundIndex].price, purchase_qty});
            cout << "Added to cart: " << inventory[foundIndex].name << " | " << inventory[foundIndex].brand << " | Qty: " << purchase_qty << '\n';

        } else if (order_action == 2) {
            if (temporary_cart.empty()) { cout << "Cart is empty.\n"; continue; }
            while (true) {
                cout << "\n--- Edit Cart ---\n";
                for (size_t i = 0; i < temporary_cart.size(); ++i)
                    cout << i+1 << ". " << temporary_cart[i].productName
                         << " | Qty: " << temporary_cart[i].quantity
                         << " | P" << fixed << setprecision(2) << temporary_cart[i].price << '\n';
                cout << "Options: 1. Change quantity  2. Remove item  3. Done Editing\n";
                cout << "Choice: ";
                int ec;
                if (!(cin >> ec)) { cin.clear(); cin.ignore(10000,'\n'); cout << "Invalid input.\n"; continue; }
                cin.ignore();
                
                if (ec == 1) {
                    int sel;
                    cout << "Item number to edit: ";
                    if (!(cin >> sel)) { cin.clear(); cin.ignore(10000,'\n'); cout << "Invalid input.\n"; continue; }
                    cin.ignore();
                    if (sel < 1 || static_cast<size_t>(sel) > temporary_cart.size()) { cout << "Out of range.\n"; continue; }
                    
                    int newQty;
                    while (true) {
                        cout << "Enter new quantity: ";
                        if (!(cin >> newQty)) { cin.clear(); cin.ignore(10000,'\n'); cout << "Invalid input.\n"; continue; }
                        cin.ignore();
                        if (newQty <= 0) { cout << "Quantity must be at least 1.\n"; continue; }
                        
                        string pid = temporary_cart[sel-1].productId;
                        int stock = 0;
                        for (size_t k = 0; k < inventory.size(); ++k)
                            if (inventory[k].id == pid) { stock = inventory[k].stockQuantity; break; }
                        if (newQty > stock) { cout << "Insufficient stock. Available: " << stock << ".\n"; continue; }
                        break;
                    }
                    temporary_cart[sel-1].quantity = newQty;
                    cout << "Quantity updated.\n";
                    
                } else if (ec == 2) {
                    int sel;
                    cout << "Item number to remove: ";
                    if (!(cin >> sel)) { cin.clear(); cin.ignore(10000,'\n'); cout << "Invalid input.\n"; continue; }
                    cin.ignore();
                    if (sel < 1 || static_cast<size_t>(sel) > temporary_cart.size()) { cout << "Selection out of range.\n"; continue; }
                    
                    cout << "Confirm remove " << temporary_cart[sel-1].productName << " (Y/N): ";
                    string conf; getline(cin, conf);
                    conf = toUpper(conf);
                    if (conf == "Y" || conf == "YES") {
                        temporary_cart.erase(temporary_cart.begin() + (sel-1));
                        cout << "Item removed.\n";
                    }
                } else if (ec == 3) {
                    break;
                } else {
                    cout << "Invalid choice.\n";
                }
            }

        } else if (order_action == 3) {
            string input_code;
            cout << "Enter Promo Voucher Code: ";
            getline(cin, input_code);
            input_code = toUpper(input_code);
            bool voucher_found = false;
            
            for (size_t i = 0; i < activeVouchers.size(); ++i) {
                if (toUpper(activeVouchers[i].code) == input_code) {
                    voucher_found = true;
                    if (activeVouchers[i].usesRemaining <= 0) {
                        cout << "Promo Alert: This voucher has no remaining uses and is no longer active.\n";
                    } else {
                        applied_voucher_code = activeVouchers[i].code;
                        applied_discount     = activeVouchers[i].discount;
                        cout << "Promo Applied: -" << static_cast<int>(applied_discount * 100) << "%"
                             << " (" << activeVouchers[i].usesRemaining << " use(s) remaining before this redemption)\n";
                    }
                    break;
                }
            }
            if (!voucher_found) cout << "Validation Error: Coupon code match failed. Voucher does not exist.\n";

        } else if (order_action == 4) {
            if (temporary_cart.empty()) { cout << "Checkout Error: Shopping cart list holds no items.\n"; continue; }
            double subtotal = 0.0;
            cout << "\n--- CHECKOUT REVIEW LIST ---\n";
            for (size_t i = 0; i < temporary_cart.size(); ++i) {
                cout << "Item: " << temporary_cart[i].productName
                     << " (Qty: " << temporary_cart[i].quantity << ") - P"
                     << fixed << setprecision(2) << (temporary_cart[i].price * temporary_cart[i].quantity) << '\n';
                subtotal += temporary_cart[i].price * temporary_cart[i].quantity;
            }
            
            double total_discount_deduction = subtotal * applied_discount;
            double final_total_price        = subtotal - total_discount_deduction;
            
            cout << "------------------------------------------------\n";
            cout << "Subtotal Summary      : P" << fixed << setprecision(2) << subtotal << '\n';
            cout << "Promo Code Applied    : " << applied_voucher_code
                 << " (-P" << fixed << setprecision(2) << total_discount_deduction << ")\n";
            cout << "Aggregate Net Amount  : P" << fixed << setprecision(2) << final_total_price << '\n';
            cout << "------------------------------------------------\n";
            cout << "Confirm financial checkout? (Y/N): ";
            string checkout_confirm;
            getline(cin, checkout_confirm);
            checkout_confirm = toUpper(checkout_confirm);
            if (checkout_confirm == "Y" || checkout_confirm == "YES") {
                string payment_method;
                cout << "Specify Mode of Payment Option: ";
                getline(cin, payment_method);

                // Make a safe filename for the receipt
                string uniqueFilename = "receipt_" + customer_name + ".txt";
                for (size_t i = 0; i < uniqueFilename.length(); ++i) {
                    char c = uniqueFilename[i];
                    if (c == ' ' || c == '/' || c == '\\' || c == ':' || c == '*' ||
                        c == '?' || c == '"' || c == '<'  || c == '>' || c == '|')
                        uniqueFilename[i] = '_';
                }

                // Update stock quantities
                for (size_t ci = 0; ci < temporary_cart.size(); ++ci) {
                    const CartItem &ciRef = temporary_cart[ci];
                    for (size_t p = 0; p < inventory.size(); ++p) {
                        if (inventory[p].id == ciRef.productId) {
                            inventory[p].stockQuantity -= ciRef.quantity;
                            if (inventory[p].stockQuantity < 0) inventory[p].stockQuantity = 0;
                            inventory[p].dateModified = CURRENT_DATE;
                            break;
                        }
                    }
                }

                // Reduce the number of uses left. If it reaches 0, remove the voucher. 
                if (applied_voucher_code != "NONE") {
                    string usedU = toUpper(applied_voucher_code);
                    for (size_t v = 0; v < activeVouchers.size(); ++v) {
                        if (toUpper(activeVouchers[v].code) == usedU) {
                            activeVouchers[v].usesRemaining -= 1;
                            if (activeVouchers[v].usesRemaining <= 0) {
                                cout << "Voucher [" << applied_voucher_code << "] is fully used up and has been removed.\n";
                                activeVouchers.erase(activeVouchers.begin() + v);
                            } else {
                                cout << "Voucher [" << applied_voucher_code << "] applied! Uses left: " 
                                     << activeVouchers[v].usesRemaining << "\n";
                            }
                            break;
                        }
                    }
                }
                
                generateReceiptFile(customer_name, order_date, customer_address, temporary_cart,
                                    subtotal, applied_voucher_code, total_discount_deduction,
                                    final_total_price, payment_method, uniqueFilename);
                cout << "Transaction saved! Receipt generated as " << uniqueFilename << ".\n";

                applied_discount     = 0.0;
                applied_voucher_code = "NONE";
                saveInventoryToFile(inventory);
                saveVouchersToFile(activeVouchers);
                return;
            } else {
                cout << "Checkout suspended. Returning to current order menu sessions.\n";
            }

        } else if (order_action == 5) {
            temporary_cart.clear();
            cout << "Order cancelled. Your cart has been cleared.\n"; 
            return;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

// -------------------- Inventory Menu --------------------
void inventoryMenu(vector<Product> &inventory, vector<Voucher> &vouchers) {
    while (true) {
        cout << "\n================================================\n";
        cout << "         INVENTORY MANAGEMENT DASHBOARD         \n";
        cout << "================================================\n";
        cout << "1. Search Products (modify/delete from results)\n";
        cout << "2. Add New Product Record\n";
        cout << "3. Update Existing Product Info (direct)\n";
        cout << "4. Delete Product Record(s)\n";
        cout << "5. Display All Current Products\n";
        cout << "6. Voucher Management\n";
        cout << "7. Return to Main Menu\n";
        cout << "Choice: ";
        
        int inv_action;
        if (!(cin >> inv_action)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Returning to menu.\n";
            continue;
        }
        cin.ignore();
        
        if (inv_action == 1) {
            searchProducts(inventory, vouchers);
        } else if (inv_action == 2) {
            bool dummy = addNewProductInteractive(inventory);
            (void)dummy;
            saveInventoryToFile(inventory);
        } else if (inv_action == 3) {
            cout << "Enter Product ID to modify directly: ";
            string id; getline(cin, id);
            bool found = false;
            for (size_t i = 0; i < inventory.size(); ++i) {
                if (inventory[i].id == id) {
                    found = true;
                    updateProductByIndex(inventory, i);
                    saveInventoryToFile(inventory);
                    break;
                }
            }
            if (!found) cout << "Product ID not found.\n";
        } else if (inv_action == 4) {
            deleteProductsInteractive(inventory);
        } else if (inv_action == 5) {
            displayAllProducts(inventory);
        } else if (inv_action == 6) {
            voucherManagementMenu(vouchers);
        } else if (inv_action == 7) {
            saveInventoryToFile(inventory);
            saveVouchersToFile(vouchers);
            return;
        } else {
            cout << "Error: Invalid selection choice!\n";
        }
    }
}

// -------------------- Add product --------------------
// Prompt user to enter details for a new product
bool addNewProductInteractive(vector<Product> &inventory) {
    while (true) {
        Product p;
        cout << "Enter New Product Details:\n";
        cout << "Product ID: ";    getline(cin, p.id);
        cout << "Type: ";  getline(cin, p.type);
        cout << "Brand: "; getline(cin, p.brand);
        cout << "Name: ";  getline(cin, p.name);
        cout << "Price: ";
        
        if (!(cin >> p.price)) { cin.clear(); cin.ignore(10000,'\n'); p.price = 0.0; }
        cout << "Stock Quantity: ";
        if (!(cin >> p.stockQuantity)) { cin.clear(); cin.ignore(10000,'\n'); p.stockQuantity = 0; }
        cin.ignore();
        
        p.dateModified = CURRENT_DATE;
        inventory.push_back(p);
        cout << "Product added: " << p.id << " - " << p.name << '\n';

        cout << "Options: 1. Add another product  2. Back to Menu\n";
        cout << "Choice: ";
        int opt;
        if (!(cin >> opt)) { cin.clear(); cin.ignore(10000,'\n'); cout << "Invalid input. Returning to Inventory Menu.\n"; return false; }
        cin.ignore();

        if (opt == 1) continue;
        return false;
    }
}