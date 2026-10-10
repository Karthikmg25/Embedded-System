#include <iostream>
#include <curl/curl.h>
#include <string>

/* This program sends an HTTP request and recives the respnse via callback function */


size_t writeCallback(void* data// pointer to recived data buffer
	, size_t size              // size of each data element
	, size_t nmemb             // number of recieved data elements
	, void* userp              // pointer to user data
)
{

	size_t bytes_received = size * nmemb;

	// store the recived data into string

	((std::string*)userp)->append((char*)data, bytes_received);

	// print recived data
	std::cout<< "\n " << (char*)data << std::endl;
	
	return bytes_received;

}

int main()
{ 
	// init libcurl library
	curl_global_init(CURL_GLOBAL_DEFAULT);

	// create and init curl object
	CURL* curl = curl_easy_init();
	if(curl == NULL){
		std::cout << "Init failed "<< std:: endl;

		curl_global_cleanup();
		return 1;
	}

	// configure object : set URL
	curl_easy_setopt(curl, CURLOPT_URL, "https://examples.com");

	// configure object : register callback function for receiving response
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback );

	// configure object : pass strings address as userp in the callback

	// create astring to store http response
	std::string response;

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	// perfomr the request
	CURLcode result = curl_easy_perform(curl);
	if(result != CURLE_OK)
	{
		std::cout << "request failed " << curl_easy_strerror(result) << std::endl;

		// clean up before return
		curl_easy_cleanup(curl);
		curl_global_cleanup();

		return 1;

	}

	std::cout << "Request successful !"<< std::endl;

	// cleanup
	curl_easy_cleanup(curl); // destroys the curl object
	curl_global_cleanup();



	// print response stored in string
	std::cout << "\n\nResponse recieved : " << response << std::endl;
	
	return 0;

}


