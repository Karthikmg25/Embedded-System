#include <iostream>
#include <curl/curl.h>
#include <string>

/* This program sends a POST request to server- https://httpbin.org/post and receives response */




	// httpbin.org/post returns a JSON response containing details about the POST request,
	// including the request headers, submitted form data, and request URL.

// write callback for receiving data
size_t writeCallback(void* data// data buffer received
	,size_t size// size of data element
	,size_t nmemb// number of data elements
	,void* userp// pointer to userdata
	)
	{
		// store recived data buffer into user string
		((std::string*)userp)->append((char*)data, size*nmemb);

		return size*nmemb;
	}
int main()
{
	// initialize curl library
	curl_global_init(CURL_GLOBAL_DEFAULT);

	// create a curl object and initialize the handle object
	CURL* curl = curl_easy_init();
	if(curl == NULL){
		std::cout << "init failed "<< std::endl;
		curl_global_cleanup();
		return 1;
	}

	// set configuration: set URL
	std::string url = "https://httpbin.org/post";
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	// set configuration : set POST request, not the default GET method
	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	// set configuration : add data to be send as the POST body

	std::string data = "name=Karthik&language=C++";
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

	// set configuration : register callback for receiving data from server
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);

	// set configuration : set userp in callback as pointer to string to collect response in a string
	std::string response;
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	// send the request
	CURLcode result = curl_easy_perform(curl);
	if(result != CURLE_OK){
		std::cout << "Request failed " << curl_easy_strerror(result);
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		return 1;
	}

	// print response
	std::cout << "Response from server : " << response << std::endl;

	// global and curl object cleanup
	curl_easy_cleanup(curl);
	curl_global_cleanup();


	return 0;
}

// Response from server : 
/*{
  "args": {},
  "data": "",
  "files": {},
  "form": {
    "language": "C  ",
    "name": "Karthik"
  },
  "headers": {         <------------------ HTTP REQUEST HEADER
    "Accept": "**",
    "Content-Length": "25",
    : "application/x-www-form-urlencoded""Content-Type", 
    "Host": "httpbin.org",
    "X-Amzn-Trace-Id": "Root=1-6aca3d7b-497852dc6122aecd6ddf48ac"
  },
  "json": null,
  "origin": "157.51.205.198",
  "url": "https://httpbin.org/post"
}

*/