#include<iostream>
#include<fstream>
#include<iomanip>
#include<ctime>
#include<sstream> 
#include <conio.h>

using namespace std;

const int RUN = 32; //use for sorting

struct Apple {
	string id, category, name;
	int stock;
	double price, cost;
	bool deleted = false;
};

struct CartItem 
{
    string ID;
    string Product_Name;
    double Price;
    int Quantity;
    double Subtotal;
};

struct User 
{
    string username;
    string email;
    string password;
    string phoneNumber;
    string role;
};
 
 class Person 
 {
	protected:
	    string username;
	    string email;
	    string password;
	    string phoneNumber;
	public:
	    Person(string uname, string em, string pass, string phone)
	        : username(uname), email(em), password(pass), phoneNumber(phone) {}
	
	    virtual string getRole() = 0; 
	    virtual double getDiscountRate() { return 0.0; }
	    virtual void printRole() = 0;
	    virtual ~Person() {} //
	    
	    string getEmail() const 
		{
	    	return email;
		}

	};
	
	class Customer : public Person 
	{
		public:
    Customer(string uname, string em, string pass, string phone)
        : Person(uname, em, pass, phone) {}

    string getRole() override { return "Customer"; }
    void printRole() override { cout << "Logged in as Customer\n"; }
};

class Member : public Person {
public:
    Member(string uname, string em, string pass, string phone)
        : Person(uname, em, pass, phone) {}

    string getRole() override { return "Member"; }
    double getDiscountRate() override { return 0.10; } // 10% 折扣
    void printRole() override { cout << "Logged in as Member (10% discount)\n"; }
};

class Admin : public Person {
public:
    Admin(string uname, string em, string pass, string phone)
        : Person(uname, em, pass, phone) {}

    string getRole() override { return "Admin"; }
    void printRole() override { cout << "Logged in as Admin\n"; }
};

class SuperAdmin : public Person {
public:
    SuperAdmin(string uname, string em, string pass, string phone)
        : Person(uname, em, pass, phone) {}

    string getRole() override { return "SuperAdmin"; }
    void printRole() override { cout << "Logged in as SuperAdmin\n"; }
};

string getPassword() 
{
	string password;
	char ch;
	while ((ch = _getch()) != '\r') { // Enter键是 '\r'
		if (ch == '\b') { // Backspace
		    if (!password.empty()) {
		        cout << "\b \b";
		        password.pop_back();
		    }
		} 
		else 
		{
		    password.push_back(ch);
		    cout << '*';
		}
	}
	cout << endl;
	return password;
}

bool isValidEmail(const string& email) 
{
	return email.find(".com") != string::npos;
}

Person* loginUser() 
{
    ifstream inFile("User.txt");
    if (!inFile) {
        cout << "Error opening User.txt!" << endl;
        return nullptr;
    }

    string emailInput, passInput;
    cout << "Enter email: ";
	cin >> emailInput;
	
	if (!isValidEmail(emailInput)) 
	{
	    cout << "Invalid email format. Email must include '.com'\n";
	    return nullptr;
	}

    cout << "Enter password: ";
    passInput = getPassword();

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        User u;

        getline(ss, u.username, ',');
        getline(ss, u.email, ',');
        getline(ss, u.password, ',');
        getline(ss, u.phoneNumber, ',');
        getline(ss, u.role, ',');

        if (u.email == emailInput && u.password == passInput) {
            cout << "Login successful!\n";

            if (u.role == "Member")
                return new Member(u.username, u.email, u.password, u.phoneNumber);
            else if (u.role == "Admin")
                return new Admin(u.username, u.email, u.password, u.phoneNumber);
            else if (u.role == "SuperAdmin")
                return new SuperAdmin(u.username, u.email, u.password, u.phoneNumber);
            else
                return new Customer(u.username, u.email, u.password, u.phoneNumber);
        }
    }

    cout << "Invalid email or password.\n";
    return nullptr;
}



class Apple_Store
{
	private:
		
		Apple queue[50]; 
   		int head, tail;	
   		int count = 0;
		
	public:
		
		Apple_Store() 
		{  
	        tail = -1;  
	        head = 0;  
	    }  
		
		// Check the queue is empty or not
		int empty() {  
		    if(head == tail+1) 
			{
			    return 1;  
			} 
			else 
			{
				return 0;
			}
		}  
		
		// Check the queue is full or not
		int full() {
			if(tail == 49) 
			{
				return 1;
			} 
			else 
			{
				return 0;
			}
		}
		
		//add new product
		// Add a new product to the queue
		void addProduct(string id, string category, string name, double price, double cost, int stock) 
		{
			if(!full()) 
			{
				tail++;
				queue[tail].id = id;
				queue[tail].category = category;				
				queue[tail].name = name;
				queue[tail].price = price;
				queue[tail].cost = cost;
				queue[tail].stock = stock;
			} 
			else 
			{
				cout << "Queue is full" << endl;
				
				saveToFile("Apple_Store.txt");
			}
		}
		
		//save all data to the txt file
		void saveToFile(const string& filename)
		{
			ofstream outFile(filename);
			
			    if(!outFile) 
				{
			        cout << "Error opening file! " << endl;
			        return;
			    }
			    
			    if(empty()) 
				{
			        cout << "Queue is empty, nothing to save." << endl;
			        outFile.close();
			        return;
			    }
			    
			    for(int i = head; i <= tail; i++) 
				{
			        outFile << queue[i].id << ","
			                << queue[i].category << ","
			                << queue[i].name << ","
			                << queue[i].price << ","
			                << queue[i].cost << ","
			                << queue[i].stock << endl;
			    }
			
			outFile.close();
		}
		
		//display our product list, with table
		void displayData() 
		{
		    cout << "+---------+--------------+------------------------------+----------+" << endl;
		    cout << "| ID      | Category     | Product Name                 | Price    |" << endl;
		    cout << "+---------+--------------+------------------------------+----------+" << endl;
		
		    if (empty()) 
			{
		        cout << "|" << setw(76) << right << "No products available." << " |" << endl;
		    } else 
			{
		        for (int i = head; i <= tail; i++) 
				{
		            cout << "| ";
		            cout.width(7); cout << left << queue[i].id << " | ";
		            cout.width(12); cout << left << queue[i].category << " | ";
		            cout.width(28); cout << left << queue[i].name << " | ";
		            cout.width(8); cout << right << fixed << setprecision(2) << queue[i].price << " | " << endl;
		        }
		    }
		    
		    cout << "+---------+--------------+------------------------------+----------+" << endl;
		}
		
		void mergeByCategory(int left, int mid, int right) ///
		{
		    int n1 = mid - left + 1;
		    int n2 = right - mid;
		    
		    Apple* leftArr = new Apple[n1];
		    Apple* rightArr = new Apple[n2];
		    
		    for (int i = 0; i < n1; i++)
		        leftArr[i] = queue[left + i];
		    for (int j = 0; j < n2; j++)
		        rightArr[j] = queue[mid + 1 + j];
		    
		    int i = 0, j = 0, k = left;
		    while (i < n1 && j < n2) 
			{
		        if (leftArr[i].category <= rightArr[j].category) 
				{
		            queue[k] = leftArr[i];
		            i++;
		        } 
				else 
				{
		            queue[k] = rightArr[j];
		            j++;
		        }
		        k++;
		    }
		    
		    while (i < n1) 
			{
		        queue[k] = leftArr[i];
		        i++;
		        k++;
		    }
		    while (j < n2) 
			{
		        queue[k] = rightArr[j];
		        j++;
		        k++;
		    }
		    
		    delete[] leftArr;
		    delete[] rightArr;
		}

		void mergeSortByCategory(int left, int right) 
		{
		    if (left < right) 
			{
		        int mid = left + (right - left) / 2;
		        mergeSortByCategory(left, mid);
		        mergeSortByCategory(mid + 1, right);
		        mergeByCategory(left, mid, right);
		    }
		}

		//using the category to search product, also show will table
		void searchByCategory(const string& target) 
		{
		    if (!empty()) 
			{
		        mergeSortByCategory(head, tail);
		    } 
			else 
			{
		        cout << "Queue is empty!" << endl;
		        return;
		    }
		    
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		    cout << "| ID      | Category     | Product Name                 | Price    | Cost     | Stock    |" << endl;
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		    
		    bool found = false;
		    int left = head;
		    int right = tail;

		    while (left <= right) 
			{
		        int mid = left + (right - left) / 2;
		        
		        if (queue[mid].category == target) 
				{

		            found = true;

		            int temp = mid;
		            while (temp >= head && queue[temp].category == target) 
					{
		                printProduct(queue[temp]);
		                temp--;
		            }

		            temp = mid + 1;
		            while (temp <= tail && queue[temp].category == target) 
					{
		                printProduct(queue[temp]);
		                temp++;
		            }
		            break;
		        }
		        else if (queue[mid].category < target) 
				{
		            left = mid + 1;
		        }
		        else 
				{
		            right = mid - 1;
		        }
		    }
		    
		    if (!found) 
			{
		        cout << "|" << setw(84) << right << "No product found in this category." << " |" << endl;
		    }
		    
		    cout << "+---------+--------------+------------------------------+----------+----------+----------+" << endl;
		}

		void printProduct(const Apple& product) 
		{
		    cout << "| ";
		    cout.width(7);  cout << left  << product.id << " | ";
		    cout.width(12); cout << left  << product.category << " | ";
		    cout.width(28); cout << left  << product.name << " | ";
		    cout.width(8);  cout << right << fixed << setprecision(2) << product.price << " | ";
		    cout.width(8);  cout << right << fixed << setprecision(2) << product.cost << " | ";
		    cout.width(8);  cout << right << product.stock << " |" << endl;
		}

		//load data from file
		void loadFile()
		{
			ifstream file("Apple_Store.txt");
			string line;
			
			if(!file.is_open())
			{
				cout << "Error opening file! " << endl;
			}
			
			//this way of loading data from the AI reference
			while (getline(file, line))
			{
				string id, category, product_name;
				double price, cost, stock;
				
				size_t pos1 = line.find(',');
		        size_t pos2 = line.find(',', pos1+1);
		        size_t pos3 = line.find(',', pos2+1);
		        size_t pos4 = line.find(',', pos3+1); 
		        size_t pos5 = line.find(',', pos4+1); 
		        
		        id = line.substr(0, pos1);
		        category = line.substr(pos1+1, pos2-pos1-1); 
		        product_name = line.substr(pos2+1, pos3-pos2-1); 
		        price = stod(line.substr(pos3+1, pos4-pos3-1)); 
		        cost = stod(line.substr(pos4+1, pos5-pos4-1)); 
		        stock = stod(line.substr(pos5+1)); 
		        

		        addProduct(id, category, product_name, price, cost, stock);
			}
			
			file.close();
		}
		
		void compactQueue() 
		{
		    int newTail = head - 1;
		    for (int i = head; i <= tail; i++) 
			{
		        if (!queue[i].deleted) 
				{
		            queue[++newTail] = queue[i];
		        }
		    }
		    tail = newTail;
		    cout << "Queue compacted. New size: " << (tail-head+1) << endl;
		}
		
		void updateStock(const string& id, double addedStock) 
		{
		    if (addedStock <= 0) {
		        cout << "Invalid input! Stock addition must be positive." << endl;
		        return;
		    }
		
		    if (empty()) {
		        cout << "Error: Queue is empty, cannot update." << endl;
		        return;
		    }
		
		    bool found = false;

		    for (int i = head; i <= tail; i++) 
			{
		        if (!queue[i].deleted && queue[i].id == id) 
				{
		            queue[i].stock += addedStock;
		            found = true;
		            
		            saveToFile("Apple_Store.txt");
		            
		            cout << "Stock updated! Product " << id 
		                 << " now has " << queue[i].stock
		                 << " units." << endl;
		            break;
		        }
		    }
		
		    if (!found) 
			{
		        cout << "Product " << id << " not found!" << endl;
		    }
		}
		
		//release memory
		void releaseMemory()
		{
		    head = 0;
        	tail = -1;
        	count = 0;
		}
		
		~Apple_Store() 
		{
        	releaseMemory();
    	}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void displayCategories() 
		{
		    string categories[10]; 
		    int catCount = 0;
		
		    cout << "=== Available Categories ===" << endl;
		    
		    for (int i = head; i <= tail; i++) 
			{
		        if (!queue[i].deleted) 
				{
		            bool exists = false;

		            for (int j = 0; j < catCount; j++) 
					{
		                if (categories[j] == queue[i].category) 
						{
		                    exists = true;
		                    break;
		                }
		            }

		            if (!exists && catCount < 10) 
					{
		                categories[catCount] = queue[i].category;
		                cout << (catCount + 1) << ". " << queue[i].category << endl;
		                catCount++;
		            }
		        }
		    }
		
		    if (catCount == 0) 
			{
		        cout << "No categories available." << endl;
		    }
		}
		
		void displayData(const string& category) 
		{
		    bool found = false;
		
		    cout << "+---------+--------------+------------------------------+----------+" << endl;
		    cout << "| ID      | Category     | Product Name                 | Price    |" << endl;
		    cout << "+---------+--------------+------------------------------+----------+" << endl;

		    for (int i = head; i <= tail; i++) 
			{
		        if (!queue[i].deleted && queue[i].category == category) 
				{
		            found = true;
		            cout << "| ";
		            cout.width(7);  cout << left << queue[i].id << " | ";
		            cout.width(12); cout << left << queue[i].category << " | ";
		            cout.width(28); cout << left << queue[i].name << " | ";
		            cout.width(8);  cout << right << fixed << setprecision(2) << queue[i].price << " | " << endl;
		        }
		    }
		
		    if (!found) 
			{
		        cout << "|" << setw(84) << right << "No product found in this category." << " |" << endl;
		    }
		
		    cout << "+---------+--------------+------------------------------+----------+" << endl;
		}
		
		int getCategoryCount() 
		{
		    string categories[10];
		    int catCount = 0;
		
		    for (int i = head; i <= tail; i++) 
			{
		        if (!queue[i].deleted) 
				{ 
		            bool exists = false;
		
		            for (int j = 0; j < catCount; j++) 
					{
		                if (categories[j] == queue[i].category) 
						{
		                    exists = true;
		                    break;
		                }
		            }
		
		            if (!exists && catCount < 10)
					{
		                categories[catCount++] = queue[i].category;
		            }
		        }
		    }
		
		    return catCount;
		}
		
		string getCategoryByIndex(int select) 
		{
		    string categories[10];
		    int catCount = 0;
		
		    for (int i = head; i <= tail; i++) 
			{
		        if (!queue[i].deleted) 
				{
		            bool exists = false;
		
		            for (int j = 0; j < catCount; j++) {
		                if (categories[j] == queue[i].category) 
						{
		                    exists = true;
		                    break;
		                }
		            }
		            if (!exists && catCount < 10) {
		                categories[catCount++] = queue[i].category;
		            }
		        }
		    }
		
		    return (select > 0 && select <= catCount) ? categories[select - 1] : "";
		}
		
		void addToCart(const string& category, Person* currentUser) 
		{
		    displayData(category);
		    
		    cout << "\nEnter Product ID to purchase: ";
		    string id;
		    cin.ignore();
		    getline(cin, id);
		
		    Apple* selectedProduct = nullptr;
		    for (int i = head; i <= tail && !selectedProduct; i++)
			{
		        if (!queue[i].deleted && queue[i].id == id && queue[i].category == category) 
				{
		            selectedProduct = &queue[i];
		        }
		    }
		
		    if (!selectedProduct) 
			{
		        cout << "Product not found in this category." << endl;
		        return;
		    }
		
		    cout << "Enter quantity to purchase: ";
		    int qty;
		    cin >> qty;
		
		    if (qty <= 0) 
			{
		        cout << "Invalid quantity!" << endl;
		        return;
		    }
		    if (selectedProduct->stock < qty) 
			{
		        cout << "Not enough stock available. Current stock: " 
		             << selectedProduct->stock << endl;
		        return;
		    }
		
		    double total = qty * selectedProduct->price;
		
		    cout << "\nPurchase successful!" << endl;
		    cout << "Product: " << selectedProduct->name << endl;
		    cout << "Quantity: " << qty << endl;
		    cout << "Total price: RM " << fixed << setprecision(2) << total << endl;
		
		    ofstream cartFile("Cart.txt", ios::app);
			if (cartFile && currentUser != nullptr) {
			    cartFile << currentUser->getEmail() << ","
			             << currentUser->getRole() << ","
			             << selectedProduct->id << ","
			             << selectedProduct->name << ","
			             << selectedProduct->price << ","
			             << qty << ","
			             << total << endl;
		        cartFile.close();
		    
		    } 
			else 
			{
		        cout << "Failed to write to cart file." << endl;
		    }
		}
		
		void viewCart(Person* currentUser)
		{
		    if (currentUser == nullptr) {
		        cout << "\nNo user logged in.\n";
		        return;
		    }
		
		    ifstream cartFile("Cart.txt");
		    if (!cartFile) {
		        cout << "\nCart file not found or cannot be opened." << endl;
		        return;
		    }
		
		    string line;
		    int index = 1;
		    double total = 0.0;
		    string currentEmail = currentUser->getEmail();
		
		    cout << "\n=== Your Shopping Cart ===" << endl;
		    cout << "+----+------------------------------+----------+--------+----------+" << endl;
		    cout << "| No | Product Name                 | Price    | Qty    | Subtotal |" << endl;
		    cout << "+----+------------------------------+----------+--------+----------+" << endl;
		
		    while (getline(cartFile, line)) {
		        string email, role, id, name, priceStr, qtyStr, subtotalStr;
		
		        stringstream ss(line);
		        getline(ss, email, ',');
		        getline(ss, role, ',');
		        getline(ss, id, ',');
		        getline(ss, name, ',');
		        getline(ss, priceStr, ',');
		        getline(ss, qtyStr, ',');
		        getline(ss, subtotalStr, ',');
		
		        if (email == currentEmail) {
		            double price = stod(priceStr);
		            int qty = stoi(qtyStr);
		            double subtotal = stod(subtotalStr);
		
		            cout << "| " << setw(2) << left << index++ << " | ";
		            cout << setw(28) << left << name << " | ";
		            cout << setw(8) << fixed << setprecision(2) << left << price << " | ";
		            cout << setw(6) << left << qty << " | ";
		            cout << setw(8) << left << subtotal << " |" << endl;
		
		            total += subtotal;
		        }
		    }
		
		    cout << "+----+------------------------------+----------+--------+----------+" << endl;
		    cout << "Total Amount: RM " << fixed << setprecision(2) << total << endl;
		
		    cartFile.close();
		}
	
		string getCurrentDateTime() 
		{
		    time_t now = time(0);                     
		    tm *ltm = localtime(&now);              
		
		    char buffer[20];                         
		    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
		
		    return string(buffer);
		}
		
		string getInv() 
		{
		    time_t now = time(0);                     
		    tm *ltm = localtime(&now);              
		
		    char buffer[20];                         
		    strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", ltm);
		
		    return string(buffer);
		}
		
		void checkoutCart(const string& currentEmail, const string& currentRole) 
		{
		    ifstream inFile("Cart.txt");
		    if (!inFile) {
		        cout << "Cart.txt not found or cannot be opened." << endl;
		        return;
		    }
		
		    ofstream orderFile("Order.txt", ios::app);
		    if (!orderFile) {
		        cout << "Failed to open Order.txt for writing." << endl;
		        return;
		    }
		
		//    ofstream tempFile("temp_cart.txt");
		//    if (!tempFile) {
		//        cout << "Failed to open temp_cart.txt." << endl;
		//        return;
		//    }
		
		    string line;
		    double grandTotal = 0.0, subTotal = 0.0;
		    int count = 1;
		
		    string Inv = getInv();
		    string now = getCurrentDateTime();
		
		    cout << "Invoice No: " << Inv << endl;
		    cout << "+--------------------------------------------------------------------+" << endl;
		    cout << "|                                                                    |" << endl;
		    cout << "|                            Apple Store                             |" << endl;
		    cout << "|                                                                    |" << endl;
		    cout << "|                                                                    |" << endl;
		    cout << "|   Invoice No : INV" << Inv << "                                   |" << endl;
		    cout << "|   Date       : " << now << "                                 |" << endl;
		    cout << "|                                                                    |" << endl;
		    cout << "|                                                                    |" << endl;
		    cout << "|                                                                    |" << endl;
		
		    while (getline(inFile, line)) {
		        stringstream ss(line);
		        string email, role, id, name, priceStr, qtyStr, subtotalStr;
		
		        getline(ss, email, ',');
		        getline(ss, role, ',');
		        getline(ss, id, ',');
		        getline(ss, name, ',');
		        getline(ss, priceStr, ',');
		        getline(ss, qtyStr, ',');
		        getline(ss, subtotalStr);
		
		        if (email == currentEmail) {
		            double price = stod(priceStr);
		            int qty = stoi(qtyStr);
		            double subtotal = stod(subtotalStr);
		            subTotal += subtotal;
		
		            // 输出小票
		            cout << "|   " << count++ << ". " << setw(28) << left << name
		                 << " RM " << setw(10) << price
		                 << "   " << qty << "   "
		                 << "RM " << setw(10) << subtotal << "|" << endl;
		
		            // 写入订单记录：加入 email 与 role
		            orderFile << "\n" << now << "," << Inv << "," << email << "," << role << ","
		                      << id << "," << name << "," << price << "," << qty << "," << subtotal;
		        } else {
		//            tempFile << line << endl;  // 不是当前用户，写回 temp 文件
		        }
		    }
		
		    double SST = subTotal * 0.10;
		    grandTotal = subTotal + SST;
			cout << "|                                                                    |" << endl;
			cout << "|                                                                    |" << endl;
			cout << "|                                                                    |" << endl;
		    cout << "|   Subtotal                                            RM " << setw(10) << subTotal << "|" << endl;
		    cout << "|   SST (10%)                                           RM " << setw(10) << SST << "|" << endl;
		    cout << "|   Grand Total                                         RM " << setw(10) << grandTotal << "|" << endl;
		    cout << "|                                                                    |" << endl;
		    cout << "|                                                                    |" << endl;
		    cout << "+--------------------------------------------------------------------+" << endl;
		
		    // 最后一行写入：email + 金额信息
		    orderFile << "\n" << fixed << setprecision(2) << subTotal << "," << SST << "," << grandTotal << endl;
		    
		    ofstream summaryFile("Summary.txt", ios::app);
				if (summaryFile) 
				{
				    summaryFile << currentEmail << "," << now << "," << Inv << "," << fixed << setprecision(2) << grandTotal << "\n";
				    summaryFile.close();
				}
		
		    inFile.close();
		    orderFile.close();
		//    tempFile.close();
		
		//    remove("Cart.txt");
		//    rename("temp_cart.txt", "Cart.txt");
		
		    cout << "\nOrder completed and saved. Cart cleared for " << currentEmail << endl;
		}


		
		void viewOrderHistory(const string& currentEmail)
		{
		    ifstream inFile("Order.txt");
		    if (!inFile)
		    {
		        cout << "Order.txt not found or cannot be opened." << endl;
		        return;
		    }
		
		    string line;
		    string currentInv = "", currentDate = "", currentRole = "";
		    double subtotal = 0.0, sst = 0.0, grandTotal = 0.0;
		    bool hasOrder = false;
		
		    cout << "\n=== Order History for " << currentEmail << " ===" << endl;
		
		    while (getline(inFile, line))
		    {
		        if (line.empty()) continue;
		
		        // 计数逗号数量以判断字段数量
		        int commaCount = 0;
		        for (size_t i = 0; i < line.length(); i++)
		        {
		            if (line[i] == ',') commaCount++;
		        }
		
		        if (commaCount == 8)
		        {
		            // 产品行: 时间, 发票号, email, role, id, name, price, qty, subtotal
		            string date, inv, email, role, id, name, priceStr, qtyStr, subtotalStr;
		
		            size_t p1 = line.find(',');
		            size_t p2 = line.find(',', p1 + 1);
		            size_t p3 = line.find(',', p2 + 1);
		            size_t p4 = line.find(',', p3 + 1);
		            size_t p5 = line.find(',', p4 + 1);
		            size_t p6 = line.find(',', p5 + 1);
		            size_t p7 = line.find(',', p6 + 1);
		            size_t p8 = line.find(',', p7 + 1);
		
		            date = line.substr(0, p1);
		            inv = line.substr(p1 + 1, p2 - p1 - 1);
		            email = line.substr(p2 + 1, p3 - p2 - 1);
		            role = line.substr(p3 + 1, p4 - p3 - 1);
		            id = line.substr(p4 + 1, p5 - p4 - 1);
		            name = line.substr(p5 + 1, p6 - p5 - 1);
		            priceStr = line.substr(p6 + 1, p7 - p6 - 1);
		            qtyStr = line.substr(p7 + 1, p8 - p7 - 1);
		            subtotalStr = line.substr(p8 + 1);
		
		            if (email != currentEmail) continue;
		
		            if (inv != currentInv)
		            {
		                if (!currentInv.empty())
		                {
		                    // 打印上一张订单总计
		                    cout << "+--------------------------------------------------------------+" << endl;
		                    cout << "Subtotal   : RM " << fixed << setprecision(2) << subtotal << endl;
		                    cout << "SST (10%)  : RM " << fixed << setprecision(2) << sst << endl;
		                    cout << "Grand Total: RM " << fixed << setprecision(2) << grandTotal << endl;
		                    cout << "+==============================================================+\n" << endl;
		                    subtotal = sst = grandTotal = 0.0;
		                }
		
		                currentInv = inv;
		                currentDate = date;
		                currentRole = role;
		                hasOrder = true;
		
		                cout << "\nDate: " << date << " | Invoice: INV" << inv << endl;
		                cout << "+--------------------------------------------------------------+" << endl;
		                cout << "| Product Name                 | Price   | Qty | Subtotal      |" << endl;
		                cout << "+--------------------------------------------------------------+" << endl;
		            }
		
		            double price = stod(priceStr);
		            int qty = stoi(qtyStr);
		            double itemTotal = stod(subtotalStr);
		
		            cout << "| " << setw(28) << left << name
		                 << " | " << setw(7) << fixed << setprecision(2) << price
		                 << " | " << setw(3) << qty
		                 << " | " << setw(13) << itemTotal << " |" << endl;
		
		            subtotal += itemTotal;
		            sst = subtotal * 0.10;
		            grandTotal = subtotal + sst;
		        }
		        else if (commaCount == 3)
		        {
		            // 最后一行小计,可忽略（我们已计算）
		            continue;
		        }
		    }
		
		    // 打印最后一张订单总计
		    if (hasOrder)
		    {
		        cout << "+--------------------------------------------------------------+" << endl;
		        cout << "Subtotal   : RM " << fixed << setprecision(2) << subtotal << endl;
		        cout << "SST (10%)  : RM " << fixed << setprecision(2) << sst << endl;
		        cout << "Grand Total: RM " << fixed << setprecision(2) << grandTotal << endl;
		        cout << "+==============================================================+\n" << endl;
		    }
		    else
		    {
		        cout << "No order history found for this email.\n";
		    }
		
		    inFile.close();
		}
		
		void buyMembership(Person*& currentUser) 
		{
		    if (currentUser == nullptr) {
		        cout << "You must be logged in to buy membership.\n";
		        return;
		    }
		
		    string role = currentUser->getRole();
		    if (role == "Member") 
			{
		        cout << "You are already a Member. Cannot purchase again.\n";
		        return;
		    }
		
		    char confirm;
		    cout << "Membership costs RM100 and provides 10% discount on all purchases.\n";
		    cout << "Do you want to purchase? (Y/N): ";
		    cin >> confirm;
		    if (tolower(confirm) != 'y') {
		        cout << "Purchase cancelled.\n";
		        return;
		    }
		
		    // Step 1: Update User.txt
		    ifstream inFile("User.txt");
		    ofstream tempFile("temp_users.txt");
		    if (!inFile || !tempFile) {
		        cout << "Error opening User.txt or temporary file.\n";
		        return;
		    }
		
		    string line;
		    string currentEmail = currentUser->getEmail();
		    while (getline(inFile, line)) {
		        stringstream ss(line);
		        string uname, email, pass, phone, r;
		        getline(ss, uname, ',');
		        getline(ss, email, ',');
		        getline(ss, pass, ',');
		        getline(ss, phone, ',');
		        getline(ss, r, ',');
		
		        if (email == currentEmail) {
		            // Write with updated role
		            tempFile << uname << "," << email << "," << pass << "," << phone << ",Member\n";
		        } else {
		            tempFile << line << "\n";
		        }
		    }
		
		    inFile.close();
		    tempFile.close();
		    remove("User.txt");
		    rename("temp_users.txt", "User.txt");
		
		    // Step 2: Replace runtime object
		    string oldName = currentUser->getRole();
		    delete currentUser;
		    currentUser = new Member(currentEmail, currentEmail, "", ""); // Note: add correct values
		
		    cout << "Membership successfully purchased! You're now a Member.\n";
		}
		
		void editCart(Person* currentUser) 
		{
		    ifstream inFile("Cart.txt");
		    ofstream tempFile("temp_cart.txt");
		
		    if (!inFile || !tempFile) {
		        cout << "Error opening Cart.txt or temp file." << endl;
		        return;
		    }
		
		    string currentEmail = currentUser->getEmail();
		    string line;
		    int itemNumber = 0;
		    string cartLines[100]; // 存储当前用户购物车项
		    int indexes[100];      // 映射编号和原始行位置
		
		    cout << "\n=== Edit Your Shopping Cart ===\n";
		    cout << "+----+------------------------------+----------+--------+----------+\n";
		    cout << "| No | Product Name                 | Price    | Qty    | Subtotal |\n";
		    cout << "+----+------------------------------+----------+--------+----------+\n";
		
		    int totalLines = 0;
		
		    while (getline(inFile, line)) {
		        stringstream ss(line);
		        string email, role, id, name, priceStr, qtyStr, subtotalStr;
		        getline(ss, email, ',');
		        getline(ss, role, ',');
		        getline(ss, id, ',');
		        getline(ss, name, ',');
		        getline(ss, priceStr, ',');
		        getline(ss, qtyStr, ',');
		        getline(ss, subtotalStr, ',');
		
		        if (email == currentEmail) {
		            cartLines[itemNumber] = line;
		            indexes[itemNumber] = totalLines;
		            itemNumber++;
		
		            double price = stod(priceStr);
		            int qty = stoi(qtyStr);
		            double subtotal = stod(subtotalStr);
		
		            cout << "| " << setw(2) << left << itemNumber << " | "
		                 << setw(28) << left << name << " | "
		                 << setw(8) << fixed << setprecision(2) << left << price << " | "
		                 << setw(6) << left << qty << " | "
		                 << setw(8) << left << subtotal << " |\n";
		        }
		        totalLines++;
		    }
		
		    cout << "+----+------------------------------+----------+--------+----------+\n";
		
		    if (itemNumber == 0) {
		        cout << "Your cart is empty.\n";
		        inFile.close();
		        tempFile.close();
		        remove("temp_cart.txt");
		        return;
		    }
		
		    int choice;
		    cout << "Enter item number to edit (0 to cancel): ";
		    cin >> choice;
		
		    if (choice <= 0 || choice > itemNumber) {
		        cout << "Cancelled or invalid choice.\n";
		        inFile.close();
		        tempFile.close();
		        remove("temp_cart.txt");
		        return;
		    }
		
		    string selectedLine = cartLines[choice - 1];
		    stringstream ss(selectedLine);
		    string email, role, id, name, priceStr, qtyStr, subtotalStr;
		    getline(ss, email, ',');
		    getline(ss, role, ',');
		    getline(ss, id, ',');
		    getline(ss, name, ',');
		    getline(ss, priceStr, ',');
		    getline(ss, qtyStr, ',');
		    getline(ss, subtotalStr, ',');
		
		    cout << "\nEditing: " << name << " (Current Qty: " << qtyStr << ")\n";
		    cout << "1. Change Quantity\n";
		    cout << "2. Remove from Cart\n";
		    cout << "Enter your choice: ";
		    int editOption;
		    cin >> editOption;
		
		    bool skipLine = false;
		    int newQty = stoi(qtyStr);
		    double price = stod(priceStr);
		    double newSubtotal;
		
		    if (editOption == 1) {
		        cout << "Enter new quantity (0 to remove): ";
		        cin >> newQty;
		        if (newQty <= 0) {
		            skipLine = true;
		            cout << name << " has been removed from your cart.\n";
		        } else {
		            newSubtotal = price * newQty;
		            cout << name << " quantity updated to " << newQty << ".\n";
		        }
		    } else if (editOption == 2) {
		        skipLine = true;
		        cout << name << " has been removed from your cart.\n";
		    } else {
		        cout << "Invalid edit option.\n";
		    }
		
		    // 重读 Cart.txt 并写入修改后数据
		    inFile.clear();
		    inFile.seekg(0);
		    int currentLine = 0;
		    while (getline(inFile, line)) {
		        bool isTargetLine = (currentLine == indexes[choice - 1]);
		
		        if (isTargetLine) {
		            if (!skipLine) {
		                tempFile << email << "," << role << "," << id << "," << name << ","
		                         << price << "," << newQty << "," << fixed << setprecision(2)
		                         << newSubtotal << "\n";
		            }
		        } else {
		            tempFile << line << "\n";
		        }
		
		        currentLine++;
		    }
		
		    inFile.close();
		    tempFile.close();
		
		    remove("Cart.txt");
		    rename("temp_cart.txt", "Cart.txt");
		
		    cout << "Cart updated successfully.\n";
		}
		
		void orderSummary(const string& currentEmail) 
		{
		    ifstream inFile("Summary.txt");
		    if (!inFile) {
		        cout << "Summary.txt not found or cannot be opened." << endl;
		        return;
		    }
		
		    string line, email, date, inv, totalStr;
		    double grandTotal;
		
		    cout << "\n=== Order Summary ===" << endl;
		    cout << "+---------------------+--------------------+---------------+" << endl;
		    cout << "|       Date          |     Invoice No     | Grand Total   |" << endl;
		    cout << "+---------------------+--------------------+---------------+" << endl;
		
		    while (getline(inFile, line)) {
		        stringstream ss(line);
		        getline(ss, email, ',');
		        getline(ss, date, ',');
		        getline(ss, inv, ',');
		        getline(ss, totalStr);
		
		        if (email != currentEmail) continue;
		
		        stringstream(totalStr) >> grandTotal;
		
		        cout << "| " << setw(19) << left << date
		             << " |  " << setw(18) << left << ("INV" + inv)
		             << "| RM " << setw(10) << fixed << setprecision(2) << grandTotal << " |" << endl;
		    }
		
		    cout << "+---------------------+--------------------+---------------+" << endl;
		    inFile.close();
		}

		void mergeSummary(double totals[], string dates[], string invoices[], int left, int mid, int right) 
		{
		    int n1 = mid - left + 1;
		    int n2 = right - mid;
		
		    double Ltotals[100], Rtotals[100];
		    string Ldates[100], Rdates[100];
		    string Linv[100], Rinv[100];
		
		    for (int i = 0; i < n1; i++) {
		        Ltotals[i] = totals[left + i];
		        Ldates[i] = dates[left + i];
		        Linv[i] = invoices[left + i];
		    }
		
		    for (int j = 0; j < n2; j++) {
		        Rtotals[j] = totals[mid + 1 + j];
		        Rdates[j] = dates[mid + 1 + j];
		        Rinv[j] = invoices[mid + 1 + j];
		    }
		
		    int i = 0, j = 0, k = left;
		    while (i < n1 && j < n2) {
		        if (Ltotals[i] <= Rtotals[j]) {
		            totals[k] = Ltotals[i];
		            dates[k] = Ldates[i];
		            invoices[k] = Linv[i];
		            i++;
		        } else {
		            totals[k] = Rtotals[j];
		            dates[k] = Rdates[j];
		            invoices[k] = Rinv[j];
		            j++;
		        }
		        k++;
		    }
		
		    while (i < n1) {
		        totals[k] = Ltotals[i];
		        dates[k] = Ldates[i];
		        invoices[k] = Linv[i];
		        i++;
		        k++;
		    }
		
		    while (j < n2) {
		        totals[k] = Rtotals[j];
		        dates[k] = Rdates[j];
		        invoices[k] = Rinv[j];
		        j++;
		        k++;
		    }
		}

		void mergeSortSummary(double totals[], string dates[], string invoices[], int left, int right) 
		{
		    if (left < right) 
			{
		        int mid = (left + right) / 2;
		        mergeSortSummary(totals, dates, invoices, left, mid);
		        mergeSortSummary(totals, dates, invoices, mid + 1, right);
		        mergeSummary(totals, dates, invoices, left, mid, right);
		    }
		}

		void viewSummarySortedByTotal(const string& currentEmail) 
		{
		    ifstream inFile("Summary.txt");
		    if (!inFile) {
		        cout << "Summary.txt not found.\n";
		        return;
		    }
		
		    const int MAX = 100;
		    string dates[MAX], invoices[MAX];
		    double totals[MAX];
		    int count = 0;
		
		    string line;
		    while (getline(inFile, line)) {
		        stringstream ss(line);
		        string email, date, inv, totalStr;
		        getline(ss, email, ',');
		        getline(ss, date, ',');
		        getline(ss, inv, ',');
		        getline(ss, totalStr);
		
		        if (email == currentEmail && count < MAX) {
		            dates[count] = date;
		            invoices[count] = inv;
		            totals[count] = stod(totalStr);
		            count++;
		        }
		    }
		    inFile.close();
		
		    // 使用 Merge Sort 排序
		    mergeSortSummary(totals, dates, invoices, 0, count - 1);
		
		    // Display sorted result
		    cout << "\n=== Order Summary Sorted by Grand Total (Low to High) ===\n";
		    cout << "+---------------------+--------------------+---------------+" << endl;
		    cout << "|       Date          |     Invoice No     | Grand Total   |" << endl;
		    cout << "+---------------------+--------------------+---------------+" << endl;
		
		    for (int i = 0; i < count; i++) {
		        cout << "| " << setw(19) << left << dates[i]
		             << " |  " << setw(18) << left << invoices[i]
		             << "| RM " << setw(10) << fixed << setprecision(2) << totals[i] << " |" << endl;
		    }
		
		    cout << "+---------------------+--------------------+---------------+" << endl;
		}


		
		void searchSummaryByMonthFromOrder(const string& currentEmail) 
		{
		    const int MAX = 200;
		    string dates[MAX], invs[MAX], emails[MAX];
		    double totals[MAX];
		    int count = 0;
		
		    ifstream inFile("Order.txt");
		    if (!inFile) {
		        cout << "Order.txt not found.\n";
		        return;
		    }
		
		    string line, date, inv, email;
		    double grandTotal = 0.0;
		
		    while (getline(inFile, line)) {
		        if (line.empty()) continue;
		
		        int commaCount = 0;
		        for (char ch : line) {
		            if (ch == ',') commaCount++;
		        }
		
		        if (commaCount >= 4) {
		            // 订单明细行：提取 date, inv, email
		            size_t p1 = line.find(',');
		            size_t p2 = line.find(',', p1 + 1);
		            size_t p3 = line.find(',', p2 + 1);
		
		            date = line.substr(0, p1);
		            inv = line.substr(p1 + 1, p2 - p1 - 1);
		            email = line.substr(p2 + 1, p3 - p2 - 1);
		        }
		        else if (commaCount == 2) {
		            // 总结行，提取 grand total
		            size_t p1 = line.find(',');
		            size_t p2 = line.find(',', p1 + 1);
		            string grand = line.substr(p2 + 1);
		            grandTotal = stod(grand);
		
		            if (count < MAX) {
		                dates[count] = date;
		                invs[count] = inv;
		                emails[count] = email;
		                totals[count] = grandTotal;
		                count++;
		            }
		        }
		    }
		    inFile.close();
		
		    if (count == 0) {
		        cout << "No order data available.\n";
		        return;
		    }
		
		    // 输入月份
		    string month;
		    cout << "Enter month to search (format: YYYY-MM): ";
		    cin >> month;
		
		    // 手动实现二分查找区间
		    int left = 0, right = count - 1;
		    int startIdx = -1, endIdx = -1;
		
		    // 找起始
		    while (left <= right) {
		        int mid = (left + right) / 2;
		        string d = dates[mid].substr(0, 7);
		        if (d >= month) {
		            if (d == month) startIdx = mid;
		            right = mid - 1;
		        } else {
		            left = mid + 1;
		        }
		    }
		
		    // 找结尾
		    left = 0; right = count - 1;
		    while (left <= right) {
		        int mid = (left + right) / 2;
		        string d = dates[mid].substr(0, 7);
		        if (d <= month) {
		            if (d == month) endIdx = mid;
		            left = mid + 1;
		        } else {
		            right = mid - 1;
		        }
		    }
		
		    // 输出结果
		    cout << "\n=== Order Summary for " << month << " ===" << endl;
		    cout << "+------------------------+--------------------+---------------+" << endl;
		    cout << "|          Date          |     Invoice No     | Grand Total   |" << endl;
		    cout << "+------------------------+--------------------+---------------+" << endl;
		
		    bool found = false;
		    for (int i = startIdx; i <= endIdx && i != -1; i++) {
		        if (emails[i] == currentEmail) {
		            cout << "| " << left << dates[i]
		                 << "  |  " << ("INV" + invs[i])
		                 << " | RM " << setw(10) << fixed << setprecision(2) << totals[i] << " |" << endl;
		            found = true;
		        }
		    }
		
		    if (!found) {
		        cout << "| No orders found for this month.                                  |" << endl;
		    }
		
		    cout << "+------------------------+--------------------+---------------+" << endl;
		}

			
};

void registerUser() 
{
    User u;
    cout << "Enter username: ";
    cin >> u.username;

    cout << "Enter email: ";
    cin >> u.email;

    cout << "Enter password: ";
    cin >> u.password;

    cout << "Enter phone number: ";
    cin >> u.phoneNumber;

    // Default role is Customer
    u.role = "Customer";

    ofstream outFile("User.txt", ios::app); // append mode
    if (!outFile) {
        cout << "Failed to open User.txt\n";
        return;
    }

    outFile << u.username << "," << u.email << "," << u.password << ","
            << u.phoneNumber << "," << u.role << "\n";

    outFile.close();
}

int main()
{
	Apple_Store A;
	A.loadFile();
	string id, product_name, category;
	double price, cost, stock, addedStock;
	int choice;
	char option;
	string target;
	
	string now = A.getCurrentDateTime();

	int mainChoice;
    Person* currentUser = nullptr;

   bool loggedIn = false;

do {
    cout << "===============================================\n";
    cout << "              Welcome to Apple Store           \n";
    cout << "===============================================\n";
    cout << "             1. Register as a new user" << endl;
    cout << "             2. Login" << endl;
    cout << "             3. Exit" << endl;
    cout << "            \nEnter your choice: ";
    cin >> mainChoice;

    switch (mainChoice) 
	{
        case 1: 
		{
		    User u;
		    cout << "\n=== Register New User ===\n";
		
		    cout << "Enter username: ";
		    cin >> u.username;
		
		    // Email validation
		    do {
		        cout << "Enter email (must include .com): ";
		        cin >> u.email;
		        if (u.email.find(".com") == string::npos) {
		            cout << "Invalid email format. Try again.\n";
		        }
		    } while (u.email.find(".com") == string::npos);
		
		    // Password with masking
		    cout << "Enter password: ";
		    u.password = getPassword();
		
		    // Phone number validation
		    do 
			{
		        cout << "Enter phone number (9-11 digits): ";
		        cin >> u.phoneNumber;
		        if (u.phoneNumber.length() < 9 || u.phoneNumber.length() > 11) 
				{
		            cout << "Invalid phone number length. Try again.\n";
		        }
		    } while (u.phoneNumber.length() < 9 || u.phoneNumber.length() > 11);
		
		    u.role = "Customer";
		
		    ofstream outFile("User.txt", ios::app);
		    if (!outFile) {
		        cout << "Failed to open User.txt\n";
		        break;
		    }
		
		    outFile << u.username << "," << u.email << "," << u.password << ","
		            << u.phoneNumber << "," << u.role << "\n";
		    outFile.close();
		
		    // 自动登录用户
		    currentUser = new Customer(u.username, u.email, u.password, u.phoneNumber);
		    loggedIn = true;
		    break;
		}

        case 2:
            currentUser = loginUser();
            if (currentUser != nullptr) 
			{
                loggedIn = true;
            }
            break;

        case 3:
            cout << "Thank you for visiting Apple Store. Goodbye!" << endl;
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
    }

    // ? 如果成功登录/注册，就退出主菜单循环
    if (loggedIn) break;

} while (mainChoice != 3);
	
	
	
	
	
	
	
	
	////////////////////////////////////////////////
	do
	{
		system("cls");
		cout << "Current time: " << now << endl;
		cout << "Welcome to Apple Store" << endl;
		cout << "1. Display Data" << endl;
		cout << "2. Display Category" << endl;
		cout << "3. Add to Cart" << endl;
		cout << "4. View Cart" << endl;
		cout << "5. Check Out" << endl;
		cout << "6. View Order History" << endl;
		cout << "7. Membership" << endl;
		cout << "8. Edit Cart" << endl;
		cout << "9. Order Summary" << endl;
		cout << "10. Sort by Summary Price" << endl;
		cout << "11. Search Order by Date" << endl;
		cout << "12. Log Out" << endl;
	
		cout << "Enter choice (1-20) : ";
		cin >> choice;
		
		if(choice < 1 || choice > 20)
		{
			cout << "Invalid Input! Please enter again." << endl;
			
			cout << "Enter choice (1-20) : ";
			cin >> choice;
		}
		
		switch(choice)
		{
			case 1: do
					{
						system("cls");

						cout << "=== Display Product ===" << endl;
						A.displayData();
						
						cout << "\nEnter 1 to exit" << endl;
						cin >> choice;
					}while(choice != 1);
					break;
					
			case 2: do
					{
						system("cls");
						A.displayCategories();
						
						int select;
						cout << "\nSelect category number: ";
						cin >> select;
						
						string selectedCategory = A.getCategoryByIndex(select);
						
						if (selectedCategory != "")
						{
						    A.displayData(selectedCategory);
						}
						else
						{
						    cout << "Invalid category selected!" << endl;
						}
						
						cout << "\nEnter 1 to exit" << endl;
						cin >> choice;
					}while(choice != 1);
					break;
					
			case 3: do
					{
				    system("cls");
				    A.displayCategories();
				
				    int select;
				    cout << "\nSelect category number to view and purchase from: ";
				    cin >> select;
				
				    string selectedCategory = A.getCategoryByIndex(select);
				    if (selectedCategory != "")
				    {
				        A.addToCart(selectedCategory, currentUser);
				    }
				    else
				    {
				        cout << "Invalid category selected!" << endl;
				    }
				
				    cout << "\nEnter 1 to exit" << endl;
				    cin >> choice;
					}while(choice != 1);
					break;
			
			case 4: do
					{
					    system("cls");
					    A.viewCart(currentUser);
					
					    cout << "\nEnter 1 to exit" << endl;
					    cin >> choice;
					}while(choice != 1);
					break;
					
			case 5: do
					{
						system("cls");
						cout << now << endl;
						A.checkoutCart(currentUser->getEmail(), currentUser->getRole()); 
						
						cout << "\nEnter 1 to exit" << endl;
				    	cin >> choice;
					}while(choice != 1);
					break;
					
			case 6: do
					{
						system("cls");
    					A.viewOrderHistory(currentUser->getEmail());
						
						cout << "\nEnter 1 to exit" << endl;
						cin >> choice;
					}while(choice != 1);
					break;
					
			case 7: do
					{
						A.buyMembership(currentUser);
						
						cout << "\nEnter 1 to exit" << endl;
						cin >> choice;
					 } while(choice != 1);
				    break;
				    
			case 8: do
					{
				        system("cls");
				        A.editCart(currentUser);
				        
				        cout << "\nEnter 1 to exit: ";
				        cin >> choice;
				    } while (choice != 1);
					break;
			
			case 9: do
					{
						system("cls");
					    A.orderSummary(currentUser->getEmail());
					    
					    cout << "\nEnter 1 to exit" << endl;
					    cin >> choice;
					} while (choice != 1);
				    break;
				    
			case 10:do 
						{
					        system("cls");
					        A.viewSummarySortedByTotal(currentUser->getEmail());
					        
					        cout << "\nEnter 1 to exit" << endl;
					        cin >> choice;
					    } while (choice != 1);
					    break;
					    
			case 11: do
						{
							system("cls");
			    			A.searchSummaryByMonthFromOrder(currentUser->getEmail());
			    			
			    			cout << "\nEnter 1 to exit" << endl;
					        cin >> choice;
						}while (choice != 1);
 						break;
					
			case 12:delete currentUser;
				    currentUser = nullptr;
				    loggedIn = false;
				    cout << "You have been logged out successfully.\n";
				    break;
			case 20: A.releaseMemory();
					return 0;
		}
			
	}while(choice != 20 && loggedIn);
	
	return 0;
}