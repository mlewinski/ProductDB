// ProductDB.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

unsigned long long id;

#pragma region Structures
	struct Product {
		int id;
		string name;
		float price;
	};

	struct ProductRepositoryNode {
		Product product;
		ProductRepositoryNode* nextElement;
	};
#pragma endregion

#pragma region RepoControl
	void AddToRepository(ProductRepositoryNode* &r , Product);
	void DisplayRepository(ProductRepositoryNode*);
	void DisplayProductInfo(Product);
	void FindProduct(ProductRepositoryNode*, string);
	void FindProduct(ProductRepositoryNode*, int);
	void FindProduct(ProductRepositoryNode*, double);
#pragma endregion

#pragma region ItemControl
	Product CreateNewProduct();
	void EditProduct(ProductRepositoryNode*, int);
#pragma endregion

#pragma region ProgramControl
	int Menu();
#pragma endregion

int main()
{
	ProductRepositoryNode* repo = NULL;
	
	for(int i=0;i<2;i++) AddToRepository(repo, CreateNewProduct());
	
	//DisplayRepository(repo);
	DisplayRepository(repo);
	EditProduct(repo, 10);
	DisplayRepository(repo);
	system("pause");
	return 0;
}

void AddToRepository(ProductRepositoryNode* &repository, Product product) {
	ProductRepositoryNode* tmp = repository;	
	ProductRepositoryNode* newProduct = new ProductRepositoryNode;
	newProduct->product = product;
	newProduct->nextElement = NULL;
	if (tmp == NULL) repository = newProduct;
	else {
		while (tmp->nextElement != NULL) tmp = tmp->nextElement;
		tmp->nextElement = newProduct;
	}	
}

void DisplayRepository(ProductRepositoryNode* repository) {
	int i = 0;
	ProductRepositoryNode* tmp = repository;
	if (tmp != NULL) {
		while (tmp != NULL) {
			cout << ">> Element number " << i << endl;
			DisplayProductInfo(tmp->product);
			i++;
			tmp = tmp->nextElement;
		}
	}
	else {
		cout << "No products found in database" << endl;
	}
}

void DisplayProductInfo(Product product) {
	cout << "=========================================================" << endl;
	cout << "> Product id : " << product.id << endl;
	cout << "> Product name : " << product.name << endl;
	cout << "> Product price : " << product.price << endl;
	cout << "=========================================================" << endl;
}

#pragma region FindProductOverloads

void FindProduct(ProductRepositoryNode* repository, string name) {
	ProductRepositoryNode* tmp = repository;
	if (tmp != NULL) {
		while (tmp != NULL) {
			if (tmp->product.name == name) {
				DisplayProductInfo(tmp->product);
			}
			tmp = tmp->nextElement;
		}
	}
	else {
		cout << "No products found in database" << endl;
	}
}

void FindProduct(ProductRepositoryNode* repository, int id) {
	ProductRepositoryNode* tmp = repository;
	if (tmp != NULL) {
		while (tmp != NULL) {
			if (tmp->product.id == id) {
				DisplayProductInfo(tmp->product);
				return;
			}
			tmp = tmp->nextElement;
		}
	}
	else {
		cout << "No products found in database" << endl;
	}
}

void FindProduct(ProductRepositoryNode* repository, double price) {
	ProductRepositoryNode* tmp = repository;
	if (tmp != NULL) {
		while (tmp != NULL) {
			if (tmp->product.price == price) {
				DisplayProductInfo(tmp->product);
			}
			tmp = tmp->nextElement;
		}
	}
	else {
		cout << "No products found in database" << endl;
	}
}

#pragma endregion

Product CreateNewProduct() {
	Product p;
	cout << "Creating new product : " << endl;
	cout << "> Product name : " << endl;
	cin >> p.name;
	cout << "> Product price : " << endl;
	cin >> p.price;
	p.id = 10;
	return p;
}

void EditProduct(ProductRepositoryNode* repository, int id) {
	ProductRepositoryNode* tmp = repository;
	while (tmp != NULL) {
		if (tmp->product.id == id) {
			int tmpID = id;
			tmp->product = CreateNewProduct();
			tmp->product.id = tmpID;
			return;
		}
	}
	cout << "Selected product does not exist" << endl;
}

int Menu() {
	cout << "------------------- Main menu -----" << endl;
	cout << "====== View =======================" << endl;
	cout << "1. Display all products in database" << endl;
	cout << "2. Find product in database" << endl;
	cout << "====== Manage =====================" << endl;
	cout << "3. Add new product to database" << endl;
	cout << "4. Edit existing product" << endl;
	cout << "5. Delete product from database" << endl;
	cout << "6. Sort database" << endl;
	cout << "\nSelect option : ";
	int option;
	cin >> option;
	if (option < 0 || option >6) return Menu();
	return option;
}