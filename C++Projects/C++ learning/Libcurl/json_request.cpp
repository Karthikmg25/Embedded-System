#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

/* This program sends a JSON data to server - https://httpbin.org/post and receives response */


size_t writeCallback(void* data, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)data, size*nmemb);

	return size*nmemb;
}
void print_jsonData(std::string &response);
void print_httpcode(CURL* curl);
int main()
{
	// init
	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL* curl = curl_easy_init();
	if(curl == NULL){
		std::cout << "Init failed ";
		curl_global_cleanup();
		return 1;
	}

	// configure curl object : set URL
	curl_easy_setopt(curl, CURLOPT_URL, "https://httpbin.org/post");

	// configure curl object : set POST request
	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	// configure curl object : set POST body as a JSON data

	// create a json object
	nlohmann::json data;
	data["name"] = "Karthik";
	data["language"] = "C++";

	// store json data in a string
	std::string json_str = data.dump();

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str.c_str());

	// configure curl object : register callback
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);

	// configure curl object : set uerp in callback as pointer to user data

	std::string response;

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);



	// configure the curl object : setting the http header

	// For POST data supplied through CURLOPT_POSTFIELDS, libcurl uses
	// application/x-www-form-urlencoded by default when appropriate.
	// Since we are sending JSON, explicitly specify application/json.

	// declare a pointer to header list
	struct curl_slist* header = NULL;

	// populate the header with json content type
	header = curl_slist_append(header, "Content-Type: application/json");

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);

	// perform request
	CURLcode result = curl_easy_perform(curl);
	if(result != CURLE_OK){
		std::cout << "Request failed " << curl_easy_strerror(result) << std::endl;
		//free memory allocated for headerlist
		curl_slist_free_all(header);
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		return 1;
	}

	// print response
	std::cout << "Received response : \n" << response << std::endl;

	// print values in json
	print_jsonData(response);

	// print response code
	print_httpcode(curl);

	//free memory allocated for headerlist
	curl_slist_free_all(header);

	// cleanup
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return 0;
}

// write a function to parse the response string and extract name and language from json data and print
void print_jsonData(std::string &response)// pass by reference to avoid making a copy
{
	// store response string as a json object
	nlohmann::json data = nlohmann::json::parse(response);

	// print name
	std::string name = data["json"]["name"].get<std::string>();
	std::cout << "\nName : "<< name << std::endl;
	
	// print language
	std::string language = data["json"]["language"].get<std::string>();
	std::cout << "\nLanguage : "<< language << std::endl;


}

// create a function to print the http response code
void print_httpcode(CURL* curl)
{
	long code;
	curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &code);

	std::cout << "\nHTTP Response code : " << code << std::endl;
}