#include <iostream>
#include <string>
#include <stdio.h>
#include "json.hpp" // JSON library by https://github.com/nlohmann/json for efficiency and security in further development of project

#ifdef BUILD_LINUX // Defined by compiler
#include <curl/curl.h>
#include <algorithm>
#include <unistd.h>
#else
#include <fstream>
#endif

using namespace std;
using json = nlohmann::json;

std::string buffer;

#ifdef BUILD_LINUX
size_t read_api_output(char *ptr, size_t size, size_t nmemb, void *stream) {
	buffer = "";
	for ( int i = 0; i < size*nmemb; i++ )
    {
        buffer.push_back(ptr[i]);
    }
	buffer.erase(std::remove(buffer.begin(), buffer.end(), '['), buffer.end()); // Remove all [ and ] characters
    buffer.erase(std::remove(buffer.begin(), buffer.end(), ']'), buffer.end()); // So string can be parsed
	return size*nmemb;
}
#endif

int main() {

#ifdef BUILD_LINUX

		CURL *http_client;
		CURLcode http_response;


    while (true) {
		http_client = curl_easy_init();
		if( http_client ) {
			curl_easy_setopt(http_client, CURLOPT_URL, "https://api.coinmarketcap.com/v1/ticker/?limit=1");
			curl_easy_setopt(http_client, CURLOPT_WRITEFUNCTION, &read_api_output);
			curl_easy_setopt(http_client, CURLOPT_FOLLOWLOCATION, 1L);
			http_response = curl_easy_perform(http_client);
			if ( http_response == CURLE_OK ) {
				curl_easy_cleanup(http_client);
				auto j = json::parse(buffer); // Parse API output into JSON object
				string price = j["price_usd"]; // Select specific object
				price.erase(std::remove(price.begin(), price.end(), '"'), price.end()); // Remove "" characters surrounding string
				cout << "\rBitcoin Price: $" << price << endl;
			}
		}
		sleep(3);

#else
    while (true) {
		system("start powershell.exe -windowstyle hidden wget https://api.coinmarketcap.com/v1/ticker/?limit=1 -OutFile out.txt"); //Uses powershell to download a pricesheet from Coinmarketcap
		system("timeout /t 5 >nul"); //Waits 5 seconds for the file to download
		ifstream file; //Sets up ifstream variable
		file.open("out.txt"); //Opens file.
		string content((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>())); //Puts the contents of the file into a string. This is magic I coppied from the internet.
		file.close(); // Close file before string manip just in case..

		//Loop goes through file character by character
		for (uint i = 0; i < content.length(); i++) {
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
		} // Would reccommend changing this code to use JSON library too
          // And also instead of using a file which is not very efficient,
		  // Use a network library to create a GET request and save it to a string

		  /*
		        I'm not running Windows right now to check but you can simply use json::parse on your 'content' string
		  */

		remove("out.txt"); //Removes file
		system("timeout /t 600 >nul"); //Waits 10 minutes for API to update.
		system("cls");
#endif
	}
	return 0;
}
