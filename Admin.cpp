#include<iostream>
#include<fstream>
#include<iomanip>
using namespace std;

const int RUN = 32; //use for sorting

struct Apple {
	string id, category, name;
	int stock;
	double price, cost;
};

class AppleStore { 
    private:  
	    Apple queue[50];    
	    int head,tail;  
	
	public:  
	
	    AppleStore() {  
	        tail = -1;  
	        head = 0;  
	    }  
		
		// Check the queue is empty or not
		int empty() {  
		    if(head == tail+1) {
			    return 1;  
			} else {
				return 0;
			}
		}  
		
		// Check the queue is full or not
		int full() {
			if(tail == 49) {
				return 1;
			} else {
				return 0;
			}
		}
		
		// Add a new product to the queue
		void append(string id, string category, string name, double price, double cost, int stock) {
			if(!full()) {
				tail++;
				queue[tail].id = id;
				queue[tail].category = category;				
				queue[tail].name = name;
				queue[tail].price = price;
				queue[tail].cost = cost;
				queue[tail].stock = stock;
			} else {
				cout << "Queue is full" << endl;
			}
		}
		
		// Serve the product from the queue
		string serve(string id) {
			
			string delProduct = "";
        	bool found = false;
			
			if(!empty()) {
				
				for (int i = head; i <= tail; i++) {
        		    if (queue[i].id == id) {
        			    found = true;
						
						delProduct = queue[i].name;
						cout << "\nProduct delete: " << delProduct << endl;
						
						// Reposition data after deleted (ensure no empty row)
						for (int j = i; j < tail; j++) {
						    queue[j] = queue[j + 1];
						}
							 
						// Reduce the tail index since one data is deleted
						tail--;
						break;
					}
					
				} 
				
				if (!found) {
			        cout << "Product not found...." << endl;
				} else {
					savetofile("Apple_Store.txt");
					savetofile("sorted_information.txt");
				}
			} else {
				cout << "Queue is empty" << endl;
			}
			
			return delProduct;
		}
		
		// Convert product IDs to a string array
		string* IDtoArray(int &size) {
		    size = tail - head + 1;
			string* arr = new string[size];
			
			for (int i = 0; i < size; ++i) {
			    arr[i] = queue[head + i].id;
			}
			
			return arr;
		}
		
		// Convert product categories to a string array
		string* toArray(int &size) {
		    size = tail - head + 1;
			string* arr = new string[size];
			
			for (int i = 0; i < size; ++i) {
			    arr[i] = queue[head + i].category; 
			}
			
			return arr;
		}
		
		// Check for duplicate product ID
		void checkid(string id) {
			
			if(!empty()) {
				for(int i=head; i<=tail; i++) {
					if(queue[i].id == id) {
						cout << "This Product ID is already exists..." << endl;
						cout << "\nEnter product ID: ";
						cin >> id;
					}
				}
			}
		}
		
		// Read products from file and store in the queue
		void readfile(string filename) {
			
			ifstream file(filename);
			
            string id, category, name;
            int stock;
            double cost, price;
            
            string line;
	
        	if(!file) {
	        	cout << "Error opening file!";
    	    } else {
    	    	//Loading data (using reference)
	    	    while(getline(file, line)) {
	    	    	
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
        void savetofile(string filename) {
        	
        	ofstream outfile(filename);
	
	        if(!outfile) {
		        cout << "Error opening file!";
	        } else {
		        for(int i=head; i<=tail; i++) {
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
        void display() {
        	
        	cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
		    cout << "| ID      | Category     | Product Name                 | Price (RM)   | Cost (RM)   | Stock    |" << endl;
		    cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
	 
        	if(!empty()) {
		
	        	for(int i=head; i<=tail; i++) { //for(int i=head; i>=0; i++)
	        	    cout << "| ";
			        cout.width(7); cout << left << queue[i].id << " | ";
			        cout.width(12); cout << left << queue[i].category << " | ";
			        cout.width(28); cout << left << queue[i].name << " | ";
			        cout.width(12); cout << right << queue[i].price << " | ";
					cout.width(11); cout << right << queue[i].cost << " | ";
					cout.width(8); cout << right << queue[i].stock << " | " << endl; //cout << queue[i] << endl;
		        }
	        } else {
	        	cout << "| ";
		        cout.width(93); cout << left << "No product available." << " | " << endl;
	        }
	        
	        cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
        }
        
        // Search product ID exits or not - for update (price, cost, or stock)
        void searchUpdate(string id) {
        	
        	string type;
        	int stock;
			double cost, price;
        	bool found = false;
        	
        	for (int i = head; i <= tail; i++) {
        		if (queue[i].id == id) {
        			cout << "\nProduct found: " << queue[i].name << endl;
        			found = true;
					break;
				}
			} 
			
			if (!found) {
			    cout << "Product not found...." << endl;
			    cout << "\nReneter product ID: ";
			    cin >> id;
			}
			
			cout << "Enter updating field(Price/Cost/Stock): ";
			cin >> type;
			    
			if(type=="Price" || type=="price") {
				cout << "Enter updated price: ";
				cin >> price;
			    update(id, type, price);
			} else if(type=="Cost" || type=="cost") {
				cout << "Enter updated cost: ";
				cin >> cost;
			    update(id, type, cost);
			} else if(type=="Stock" || type=="stock") {
				cout << "Enter updated stock: ";
				cin >> stock;
			    updateStock(id, type, stock);
			} else {
				cout << "Invalid update type..." << endl;
				cont();
			}
		}
        
        // Update price or cost of product
        void update(string id, string type, double change) {
        	
        	string filename = "Apple_Store.txt";
        	
        	for (int i = head; i <= tail; i++) {
        		if (queue[i].id == id) {
					
					if(type=="Price" || type=="price") {
						queue[i].price = change;
					} else if(type=="Cost" || type=="cost") {
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
		void updateStock(string id, string type, int changeStock) {
        	
        	string filename = "Apple_Store.txt";
        	
        	for (int i = head; i <= tail; i++) {
        		if (queue[i].id == id) {
					
					if(type=="Stock" || type=="stock") {
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
        
        // Search and display products by category
        void searchByCategory(string key) {
        	
        	cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;
			cout << "| ID      | Category     | Product Name                 | Price (RM)   | Cost (RM)   | Stock    |" << endl;
			cout << "+---------+--------------+------------------------------+--------------+-------------+----------+" << endl;

			for (int i = head; i <= tail; i++) {
			    if (queue[i].category == key) {
		
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
		void cont() {
			string key;
			
			cout << "\n[Enter any key and press enter to continue]";
			cin >> key;
		}
		
		//left = Left position : starting index of the left sorted subarray
		//right = 	Right position – starting index of the right sorted subarray
		//lend = left end : last index for part left
		//rend = right end : last index for part right
		//num = Total Number of Elements to be Merge
		//temp = Index for Inserting into TmpArray
		
		void Merge(int left, int right, int rend) {
		    int i, lend, num, tmp;
			lend = right - 1;
			tmp = left;  
			num = rend - left + 1;
			
			Apple* TmpArray = new Apple[50];
			
			while( left <= lend && right <= rend ) {
			    if( queue[left].id <= queue[right].id ) {
				    TmpArray[tmp++] = queue[left++]; 
				} else {
					TmpArray[tmp++] = queue[right++]; 
				}
			}
			
			while(left <= lend) {
			    TmpArray[tmp++] = queue[left++]; 
			}
				
			while(right <= rend) {
				TmpArray[tmp++] = queue[right++];
			}
				
			//for(i = 0 ; i < num; i++, rend--) {
				//	queue[rend] =TmpArray[rend];  
			//}
				
			// Correct copying back from TmpArray to queue
			for(i = 0; i < num; i++) {
				queue[rend - num + 1 + i] = TmpArray[rend - num + 1 + i];
			}
			
			delete[] TmpArray;
		}  
		
		//first = The starting index of the portion of the array to be sorted
		//last = The ending index of the portion of the array to be sorted
		//center = 	The middle index used to split the array into two halves
		
		void MergeSort (int first, int last) {
		    int center;  
			
			if(first < last) {
			    center = (first + last)/2;
				
				//breaks the array into smaller parts
				MergeSort(first, center); 
				MergeSort(center+1, last);
				Merge(first, center+1, last);
			}
		}

};

int BinarySearch() {
	int table [200];
	int target,first,last,mid;
	int i; 
	int found;
	
	//Setting up the array
	for(i=0; i<200; i++) {
		table[i] = 2*i;
	}
	
	first = 0;
	last = 199;
	found = 0;
	
	//Input the target 
	cout<<"Input the target"; 
	cin>>target;
	
	//The binary search 
	while( (found==0) && (first <= last) ) {
		mid = (first + last) / 2; 
		
		if( target == table[mid]) {
			found = 1; 
		} else {
			if( target<table[mid] ) {
				last = mid - 1;
			} else {
				first = mid + 1;
			}
		}
	}
	
	if(found) {
		cout<<"\nPosition ="<<mid;
	} else {
		cout<<"\nTarget not found";
	}
}

int main() {
	AppleStore q; // Create AppleStore object

	string filename = "Apple_Store.txt";
    q.readfile(filename); // Load data from file
	 
	int choice;
	
	string id, name, category, type;
	int stock;
	double cost, price;
    
    string target;
	int result;
    int size;
	string* arr;
	int n;
	
	do {
		system("cls"); // Clear screen
		
		// Display menu
		cout << "Welcome to Apple Store";
		cout << "\n1. Display all product";
		cout << "\n2. Add new product";
		cout << "\n3. Update product details";
		cout << "\n4. Delete product";
		cout << "\n5. Sort product";
		cout << "\n6. Search product";
		cout << "\n7. Display summary report";
		cout << "\n8. Exit.";
		
		cout << "\n\nEnter your choice: ";
		cin  >> choice;
		
		// Input validation
		while ( choice<1 || choice>8) {
		    cout << "Invalid input.... Please try again.";
			cout << endl;
			
			cout << "\nReenter your choice: ";
			cin  >> choice;   
		}
		
		switch(choice) {
		    case 1: // Display all products
		        cout << "\n-----Display Product-----" << endl;
			    q.display(); 
			    q.cont();
				break;
				
			case 2: // Add new product
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
				cout << "Data have been save successful! " << endl;
				q.cont();
				break;
				
			case 3: // Update product
			    q.display();
			    cout << "\n-----Update Product Details-----" << endl;
			    cout << "Enter product ID for updating: ";
			    cin >> id;
			    
			    q.searchUpdate(id); // Update specific product
			    break;
			    
			case 4: 
			    q.display();
			    cout << "\n----Delete Product----" << endl;
			    cout << "Enter product ID for deleting: ";
			    cin >> id;
			    
			    q.serve(id);
			    
			    cout << "\n----Product List After Deletion----" << endl;
			    q.display();
			    q.cont();
			    break;
				
			case 5: // Sort and display by ID
				cout << "\n-----Display Sorted Product-----" << endl;
				arr = q.IDtoArray(size); // Convert to array
				q.MergeSort(0, size-1); // Sort by ID
				//Elements after being sorted 
				cout<<"The elements after Merge Sort"<<endl; 
				
				q.display();
				
				q.cont();
				break;
				
			case 6: // Search by category
				cout << "\n-----Search Product By Category-----" << endl;
				arr = q.toArray(size); // Convert to array
				//q.TimSort(size); // Sort by category
				
				cout << "Enter category to search: ";
				cin  >> target;
				
				//result = q.TernarySearch(arr, 0, size - 1, target); // Search
				
				if (result != -1) {
			        //q.searchByCategory(target); // Display
				} else {
					cout << "Category \'" << target << "\' not found";
				}
				q.cont();
				break;
				
			case 7: // Save sorted data
			    arr = q.IDtoArray(size);
				//q.IDTimSort(size);
			    q.savetofile("sorted_information.txt");
				cout << "Sorted data save successfully." << endl;
				
				q.cont();
				break;
		}
	
	} while (choice != 8); // Repeat until exit
	
	cout << "Goodbye.";
}
