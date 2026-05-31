

#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

// ─────────────────────────────────────────────
//  QUEUE (Order Processing)
// ─────────────────────────────────────────────
struct node {
    string foodname;
    int price;
    int quantity;
    node* next;
};

class Queue {
    node* front;
    node* rear;

public:
    Queue() : front(nullptr), rear(nullptr) {}

    void push(node input) {
        node* newNode = new node;
        newNode->foodname = input.foodname;
        newNode->price    = input.price;
        newNode->quantity = input.quantity;
        newNode->next     = nullptr;

        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear       = newNode;
        }
    }

    node pop() {
        if (front == nullptr)
            throw runtime_error("Queue is empty.");
        node* temp   = front;
        node  output = *front;
        front        = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;
        return output;
    }

    bool check() const { return front != nullptr; }

    ~Queue() {
        while (front != nullptr) {
            node* temp = front;
            front      = front->next;
            delete temp;
        }
    }
};

// ─────────────────────────────────────────────
//  BST (Food Menu)
// ─────────────────────────────────────────────
class fooditems {
public:
    string     foodname;
    int        price;
    int        amountearned;
    fooditems* left;
    fooditems* right;

    fooditems(string fn, int p)
        : foodname(fn), price(p), amountearned(0),
          left(nullptr), right(nullptr) {}
};

class BST {
public:
    fooditems* create(string foodname, int price) {
        return new fooditems(foodname, price);
    }

    fooditems* insert(fooditems* root, string foodname, int price) {
        if (root == nullptr) return create(foodname, price);
        if (foodname < root->foodname)
            root->left  = insert(root->left,  foodname, price);
        else if (foodname > root->foodname)
            root->right = insert(root->right, foodname, price);
        else
            cout << "\"" << foodname << "\" already exists in the menu.\n";
        return root;
    }

    fooditems* minvaluenode(fooditems* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    fooditems* Deleteitem(fooditems* root, string foodname) {
        if (root == nullptr) return nullptr;
        if (foodname < root->foodname)
            root->left  = Deleteitem(root->left,  foodname);
        else if (foodname > root->foodname)
            root->right = Deleteitem(root->right, foodname);
        else {
            if (!root->left)  { fooditems* t = root->right; delete root; return t; }
            if (!root->right) { fooditems* t = root->left;  delete root; return t; }
            fooditems* t    = minvaluenode(root->right);
            root->foodname  = t->foodname;
            root->price     = t->price;
            root->amountearned = t->amountearned;
            root->right     = Deleteitem(root->right, t->foodname);
        }
        return root;
    }

    void Displaymenu(fooditems* root) {
        if (!root) return;
        Displaymenu(root->left);
        cout << "  * " << root->foodname << "\t\tRs." << root->price << "\n";
        Displaymenu(root->right);
    }

    void Displayamountearned(fooditems* root) {
        if (!root) return;
        Displayamountearned(root->left);
        cout << "  * " << root->foodname << "\t\tEarned: Rs." << root->amountearned << "\n";
        Displayamountearned(root->right);
    }

    fooditems* SearchBST(fooditems* root, string foodname) {
        if (!root || root->foodname == foodname) return root;
        if (foodname < root->foodname) return SearchBST(root->left,  foodname);
        return SearchBST(root->right, foodname);
    }

    int orderfood(fooditems* root) {
        if (!root) { cout << "Menu is empty!\n"; return 0; }

        Queue  q;
        node   ob;
        int    totalsum = 0;
        int    again    = 1;

        while (again == 1) {
            cout << "\n--- Menu ---\n";
            Displaymenu(root);
            cout << "\nEnter food item name: ";
            string n;
            cin >> n;

            fooditems* temp = SearchBST(root, n);
            if (!temp) {
                cout << "Item not found. Try again.\n";
            } else {
                int quantity = 0;
                cout << "Enter quantity: ";
                cin >> quantity;
                if (cin.fail() || quantity <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid quantity. Entry skipped.\n";
                } else {
                    ob.foodname = n;
                    ob.price    = temp->price;
                    ob.quantity = quantity;
                    temp->amountearned += temp->price * quantity;
                    cout << "Subtotal: Rs." << temp->price * quantity << "\n";
                    q.push(ob);
                }
            }

            while (true) {
                cout << "Order another item? (1=Yes / 0=No): ";
                cin >> again;
                if (cin.fail() || (again != 0 && again != 1)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid choice.\n";
                } else break;
            }
        }

        fstream f("Receipt.txt", ios::app);
        cout << "\n========== RECEIPT ==========\n";
        if (f.is_open()) f << "========== RECEIPT ==========\n";

        while (q.check()) {
            ob = q.pop();
            int sub = ob.price * ob.quantity;
            cout << ob.foodname << " x" << ob.quantity << "   Rs." << sub << "\n";
            if (f.is_open()) f << ob.foodname << " x" << ob.quantity << "   Rs." << sub << "\n";
            totalsum += sub;
        }
        cout << "Total Bill: Rs." << totalsum << "\n";
        if (f.is_open()) f << "Total Bill: Rs." << totalsum << "\n";

        return totalsum;
    }

    void clearTree(fooditems* node) {
        if (!node) return;
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
};

// ─────────────────────────────────────────────
//  CUSTOMER NODE (Doubly Linked List)
// ─────────────────────────────────────────────
struct CustomerStruct {
    string username, password, email, address;
    int    customersum;
};

class Customer {
    string username, password, email, address;
    int    customersum;

public:
    Customer* next = nullptr;
    Customer* prev = nullptr;

    Customer(string n, string p, string e, string a)
        : username(n), password(p), email(e), address(a), customersum(0) {}

    bool checkUsername(const string& s) const { return username == s; }
    bool checkPassword(const string& s) const { return password == s; }

    void setUsername(string s) { username = s; }
    void setPassword(string s) { password = s; }
    void setEmail(string s)    { email    = s; }
    void setAddress(string s)  { address  = s; }
    void addToSum(int v)       { customersum += v; }

    string getName()    const { return username; }
    string getPass()    const { return password; }
    string getEmail()   const { return email; }
    string getAddress() const { return address; }
    int    getSum()     const { return customersum; }
};

// ─────────────────────────────────────────────
//  CUSTOMER LIST  (uses BST via composition)
// ─────────────────────────────────────────────
class CustomerList {
// ── FIX: changed from private to protected so TestableCustomerList can access ──
protected:
    Customer*  head       = nullptr;
    Customer*  tail       = nullptr;
    BST        bst;
    fooditems* menuRoot   = nullptr;
    int        companysum = 0;

private:
    // ── helpers ──────────────────────────────

    string promptUsername() {
        string name;
        while (true) {
            cout << "Username: ";
            if (cin.peek() == '\n') cin.ignore();
            getline(cin, name);
            if (name.empty()) { cout << "Cannot be empty.\n"; continue; }

            bool taken = false;
            for (Customer* t = head; t; t = t->next)
                if (t->checkUsername(name)) { taken = true; break; }

            if (taken) cout << "Username taken, try another.\n";
            else       break;
        }
        return name;
    }

    string promptPassword() {
        string pw;
        while (true) {
            cout << "Password (needs 1 uppercase + 1 digit): ";
            if (cin.peek() == '\n') cin.ignore();
            getline(cin, pw);
            if (pw.empty()) { cout << "Cannot be empty.\n"; continue; }
            bool hasUpper = false, hasDigit = false;
            for (char c : pw) {
                if (c >= 'A' && c <= 'Z') hasUpper = true;
                if (c >= '0' && c <= '9') hasDigit = true;
            }
            if (!hasUpper || !hasDigit) cout << "Requirements not met.\n";
            else break;
        }
        return pw;
    }

    string promptEmail() {
        string email;
        while (true) {
            cout << "Email (must end in @gmail.com): ";
            cin >> email;
            size_t at = email.find('@');
            if (at != string::npos && email.substr(at) == "@gmail.com" && at > 0)
                break;
            cout << "Invalid format.\n";
        }
        return email;
    }

    string promptAddress() {
        string addr;
        while (true) {
            cout << "Home address: ";
            if (cin.peek() == '\n') cin.ignore();
            getline(cin, addr);
            if (!addr.empty()) break;
            cout << "Cannot be empty.\n";
        }
        return addr;
    }

    void addCustomer(string n, string p, string e, string a) {
        Customer* node = new Customer(n, p, e, a);
        if (!head) { head = tail = node; }
        else { tail->next = node; node->prev = tail; tail = node; }
    }

    int listSize() {
        int i = 0;
        for (Customer* t = head; t; t = t->next) i++;
        return i;
    }

    void copyToArray(CustomerStruct* arr) {
        int i = 0;
        for (Customer* t = head; t; t = t->next, i++) {
            arr[i] = { t->getName(), t->getPass(),
                       t->getEmail(), t->getAddress(), t->getSum() };
        }
    }

public:
    CustomerList() {
        const pair<string,int> items[] = {
            {"Biryani",150},{"Burger",200},{"Cheese-Burger",250},
            {"Chicken-Karahi",890},{"Chilli-Chicken",999},{"Chow-mein",500},
            {"Chocolate-Cake",500},{"Drink-Large",200},{"Drink-Small",60},
            {"Dumplings",750},{"Fried-Rice",650},{"Haleem",250},
            {"Icecream",150},{"Manchurian",700},{"Nihari",600},
            {"Noodle-Soup",290},{"Spaghetti",895},{"Wonton",675}
        };
        for (auto& p : items)
            menuRoot = bst.insert(menuRoot, p.first, p.second);
    }

    // ── Sign-up ──────────────────────────────
    void signup() {
        int opt = 1;
        cout << "\n=== Sign-Up ===\n";
        while (opt != 2) {
            string n = promptUsername();
            string p = promptPassword();
            string e = promptEmail();
            string a = promptAddress();
            addCustomer(n, p, e, a);
            cout << "Account created for \"" << n << "\"!\n";

            while (true) {
                cout << "Sign up another user? (1=Yes / 2=No): ";
                cin >> opt;
                if (cin.fail() || opt < 1 || opt > 2) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid.\n";
                } else break;
            }
        }
    }

    // ── Login ────────────────────────────────
    void login() {
        if (!head) { cout << "No customers registered yet.\n"; return; }
        cout << "\n=== Login ===\n";

        string name;
        cout << "Username: "; cin >> name;
        Customer* user = nullptr;
        for (Customer* t = head; t; t = t->next)
            if (t->checkUsername(name)) { user = t; break; }

        if (!user) { cout << "Username not found.\n"; return; }

        bool auth = false;
        for (int attempt = 1; attempt <= 3; attempt++) {
            string pw;
            cout << "Password: "; cin >> pw;
            if (user->checkPassword(pw)) { auth = true; break; }
            cout << "Wrong password (" << attempt << "/3).\n";
        }
        if (!auth) { cout << "Too many failed attempts.\n"; return; }

        int opt = 0;
        cout << "\nWelcome, " << user->getName() << "!\n";
        while (opt != 7) {
            cout << "\n--- User Menu ---\n"
                 << "1. Order Food\n2. Change Username\n3. Change Password\n"
                 << "4. Change Email\n5. Change Address\n6. View My Details\n"
                 << "7. Logout\nChoice: ";
            cin >> opt;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            switch (opt) {
                case 1: {
                    int bill = bst.orderfood(menuRoot);
                    fstream f("Receipt.txt", ios::app);
                    cout << "Customer : " << user->getName() << "\n"
                         << "Address  : " << user->getAddress() << "\n"
                         << "=============================\n";
                    if (f.is_open())
                        f << "Customer: " << user->getName()
                          << "\nAddress: "  << user->getAddress()
                          << "\n-----------------------------\n";
                    companysum += bill;
                    user->addToSum(bill);
                    break;
                }
                case 2: user->setUsername(promptUsername()); break;
                case 3: user->setPassword(promptPassword()); break;
                case 4: user->setEmail(promptEmail());       break;
                case 5: user->setAddress(promptAddress());   break;
                case 6:
                    cout << "\nName    : " << user->getName()
                         << "\nEmail   : " << user->getEmail()
                         << "\nAddress : " << user->getAddress()
                         << "\nSpent   : Rs." << user->getSum() << "\n";
                    break;
                case 7: cout << "Logged out.\n"; break;
                default: cout << "Invalid option.\n";
            }
        }
    }

    // ── Manager Menu ─────────────────────────
    void managermenu() {
        const string MANAGER_PASS = "Manager123";
        int failCount = 0;
        while (failCount < 3) {
            string input;
            cout << "Manager Password: "; cin >> input;
            if (input == MANAGER_PASS) break;
            cout << "Incorrect (" << ++failCount << "/3).\n";
        }
        if (failCount == 3) { cout << "Access denied.\n"; return; }

        int opt = 0;
        while (opt != 9) {
            cout << "\n=== Manager Panel ===\n"
                 << "1. Company Revenue\n2. Customer List\n3. Customers A-Z\n"
                 << "4. Delete Customer\n5. View Menu\n6. Delete Food Item\n"
                 << "7. Add Food Item\n8. Food Earnings\n9. Exit\nChoice: ";
            cin >> opt;
            if (cin.fail() || opt < 1 || opt > 9) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            string s; int price;
            switch (opt) {
                case 1:
                    cout << "Total Revenue: Rs." << companysum << "\n"; break;
                case 2:
                    displayAllCustomers(); break;
                case 3:
                    displaySortedCustomers(); break;
                case 4:
                    cout << "Username to delete: "; cin >> s;
                    deleteCustomer(s); break;
                case 5:
                    cout << "\n--- Menu ---\n";
                    bst.Displaymenu(menuRoot); break;
                case 6:
                    cout << "Food item to delete: "; cin >> s;
                    if (!bst.SearchBST(menuRoot, s)) cout << "Not found.\n";
                    else menuRoot = bst.Deleteitem(menuRoot, s);
                    break;
                case 7:
                    cout << "New item name : "; cin >> s;
                    cout << "Price         : "; cin >> price;
                    if (cin.fail() || price <= 0) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid price.\n";
                    } else {
                        menuRoot = bst.insert(menuRoot, s, price);
                        cout << "Item added.\n";
                    }
                    break;
                case 8:
                    cout << "\n--- Earnings per Item ---\n";
                    bst.Displayamountearned(menuRoot); break;
            }
        }
    }

    // ── Display helpers ──────────────────────
    void displayAllCustomers() {
        if (!head) { cout << "No customers.\n"; return; }
        int i = 1;
        for (Customer* t = head; t; t = t->next, i++)
            cout << i << ". " << t->getName()
                 << "  |  Rs." << t->getSum() << "\n";
    }

    void displaySortedCustomers() {
        int n = listSize();
        if (n == 0) { cout << "No customers.\n"; return; }
        CustomerStruct* arr = new CustomerStruct[n];
        copyToArray(arr);

        for (int i = 0; i < n - 1; i++)
            for (int j = 0; j < n - 1 - i; j++)
                if (arr[j].username > arr[j + 1].username)
                    swap(arr[j], arr[j + 1]);

        cout << "\n--- Customers A-Z ---\n";
        for (int i = 0; i < n; i++)
            cout << i + 1 << ". " << arr[i].username
                 << " (" << arr[i].email << ")  Rs." << arr[i].customersum << "\n";
        delete[] arr;
    }

    void deleteCustomer(const string& name) {
        if (!head) { cout << "No customers.\n"; return; }
        Customer* t = head;
        while (t && t->getName() != name) t = t->next;
        if (!t) { cout << "Not found.\n"; return; }

        if (t == head) {
            head = head->next;
            if (head) head->prev = nullptr;
            else      tail = nullptr;
        } else if (t == tail) {
            tail = tail->prev;
            if (tail) tail->next = nullptr;
        } else {
            t->prev->next = t->next;
            t->next->prev = t->prev;
        }
        delete t;
        cout << "Customer \"" << name << "\" deleted.\n";
    }

    ~CustomerList() {
        while (head) {
            Customer* t = head;
            head = head->next;
            delete t;
        }
        bst.clearTree(menuRoot);
    }
};

// ─────────────────────────────────────────────
//  MAIN  — guarded so test file can include this
// ─────────────────────────────────────────────
#ifndef TESTING
int main() {
    CustomerList system;
    int choice = 0;

    cout << "========================================\n";
    cout << "   Welcome to the Restaurant System\n";
    cout << "========================================\n";

    while (true) {
        cout << "\n===== Main Menu =====\n"
             << "1. Manager Menu\n"
             << "2. Sign Up\n"
             << "3. Login\n"
             << "4. Exit\n"
             << "Choice: ";
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter 1-4.\n";
            continue;
        }

        if      (choice == 1) system.managermenu();
        else if (choice == 2) system.signup();
        else if (choice == 3) system.login();
        else {
            cout << "Goodbye!\n";
            break;
        }
    }
    return 0;
}
#endif