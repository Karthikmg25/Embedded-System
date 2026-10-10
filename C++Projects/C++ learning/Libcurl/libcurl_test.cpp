#include <iostream>
#include <curl/curl.h>

/* This program sends an HTTP request to a server */

int main()
{
	// print version
	std::cout << "Curl Version : " << curl_version() << std::endl;

	// initialize the curl library
	/********************************************************************************************************/
	curl_global_init(CURL_GLOBAL_DEFAULT);
	


	// Create and initialize curl object
	/********************************************************************************************************/

	// declare a handle pointer to curl object
	CURL* curl;

	curl = curl_easy_init();

	if(curl == NULL){
		std::cout << "Init failed" << std::endl;
		return 1;

		curl_easy_cleanup(curl); // destroys the curl object
		curl_global_cleanup();
	}
	std::cout << "Curl initialized successfully"<< std::endl;

	// configure the curl object : setting the URL
	/********************************************************************************************************/
	curl_easy_setopt(curl, CURLOPT_URL, "https://examples.com");

	// perform HTTP request
	/********************************************************************************************************/
	CURLcode result = curl_easy_perform(curl);

	if(result != CURLE_OK)
	{
		std::cout << "Request failed" <<
		curl_easy_strerror(result) << std::endl;

		curl_easy_cleanup(curl); // destroys the curl object
		curl_global_cleanup();

		return 1;
	}
	std::cout << "Request successfull " << std::endl;

	// print the http response code
	long response_code;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

	std::cout << "Response code : " << response_code << std::endl;

	// cleanup
	/********************************************************************************************************/

	curl_easy_cleanup(curl); // destroys the curl object
	curl_global_cleanup();
	
	return 0;


	// By default, libcurl automatically writes the received response data to stdout //
}

