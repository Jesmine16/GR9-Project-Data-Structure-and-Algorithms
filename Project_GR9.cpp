#include<iostream>
#include<fstream>
#include<iomanip>
#include<ctime>
#include<sstream> 
#include <conio.h>
#include <cctype>

using namespace std;

const int RUN = 32; //use for sorting
const int MAX_PRODUCTS = 70;
const int MAX_ITEMS = 70;

struct Apple 
{
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
	
	Person* currentUser = nullptr;
	
	class Customer : public Person 
	{
		public:
    Customer(string uname, string em, string pass, string phone)
        : Person(uname, em, pass, phone) {}

    string getRole() override { return "Customer"; }
    void printRole() override { cout << "Logged in as Customer\n"; }
	};

	class Member : public Person 
	{
	public:
	    Member(string uname, string em, string pass, string phone)
	        : Person(uname, em, pass, phone) {}
	
	    string getRole() override { return "Member"; }
	    double getDiscountRate() override { return 0.10; } // 10% 折扣
	    void printRole() override { cout << "Logged in as Member (10% discount)\n"; }
	};

	class Admin : public Person 
	{
	public:
	    Admin(string uname, string em, string pass, string phone)
	        : Person(uname, em, pass, phone) {}
	
	    string getRole() override { return "Admin"; }
	    void printRole() override { cout << "Logged in as Admin\n"; }
	};
	
	class SuperAdmin : public Person
	{
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

int countCommas(const string& str) 
{
    int count = 0;
    for (char c : str) 
	{
        if (c == ',') count++;
    }
    return count;
}

bool isValidEmail(const string& email) 
{
	return email.find(".com") != string::npos;
}

bool isDigits(const string& s) {
	for (char c : s) {
		if (!isdigit(c)) return false;
	}
	return true;
}

string inputCardNumber() 
{
	string raw;
	while (true) 
	{
		cout << "\nCard Number (16 digits): ";
		cin >> ws;
		getline(cin, raw);

		string clean = "";
		for (char c : raw) 
		{
			if (isdigit(c)) clean += c;
		}

		if (clean.length() != 16 || !isDigits(clean)) 
		{
			cout << "Invalid card number. Must be exactly 16 digits.\n";
			continue;
		}

		// 格式化成 XXXX XXXX XXXX XXXX
		string formatted = "";
		for (int i = 0; i < 16; ++i) 
		{
			formatted += clean[i];
			if ((i + 1) % 4 == 0 && i != 15) formatted += ' ';
		}
		return formatted;
	}
}

// 输入并验证 Expiry Date（格式 MM/YY）
string inputExpDate() 
{
	string input;
	while (true) 
	{
		cout << "Expiry Date (MM/YY): ";
		cin >> input;

		// 自动补加 /
		if (input.length() == 4 && isDigits(input)) 
		{
			input = input.substr(0, 2) + "/" + input.substr(2, 2);
		}

		if (input.length() != 5 || input[2] != '/' ||
			!isdigit(input[0]) || !isdigit(input[1]) ||
			!isdigit(input[3]) || !isdigit(input[4])) 
			{
			cout << "Invalid format. Please use MM/YY.\n";
			continue;
		}

		int mm = stoi(input.substr(0, 2));
		if (mm < 1 || mm > 12) 
		{
			cout << "Invalid month. MM must be between 01 and 12.\n";
			continue;
		}
		return input;
	}
}

string inputCVV() 
{
	string input;
	while (true) 
	{
		cout << "CVV (3 digits): ";
		cin >> input;
		if (input.length() == 3 && isDigits(input)) 
		{
			return input;
		}
		cout << "Invalid CVV. Must be exactly 3 digits.\n";
	}
}

Person* loginUser() 
{
    ifstream inFile("User.txt");
    if (!inFile) 
	{
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
    while (getline(inFile, line)) 
	{
        stringstream ss(line);
        User u;

        getline(ss, u.username, ',');
        getline(ss, u.email, ',');
        getline(ss, u.password, ',');
        getline(ss, u.phoneNumber, ',');
        getline(ss, u.role, ',');

        if (u.email == emailInput && u.password == passInput) 
		{
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

class AppleStore 
{ 
    private:  
	    Apple queue[50];    
	    int head,tail;  
	
	public:  
	
	    AppleStore() 
		{  
	        tail = -1;  
	        head = 0;  
	    }  
		
		// Check the queue is empty or not
		int empty() 
		{  
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
		
		// Add a new product to the queue
		void append(string id, string category, string name, double price, double cost, int stock) 
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
			}
		}
		
		// Serve the product from the queue
		string serve(string id) 
		{
			
			string delProduct = "";
        	bool found = false;
			
			if(!empty()) {
				
				for (int i = head; i <= tail; i++) 
				{
        		    if (queue[i].id == id) 
					{
        			    found = true;
						
						delProduct = queue[i].name;
						cout << "\nProduct delete: " << delProduct << endl;
						
						// Reposition data after deleted (ensure no empty row)
						for (int j = i; j < tail; j++) 
						{
						    queue[j] = queue[j + 1];
						}
							 
						// Reduce the tail index since one data is deleted
						tail--;
						break;
					}
					
				} 
				
				if (!found) 
				{
			        cout << "Product not found...." << endl;
				} 
				else 
				{
					savetofile("Apple_Store.txt");
					savetofile("sorted_information.txt");
				}
			} 
			else 
			{
				cout << "Queue is empty" << endl;
			}
			
			return delProduct;
		}
		
		// Convert product IDs to a string array
		string* IDtoArray(int &size) 
		{
		    size = tail - head + 1;
			string* arr = new string[size];
			
			for (int i = 0; i < size; ++i) 
			{
			    arr[i] = queue[head + i].id;
			}
			
			return arr;
		}
		
		// Convert product categories to a string array
		string* CategorytoArray(int &size) 
		{
		    size = tail - head + 1;
			string* arr = new string[size];
			
			for (int i = 0; i < size; ++i) 
			{
			    arr[i] = queue[head + i].category; 
			}
			
			return arr;
		}
		
		// Convert product price to a string array
		string* PricetoArray(int &size) 
		{
		    size = tail - head + 1;
			string* arr = new string[size];
			
			for (int i = 0; i < size; ++i) 
			{
			    arr[i] = queue[head + i].price; 
			}
			
			return arr;
		}
		
		// Check for duplicate product ID
		void checkid(string id) 
		{
			
			if(!empty()) 
			{
				for(int i=head; i<=tail; i++) 
				{
					if(queue[i].id == id) 
					{
						cout << "This Product ID is already exists..." << endl;
						cout << "\nEnter product ID: ";
						cin >> id;
					}
				}
			}
		}
		
		// Read products from file and store in the queue
		void readfile(string filename) 
		{
			
			ifstream file(filename);
			
            string id, category, name;
            int stock;
            double cost, price;
            
            string line;
	
        	if(!file) 
			{
	        	cout << "Error opening file!";
    	    } 
			else 
			{
    	    	//Loading data (using reference)
	    	    while(getline(file, line)) 
				{
	    	    	
	    	    	// Parse the comma-separated line
					size_t pos1 = line.find(',');
					size_t pos2 = line.find(',', pos1+1);
					size_t pos3 = line.find(',', pos2+1);
					size_t pos4 = line.find(',', pos3+1); 
					size_t pos5 = line.find(',', pos4+1);
					
					id = line.substr(0, pos1);
					category = line.substr(pos1+1, pos2-pos1-1); 
					name = line.substr(pos2+1, pos3-pos2-1); 
					price = stod(line.substr(pos3+1, pos4-pos3-1)); 
					cost = stod(line.substr(pos4+1, pos5-pos4-1)); 
					stock = stoi(line.substr(pos5+1)); 
	    	    	
		    	    append(id, category, name, price, cost, stock); // Add to queue
		        }
		
    		    file.close();
	        }
        }

        // Save all product data to a file
        void savetofile(string filename) 
		{
        	
        	ofstream outfile(filename);
	
	        if(!outfile) 
			{
		        cout << "Error opening file!";
	        } 
			else 
			{
		        for(int i=head; i<=tail; i++) 
				{
			        outfile << queue[i].id << "," 
					     << queue[i].category << "," 
						 << queue[i].name << ","
						 << queue[i].price << "," 
						 << queue[i].cost << "," 
						 << queue[i].stock << endl; //file << queue[i] << endl;
		        }
		        
		        outfile.close();
	        }
        }
        
        // Display product list in a table format
        void display() 
		{
        	
        	cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
		    cout << "| ID      | Category     | Product Name                 | Price (RM)   | Cost (RM)   | Stock    |" << endl;
		    cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
	 
        	if(!empty()) 
			{
		
	        	for(int i=head; i<=tail; i++) 
				{ //for(int i=head; i>=0; i++)
	        	    cout << "| ";
			        cout.width(7); cout << left << queue[i].id << " | ";
			        cout.width(12); cout << left << queue[i].category << " | ";
			        cout.width(28); cout << left << queue[i].name << " | ";
			        cout.width(12); cout << right << queue[i].price << " | ";
					cout.width(11); cout << right << queue[i].cost << " | ";
					cout.width(8); cout << right << queue[i].stock << " | " << endl; //cout << queue[i] << endl;
		        }
	        } 
			else 
			{
	        	cout << "| ";
		        cout.width(93); cout << left << "No product available." << " | " << endl;
	        }
	        
	        cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
        }
        
        // Search product ID exits or not - for update (price, cost, or stock)
        void searchUpdate(string id) 
		{
        	
        	string type;
        	int stock;
			double cost, price;
        	bool found = false;
        	
        	for (int i = head; i <= tail; i++) 
			{
        		if (queue[i].id == id)
				{
        			cout << "\nProduct found: " << queue[i].name << endl;
        			found = true;
					break;
				}
			} 
			
			if (!found) 
			{
			    cout << "Product not found...." << endl;
			    cout << "\nReneter product ID: ";
			    cin >> id;
			}
			
			cout << "Enter updating field(Price/Cost/Stock): ";
			cin >> type;
			    
			if(type=="Price" || type=="price") 
			{
				cout << "Enter updated price: ";
				cin >> price;
			    update(id, type, price);
			} 
			else if(type=="Cost" || type=="cost") 
			{
				cout << "Enter updated cost: ";
				cin >> cost;
			    update(id, type, cost);
			} 
			else if(type=="Stock" || type=="stock") 
			{
				cout << "Enter updated stock: ";
				cin >> stock;
			    updateStock(id, type, stock);
			} 
			else 
			{
				cout << "Invalid update type..." << endl;
				cont();
			}
		}
        
        // Update price or cost of product
        void update(string id, string type, double change) 
		{
        	
        	string filename = "Apple_Store.txt";
        	
        	for (int i = head; i <= tail; i++) 
			{
        		if (queue[i].id == id) 
				{
					
					if(type=="Price" || type=="price") 
					{
						queue[i].price = change;
					} 
					else if(type=="Cost" || type=="cost") 
					{
						queue[i].cost = change;
					} 
					
					cout << "Updating value....." << endl;
					savetofile(filename); // Save after update
					
					// Show updated product
					cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
		    		cout << "| ID      | Category     | Product Name                 | Price (RM)   | Cost (RM)   | Stock    |" << endl;
		    		cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
		
	        	    cout << "| ";
	    		    cout.width(7); cout << left << queue[i].id << " | ";
		    	    cout.width(12); cout << left << queue[i].category << " | ";
		    	    cout.width(28); cout << left << queue[i].name << " | ";
		        	cout.width(12); cout << right << queue[i].price << " | ";
			    	cout.width(11); cout << right << queue[i].cost << " | ";
					cout.width(8); cout << right << queue[i].stock << " | " << endl; 
	            	
	            	cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
				} 
			}
			
			cont(); // Pause function
		}
		
		// Update stock value only
		void updateStock(string id, string type, int changeStock) 
		{
        	
        	string filename = "Apple_Store.txt";
        	
        	for (int i = head; i <= tail; i++) 
			{
        		if (queue[i].id == id) 
				{
					
					if(type=="Stock" || type=="stock") 
					{
						queue[i].stock = changeStock;
					} 
					
					cout << "Updating value....." << endl;
					savetofile(filename); // Save after update
					
					// Show updated product
					cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
		    		cout << "| ID      | Category     | Product Name                 | Price (RM)   | Cost (RM)   | Stock    |" << endl;
		    		cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
		
	        	    cout << "| ";
	    		    cout.width(7); cout << left << queue[i].id << " | ";
		    	    cout.width(12); cout << left << queue[i].category << " | ";
		    	    cout.width(28); cout << left << queue[i].name << " | ";
		        	cout.width(12); cout << right << queue[i].price << " | ";
			    	cout.width(11); cout << right << queue[i].cost << " | ";
					cout.width(8); cout << right << queue[i].stock << " | " << endl; 
	            	
	            	cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
				} 
			}
			
			cont(); // Pause function
		}
		
		// Search and display products by ID
        void searchByID(string key) {
        	
        	cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
			cout << "| ID      | Category     | Product Name                 | Price (RM)   | Cost (RM)   | Stock    |" << endl;
			cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;

			for (int i = head; i <= tail; i++) 
			{
			    if (queue[i].id == key) 
				{
		
	        	    cout << "| ";
	    		    cout.width(7); cout << left << queue[i].id << " | ";
		    	    cout.width(12); cout << left << queue[i].category << " | ";
		    	    cout.width(28); cout << left << queue[i].name << " | ";
		        	cout.width(12); cout << right << queue[i].price << " | ";
			    	cout.width(11); cout << right << queue[i].cost << " | ";
					cout.width(8); cout << right << queue[i].stock << " | " << endl; 
				} 
			}	
			
			cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;		
		}
        
        // Search and display products by category
        void searchByCategory(string key) 
		{
        	
        	cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
			cout << "| ID      | Category     | Product Name                 | Price (RM)   | Cost (RM)   | Stock    |" << endl;
			cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;

			for (int i = head; i <= tail; i++) 
			{
			    if (queue[i].category == key) 
				{
		
	        	    cout << "| ";
	    		    cout.width(7); cout << left << queue[i].id << " | ";
		    	    cout.width(12); cout << left << queue[i].category << " | ";
		    	    cout.width(28); cout << left << queue[i].name << " | ";
		        	cout.width(12); cout << right << queue[i].price << " | ";
			    	cout.width(11); cout << right << queue[i].cost << " | ";
					cout.width(8); cout << right << queue[i].stock << " | " << endl; 
				} 
			}	
			
			cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;		
		}
		
		// Pause function to continue execution after user input
		void cont() 
		{
			string key;
			
			cout << "\n[Enter any key and press enter to continue]";
			cin >> key;
		}
		
		//left = Left position : starting index of the left sorted subarray
		//right = 	Right position ? starting index of the right sorted subarray
		//lend = left end : last index for part left
		//rend = right end : last index for part right
		//num = Total Number of Elements to be Merge
		//temp = Index for Inserting into TmpArray
		
		void IDMerge(int left, int right, int rend) 
		{
		    int i, lend, num, tmp;
			lend = right - 1;
			tmp = left;  
			num = rend - left + 1;
			
			Apple* TmpArray = new Apple[50];
			
			while( left <= lend && right <= rend ) 
			{
			    if( queue[left].id <= queue[right].id ) 
				{
				    TmpArray[tmp++] = queue[left++]; 
				} 
				else 
				{
					TmpArray[tmp++] = queue[right++]; 
				}
			}
			
			while(left <= lend) 
			{
			    TmpArray[tmp++] = queue[left++]; 
			}
				
			while(right <= rend) 
			{
				TmpArray[tmp++] = queue[right++];
			}
				
			//for(i = 0 ; i < num; i++, rend--) {
				//	queue[rend] =TmpArray[rend];  
			//}
				
			// Correct copying back from TmpArray to queue
			for(i = 0; i < num; i++) 
			{
				queue[rend - num + 1 + i] = TmpArray[rend - num + 1 + i];
			}
			
			delete[] TmpArray;
		}  
		
		//first = The starting index of the portion of the array to be sorted
		//last = The ending index of the portion of the array to be sorted
		//center = 	The middle index used to split the array into two halves
		
		void IDMergeSort (int first, int last) 
		{
		    int center;  
			
			if(first < last) 
			{
			    center = (first + last)/2;
				
				//breaks the array into smaller parts
				IDMergeSort(first, center); 
				IDMergeSort(center+1, last);
				IDMerge(first, center+1, last);
			}
		}
		
		void CategoryMerge(int left, int right, int rend) 
		{
		    int i, lend, num, tmp;
			lend = right - 1;
			tmp = left;  
			num = rend - left + 1;
			
			Apple* TmpArray = new Apple[50];
			
			while( left <= lend && right <= rend ) 
			{
			    if( queue[left].category <= queue[right].category ) 
				{
				    TmpArray[tmp++] = queue[left++]; 
				} 
				else 
				{
					TmpArray[tmp++] = queue[right++]; 
				}
			}
			
			while(left <= lend) 
			{
			    TmpArray[tmp++] = queue[left++]; 
			}
				
			while(right <= rend) 
			{
				TmpArray[tmp++] = queue[right++];
			}
				
			// Correct copying back from TmpArray to queue
			for(i = 0; i < num; i++) 
			{
				queue[rend - num + 1 + i] = TmpArray[rend - num + 1 + i];
			}
			
			delete[] TmpArray;
		}  
		
		void CategoryMergeSort (int first, int last) 
		{
		    int center;  
			
			if(first < last) 
			{
			    center = (first + last)/2;
				
				//breaks the array into smaller parts
				CategoryMergeSort(first, center); 
				CategoryMergeSort(center+1, last);
				CategoryMerge(first, center+1, last);
			}
		}
		
		void PriceMerge(int left, int right, int rend) 
		{
		    int i, lend, num, tmp;
			lend = right - 1;
			tmp = left;  
			num = rend - left + 1;
			
			Apple* TmpArray = new Apple[50];
			
			while( left <= lend && right <= rend )
			{
			    if( queue[left].price <= queue[right].price ) 
				{
				    TmpArray[tmp++] = queue[left++]; 
				} 
				else 
				{
					TmpArray[tmp++] = queue[right++]; 
				}
			}
			
			while(left <= lend) 
			{
			    TmpArray[tmp++] = queue[left++]; 
			}
				
			while(right <= rend) 
			{
				TmpArray[tmp++] = queue[right++];
			}
				
			// Correct copying back from TmpArray to queue
			for(i = 0; i < num; i++) 
			{
				queue[rend - num + 1 + i] = TmpArray[rend - num + 1 + i];
			}
			
			delete[] TmpArray;
		}  
		
		void PriceMergeSort (int first, int last) 
		{
		    int center;  
			
			if(first < last) 
			{
			    center = (first + last)/2;
				
				//breaks the array into smaller parts
				PriceMergeSort(first, center); 
				PriceMergeSort(center+1, last);
				PriceMerge(first, center+1, last);
			}
		}
		
		//Searching Algorithm - Binary Search - ID
		int IDBinarySearch(string arr[], int first, int last, string target) 
		{

			int mid;
			int i; 
			int found = 0;
			
			//The binary search 
			while( (found==0) && (first <= last) ) 
			{
			    mid = (first + last) / 2; 
				
				if( target == arr[mid]) 
				{
				    found = 1; 
				    
				} 
				else 
				{
					if( target<arr[mid] ) 
					{
					    last = mid - 1;
					} 
					else 
					{
						first = mid + 1;
					}
				}
				
			}
			
			if(found) 
			{
			    searchByID(target);
			} 
			else 
			{
			    cout << "Product ID \'" << target << "\' not found";
			}
		}
		
		//Searching Algorithm - Binary Search - Category
		int CategoryBinarySearch(string arr[], int first, int last, string target) 
		{

			int mid;
			int i; 
			int found = 0;
			
			//The binary search 
			while( (found==0) && (first <= last) ) 
			{
			    mid = (first + last) / 2; 
				
				if( target == arr[mid]) 
				{
				    found = 1; 
				} 
				else 
				{
					if( target<arr[mid] ) 
					{
					    last = mid - 1;
					} 
					else 
					{
						first = mid + 1;
					}
				}
				
			}
			
			if(found) 
			{
			    searchByCategory(target);
			} 
			else 
			{
			    cout << "Category \'" << target << "\' not found";
			}
		}

		void viewAllOrdersByAdmin(int filterOption) 
		{
    		ifstream inFile("Order.txt");
		    if (!inFile) 
			{
		        cout << "Order.txt not found or cannot be opened." << endl;
		        return;
		    }

	    string line;
	    string currentInv = "", currentDate = "", currentEmail = "", currentRole = "";
	    double subtotal = 0.0, sst = 0.0, grandTotal = 0.0;
	    bool hasOrder = false;
	
	    while (getline(inFile, line)) 
		{
	        if (line.empty()) continue;
	
	        // 手动统计逗号数
	        int commaCount = 0;
	        for (size_t i = 0; i < line.length(); ++i) 
			{
	            if (line[i] == ',') commaCount++;
	        }
	
	        if (commaCount == 8) 
			{
	            // 是订单商品的第一行（含头信息）
	            size_t p1 = line.find(',');
	            size_t p2 = line.find(',', p1 + 1);
	            size_t p3 = line.find(',', p2 + 1);
	            size_t p4 = line.find(',', p3 + 1);
	            size_t p5 = line.find(',', p4 + 1);
	            size_t p6 = line.find(',', p5 + 1);
	            size_t p7 = line.find(',', p6 + 1);
	            size_t p8 = line.find(',', p7 + 1);
	
	            string date = line.substr(0, p1);
	            string inv = line.substr(p1 + 1, p2 - p1 - 1);
	            string email = line.substr(p2 + 1, p3 - p2 - 1);
	            string role = line.substr(p3 + 1, p4 - p3 - 1);
	            string id = line.substr(p4 + 1, p5 - p4 - 1);
	            string name = line.substr(p5 + 1, p6 - p5 - 1);
	            string priceStr = line.substr(p6 + 1, p7 - p6 - 1);
	            string qtyStr = line.substr(p7 + 1, p8 - p7 - 1);
	            string subtotalStr = line.substr(p8 + 1);
	
	            // 筛选
	            if ((filterOption == 2 && role != "Customer") ||
	                (filterOption == 3 && role != "Member")) 
					{
		                continue;
		            }
	
	            if (inv != currentInv) 
				{
	                if (!currentInv.empty()) 
					{
	                    cout << "+--------------------------------------------------------------+" << endl;
	                    cout << "Subtotal   : RM " << fixed << setprecision(2) << subtotal << endl;
	                    cout << "SST (10%)  : RM " << fixed << setprecision(2) << sst << endl;
	                    cout << "Grand Total: RM " << fixed << setprecision(2) << grandTotal << endl;
	                    cout << "+==============================================================+\n" << endl;
	                }
	
	                // 新订单
	                currentInv = inv;
	                currentDate = date;
	                currentEmail = email;
	                currentRole = role;
	                subtotal = sst = grandTotal = 0.0;
	                hasOrder = true;
	
	                cout << "Date: " << date << " | Invoice: INV" << inv
	                     << " | Email: " << email << " | Role: " << role << endl;
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
	        else if (commaCount == 4) 
			{
	            // 商品继续行（无头信息）
	            size_t p1 = line.find(',');
	            size_t p2 = line.find(',', p1 + 1);
	            size_t p3 = line.find(',', p2 + 1);
	            size_t p4 = line.find(',', p3 + 1);
	
	            string id = line.substr(0, p1);
	            string name = line.substr(p1 + 1, p2 - p1 - 1);
	            string priceStr = line.substr(p2 + 1, p3 - p2 - 1);
	            string qtyStr = line.substr(p3 + 1, p4 - p3 - 1);
	            string subtotalStr = line.substr(p4 + 1);
	
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
	    }
	
	    // 打印最后订单总计
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
	        cout << "No matching order found.\n";
	    }
	
	    inFile.close();
}

	void viewProfitReport() 
	{
	    ifstream orderFile("Order.txt");
	    if (!orderFile) 
		{
	        cout << "Order.txt not found or cannot be opened." << endl;
	        return;
	    }
	
	    // 读取所有商品信息到数组
	    Apple allProducts[100];
	    int productCount = 0;
	
	    ifstream productFile("Apple_Store.txt");
	    if (!productFile) 
		{
	        cout << "Apple_Store.txt not found or cannot be opened." << endl;
	        return;
	    }
	
	    string line;
	    while (getline(productFile, line)) 
		{
	        if (line.empty()) continue;
	
	        size_t p1 = line.find(',');
	        size_t p2 = line.find(',', p1 + 1);
	        size_t p3 = line.find(',', p2 + 1);
	        size_t p4 = line.find(',', p3 + 1);
	        size_t p5 = line.find(',', p4 + 1);
	
	        allProducts[productCount].id = line.substr(0, p1);
	        allProducts[productCount].category = line.substr(p1 + 1, p2 - p1 - 1);
	        allProducts[productCount].name = line.substr(p2 + 1, p3 - p2 - 1);
	        allProducts[productCount].price = stod(line.substr(p3 + 1, p4 - p3 - 1));
	        allProducts[productCount].cost = stod(line.substr(p4 + 1, p5 - p4 - 1));
	        productCount++;
	    }
	    productFile.close();
	
	    int mode;
	    cout << "View Profit Report:\n";
	    cout << "1. Search by Day (DD-MM-YYYY)\n";
	    cout << "2. Search by Month (MM-YYYY)\n";
	    cout << "3. View All\n";
	    cout << "\nEnter your choice: ";
	    cin >> mode;
	
	    string keyword;
	    if (mode == 1) 
		{
	        cout << "\nEnter date (DD-MM-YYYY): ";
	        cin >> keyword;
	    } 
		else if (mode == 2) 
		{
	        cout << "\nEnter month (MM-YYYY): ";
	        cin >> keyword;
	    }
	
	    // 用于存储利润数据
	    struct ProfitRecord 
		{
	        string id;
	        string name;
	        int totalQty;
	        double profit;
	    } profits[100];
	    int profitCount = 0;
	
	    while (getline(orderFile, line)) 
		{
	        if (line.empty()) continue;
	
	        // 判断是否为商品行
	        int commaCount = 0;
	        for (int i = 0; i < line.length(); i++) 
			{
	            if (line[i] == ',') commaCount++;
	        }
	
	        if (commaCount != 8) continue;
	
	        string date = line.substr(0, 10);
	        if ((mode == 1 && date != keyword) || (mode == 2 && date.substr(0, 7) != keyword))
	            continue;
	
	        // 解析字段
	        size_t p1 = line.find(',');
	        size_t p2 = line.find(',', p1 + 1);
	        size_t p3 = line.find(',', p2 + 1);
	        size_t p4 = line.find(',', p3 + 1);
	        size_t p5 = line.find(',', p4 + 1);
	        size_t p6 = line.find(',', p5 + 1);
	        size_t p7 = line.find(',', p6 + 1);
	        size_t p8 = line.find(',', p7 + 1);
	
	        string id = line.substr(p4 + 1, p5 - p4 - 1);
	        string name = line.substr(p5 + 1, p6 - p5 - 1);
	        int qty = stoi(line.substr(p7 + 1, p8 - p7 - 1));
	
	        // 查找对应商品的成本
	        double cost = 0.0, price = 0.0;
	        for (int i = 0; i < productCount; i++) 
			{
	            if (allProducts[i].id == id) 
				{
	                cost = allProducts[i].cost;
	                price = allProducts[i].price;
	                name = allProducts[i].name;
	                break;
	            }
	        }
	
	        double totalProfit = (price - cost) * qty;
	
	        // 检查是否已存在该商品记录
	        int index = -1;
	        for (int i = 0; i < profitCount; i++) 
			{
	            if (profits[i].id == id) 
				{
	                index = i;
	                break;
	            }
	        }
	
	        if (index != -1) 
			{
	            profits[index].totalQty += qty;
	            profits[index].profit += totalProfit;
	        } 
			else 
			{
	            profits[profitCount].id = id;
	            profits[profitCount].name = name;
	            profits[profitCount].totalQty = qty;
	            profits[profitCount].profit = totalProfit;
	            profitCount++;
	        }
	    }
	    orderFile.close();
	
	    cout << "\n+---------------------------------------------------------+" << endl;
	    cout << "| ID   | Product Name                 | Qty | Profit (RM) |" << endl;
	    cout << "+---------------------------------------------------------+" << endl;
	
	    for (int i = 0; i < profitCount; i++) 
		{
	        cout << "| " << setw(4) << left << profits[i].id
	             << " | " << setw(27) << left << profits[i].name
	             << "  | " << setw(3) << right << profits[i].totalQty
	             << " | " << setw(11) << fixed << setprecision(2) << profits[i].profit << " |" << endl;
	    }
	    cout << "+---------------------------------------------------------+" << endl;
	}
	
	bool isRewardIDUnique(const string& rewardID) 
	{
	    ifstream inFile("Rewards.txt");
	    if (!inFile) return true;
	
	    string line;
	    while (getline(inFile, line)) 
		{
	        stringstream ss(line);
	        string id;
	        getline(ss, id, ',');
	        
	        if (id == rewardID) 
			{
	            inFile.close();
	            return false;
	        }
	    }
	    inFile.close();
	    return true;
	}
	
	void addRewardItem() 
	{
	    string rewardID, rewardName;
	    int pointsRequired, stock;
	
	    cout << "\n=======Add Reward Item (Points Redemption)========\n";

	    do 
		{
		    cout << "\nEnter Reward ID (e.g. R001): ";
		    cin >> rewardID;

		    bool validFormat = false;
		    if (rewardID.length() == 4 && rewardID[0] == 'R' &&
		        isdigit(rewardID[1]) && isdigit(rewardID[2]) && isdigit(rewardID[3])) 
		    {
		        validFormat = true;
		    }
		
		    if (!validFormat) 
			{
		        cout << "Invalid format. Reward ID must be in the format R followed by 3 digits (e.g. R001).\n";
		        continue;
		    }
		
		    if (!isRewardIDUnique(rewardID)) 
			{
		        cout << "Reward ID already exists. Please enter a unique one.\n";
		    }
		
		} while (!isRewardIDUnique(rewardID) || rewardID.length() != 4 || rewardID[0] != 'R' ||
		         !isdigit(rewardID[1]) || !isdigit(rewardID[2]) || !isdigit(rewardID[3]));

	
	    cin.ignore();
	    cout << "\nEnter Reward Name: ";
	    getline(cin, rewardName);
	
	    cout << "\nEnter Points Required: ";
	    while (!(cin >> pointsRequired) || pointsRequired <= 0) 
		{
	        cout << "Invalid input. Enter a positive number for points: ";
	        cin.clear(); cin.ignore(1000, '\n');
	    }
	
	    cout << "\nEnter Available Stock: ";
	    while (!(cin >> stock) || stock < 0) 
		{
	        cout << "Invalid input. Enter a non-negative stock number: ";
	        cin.clear(); cin.ignore(1000, '\n');
	    }
	
	    ofstream outFile("Rewards.txt", ios::app);
	    if (!outFile) 
		{
	        cout << "Error opening Rewards.txt\n";
	        return;
	    }
	
	    outFile << rewardID << "," << rewardName << "," << pointsRequired << "," << stock << "\n";
	    outFile.close();
	
	    cout << "\nReward item added successfully!\n";
	}
	
	void restockRewards() 
	{
	    ifstream inFile("Rewards.txt");
	    if (!inFile) 
		{
	        cout << "Error: Cannot open Rewards.txt\n";
	        return;
	    }
	
	    string lines[100];
	    int count = 0;
	
	    cout << "\n+---------------------------------------------------------+" << endl;
	    cout << "| ID     | Reward Name                 | Points | Stock   |" << endl;
	    cout << "+---------------------------------------------------------+" << endl;
	
	    string line;
	    while (getline(inFile, line)) 
		{
	        if (line.empty()) continue;
	        lines[count++] = line;
	
	        stringstream ss(line);
	        string id, name, points, stock;
	        getline(ss, id, ',');
	        getline(ss, name, ',');
	        getline(ss, points, ',');
	        getline(ss, stock);
	
	        cout << "| " << setw(6) << left << id
	             << " | " << setw(27) << left << name
	             << " | " << setw(6) << left << points
	             << " | " << setw(7) << left << stock << " |" << endl;
	    }
	    cout << "+---------------------------------------------------------+" << endl;
	
	    inFile.close();
	
	    string targetID;
	    int restockAmount;
	    bool found = false;
	
	    cout << "\nEnter Reward ID to restock: ";
	    cin >> targetID;
	
	    ifstream reIn("Rewards.txt");
	    ofstream tempFile("TempRewards.txt");
	
	    if (!reIn || !tempFile) 
		{
	        cout << "Error: Cannot open reward files.\n";
	        return;
	    }
	
	    while (getline(reIn, line)) 
		{
	        stringstream ss(line);
	        string id, name, points, stockStr;
	        getline(ss, id, ',');
	        getline(ss, name, ',');
	        getline(ss, points, ',');
	        getline(ss, stockStr);
	
	        if (id == targetID) 
			{
	            found = true;
	            int stock = stoi(stockStr);

	            cout << "\nEnter quantity to restock: ";
	            while (!(cin >> restockAmount) || restockAmount < 1) 
				{
	                cout << "Invalid input. Enter a positive number: ";
	                cin.clear();
	                cin.ignore(1000, '\n');
	            }
	            stock += restockAmount;
	            tempFile << id << "," << name << "," << points << "," << stock << endl;
	            cout << "\nRestocked successfully.\n";
	            cout << "Updated stock for " << name << " : " << stock << " units\n";
	        } 
			else 
			{
	            tempFile << line << endl;
	        }
	    }
	
	    reIn.close();
	    tempFile.close();
	
	    remove("Rewards.txt");
	    rename("TempRewards.txt", "Rewards.txt");
	
	    if (!found) 
		{
	        cout << "Reward ID not found.\n";
	    }
	}



	
		void releaseMemory()
		{
		    tail = -1;  
	        head = 0;  
		}
		
		~AppleStore() 
		{
        	releaseMemory();
    	}
};

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
			if (cartFile && currentUser != nullptr) 
			{
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
		    if (currentUser == nullptr) 
			{
		        cout << "\nNo user logged in.\n";
		        return;
		    }
		
		    ifstream cartFile("Cart.txt");
		    if (!cartFile)
			{
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
		
		    while (getline(cartFile, line)) 
			{
		        string email, role, id, name, priceStr, qtyStr, subtotalStr;
		
		        stringstream ss(line);
		        getline(ss, email, ',');
		        getline(ss, role, ',');
		        getline(ss, id, ',');
		        getline(ss, name, ',');
		        getline(ss, priceStr, ',');
		        getline(ss, qtyStr, ',');
		        getline(ss, subtotalStr, ',');
		
		        if (email == currentEmail) 
				{
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
		    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", ltm);
		
		    return string(buffer);
		}
		
		string getInv() 
		{
		    time_t now = time(0);                     
		    tm *ltm = localtime(&now);              
		
		    char buffer[20];                         
		    strftime(buffer, sizeof(buffer), "%d%m%Y%H%M%S", ltm);
		
		    return string(buffer);
		}
		
		void upgradeToMember(const string& email, double grandTotal) 
		{
		    if (grandTotal < 10000.0) return;	
		
		    ifstream userFile("User.txt");
		    ofstream tempFile("TempUser.txt");
		
		    if (!userFile || !tempFile) 
			{
		        cout << "Error: Unable to open User.txt or TempUser.txt." << endl;
		        return;
		    }
		
		    string line;
		    bool updated = false;
		
		    while (getline(userFile, line)) 
			{
		        stringstream ss(line);
		        string uname, uemail, upass, uphone, urole;
		        getline(ss, uname, ',');
		        getline(ss, uemail, ',');
		        getline(ss, upass, ',');
		        getline(ss, uphone, ',');
		        getline(ss, urole);
		
		        if (uemail == email && urole == "Customer") 
				{
		            urole = "Member";
		            updated = true;
		            cout << "\n?? Congratulations! Your single purchase exceeded RM10000.\n";
		            cout << "You are now upgraded to Member status.\n";
		        }
		
		        tempFile << uname << "," << uemail << "," << upass << "," << uphone << "," << urole << endl;
		    }
		
		    userFile.close();
		    tempFile.close();
		
		    remove("User.txt");
		    rename("TempUser.txt", "User.txt");
		}
		
		void deductStockAfterPurchase(const string& email) 
		{
		    ifstream orderFile("Order.txt");
		    if (!orderFile) 
			{
		        cout << "? Failed to open Order.txt" << endl;
		        return;
		    }
		
		    string line;
		    string latestOrderLines[50]; // 暂存订单行（最多50行）
		    int lineCount = 0;
		    string currentInv = "", latestInv = "";
		
		    // Step 1: 找出该用户最新订单的 Invoice 编号
		    while (getline(orderFile, line)) 
			{
		        if (line.empty()) continue;
		
		        int commaCount = 0;
		        for (size_t i = 0; i < line.length(); ++i)
		            if (line[i] == ',') commaCount++;
		
		        if (commaCount == 8) 
				{
		            size_t pos1 = line.find(',');                // date
		            size_t pos2 = line.find(',', pos1 + 1);      // invoice
		            size_t pos3 = line.find(',', pos2 + 1);      // email
		            string currentEmail = line.substr(pos2 + 1, pos3 - pos2 - 1);
		            if (currentEmail == email) 
					{
		                currentInv = line.substr(pos1 + 1, pos2 - pos1 - 1);
		                latestInv = currentInv; // 每次记录，最后一次即为最新
		            }
		        }
		    }
		
		    orderFile.clear();              // 重置流状态
		    orderFile.seekg(0, ios::beg);   // 回到文件开头
		
		    // Step 2: 提取该发票对应的所有行
		    while (getline(orderFile, line)) 
			{
		        if (line.empty()) continue;
		
		        int commaCount = 0;
		        for (size_t i = 0; i < line.length(); ++i)
		            if (line[i] == ',') commaCount++;
		
		        if (commaCount == 8) 
				{
		            size_t pos1 = line.find(',');
		            size_t pos2 = line.find(',', pos1 + 1);
		            string inv = line.substr(pos1 + 1, pos2 - pos1 - 1);
		
		            if (inv == latestInv) 
					{
		                latestOrderLines[lineCount++] = line;
		            }
		        }
		    }
		    orderFile.close();
		
		    if (lineCount == 0) 
			{
		        cout << "No valid order found for this user." << endl;
		        return;
		    }
		
		    // Step 3: 提取产品ID与购买数量
		    string productIDs[50];
		    int productQtys[50];
		    int itemCount = 0;
		
		    for (int i = 0; i < lineCount; ++i) 
			{
		        stringstream ss(latestOrderLines[i]);
		        string date, inv, uemail, role, id, name, priceStr, qtyStr, subtotalStr;
		
		        getline(ss, date, ',');
		        getline(ss, inv, ',');
		        getline(ss, uemail, ',');
		        getline(ss, role, ',');
		        getline(ss, id, ',');
		        getline(ss, name, ',');
		        getline(ss, priceStr, ',');
		        getline(ss, qtyStr, ',');
		        getline(ss, subtotalStr);
		
		        productIDs[itemCount] = id;
		        productQtys[itemCount] = stoi(qtyStr);
		        itemCount++;
		    }
		
		    // Step 4: 修改 Apple_Store.txt 库存
		    ifstream storeFile("Apple_Store.txt");
		    if (!storeFile) 
			{
		        cout << "? Failed to open Apple_Store.txt" << endl;
		        return;
		    }
		
		    Apple updatedProducts[100];
		    int productCount = 0;
		
		    while (getline(storeFile, line)) 
			{
		        stringstream ss(line);
		        string id, category, name, priceStr, costStr, stockStr;
		
		        getline(ss, id, ',');
		        getline(ss, category, ',');
		        getline(ss, name, ',');
		        getline(ss, priceStr, ',');
		        getline(ss, costStr, ',');
		        getline(ss, stockStr);
		
		        updatedProducts[productCount].id = id;
		        updatedProducts[productCount].category = category;
		        updatedProducts[productCount].name = name;
		        updatedProducts[productCount].price = stod(priceStr);
		        updatedProducts[productCount].cost = stod(costStr);
		        updatedProducts[productCount].stock = stoi(stockStr);
		
		        // 如果订单中有这个产品
		        for (int j = 0; j < itemCount; ++j) 
				{
		            if (updatedProducts[productCount].id == productIDs[j]) 
					{
		                if (updatedProducts[productCount].stock >= productQtys[j])
		                    updatedProducts[productCount].stock -= productQtys[j];
		                else
		                    updatedProducts[productCount].stock = 0;
		            }
		        }
		        productCount++;
		    }
		    storeFile.close();
		
		    // Step 5: 保存库存回 Apple_Store.txt
		    ofstream saveFile("Apple_Store.txt");
		    if (!saveFile) 
			{
		        cout << "? Failed to save Apple_Store.txt" << endl;
		        return;
		    }
		
		    for (int i = 0; i < productCount; ++i) 
			{
		        saveFile << updatedProducts[i].id << ","
		                 << updatedProducts[i].category << ","
		                 << updatedProducts[i].name << ","
		                 << fixed << setprecision(2) << updatedProducts[i].price << ","
		                 << fixed << setprecision(2) << updatedProducts[i].cost << ","
		                 << updatedProducts[i].stock << endl;
		    }
		
		    saveFile.close();
		}

		void updateUserPoints(const string& email, double grandTotal) 
		{
		    int newPoints = (int)(grandTotal / 100.0);
		
		    // Step 1: 读取 Point.txt 内容
		    ifstream inFile("Point.txt");
		    ofstream outFile("TempPoint.txt");
		    bool found = false;
		    string line;
		
		    while (getline(inFile, line)) 
			{
		        stringstream ss(line);
		        string userEmail, pointStr;
		        getline(ss, userEmail, ',');
		        getline(ss, pointStr);
		
		        if (userEmail == email)
				{
		            int existingPoints = stoi(pointStr);
		            existingPoints += newPoints;
		            outFile << userEmail << "," << existingPoints << endl;
		            found = true;
		        } 
				else 
				{
		            outFile << line << endl;
		        }
		    }
		
		    // Step 2: 如果用户不存在，新增记录
		    if (!found) {
		        outFile << email << "," << newPoints << endl;
		    }
		
		    inFile.close();
		    outFile.close();
		
		    remove("Point.txt");
		    rename("TempPoint.txt", "Point.txt");
		
		    cout << "\nYou earned " << newPoints << " points for this purchase.\n";
		}
		
		void viewMyPoints(const string& email) 
		{
		    ifstream inFile("Point.txt");
		    if (!inFile) 
			{
		        cout << "Failed to open Point.txt\n";
		        return;
		    }
		
		    string line;
		    bool found = false;
		    while (getline(inFile, line)) 
			{
		        stringstream ss(line);
		        string userEmail, pointStr;
		        getline(ss, userEmail, ',');
		        getline(ss, pointStr);
		
		        if (userEmail == email) 
				{
		            cout << "\nYour current point balance: " << pointStr << " points.\n";
		            found = true;
		            break;
		        }
		    }
		
		    if (!found) 
			{
		        cout << "\nNo points found for your account.\n";
		    }
		
		    inFile.close();
		}

		void checkoutCart(const string& currentEmail, const string& currentRole) 
		{
		    ifstream inFile("Cart.txt");
		    if (!inFile) 
			{
		        cout << "Cart.txt not found or cannot be opened." << endl;
		        return;
		    }
		
		    ofstream orderFile("Order.txt", ios::app);
		    if (!orderFile) 
			{
		        cout << "Failed to open Order.txt for writing." << endl;
		        return;
		    }
		
		    ofstream tempFile("temp_cart.txt");
		    if (!tempFile) 
			{
		        cout << "Failed to open temp_cart.txt." << endl;
		        return;
		    }
		
		    string line;
		    double grandTotal = 0.0, subTotal = 0.0;
		    int count = 1;
		    int itemFound = 0;
		
		    string Inv = getInv();
		    string now = getCurrentDateTime();
		
		    cout << "Invoice No: " << Inv << endl;
		    cout << "+----------------------------------------------------------------------+" << endl;
		    cout << "|                                                                      |" << endl;
		    cout << "|                             Apple Store                              |" << endl;
		    cout << "|                                                                      |" << endl;
		    cout << "|                                                                      |" << endl;
		    cout << "|   Invoice No : INV" << Inv << "                                     |" << endl;
		    cout << "|   Date       : " << now << "                                   |" << endl;
		    cout << "|                                                                      |" << endl;
		    cout << "|                                                                      |" << endl;
		    cout << "|                                                                      |" << endl;
		
		    while (getline(inFile, line)) 
			{
		        stringstream ss(line);
		        string email, role, id, name, priceStr, qtyStr, subtotalStr;
		
		        getline(ss, email, ',');
		        getline(ss, role, ',');
		        getline(ss, id, ',');
		        getline(ss, name, ',');
		        getline(ss, priceStr, ',');
		        getline(ss, qtyStr, ',');
		        getline(ss, subtotalStr);
		
		        if (email == currentEmail) 
				{
		            double price = stod(priceStr);
		            int qty = stoi(qtyStr);
		            double subtotal = stod(subtotalStr);
		            subTotal += subtotal;
		            itemFound++;
		
		            // 输出小票
		            cout << "|   " << count++ << ". " << setw(28) << left << name
		                 << " RM " << setw(8) << price
		                 << "   " << setw(3) << qty << "   "
		                 << "RM " << setw(12) << subtotal << "|" << endl;
		
		            // 写入订单记录：加入 email 与 role
		            orderFile << "\n" << now << "," << Inv << "," << email << "," << role << ","
		                      << id << "," << name << "," << price << "," << qty << "," << subtotal;
		        } 
				else 
				{
		            tempFile << line << endl;  // 不是当前用户，写回 temp 文件
		        }
		    }
		    
		    if (itemFound == 0) 
			{
				system("cls"); 
				cout << "\nYour cart is empty. Cannot proceed to checkout." << endl;
				inFile.close();
				orderFile.close();
				tempFile.close();
				remove("temp_cart.txt"); // 清理空文件
			    return;
			}
		    
		    double SST = subTotal * 0.10;
		    grandTotal = subTotal + SST;
			cout << "|                                                                      |" << endl;
			cout << "|                                                                      |" << endl;
			cout << "|                                                                      |" << endl;
		    cout << "|   Subtotal                                            RM " << setw(10) << subTotal << "  |" << endl;
		    cout << "|   SST (10%)                                           RM " << setw(10) << SST << "  |" << endl;
		    cout << "|   Grand Total                                         RM " << setw(10) << grandTotal << "  |" << endl;
		    cout << "|                                                                      |" << endl;
		    cout << "|                                                                      |" << endl;
		    cout << "+----------------------------------------------------------------------+" << endl;
			
			string cardNum = inputCardNumber();
			string ExpDate = inputExpDate();
			string CVV = inputCVV();
			
			cout << "\n\nPayment Successful. You may view your product in order history. " << endl;
			
			upgradeToMember(currentEmail, grandTotal);
		
		    orderFile << "\n" << fixed << setprecision(2) << subTotal << "," << SST << "," << grandTotal << endl;
		    deductStockAfterPurchase(currentEmail);
		    updateUserPoints(currentEmail, grandTotal);
		    
		    ofstream summaryFile("Summary.txt", ios::app);
				if (summaryFile) 
				{
				    summaryFile << currentEmail << "," << now << "," << Inv << "," << fixed << setprecision(2) << grandTotal << "\n";
				    summaryFile.close();
				}
		
		    inFile.close();
		    orderFile.close();
		    tempFile.close();
		
		    remove("Cart.txt");
		    rename("temp_cart.txt", "Cart.txt");
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
		
		void editCart(Person* currentUser) 
		{
		    ifstream inFile("Cart.txt");
		    ofstream tempFile("temp_cart.txt");
		
		    if (!inFile || !tempFile) 
			{
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
		
		    while (getline(inFile, line)) 
			{
		        stringstream ss(line);
		        string email, role, id, name, priceStr, qtyStr, subtotalStr;
		        getline(ss, email, ',');
		        getline(ss, role, ',');
		        getline(ss, id, ',');
		        getline(ss, name, ',');
		        getline(ss, priceStr, ',');
		        getline(ss, qtyStr, ',');
		        getline(ss, subtotalStr, ',');
		
		        if (email == currentEmail) 
				{
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
		
		    if (itemNumber == 0) 
			{
		        cout << "Your cart is empty.\n";
		        inFile.close();
		        tempFile.close();
		        remove("temp_cart.txt");
		        return;
		    }
		
		    int choice;
		    cout << "Enter item number to edit (0 to cancel): ";
		    cin >> choice;
		
		    if (choice <= 0 || choice > itemNumber) 
			{
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
		
		    if (editOption == 1) 
			{
		        cout << "Enter new quantity (0 to remove): ";
		        cin >> newQty;
		        if (newQty <= 0) 
				{
		            skipLine = true;
		            cout << name << " has been removed from your cart.\n";
		        } 
				else 
				{
		            newSubtotal = price * newQty;
		            cout << name << " quantity updated to " << newQty << ".\n";
		        }
		    } 
			else if (editOption == 2) 
			{
		        skipLine = true;
		        cout << name << " has been removed from your cart.\n";
		    } 
			else 
			{
		        cout << "Invalid edit option.\n";
		    }
		
		    // 重读 Cart.txt 并写入修改后数据
		    inFile.clear();
		    inFile.seekg(0);
		    int currentLine = 0;
		    while (getline(inFile, line)) 
			{
		        bool isTargetLine = (currentLine == indexes[choice - 1]);
		
		        if (isTargetLine) 
				{
		            if (!skipLine) 
					{
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
		    if (!inFile)
			{
		        cout << "Summary.txt not found or cannot be opened." << endl;
		        return;
		    }
		
		    string line, email, date, inv, totalStr;
		    double grandTotal;
		
		    cout << "\n=== Order Summary ===" << endl;
		    cout << "+---------------------+--------------------+---------------+" << endl;
		    cout << "|       Date          |     Invoice No     | Grand Total   |" << endl;
		    cout << "+---------------------+--------------------+---------------+" << endl;
		
		    while (getline(inFile, line)) 
			{
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
		
		    for (int i = 0; i < n1; i++) 
			{
		        Ltotals[i] = totals[left + i];
		        Ldates[i] = dates[left + i];
		        Linv[i] = invoices[left + i];
		    }
		
		    for (int j = 0; j < n2; j++) 
			{
		        Rtotals[j] = totals[mid + 1 + j];
		        Rdates[j] = dates[mid + 1 + j];
		        Rinv[j] = invoices[mid + 1 + j];
		    }
		
		    int i = 0, j = 0, k = left;
		    while (i < n1 && j < n2) 
			{
		        if (Ltotals[i] <= Rtotals[j]) 
				{
		            totals[k] = Ltotals[i];
		            dates[k] = Ldates[i];
		            invoices[k] = Linv[i];
		            i++;
		        } 
				else 
				{
		            totals[k] = Rtotals[j];
		            dates[k] = Rdates[j];
		            invoices[k] = Rinv[j];
		            j++;
		        }
		        k++;
		    }
		
		    while (i < n1) 
			{
		        totals[k] = Ltotals[i];
		        dates[k] = Ldates[i];
		        invoices[k] = Linv[i];
		        i++;
		        k++;
		    }
		
		    while (j < n2) 
			{
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
		    if (!inFile) 
			{
		        cout << "Summary.txt not found.\n";
		        return;
		    }
		
		    const int MAX = 100;
		    string dates[MAX], invoices[MAX];
		    double totals[MAX];
		    int count = 0;
		
		    string line;
		    while (getline(inFile, line)) 
			{
		        stringstream ss(line);
		        string email, date, inv, totalStr;
		        getline(ss, email, ',');
		        getline(ss, date, ',');
		        getline(ss, inv, ',');
		        getline(ss, totalStr);
		
		        if (email == currentEmail && count < MAX) 
				{
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
		
		    for (int i = 0; i < count; i++) 
			{
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
		    if (!inFile) 
			{
		        cout << "Order.txt not found.\n";
		        return;
		    }
		
		    string line, date, inv, email;
		    double grandTotal = 0.0;
		
		    while (getline(inFile, line)) 
			{
		        if (line.empty()) continue;
		
		        int commaCount = 0;
		        for (char ch : line) 
				{
		            if (ch == ',') commaCount++;
		        }
		
		        if (commaCount >= 4) 
				{
		            // 订单明细行：提取 date, inv, email
		            size_t p1 = line.find(',');
		            size_t p2 = line.find(',', p1 + 1);
		            size_t p3 = line.find(',', p2 + 1);
		
		            date = line.substr(0, p1);
		            inv = line.substr(p1 + 1, p2 - p1 - 1);
		            email = line.substr(p2 + 1, p3 - p2 - 1);
		        }
		        else if (commaCount == 2)
				{
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
		
		    if (count == 0) 
			{
		        cout << "No order data available.\n";
		        return;
		    }
		
		    // 输入月份
		    string monthInput, searchMonth;
			cout << "Enter month to search (format: MM-YYYY): ";
			cin >> monthInput;
			
			// 手动验证格式 MM-YYYY
			if (monthInput.length() == 7 && monthInput[2] == '-' &&
			    isdigit(monthInput[0]) && isdigit(monthInput[1]) &&
			    isdigit(monthInput[3]) && isdigit(monthInput[4]) &&
			    isdigit(monthInput[5]) && isdigit(monthInput[6])) 
			{
			    int mm = stoi(monthInput.substr(0, 2));
			    if (mm >= 1 && mm <= 12) 
			    {
			        searchMonth = monthInput.substr(3, 4) + "-" + monthInput.substr(0, 2); // 转换为 YYYY-MM
			    } 
			    else 
			    {
			        cout << "Invalid month. Please use MM-YYYY format with valid month (01–12)." << endl;
			        return;
			    }
			} 
			else 
			{
			    cout << "Invalid format. Please enter in MM-YYYY format." << endl;
			    return;
			}
		
		    // 手动实现二分查找区间
		    int left = 0, right = count - 1;
		    int startIdx = -1, endIdx = -1;
		
		    // 找起始
		    while (left <= right) 
			{
		        int mid = (left + right) / 2;
		        string d = dates[mid].substr(0, 7);
		        if (d >= searchMonth) 
				{
		            if (d == searchMonth) startIdx = mid;
		            right = mid - 1;
		        } 
				else 
				{
		            left = mid + 1;
		        }
		    }
		
		    // 找结尾
		    left = 0; right = count - 1;
		    while (left <= right) 
			{
		        int mid = (left + right) / 2;
		        string d = dates[mid].substr(0, 7);
		        if (d <= searchMonth) 
				{
		            if (d == searchMonth) endIdx = mid;
		            left = mid + 1;
		        } 
				else 
				{
		            right = mid - 1;
		        }
		    }
		
		    // 输出结果
		    cout << "\n=== Order Summary for " << searchMonth << " ===" << endl;
		    cout << "+------------------------+--------------------+---------------+" << endl;
		    cout << "|          Date          |     Invoice No     | Grand Total   |" << endl;
		    cout << "+------------------------+--------------------+---------------+" << endl;
		
		    bool found = false;
		    for (int i = startIdx; i <= endIdx && i != -1; i++) 
			{
		        if (emails[i] == currentEmail) 
				{
		            cout << "| " << left << dates[i]
		                 << "  |  " << ("INV" + invs[i])
		                 << " | RM " << setw(10) << fixed << setprecision(2) << totals[i] << " |" << endl;
		            found = true;
		        }
		    }
		
		    if (!found) 
			{
		        cout << "| No orders found for this month.                                  |" << endl;
		    }
		
		    cout << "+------------------------+--------------------+---------------+" << endl;
		}
		
		void redeemReward(const string& email) 
		{
		    string rewardID;
		    cout << "\nEnter Reward ID to redeem: ";
		    cin >> rewardID;
		
		    // Step 1: Load Rewards
		    ifstream rewardFile("Rewards.txt");
		    ofstream tempRewardFile("TempRewards.txt");
		    if (!rewardFile || !tempRewardFile) 
			{
		        cout << "Failed to open Rewards.txt.\n";
		        return;
		    }
		
		    bool rewardFound = false;
		    bool rewardSuccess = false;
		    int requiredPoints = 0;
		    int availableStock = 0;
		    string rewardName;
		
		    string line;
		    while (getline(rewardFile, line)) 
			{
		        stringstream ss(line);
		        string id, name, pointsStr, stockStr;
		        getline(ss, id, ',');
		        getline(ss, name, ',');
		        getline(ss, pointsStr, ',');
		        getline(ss, stockStr);
		
		        if (id == rewardID) 
				{
		            rewardFound = true;
		            requiredPoints = stoi(pointsStr);
		            availableStock = stoi(stockStr);
		            rewardName = name;
		
		            if (availableStock > 0) 
					{
		                availableStock--;
		                rewardSuccess = true;
		            } 
					else 
					{
		                cout << "Reward is out of stock.\n";
		            }
		
		            tempRewardFile << id << "," << name << "," << pointsStr << "," << availableStock << "\n";
		        } 
				else 
				{
		            tempRewardFile << line << "\n";
		        }
		    }
		
		    rewardFile.close();
		    tempRewardFile.close();
		    remove("Rewards.txt");
		    rename("TempRewards.txt", "Rewards.txt");
		
		    if (!rewardFound) 
			{
		        cout << "Reward ID not found.\n";
		        return;
		    }
		    if (!rewardSuccess) return;
		
		    // Step 2: Load and Update Points
		    ifstream pointFile("Point.txt");
		    ofstream tempPointFile("TempPoints.txt");
		    if (!pointFile || !tempPointFile) 
			{
		        cout << "Failed to open Points.txt.\n";
		        return;
		    }
		
		    bool userFound = false;
		    while (getline(pointFile, line)) 
			{
		        stringstream ss(line);
		        string uemail, pointStr;
		        getline(ss, uemail, ',');
		        getline(ss, pointStr);
		        int point = stoi(pointStr);
		
		        if (uemail == email) 
				{
		            userFound = true;
		            if (point >= requiredPoints) 
					{
		                point -= requiredPoints;
		                cout << "\nSuccessfully redeemed: " << rewardName;
		                cout << "\nRemaining Points: " << point << endl;
		                tempPointFile << uemail << "," << point << "\n";
		            } 
					else 
					{
		                cout << "Not enough points to redeem this reward.\n";
		                tempPointFile << line << "\n"; // no change
		            }
		        } 
				else 
				{
		            tempPointFile << line << "\n";
		        }
		    }
		
		    pointFile.close();
		    tempPointFile.close();
		    remove("Point.txt");
		    rename("TempPoints.txt", "Point.txt");
		
		    if (!userFound) 
			{
		        cout << "You don’t have any points record.\n";
		    }
		}
		
		void displayAllRewardsTable() 
		{
		    ifstream inFile("Rewards.txt");
		    if (!inFile) {
		        cout << "Rewards.txt not found.\n";
		        return;
		    }
		
		    string line;
		    string id, name, pointsStr, stockStr;
		
		    cout << "\n================= Available Rewards =================" << endl;
		    cout << "+------------+---------------------------+-----------+" << endl;
		    cout << "| Reward ID  | Reward Name               | Points    |" << endl;
		    cout << "+------------+---------------------------+-----------+" << endl;
		
		    while (getline(inFile, line)) {
		        stringstream ss(line);
		        getline(ss, id, ',');
		        getline(ss, name, ',');
		        getline(ss, pointsStr, ',');
		        getline(ss, stockStr);  // stock 不显示
		
		        cout << "|  " << setw(10) << left << id
		             << "| " << setw(26) << left << name
		             << "| " << setw(9) << left << pointsStr << " |" << endl;
		    }
		
		    cout << "+------------+---------------------------+-----------+" << endl;
		
		    inFile.close();
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
    if (!outFile) 
	{
        cout << "Failed to open User.txt\n";
        return;
    }

    outFile << u.username << "," << u.email << "," << u.password << ","
            << u.phoneNumber << "," << u.role << "\n";

    outFile.close();
}

void addNewAdmin() 
{
    ofstream outFile("User.txt", ios::app);
    if (!outFile) 
	{
        cout << "Error opening User.txt for writing." << endl;
        return;
    }

    string username, email, password, phone;

    cin.ignore(); // 清除输入缓存

    cout << "Enter new admin's username: ";
    getline(cin, username);

    do {
        cout << "Enter email (must contain '.com'): ";
        getline(cin, email);
        if (email.find(".com") == string::npos) 
		{
            cout << "Invalid email. Try again." << endl;
        }
    } while (email.find(".com") == string::npos);

    cout << "Enter password: ";
    char ch;
    password = "";
    while ((ch = _getch()) != '\r') 
	{
        if (ch == '\b') 
		{ // 处理退格
            if (!password.empty()) 
			{
                cout << "\b \b";
                password.pop_back();
            }
        } 
		else 
		{
            password += ch;
            cout << "*";
        }
    }
    cout << endl;

    cout << "Enter phone number: ";
    getline(cin, phone);

    outFile << username << "," << email << "," << password << "," << phone << ",Admin" << endl;

    cout << "\nNew admin registered successfully!" << endl;

    outFile.close();
}


void runAdminModule();

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

   bool loggedIn = false;

do {
	
    
    do {
		    cout << "===============================================\n";
		    cout << "              Welcome to Apple Store           \n";
		    cout << "===============================================\n";
		    cout << "             1. Register as a new user" << endl;
		    cout << "             2. Login" << endl;
		    cout << "             3. Exit" << endl;
		    cout << "            \nEnter your choice: ";
		    cin >> mainChoice;
		
		    if (cin.fail() || mainChoice < 1 || mainChoice > 3) 
			{
		        cin.clear(); 
		        cin.ignore(10000, '\n');
		        cout << "Invalid input. Please enter 1, 2, or 3.\n\n";
		    }
		} while (mainChoice < 1 || mainChoice > 3);

    switch (mainChoice) 
	{
        case 1: 
			{
			    User u;
			    cout << "\n=== Register New User ===\n";
			
			    cout << "Enter username: ";
			    cin >> u.username;
			
			    // Email validation
			    do 
			    {
			        cout << "Enter email (must include .com): ";
			        cin >> u.email;
			        if (u.email.find(".com") == string::npos) 
			        {
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
			
			    // === Check for duplicate email or phone number ===
			    ifstream checkFile("User.txt");
			    string line;
			    bool duplicateFound = false;
			
			    while (getline(checkFile, line)) 
				{
			        stringstream ss(line);
			        string uname, email, pass, phone, role;
			        getline(ss, uname, ',');
			        getline(ss, email, ',');
			        getline(ss, pass, ',');
			        getline(ss, phone, ',');
			        getline(ss, role);
			
			        if (email == u.email || phone == u.phoneNumber) 
					{
			            duplicateFound = true;
			            break;
			        }
			    }
			    checkFile.close();
			
			    if (duplicateFound) 
				{
			        cout << "\nEmail or Phone Number already exists. Returning to main menu...\n";
			        break; 
			    }
			
			    u.role = "Customer";
			
			    ofstream outFile("User.txt", ios::app);
			    if (!outFile) 
			    {
			        cout << "Failed to open User.txt\n";
			        break;
			    }
			
			    outFile << u.username << "," << u.email << "," << u.password << ","
			            << u.phoneNumber << "," << u.role << "\n";
			    outFile.close();

			    currentUser = new Customer(u.username, u.email, u.password, u.phoneNumber);
			    loggedIn = true;
			    break;
			}

        case 2:
		    currentUser = loginUser(); 
		    if (currentUser != nullptr) 
		    {
		        loggedIn = true;
		
		        string role = currentUser->getRole();
		        if (role == "Admin" || role == "SuperAdmin") 
				{
		            runAdminModule(); 
		            delete currentUser;
		            currentUser = nullptr;
		            loggedIn = false;
		        }
		    }
		    break;

	        case 3:
	            cout << "Thank you for visiting Apple Store. Goodbye!" << endl;
	            break;
	
	        default:
	            cout << "Invalid choice. Please try again.\n";
	    }
	
	    if (loggedIn) break;
	
	} while (mainChoice != 3);
	
	string input;
	do
	{
		system("cls");
		cout << "Current time: " << now << endl;
		cout << "===================================================" << endl; 
		cout << "                Welcome to Apple Store             " << endl;
		cout << "===================================================" << endl; 
		cout << "\n            1.	View all product" << endl;
		cout << "            2.	Add product to Cart" << endl;
		cout << "            3.	View Cart" << endl;
		cout << "            4.	Edit Cart" << endl;
		cout << "            5.	Check Out" << endl;
		cout << "            6.	View Order History" << endl;
		cout << "            7.	Order Summary" << endl;
		cout << "            8.	Sort by Summary Price" << endl;
		cout << "            9.	Search Order by Date" << endl;
		cout << "           10.	View Point or Redeem." << endl;
		cout << "           11.	Log Out" << endl;
	
		cout << "\n\nEnter choice (1-11) : ";
		cin >> input;
		
		bool isValid = true;
	    for (size_t i = 0; i < input.length(); ++i) 
		{
	        if (!isdigit(input[i])) 
			{
	            isValid = false;
	            break;
	        }
	    }
	
	    if (!isValid) 
		{
	        cout << "\nInvalid input. Please enter a number between 1 and 11.\n";
	        system("pause");
	        continue;
	    }
	
	    choice = stoi(input); 
	
	    if (choice < 1 || choice > 11) 
		{
	        cout << "\nChoice out of range. Please enter a number between 1 and 11.\n";
	        system("pause");
	        continue;
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
					    cout << "\nSelect category number to view and purchase from: ";
					    cin >> input;
	
				        bool isNumeric = true;
				        for (char ch : input) 
						{
				            if (!isdigit(ch)) 
							{
				                isNumeric = false;
				                break;
				            }
				        }
				
				        if (!isNumeric) 
						{
				            cout << "\n? Invalid input. Please enter a number." << endl;
				            system("pause");
				            continue;
				        }
				
				        select = stoi(input);
				
				        string selectedCategory = A.getCategoryByIndex(select);
				        if (selectedCategory != "") 
						{
				            A.addToCart(selectedCategory, currentUser);
				        } 
						else 
						{
				            cout << "Invalid category selected!" << endl;
				        }
				
				        cout << "\nEnter 1 to exit: ";
				        cin >> input;

				        bool isExitValid = true;
				        for (char ch : input) 
						{
				            if (!isdigit(ch)) 
							{
				                isExitValid = false;
				                break;
				            }
				        }
				
				        if (!isExitValid || stoi(input) != 1) 
						{
				            choice = 0; // 不退出
				        } 
						else 
						{
				            choice = 1; // 正常退出
				        }
					}while(choice != 1);
					break;
			
			case 3: do
					{
					    system("cls");
					    A.viewCart(currentUser);
					
					    cout << "\nEnter 1 to exit" << endl;
					    cin >> choice;
					}while(choice != 1);
					break;
					
			case 4: do
					{
				        system("cls");
				        A.editCart(currentUser);
				        
				        cout << "\nEnter 1 to exit: ";
				        cin >> choice;
				    } while (choice != 1);
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
						system("cls");
					    A.orderSummary(currentUser->getEmail());
					    
					    cout << "\nEnter 1 to exit" << endl;
					    cin >> choice;
					}while (choice != 1);
				    break;
				    
			case 8: do 
					{
					    system("cls");
					    A.viewSummarySortedByTotal(currentUser->getEmail());
					        
					    cout << "\nEnter 1 to exit" << endl;
					    cin >> choice;
					}while (choice != 1);
					break;
					    
			case 9: do
					{
						system("cls");
			    		A.searchSummaryByMonthFromOrder(currentUser->getEmail());
			    			
			    		cout << "\nEnter 1 to exit" << endl;
					    cin >> choice;
					}while (choice != 1);
 					break;
 						
 										    
			case 10:
				{
				    int subChoice;
				    do {
				        system("cls");
				        cout << "\n=== My Rewards Menu ===\n";
				        cout << "1. View My Points" << endl;
				        cout << "2. Redeem Rewards" << endl;
				        cout << "3. Exit to Main Menu" << endl;
				        cout << "\nEnter your choice: ";
				        cin >> subChoice;
				
				        while (cin.fail() || subChoice < 1 || subChoice > 3) 
						{
				            cin.clear();
				            cin.ignore(10000, '\n');
				            cout << "Invalid input. Please enter a number between 1 and 3: ";
				            cin >> subChoice;
				        }
				
				        switch (subChoice) 
						{
				            case 1:
				                A.viewMyPoints(currentUser->getEmail());
				                break;
				            case 2:
				            	system("cls"); 
				            	A.displayAllRewardsTable();
				                A.redeemReward(currentUser->getEmail());
				                break;
				            case 3:
				                cout << "Returning to main menu...\n";
				                break;
				        }
				
				        if (subChoice != 3) 
						{
				            cout << "\nPress Enter to continue...";
				            cin.ignore();
				            cin.get();
				        }
				
				    } while (subChoice != 3);
				    break;
				}
				
			case 11 :
					A.releaseMemory();
					delete currentUser;
				    currentUser = nullptr;
				    loggedIn = false;
				    cout << "You have been logged out successfully.\n";
				    break;
				    return 0;
		}
			
	}while(choice != 11 && loggedIn);
	
	return 0;
}

void runAdminModule() 
{
	AppleStore q; // Create AppleStore object

	string filename = "Apple_Store.txt";
    q.readfile(filename); // Load data from file
	 
	int choice, sortChoice, searchChoice, reportChoice;
	
	string id, name, category, type;
	int stock;
	double cost, price;
    
    string target;
	int result;
    int size;
	string* arr;
	int n;
	
	bool adminLoggedIn = true;
	
	do 
	{
		system("cls"); // Clear screen
		
		// Display menu
		cout << "===============================================\n";
		cout << "        Welcome to Apple Inventory Store       \n";
		cout << "===============================================\n";
		cout << "            1. Display all product\n";
		cout << "            2. Add new product\n";
		cout << "            3. Update product details\n";
		cout << "            4. Delete product\n";
		cout << "            5. Sort product\n";
		cout << "            6. Search product\n";
		cout << "            7. Display summary report\n";
		cout << "            8. Save sorted data (by id)\n";
		cout << "            9. Display order history\n";
		cout << "           10. Display profit\n";
		cout << "           11. Add new admin\n";
		cout << "           12. Add new rewards\n";
		cout << "           13. Restock rewards\n";
		cout << "           14. Log Out\n";
		
		
		cout << "\n\nEnter your choice: ";
		cin  >> choice;
		
		// Input validation
		while (cin.fail() || choice < 1 || choice > 14) 
		{
		    cin.clear();
		    cin.ignore(1000, '\n');
		    cout << "Invalid input. Please enter a number between 1 and 14: ";
		    cin >> choice;
		}
			
		switch(choice) 
		{
		    case 1: // Display all products
		    	system("cls");
		        cout << "\n-----Display Product-----" << endl;
			    q.display(); 
			    q.cont();
				break;
				
			case 2: // Add new product
				system("cls");
			    cout << "\n-----Add New Product-----" << endl;
			    
				cout << "Enter product ID: "; 
				cin >> id; 
				q.checkid(id); // Ensure ID is unique
				
				cin.ignore();
				cout << "Enter product category: "; 
				getline(cin, category);
				
				cout << "Enter product name: "; 
				getline(cin, name); 
				
				cout << "Enter product selling price: RM "; 
				cin >> price; 
							
				cout << "Enter product cost: RM "; 
				cin >> cost; 
				
				cout << "Enter product stock: ";
				cin >> stock;
							
				q.append(id, category, name, price, cost, stock);
				q.savetofile(filename);  // Save new data 
				cout << "Data save successfully. " << endl;
				q.cont();
				break;
				
			case 3: // Update product
				system("cls");
			    q.display();
			    cout << "\n-----Update Product Details-----" << endl;
			    cout << "Enter product ID for updating: ";
			    cin >> id;
			    
			    q.searchUpdate(id); // Update specific product
			    break;
			    
			case 4: 
				system("cls");
			    q.display();
			    cout << "\n----Delete Product----" << endl;
			    cout << "Enter product ID for deleting: ";
			    cin >> id;
			    
			    q.serve(id);
			    
			    cout << "\n----Product List After Deletion----" << endl;
			    q.display();
			    q.cont();
			    break;
				
			case 5: // Sort and display
				system("cls");
			    cout << "\nSort product by:" << endl;
			    cout << "1. Product ID" << endl;
			    cout << "2. Category" << endl;
			    cout << "3. Price" << endl;
			    
			    cout << "\nEnter your choice: ";
			    cin >> sortChoice;
			    
			    while (sortChoice<1 || sortChoice>3) 
				{
			    	cout << "Invalid input.... Please try again.";
					cout << endl;
					
					cout << "\nReenter your choice: ";
					cin  >> choice; 
				}
			    
			    if (sortChoice == 1) 
				{
			    	arr = q.IDtoArray(size);
			    	cout << "\n-----Display Product by ID-----" << endl;
			    	q.IDMergeSort(0, size-1);
				} 
				if (sortChoice == 2) 
				{
					arr = q.CategorytoArray(size);
			    	cout << "\n-----Display Product by Category-----" << endl;
			    	q.CategoryMergeSort(0, size-1);
				} 
				if (sortChoice == 3) 
				{
					arr = q.PricetoArray(size);
					cout << "\n-----Display Product by Price-----" << endl;
					q.PriceMergeSort(0, size-1);
				} 
				
				q.display();
				
				q.cont();
				break;
				
			case 6: // Searching
				system("cls");
			    cout << "\nSearch product by:" << endl;
			    cout << "1. Product ID" << endl;
			    cout << "2. Category" << endl;
			    
			    cout << "\nEnter your choice: ";
			    cin >> searchChoice;
			    
			    while (searchChoice<1 || searchChoice>2) 
				{
			    	cout << "Invalid input.... Please try again.";
					cout << endl;
					
					cout << "\nReenter your choice: ";
					cin  >> searchChoice; 
				}
			    
			    if (searchChoice == 1) 
				{
			    	arr = q.IDtoArray(size);
			    	q.IDMergeSort(0, size-1);
			    	
			    	arr = q.IDtoArray(size);
			    	q.IDMergeSort(0, size-1);
			    	
			    	cout << "\n-----Search Product by ID-----" << endl;
			    	
			    	cout << "Enter product ID to search: ";
					cin  >> target;
					
					result = q.IDBinarySearch(arr, 0, size-1, target);
				} 
				if (searchChoice == 2) 
				{
					arr = q.CategorytoArray(size);
					//q.CategoryMergeSort(0, size-1);
			    	cout << "\n-----Search Product by Category-----" << endl;
			    	
			    	cout << "Enter category to search: ";
					cin  >> target;
					
					result = q.CategoryBinarySearch(arr, 0, size-1, target);
				} 

				q.cont();
				break;
				
			case 7: // Save sorted data
				system("cls");
			    cout << "\nDisplay report:" << endl;
			    cout << "1. Daily sales report" << endl;
			    cout << "2. Weekly sales report" << endl;
			    
			    cout << "\nEnter your choice: ";
			    cin >> reportChoice;
			    
			    while (reportChoice<1 || reportChoice>2) 
				{
			    	cout << "Invalid input.... Please try again.";
					cout << endl;
					
					cout << "\nReenter your choice: ";
					cin  >> reportChoice; 
				}
			    
				
				q.cont();
				break;
				
			case 8: // Save sorted data
				system("cls");
			    arr = q.IDtoArray(size);
				q.IDMergeSort(0, size-1);
			    q.savetofile("sorted_information.txt");
				cout << "Sorted data save successfully." << endl;
				
				q.cont();
				break;
				
			case 9: //View Order
				system("cls");
				cout << "\nView Order History:\n";
				cout << "1. All Orders\n";
				cout << "2. Customer Orders Only\n";
				cout << "3. Member Orders Only\n";
				cout << "\nEnter choice: ";
				cin >> choice;
				cout << endl;
				q.viewAllOrdersByAdmin(choice);
				
				q.cont();
    			break;
    			
    		case 10: //Daily report
    			system("cls");
			    q.viewProfitReport();
			    
			    q.cont();
			    break;
			    
			case 11:
			      if (currentUser->getRole() == "SuperAdmin") 
				   	{
				   		system("cls");
			            addNewAdmin(); // 只有SuperAdmin可以执行
			      	} 
					else 
					{
			            cout << "Access denied." << endl;
			        }
			    
			    q.cont();
			    break;
			    
			case 12:  
				system("cls");
				q.addRewardItem();
				
			    q.cont();
			    break;
				    
			case 13:
				system("cls");
			    q.restockRewards();
			    
			    q.cont();
			    break;

			case 14: // Logout
				q.releaseMemory();
	            cout << "\nLogging out..." << endl;
	            adminLoggedIn = false;
	            break;
		}
	
	} while (adminLoggedIn);// Repeat until exit
	
	cout << "Goodbye." << endl;
}