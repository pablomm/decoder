
#include "interface.h"
#include "pop3.h"
#include "decode.h"
#include "keys.h"

#include <getopt.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <termios.h>
#include <fstream>

#define BUFFER_LEN 120

using namespace std;

void sig_handler(int sig){
	restore_termios(stdin);
	close_session();
	exit(EXIT_FAILURE);
}

int proccess_message(int n_message){
	Msg *msg = NULL;
	
	get_message(n_message,&msg, GMAIL_POP3S);
	
	if(msg && msg->flag) {

		string subject;

		try {
			subject = Decode::DecryptB64(std::string(msg->subject));
		} catch(std::exception const & ex){
			subject = std::string(msg->subject);
		}

		string message;
		try {
			message = Decode::DecryptB64(std::string(msg->msg));
			message = KEYS::readable_string(message);
		} catch(std::exception const & ex){
			message = std::string(msg->msg);
		}

		ofstream file;
		file.open(subject, ofstream::app);

		file << "[" << string(msg->date) << "]" << endl << message << endl << endl;
		file.close();
	}

	free_msg(msg);
	return 0;
}

int main(int argc, char *argv[])
{
	char c; 
	std::string email;
	char *password = NULL;
	char buffer[BUFFER_LEN];
	int n_messages = 0, i;
	size_t n = BUFFER_LEN;

	static struct option long_options[] =
        { {"email", required_argument, 0, 'e'},
          {"password", required_argument, 0, 'p'}};

	while ((c = getopt_long (argc, argv, "e:p:h", long_options, &i)) != -1){
		switch (c) {
			case 'e':
				email = string(optarg);
				break;
			case 'p':
				n = strlen(optarg) +1;
				password = (char*) calloc(n , sizeof(char));
				memcpy(password,optarg, n);
				break;
			default:
				return 1;
		}
	}

	if(email.empty()){
        cout << "Gmail account: ";
		getline(cin, email);
	}

	signal(SIGINT, sig_handler);

	if(!password){
		cout << "Password: ";
		getpass_custom (&password, &n, stdin);
	}

	/* Setting session */
	init_session(email.c_str(),password);

	memset(password,0, strlen(password));
	free(password);

	progress_m(0, 1, "Login account");

	if(inbox_messages(GMAIL_POP3S, &n_messages)){
		close_session();
		return 1;
	}


	if(!n_messages){
		progress_m(1, 1, "Completed");
		cout << endl << "The inbox is empty" << endl;
		close_session();
		return 0;
	}
	for(i=1; i <= n_messages; i++){
		snprintf(buffer,BUFFER_LEN, "Processing message %d/%d",i,n_messages);
		progress_m(i-1, n_messages, buffer);
		proccess_message(i);

	}
	
	snprintf(buffer,BUFFER_LEN, "Completed %d/%d",n_messages,n_messages);
	progress_m(n_messages, n_messages, buffer);
	cout << endl;

	close_session();
	return 0;
}

