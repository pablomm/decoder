
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "pop3.h"

CURL *curl;

Msg *init_msg(void)
{
	return (Msg*) calloc(1,sizeof(Msg));
}

void free_msg(Msg *msg)
{	
	if(msg) {
		if(msg->subject) free(msg->subject);
		if(msg->date) free(msg->date);
		if(msg->msg) free(msg->msg);
		free(msg);
	}
}

size_t inbox_number(void *ptr, size_t size, size_t nmemb, void *stream)
{
	char *str;
	size_t len = size*nmemb;
	int *n_messages = (int *) stream;
	int number = -1;

	if(len > 2) 
	{
		str = calloc(len + 1,sizeof(char));
		if(!str) 
			return -1;

		memcpy(str,ptr,len);
     	number = atoi(strtok(str, " \n\r\t"));
		*n_messages = (*n_messages < number) ? number : *n_messages;
		free(str);
	}

    return len;
}

size_t parse_message(void *ptr, size_t size, size_t nmemb, void *stream)
{
	char *str;
	char *it;
	size_t len_it;
	size_t len = size*nmemb;
	Msg *msg = (Msg*) stream;

	if(len > 2) 
	{
		str = calloc(len + 1,sizeof(char));
		if(!str) 
			return -1;

		memcpy(str,ptr,len);
     	/*printf("-->%s\n", (char*)str);*/

		if(msg->flag) {
			it = strtok(str,"=");
			len_it = strlen(it);
			msg->msg = realloc(msg->msg, msg->len + len_it + 1);
			memcpy(msg->msg + msg->len,it,len_it);
			msg->msg[msg->len + len_it] = '\0';
			msg->len += len_it;

		} else if(!(it = strtok(str,":"))) {
			fprintf(stderr, "\nWarning: Request could be corrupt\n");
		
		} else if(!strcmp(it,"Subject")){
			it = strtok(NULL,"");
			len_it = strlen(it);
			msg->subject = calloc(len_it+1, sizeof(char));
			memcpy(msg->subject,it,len_it);
			msg->subject[len_it] = '\0';

		} else if (!strcmp(it,"Date")){
			it = strtok(NULL,"");
			len_it = strlen(it);
			msg->date = calloc(len_it+1, sizeof(char));
			memcpy(msg->date,it,len_it);
			msg->date[len_it] = '\0';

		} else if (!strcmp(it,"Msg")){
			msg->flag=1;
			it = strtok(NULL,"=");
			len_it = strlen(it);
			msg->msg = calloc(len_it+1, sizeof(char));
			memcpy(msg->msg,it,len_it);
			msg->msg[len_it] = '\0';
			msg->len = len_it;
		}
		free(str);
	}
    return len;
}


void init_session(const char *email, const char *password)
{
	if((curl = curl_easy_init())) {
		/* Set username and password */
		curl_easy_setopt(curl, CURLOPT_USERNAME, email);
		curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
	}
}

int pop_request(const char *url,const char *request)
{
	char *str = NULL;
	CURLcode res = CURLE_OK;

	str = calloc(strlen(url) + strlen(request) + 1, sizeof(char));
	if(!str) return -1;
	sprintf(str,"%s%s",url,request);
    curl_easy_setopt(curl, CURLOPT_URL, str);

    /* Perform the retr */
    res = curl_easy_perform(curl);

    /* Check for errors */
  	if(res != CURLE_OK)
      fprintf(stderr, "\n%s\n", curl_easy_strerror(res));

	free(str);

	return (int) res;
}

void close_session(void)
{
    curl_easy_cleanup(curl);
	curl_global_cleanup();
}

int inbox_messages(const char *pop3_server, int *n_messages)
{
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, inbox_number);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, n_messages);

	return (int) pop_request(pop3_server,"");

}

int get_message(int n_message, Msg **msg,const  char *pop3_server)
{	
	char n_message_str[MAX_NUM_LEN];
	*msg = init_msg();
	snprintf(n_message_str,MAX_NUM_LEN,"%d",n_message);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, parse_message);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, *msg);
	pop_request(pop3_server,n_message_str);

	return 0;
}

