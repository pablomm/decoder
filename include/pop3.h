
#ifndef POP3_H
#define POP3_H

#define GMAIL_POP3S "pop3s://pop.gmail.com:995/"
#define MAX_NUM_LEN 10

#ifdef __cplusplus

#include <stdlib.h>


extern "C" {
#endif

typedef struct _msg
{
	char *subject;
	char *date;
	char *msg;
	int flag;
	int len;
} Msg;

Msg *init_msg(void);

void free_msg(Msg *msg);

size_t inbox_number(void *ptr, size_t size, size_t nmemb, void *stream);

size_t parse_message(void *ptr, size_t size, size_t nmemb, void *stream);

void init_session(const char *email,const  char *password);

int pop_request(const char *url,const char *request);

void close_session(void);

int inbox_messages(const char *pop3_server, int *n_messages);

int get_message(int n_message, Msg **msg,const char *pop3_server);

#ifdef __cplusplus
}
#endif

#endif /* POP3_H */

