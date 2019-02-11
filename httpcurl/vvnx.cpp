/*
***
* frameworks/base/cmds/
* 
* adb push out/target/product/mido/system/bin/httpvvnx /system/bin
* 
* https://curl.haxx.se/libcurl/c/http-post.html
* 
 */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>





int main()
{
	


    fprintf(stdout, "Début de main de httpvvnx\n");
    
      CURL *curl;
  CURLcode res;
 
  /* In windows, this will init the winsock stuff */ 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */ 
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */ 
    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.7:8000");
    /* Now specify the POST data */ 
    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();

    

    return 0;
}
