#include <iostream>
#include "Number.h"

using namespace std;

int main() {
	Number num;
	int choice = 1;
	int newBase = 0;

	while (choice != 0) {
		switch (choice) {
		case 1:
			cout << "-----------" << endl;
			while (true) {
				cout << "Input number and base: ";
				try {
					cin >> num;
				}
				catch (const char* e) {
					cout << e << endl;
					continue;
				}
				break;
			}
			break;

		case 2:
			cout << "-----------" << endl << "Convert number to base: ";
			cin >> newBase;
			try {
				num.convertBase(newBase);
			}
			catch (const char* e) {
				cout << e << endl;
				system("PAUSE");
				break;
			}
			break;
		default:
			break;
		}
		
		if (choice > 2 && choice < 0) {
			cout << "Invalid choice!" << endl;
			system("PAUSE");
			continue;
		}

		system("CLS");
		cout << "ENTERED: " << num << endl << "---------" << endl;
		cout << "1. Change number & base" << endl;
		cout << "2. Convert to other base" << endl;
		cout << "0. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;
	}

	system("PAUSE");
	return 0;
}
