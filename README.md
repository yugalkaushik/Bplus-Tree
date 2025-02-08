# *B+ Tree Database*

## Overview  

This project is a **lightweight database system** that efficiently implements the **B+ Tree** in C++ for **fast and structured file access**.  
The database tuples are stored as `.txt` files in the **DBFiles** directory, and corresponding `FILE*` pointers are maintained in the **leaf nodes** of the B+ Tree for efficient retrieval.  

The project showcases an **optimized indexing mechanism** using B+ Trees, ensuring quick search, insertion, and deletion operations—making it a great reference for learning about **database indexing techniques**.  

---

## Key Features  

- **Efficient B+ Tree Implementation** - Supports fast search, insert, and delete operations  
- **File-Based Data Storage** - Tuples stored as `.txt` files for persistence  
- **Primary Key Constraint** - Ensures unique entries with no duplicates  
- **Right-Biased Splitting** - Maintains balanced tree structure for optimal performance  
- **Scalable & Modular Code** - Easily extendable for additional database functionalities  

---

## Technical Assumptions & Constraints  

### Right-Biased Tree Structure  
- When a node is **split**, the **right sibling** receives one extra key compared to the left.  
- This ensures **better balancing and efficient searching** in the tree.  

### Primary Key-Based Insertion  
- Data is inserted using **primary keys**, enforcing **uniqueness** in the database.  
- No duplicate entries are allowed for the same key.  

### Fixed Degree Constraints  
- The **maximum number of children** per internal node (`maxIntChildLimit`) and  
  **maximum keys per leaf node** (`maxLeafNodeLimit`) are pre-defined for optimized performance.  

---
## How to Use  

### Clone the Repository  
```sh
git clone https://github.com/yourusername/B-Plus-Tree-Database.git
cd B-Plus-Tree-Database 
```
### Compile the Code
```sh
g++ *.cpp -o main.exe
```
### Run the Program
```sh
./main.exe
```

## Contributing
Contributions are welcome! Feel free to fork the repository and submit a pull request with your improvements.
