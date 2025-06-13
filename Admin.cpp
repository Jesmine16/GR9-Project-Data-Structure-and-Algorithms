#include<iostream>
#include<fstream>
#include<iomanip>
#define MAX 7
using namespace std;

int array[MAX];

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
		
		// Perform ternary search in a sorted array
		int TernarySearch(string arr[], int first, int last, string target) {
		    if(first <= last) {
			    int mid1 = first + (last - first) / 3;
				int mid2 = last - (last - first) /3;
				
				if(arr[mid1] == target) {
				    return mid1;
				} 
				
				if(arr[mid2] == target) {
				    return mid2;
				}
					
				if(target < arr[mid1] ) {
					return TernarySearch(arr, first, mid1, target);
				} 
					
				if(target > arr[mid2] ) {
					return TernarySearch(arr, mid2 + 1, last, target);
				} 
					
				return TernarySearch(arr, mid1, mid2, target);
			} else {
			    return -1; // Not found
			}
		}
		
		// Insertion sort based on product category
		void InsertionSort(int first, int last) {
		    for (int i = first + 1; i <= last; i++) {
			    Apple temp = queue[i]; // Store current element
				int j = i - 1;
				
				while (j >= first && queue[j].category > temp.category) {
				    queue[j + 1] = queue[j];
					j--;
				}
				queue[j + 1] = temp;
			}
		}
		
		// Merge sort based on product category
		void MergeSort(int f, int mid, int l) {
		    int len1 = mid - f + 1, len2 = l - mid;
		    
			Apple* first = new Apple[len1];
			Apple* last = new Apple[len2];
			
			for (int i = 0; i < len1; i++) {
				first[i] = queue[f + i];
			}
			
			for (int i = 0; i < len2; i++) {
				last[i] = queue[mid + 1 + i];
			}
			
			int i = 0;
			int j = 0;
			int k = f;
			
			while (i < len1 && j < len2) {
			    if (first[i].category <= last[j].category) {
				    queue[k] = first[i];
					i++;
				} else {
				    queue[k] = last[j];
					j++;
				}
				k++;
			}
				
			while (i < len1) {
				queue[k] = first[i];
				k++;
				i++;
			}
				
			while (j < len2) {
				queue[k] = last[j];
				k++;
				j++;
			}
		}
		
		//Tim sort based on product category
		void TimSort(int n) {
			
			for (int i = 0; i < n; i += RUN) {
				InsertionSort(i, min((i + RUN - 1), (n - 1)));
			}
			
			for (int size = RUN; size < n; size = 2 * size) {
			    for (int first = 0; first < n; first += 2 * size) {
				    int mid = first + size - 1;
					int last = min((first + 2 * size - 1), (n - 1));
					
					if (mid < last) {
						MergeSort(first, mid, last);
					}
				}
			}
		}
		
		// Insertion sort based on product ID
		void IDInsertionSort(int first, int last) {
		    for (int i = first + 1; i <= last; i++) {
			    Apple temp = queue[i];
				int j = i - 1;
				
				while (j >= first && queue[j].id > temp.id) {
				    queue[j + 1] = queue[j];
					j--;
				}
				queue[j + 1] = temp;
			}
		}
		
		// Merge sort based on product ID
		void IDMergeSort(int f, int mid, int l) {
		    int len1 = mid - f + 1, len2 = l - mid;
		    
			Apple* first = new Apple[len1];
			Apple* last = new Apple[len2];
			
			for (int i = 0; i < len1; i++) {
				first[i] = queue[f + i];
			}
			
			for (int i = 0; i < len2; i++) {
				last[i] = queue[mid + 1 + i];
			}
			
			int i = 0;
			int j = 0;
			int k = f;
			
			while (i < len1 && j < len2) {
			    if (first[i].id <= last[j].id) {
				    queue[k] = first[i];
					i++;
				} else {
				    queue[k] = last[j];
					j++;
				}
				k++;
			}
				
			while (i < len1) {
				queue[k] = first[i];
				k++;
				i++;
			}
				
			while (j < len2) {
				queue[k] = last[j];
				k++;
				j++;
			}
		}
		
		// Tim sort based on product ID
		void IDTimSort(int n) {
			
			for (int i = 0; i < n; i += RUN) {
				IDInsertionSort(i, min((i + RUN - 1), (n - 1)));
			}
			
			for (int size = RUN; size < n; size = 2 * size) {
			    for (int first = 0; first < n; first += 2 * size) {
				    int mid = first + size - 1;
					int last = min((first + 2 * size - 1), (n - 1));
					
					if (mid < last) {
						IDMergeSort(first, mid, last);
					}
				}
			}
		}
		
		// Pause function to continue execution after user input
		void cont() {
			string key;
			
			cout << "\n[Enter any key and press enter to continue]";
			cin >> key;
		}

};

void Merge(int lpos, int rpos, int rend) {
	int i, lend, numelements, tmppos, TmpArray[MAX];
	lend = rpos - 1;
	tmppos = lpos;  
	numelements = rend - lpos + 1;
	
	while( lpos<=lend && rpos <= rend ) {
		if( array[lpos] <= array[rpos] ) {
			TmpArray[tmppos++] = array[lpos++]; 
		} else {
			TmpArray[tmppos++] = array[rpos++]; 
		}
	}
	
	while(lpos <= lend) {
		TmpArray[tmppos++] = array[lpos++]; 
	}
	
	while(rpos <= rend) {
		TmpArray[tmppos++] = array[rpos++];
	}
	
	for(i = 0 ; i < numelements; i++, rend--) {
		array[rend]=TmpArray[rend];  
	}
}  

void MergeSort (int left, int right) {
	int center;  
	
	if(left < right) {
		center = (left + right)/2;
		
		MergeSort(left, center);
		MergeSort(center+1, right);
		Merge(left, center+1, right);
	}
}

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
				q.IDTimSort(size); // Sort by ID
				q.display();
				
				q.cont();
				break;
				
			case 6: // Search by category
				cout << "\n-----Search Product By Category-----" << endl;
				arr = q.toArray(size); // Convert to array
				q.TimSort(size); // Sort by category
				
				cout << "Enter category to search: ";
				cin  >> target;
				
				result = q.TernarySearch(arr, 0, size - 1, target); // Search
				
				if (result != -1) {
			        q.searchByCategory(target); // Display
				} else {
					cout << "Category \'" << target << "\' not found";
				}
				q.cont();
				break;
				
			case 7: // Save sorted data
			    arr = q.IDtoArray(size);
				q.IDTimSort(size);
			    q.savetofile("sorted_information.txt");
				cout << "Sorted data save successfully." << endl;
				
				q.cont();
				break;
		}
	
	} while (choice != 8); // Repeat until exit
	
	cout << "Goodbye.";
}
