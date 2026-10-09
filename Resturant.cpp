#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
using namespace std;


const int MAX_MENU   = 100;
const int MAX_CUST   = 100;
const int MAX_ORDERS = 200;
const int MAX_ITEMS_PER_ORDER = 20;

struct MenuItem {
    int    id;
    char   name[50];
    char   category[30];
    float  price;
    int    quantity;
    int    totalSold;
};

struct Customer {
    int  id;
    char name[50];
    char contact[15];
    char address[100];
    int  totalOrders;
};

struct OrderItem {
    int menuId;
    char foodName[50];
    int  qty;
    float unitPrice;
};

struct Order {
    int       orderId;
    int       customerId;
    char      customerName[50];
    OrderItem items[MAX_ITEMS_PER_ORDER];
    int       itemCount;
    float     subtotal;
    float     deliveryCharge;
    float     discount;
    float     total;
    int       cancelled;
};



MenuItem menu[MAX_MENU];
int      menuCount = 0;

Customer customers[MAX_CUST];
int      custCount = 0;

Order    orders[MAX_ORDERS];
int      orderCount = 0;

int nextMenuId   = 1001;
int nextCustId   = 2001;
int nextOrderId  = 3001;








void clearScreen() {
    cout << "\n\n";
}

void pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void printLine(char c = '-', int n = 60) {
    for (int i = 0; i < n; i++) cout << c;
    cout << "\n";
}

// ---------------------------------------------
//  FILE HANDLING
// ---------------------------------------------
void saveMenu() {
    ofstream f("menu.txt");
    f << menuCount << "\n" << nextMenuId << "\n";
    for (int i = 0; i < menuCount; i++) {
        MenuItem* m = &menu[i];
        f << m->id << "\n"
          << m->name << "\n"
          << m->category << "\n"
          << m->price << "\n"
          << m->quantity << "\n"
          << m->totalSold << "\n";
    }
    f.close();
}

void loadMenu() {
    ifstream f("menu.txt");
    if (!f) return;
    f >> menuCount >> nextMenuId;
    f.ignore();
    for (int i = 0; i < menuCount; i++) {
        MenuItem* m = &menu[i];
        f >> m->id;
        f.ignore();
        f.getline(m->name, 50);
        f.getline(m->category, 30);
        f >> m->price >> m->quantity >> m->totalSold;
        f.ignore();
    }
    f.close();
}

void saveCustomers() {
    ofstream f("customers.txt");
    f << custCount << "\n" << nextCustId << "\n";
    for (int i = 0; i < custCount; i++) {
        Customer* c = &customers[i];
        f << c->id << "\n"
          << c->name << "\n"
          << c->contact << "\n"
          << c->address << "\n"
          << c->totalOrders << "\n";
    }
    f.close();
}

void loadCustomers() {
    ifstream f("customers.txt");
    if (!f) return;
    f >> custCount >> nextCustId;
    f.ignore();
    for (int i = 0; i < custCount; i++) {
        Customer* c = &customers[i];
        f >> c->id;
        f.ignore();
        f.getline(c->name, 50);
        f.getline(c->contact, 15);
        f.getline(c->address, 100);
        f >> c->totalOrders;
        f.ignore();
    }
    f.close();
}

void saveOrders() {
    ofstream f("orders.txt");
    f << orderCount << "\n" << nextOrderId << "\n";
    for (int i = 0; i < orderCount; i++) {
        Order* o = &orders[i];
        f << o->orderId << "\n"
          << o->customerId << "\n"
          << o->customerName << "\n"
          << o->itemCount << "\n";
        for (int j = 0; j < o->itemCount; j++) {
            f << o->items[j].menuId << "\n"
              << o->items[j].foodName << "\n"
              << o->items[j].qty << "\n"
              << o->items[j].unitPrice << "\n";
        }
        f << o->subtotal << "\n"
          << o->deliveryCharge << "\n"
          << o->discount << "\n"
          << o->total << "\n"
          << o->cancelled << "\n";
    }
    f.close();
}

void loadOrders() {
    ifstream f("orders.txt");
    if (!f) return;
    f >> orderCount >> nextOrderId;
    f.ignore();
    for (int i = 0; i < orderCount; i++) {
        Order* o = &orders[i];
        f >> o->orderId >> o->customerId;
        f.ignore();
        f.getline(o->customerName, 50);
        f >> o->itemCount;
        f.ignore();
        for (int j = 0; j < o->itemCount; j++) {
            f >> o->items[j].menuId;
            f.ignore();
            f.getline(o->items[j].foodName, 50);
            f >> o->items[j].qty >> o->items[j].unitPrice;
            f.ignore();
        }
        f >> o->subtotal >> o->deliveryCharge >> o->discount >> o->total >> o->cancelled;
        f.ignore();
    }
    f.close();
}

void exportReport(const char* filename, const char* content) {
    ofstream f(filename);
    f << content;
    f.close();
    cout << "Report exported to " << filename << "\n";
}


void traverseMenu(MenuItem* ptr, int count) {
    printLine();
    cout << "ID\tName\t\t\tCategory\tPrice\tQty\tSold\n";
    printLine();
    for (int i = 0; i < count; i++, ptr++) {
        cout << ptr->id << "\t"
             << ptr->name << "\t\t"
             << ptr->category << "\t"
             << ptr->price << "\t"
             << ptr->quantity << "\t"
             << ptr->totalSold << "\n";
    }
}

void traverseCustomers(Customer* ptr, int count) {
    printLine();
    cout << "ID\tName\t\t\tContact\t\tOrders\n";
    printLine();
    for (int i = 0; i < count; i++, ptr++) {
        cout << ptr->id << "\t"
             << ptr->name << "\t\t"
             << ptr->contact << "\t"
             << ptr->totalOrders << "\n";
    }
}


float recursiveRevenue(int index) {
    if (index >= orderCount) return 0.0f;
    if (orders[index].cancelled) return recursiveRevenue(index + 1);
    return orders[index].total + recursiveRevenue(index + 1);
}

int recursiveItemCount(int index) {
    if (index >= orderCount) return 0;
    if (orders[index].cancelled) return recursiveItemCount(index + 1);
    return orders[index].itemCount + recursiveItemCount(index + 1);
}


int linearSearchMenu(int id) {
    for (int i = 0; i < menuCount; i++)
        if (menu[i].id == id) return i;
    return -1;
}

int linearSearchCustomer(int id) {
    for (int i = 0; i < custCount; i++)
        if (customers[i].id == id) return i;
    return -1;
}

// Binary search requires sorted array by id
int binarySearchMenu(int id) {
    int low = 0, high = menuCount - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (menu[mid].id == id)  return mid;
        else if (menu[mid].id < id) low  = mid + 1;
        else                         high = mid - 1;
    }
    return -1;
}

int binarySearchCustomer(int id) {
    int low = 0, high = custCount - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (customers[mid].id == id)  return mid;
        else if (customers[mid].id < id) low  = mid + 1;
        else                              high = mid - 1;
    }
    return -1;
}

// ---------------------------------------------
//  SORTING ALGORITHMS
// ---------------------------------------------
void bubbleSortMenuByPrice() {
    for (int i = 0; i < menuCount - 1; i++) {
        for (int j = 0; j < menuCount - i - 1; j++) {
            if (menu[j].price > menu[j + 1].price) {
                MenuItem tmp = menu[j];
                menu[j]      = menu[j + 1];
                menu[j + 1]  = tmp;
            }
        }
    }
}

void selectionSortMenuByName() {
    for (int i = 0; i < menuCount - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < menuCount; j++)
            if (strcmp(menu[j].name, menu[minIdx].name) < 0)
                minIdx = j;
        if (minIdx != i) {
            MenuItem tmp   = menu[i];
            menu[i]        = menu[minIdx];
            menu[minIdx]   = tmp;
        }
    }
}

void bubbleSortCustomersByName() {
    for (int i = 0; i < custCount - 1; i++) {
        for (int j = 0; j < custCount - i - 1; j++) {
            if (strcmp(customers[j].name, customers[j + 1].name) > 0) {
                Customer tmp    = customers[j];
                customers[j]    = customers[j + 1];
                customers[j + 1] = tmp;
            }
        }
    }
}

void selectionSortMenuBySold() {
    for (int i = 0; i < menuCount - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < menuCount; j++)
            if (menu[j].totalSold > menu[maxIdx].totalSold)
                maxIdx = j;
        if (maxIdx != i) {
            MenuItem tmp   = menu[i];
            menu[i]        = menu[maxIdx];
            menu[maxIdx]   = tmp;
        }
    }
}

// ---------------------------------------------
//  MENU ITEM MANAGEMENT
// ---------------------------------------------
void addMenuItem() {
    if (menuCount >= MAX_MENU) { cout << "Menu is full!\n"; return; }
    MenuItem* m = &menu[menuCount];
    m->id        = nextMenuId++;
    m->totalSold = 0;
    cout << "Food Name: ";     cin.ignore(); cin.getline(m->name, 50);
    cout << "Category: ";      cin.getline(m->category, 30);
    cout << "Price (Rs.): ";   cin >> m->price;
    cout << "Quantity: ";      cin >> m->quantity;
    menuCount++;
    saveMenu();
    cout << "Menu item added. ID = " << m->id << "\n";
}

void displayMenu() {
    if (menuCount == 0) { cout << "No menu items.\n"; return; }
    traverseMenu(menu, menuCount);
}

void updateMenuItem() {
    int id;
    cout << "Enter Food ID to update: "; cin >> id;
    int idx = linearSearchMenu(id);
    if (idx == -1) { cout << "Not found.\n"; return; }
    MenuItem* m = &menu[idx];
    cout << "1.Name  2.Category  3.Price  4.Quantity\nChoice: ";
    int ch; cin >> ch;
    switch (ch) {
        case 1: cout << "New name: ";     cin.ignore(); cin.getline(m->name, 50);     break;
        case 2: cout << "New category: "; cin.ignore(); cin.getline(m->category, 30); break;
        case 3: cout << "New price: ";    cin >> m->price;    break;
        case 4: cout << "New quantity: "; cin >> m->quantity; break;
        default: cout << "Invalid.\n"; return;
    }
    saveMenu();
    cout << "Updated.\n";
}

void deleteMenuItem() {
    int id;
    cout << "Enter Food ID to delete: "; cin >> id;
    int idx = linearSearchMenu(id);
    if (idx == -1) { cout << "Not found.\n"; return; }
    for (int i = idx; i < menuCount - 1; i++)
        menu[i] = menu[i + 1];
    menuCount--;
    saveMenu();
    cout << "Deleted.\n";
}

void searchMenuUI() {
    cout << "1.Linear Search  2.Binary Search\nChoice: ";
    int ch; cin >> ch;
    int id; cout << "Enter Food ID: "; cin >> id;
    int idx = -1;
    if (ch == 1) idx = linearSearchMenu(id);
    else {
        bubbleSortMenuByPrice(); // ensure sorted by id for correct binary search
        // Sort by id for binary search
        for (int i = 0; i < menuCount - 1; i++)
            for (int j = 0; j < menuCount - i - 1; j++)
                if (menu[j].id > menu[j+1].id) { MenuItem t=menu[j]; menu[j]=menu[j+1]; menu[j+1]=t; }
        idx = binarySearchMenu(id);
    }
    if (idx == -1) cout << "Not found.\n";
    else {
        MenuItem* m = &menu[idx];
        cout << "Found: " << m->id << " | " << m->name << " | " << m->category
             << " | Rs." << m->price << " | Qty:" << m->quantity << "\n";
    }
}

void sortMenuUI() {
    cout << "1.Bubble Sort by Price  2.Selection Sort by Name  3.Selection Sort by Most Sold\nChoice: ";
    int ch; cin >> ch;
    if      (ch == 1) bubbleSortMenuByPrice();
    else if (ch == 2) selectionSortMenuByName();
    else if (ch == 3) selectionSortMenuBySold();
    else { cout << "Invalid.\n"; return; }
    displayMenu();
    saveMenu();
}

// ---------------------------------------------
//  CUSTOMER MANAGEMENT
// ---------------------------------------------
void addCustomer() {
    if (custCount >= MAX_CUST) { cout << "Customer list full!\n"; return; }
    Customer* c  = &customers[custCount];
    c->id        = nextCustId++;
    c->totalOrders = 0;
    cout << "Name: ";    cin.ignore(); cin.getline(c->name, 50);
    cout << "Contact: "; cin.getline(c->contact, 15);
    cout << "Address: "; cin.getline(c->address, 100);
    custCount++;
    saveCustomers();
    cout << "Customer added. ID = " << c->id << "\n";
}

void displayCustomers() {
    if (custCount == 0) { cout << "No customers.\n"; return; }
    traverseCustomers(customers, custCount);
}

void updateCustomer() {
    int id;
    cout << "Enter Customer ID: "; cin >> id;
    int idx = linearSearchCustomer(id);
    if (idx == -1) { cout << "Not found.\n"; return; }
    Customer* c = &customers[idx];
    cout << "1.Name  2.Contact  3.Address\nChoice: ";
    int ch; cin >> ch;
    switch (ch) {
        case 1: cout << "New name: ";    cin.ignore(); cin.getline(c->name, 50);     break;
        case 2: cout << "New contact: "; cin.ignore(); cin.getline(c->contact, 15);  break;
        case 3: cout << "New address: "; cin.ignore(); cin.getline(c->address, 100); break;
        default: cout << "Invalid.\n"; return;
    }
    saveCustomers();
    cout << "Updated.\n";
}

void deleteCustomer() {
    int id;
    cout << "Enter Customer ID to delete: "; cin >> id;
    int idx = linearSearchCustomer(id);
    if (idx == -1) { cout << "Not found.\n"; return; }
    for (int i = idx; i < custCount - 1; i++)
        customers[i] = customers[i + 1];
    custCount--;
    saveCustomers();
    cout << "Deleted.\n";
}

void searchCustomerUI() {
    cout << "1.Linear Search  2.Binary Search\nChoice: ";
    int ch; cin >> ch;
    int id; cout << "Enter Customer ID: "; cin >> id;
    int idx = -1;
    if (ch == 1) idx = linearSearchCustomer(id);
    else {
        // sort by id first
        for (int i = 0; i < custCount - 1; i++)
            for (int j = 0; j < custCount - i - 1; j++)
                if (customers[j].id > customers[j+1].id) { Customer t=customers[j]; customers[j]=customers[j+1]; customers[j+1]=t; }
        idx = binarySearchCustomer(id);
    }
    if (idx == -1) cout << "Not found.\n";
    else {
        Customer* c = &customers[idx];
        cout << "Found: " << c->id << " | " << c->name << " | " << c->contact
             << " | " << c->address << " | Orders: " << c->totalOrders << "\n";
    }
}

void sortCustomersUI() {
    cout << "1.Bubble Sort by Name  2.Selection Sort by Orders\nChoice: ";
    int ch; cin >> ch;
    if (ch == 1) {
        bubbleSortCustomersByName();
    } else if (ch == 2) {
        // selection sort descending by totalOrders
        for (int i = 0; i < custCount - 1; i++) {
            int maxIdx = i;
            for (int j = i + 1; j < custCount; j++)
                if (customers[j].totalOrders > customers[maxIdx].totalOrders)
                    maxIdx = j;
            if (maxIdx != i) { Customer t=customers[i]; customers[i]=customers[maxIdx]; customers[maxIdx]=t; }
        }
    } else { cout << "Invalid.\n"; return; }
    displayCustomers();
    saveCustomers();
}

// ---------------------------------------------
//  ORDER PROCESSING & BILLING
// ---------------------------------------------
void generateBill(Order* o) {
    printLine('=');
    cout << "          FOOD DELIVERY & RESTAURANT\n";
    cout << "                   BILL\n";
    printLine('=');
    cout << "Order ID   : " << o->orderId << "\n";
    cout << "Customer   : " << o->customerName << " (ID: " << o->customerId << ")\n";
    printLine();
    cout << "Item\t\t\tQty\tPrice\tTotal\n";
    printLine();
    for (int i = 0; i < o->itemCount; i++) {
        OrderItem* it = &o->items[i];
        cout << it->foodName << "\t\t\t"
             << it->qty << "\t"
             << it->unitPrice << "\t"
             << it->qty * it->unitPrice << "\n";
    }
    printLine();
    cout << "Subtotal       : Rs. " << o->subtotal << "\n";
    cout << "Delivery Charge: Rs. " << o->deliveryCharge << "\n";
    cout << "Discount       : Rs. " << o->discount << "\n";
    printLine();
    cout << "TOTAL          : Rs. " << o->total << "\n";
    printLine('=');
}

void placeOrder() {
    if (orderCount >= MAX_ORDERS) { cout << "Order limit reached!\n"; return; }
    int cid;
    cout << "Enter Customer ID: "; cin >> cid;
    int cidx = linearSearchCustomer(cid);
    if (cidx == -1) { cout << "Customer not found.\n"; return; }

    Order* o         = &orders[orderCount];
    o->orderId       = nextOrderId++;
    o->customerId    = cid;
    strcpy(o->customerName, customers[cidx].name);
    o->itemCount     = 0;
    o->subtotal      = 0;
    o->cancelled     = 0;

    cout << "\n--- Available Menu ---\n";
    traverseMenu(menu, menuCount);

    char more = 'y';
    while (more == 'y' || more == 'Y') {
        if (o->itemCount >= MAX_ITEMS_PER_ORDER) { cout << "Max items reached.\n"; break; }
        int fid; cout << "\nEnter Food ID: "; cin >> fid;
        int midx = linearSearchMenu(fid);
        if (midx == -1) { cout << "Item not found.\n"; continue; }
        if (menu[midx].quantity == 0) { cout << "Out of stock.\n"; continue; }

        int qty; cout << "Quantity: "; cin >> qty;
        if (qty > menu[midx].quantity) { cout << "Only " << menu[midx].quantity << " available.\n"; continue; }

        OrderItem* it   = &o->items[o->itemCount];
        it->menuId      = fid;
        strcpy(it->foodName, menu[midx].name);
        it->qty         = qty;
        it->unitPrice   = menu[midx].price;

        menu[midx].quantity -= qty;
        menu[midx].totalSold += qty;

        o->subtotal += qty * menu[midx].price;
        o->itemCount++;

        cout << "Add more? (y/n): "; cin >> more;
    }

    if (o->itemCount == 0) { cout << "No items ordered. Order cancelled.\n"; nextOrderId--; return; }

    o->deliveryCharge = 50.0f;
    o->discount       = (o->subtotal > 500) ? o->subtotal * 0.10f : 0.0f;
    o->total          = o->subtotal + o->deliveryCharge - o->discount;

    customers[cidx].totalOrders++;
    orderCount++;

    saveMenu();
    saveCustomers();
    saveOrders();

    generateBill(o);
}

void cancelOrder() {
    int oid;
    cout << "Enter Order ID to cancel: "; cin >> oid;
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].orderId == oid) {
            if (orders[i].cancelled) { cout << "Already cancelled.\n"; return; }
            orders[i].cancelled = 1;
            // Restore stock
            for (int j = 0; j < orders[i].itemCount; j++) {
                int midx = linearSearchMenu(orders[i].items[j].menuId);
                if (midx != -1) {
                    menu[midx].quantity  += orders[i].items[j].qty;
                    menu[midx].totalSold -= orders[i].items[j].qty;
                }
            }
            // Decrement customer order count
            int cidx = linearSearchCustomer(orders[i].customerId);
            if (cidx != -1) customers[cidx].totalOrders--;

            saveOrders(); saveMenu(); saveCustomers();
            cout << "Order " << oid << " cancelled.\n";
            return;
        }
    }
    cout << "Order not found.\n";
}

void viewOrderBill() {
    int oid;
    cout << "Enter Order ID: "; cin >> oid;
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].orderId == oid) {
            if (orders[i].cancelled) cout << "This order was cancelled.\n";
            else generateBill(&orders[i]);
            return;
        }
    }
    cout << "Order not found.\n";
}

// ---------------------------------------------
//  REPORTS
// ---------------------------------------------
void salesReport() {
    printLine('=');
    cout << "              SALES REPORT\n";
    printLine('=');
    cout << "FoodID\tName\t\t\tCategory\tSold\n";
    printLine();

    // sort copy by totalSold descending
    MenuItem sorted[MAX_MENU];
    for (int i = 0; i < menuCount; i++) sorted[i] = menu[i];
    for (int i = 0; i < menuCount - 1; i++)
        for (int j = 0; j < menuCount - i - 1; j++)
            if (sorted[j].totalSold < sorted[j+1].totalSold) { MenuItem t=sorted[j]; sorted[j]=sorted[j+1]; sorted[j+1]=t; }

    char buf[4096] = "";
    strcat(buf, "SALES REPORT\n");
    for (int i = 0; i < menuCount; i++) {
        cout << sorted[i].id << "\t" << sorted[i].name << "\t\t\t"
             << sorted[i].category << "\t" << sorted[i].totalSold << "\n";
        char line[200];
        sprintf(line, "%d | %s | %s | Sold: %d\n", sorted[i].id, sorted[i].name, sorted[i].category, sorted[i].totalSold);
        strcat(buf, line);
    }
    exportReport("report_sales.txt", buf);
}

void revenueReport() {
    float total = recursiveRevenue(0);
    int   items = recursiveItemCount(0);
    printLine('=');
    cout << "              REVENUE REPORT\n";
    printLine('=');
    cout << "Total Active Orders : " << orderCount << "\n";
    cout << "Total Items Ordered : " << items << "\n";
    cout << "Total Revenue       : Rs. " << total << "\n";
    printLine('=');

    char buf[512];
    sprintf(buf, "REVENUE REPORT\nTotal Orders: %d\nTotal Items: %d\nTotal Revenue: Rs. %.2f\n",
            orderCount, items, total);
    exportReport("report_revenue.txt", buf);
}

void inventoryReport() {
    printLine('=');
    cout << "           INVENTORY (LOW STOCK) REPORT\n";
    printLine('=');
    cout << "Items with Quantity <= 5:\n";
    printLine();
    char buf[2048] = "LOW STOCK REPORT\n";
    bool any = false;
    for (int i = 0; i < menuCount; i++) {
        if (menu[i].quantity <= 5) {
            cout << menu[i].id << " | " << menu[i].name << " | Qty: " << menu[i].quantity << "\n";
            char line[200];
            sprintf(line, "%d | %s | Qty: %d\n", menu[i].id, menu[i].name, menu[i].quantity);
            strcat(buf, line);
            any = true;
        }
    }
    if (!any) cout << "No low stock items.\n";
    exportReport("report_inventory.txt", buf);
}

void customerActivityReport() {
    printLine('=');
    cout << "         CUSTOMER ACTIVITY REPORT\n";
    printLine('=');
    cout << "ID\tName\t\t\tContact\t\tOrders\n";
    printLine();
    char buf[4096] = "CUSTOMER ACTIVITY REPORT\n";
    for (int i = 0; i < custCount; i++) {
        cout << customers[i].id << "\t" << customers[i].name << "\t\t"
             << customers[i].contact << "\t" << customers[i].totalOrders << "\n";
        char line[200];
        sprintf(line, "%d | %s | %s | Orders: %d\n",
                customers[i].id, customers[i].name, customers[i].contact, customers[i].totalOrders);
        strcat(buf, line);
    }
    exportReport("report_customers.txt", buf);
}

void mostLeastOrderedReport() {
    if (menuCount == 0) { cout << "No menu data.\n"; return; }
    int maxIdx = 0, minIdx = 0;
    for (int i = 1; i < menuCount; i++) {
        if (menu[i].totalSold > menu[maxIdx].totalSold) maxIdx = i;
        if (menu[i].totalSold < menu[minIdx].totalSold) minIdx = i;
    }
    printLine('=');
    cout << "Most Ordered : " << menu[maxIdx].name << " (" << menu[maxIdx].totalSold << " sold)\n";
    cout << "Least Ordered: " << menu[minIdx].name << " (" << menu[minIdx].totalSold << " sold)\n";
    printLine('=');
}

// ---------------------------------------------
//  MENUS
// ---------------------------------------------
void menuManagementMenu() {
    int ch;
    do {
        printLine('=');
        cout << "       MENU ITEM MANAGEMENT\n";
        printLine('=');
        cout << "1. Add Item\n2. Display All\n3. Update Item\n4. Delete Item\n"
             << "5. Search Item\n6. Sort Items\n0. Back\n> ";
        cin >> ch;
        switch (ch) {
            case 1: addMenuItem();    break;
            case 2: displayMenu();    break;
            case 3: updateMenuItem(); break;
            case 4: deleteMenuItem(); break;
            case 5: searchMenuUI();   break;
            case 6: sortMenuUI();     break;
            case 0: break;
            default: cout << "Invalid.\n";
        }
    } while (ch != 0);
}

void customerManagementMenu() {
    int ch;
    do {
        printLine('=');
        cout << "       CUSTOMER MANAGEMENT\n";
        printLine('=');
        cout << "1. Add Customer\n2. Display All\n3. Update Customer\n4. Delete Customer\n"
             << "5. Search Customer\n6. Sort Customers\n0. Back\n> ";
        cin >> ch;
        switch (ch) {
            case 1: addCustomer();       break;
            case 2: displayCustomers();  break;
            case 3: updateCustomer();    break;
            case 4: deleteCustomer();    break;
            case 5: searchCustomerUI();  break;
            case 6: sortCustomersUI();   break;
            case 0: break;
            default: cout << "Invalid.\n";
        }
    } while (ch != 0);
}

void orderMenu() {
    int ch;
    do {
        printLine('=');
        cout << "       ORDER PROCESSING\n";
        printLine('=');
        cout << "1. Place Order\n2. Cancel Order\n3. View Bill\n0. Back\n> ";
        cin >> ch;
        switch (ch) {
            case 1: placeOrder();   break;
            case 2: cancelOrder();  break;
            case 3: viewOrderBill(); break;
            case 0: break;
            default: cout << "Invalid.\n";
        }
    } while (ch != 0);
}

void reportsMenu() {
    int ch;
    do {
        printLine('=');
        cout << "       REPORTS\n";
        printLine('=');
        cout << "1. Sales Report\n2. Revenue Report\n3. Inventory/Low Stock\n"
             << "4. Customer Activity\n5. Most & Least Ordered\n0. Back\n> ";
        cin >> ch;
        switch (ch) {
            case 1: salesReport();            break;
            case 2: revenueReport();          break;
            case 3: inventoryReport();        break;
            case 4: customerActivityReport(); break;
            case 5: mostLeastOrderedReport(); break;
            case 0: break;
            default: cout << "Invalid.\n";
        }
    } while (ch != 0);
}

// ---------------------------------------------
//  MAIN
// ---------------------------------------------
int main() {
    loadMenu();
    loadCustomers();
    loadOrders();

    int ch;
    do {
        printLine('=');
        cout << "   FOOD DELIVERY & RESTAURANT MANAGEMENT\n";
        printLine('=');
        cout << "1. Menu Item Management\n"
             << "2. Customer Management\n"
             << "3. Order Processing & Billing\n"
             << "4. Reports\n"
             << "0. Exit\n> ";
        cin >> ch;
        switch (ch) {
            case 1: menuManagementMenu();    break;
            case 2: customerManagementMenu(); break;
            case 3: orderMenu();             break;
            case 4: reportsMenu();           break;
            case 0: cout << "Goodbye!\n";   break;
            default: cout << "Invalid.\n";
        }
    } while (ch != 0);

    return 0;
}
