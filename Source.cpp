#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>

using namespace std;

int main() {
	while (true) {
		system("start powershell.exe -windowstyle hidden wget https://api.coinmarketcap.com/v1/ticker/?limit=1 -OutFile out.txt"); //Uses powershell to download a pricesheet from Coinmarketcap
		system("timeout /t 4 >nul"); //Waits 4 seconds for the file to download
		ifstream file; //Sets up ifstream variable
		file.open("out.txt"); //Opens file.
		string content((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>())); //Puts the contents of the file into a string. This is magic I coppied from the internet.

		//Loop goes through file character by character
		for (int i = 0; i < content.length(); i++) {
			if (content[i] == 'B' && content[i + 2] == 't' && content[i + 6] == 'n' && content[i + 8] == ',') {
				cout << content[i] << content[i + 1] << content[i + 2] << content[i + 3] << content[i + 4] << content[i + 5] << content[i + 6] << endl; //Prints the word bitcoin from the file
			}
			if (content[i] == 'p' && content[i + 6] == 'u') {
				cout << "Price: " << "$" << content[i + 13] << content[i + 14] << content[i + 15] << content[i + 16] << content[i + 17] << content[i + 18] << content[i + 19] << "0" << endl; //Prints the price
			}
			if (content[i] == 'p' && content[i + 15] == '2' && content[i + 16] == '4' && content[i+17] == 'h') { //Prints the percentage change over the last 24 hours
				if (content[i + 22] == '-') {
					cout << "-" << content[i + 23] << content[i + 24] << content[i + 25] << "%" << endl;
				}
				else {
					cout << "+" << content[i + 22] << content[i + 23] << content[i + 24] << "%" << endl;
				}
			}
		}
		file.close(); //Closes file
		remove("out.txt"); //Removes file
		system("timeout /t 600 >nul"); //Waits 10 minutes for API to update.
		system("cls");
	}
	return 0;
}