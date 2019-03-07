
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <curl/curl.h>
#include <unistd.h>

#define BUFSIZE   1024

#define QC_CLASSIFY_1   5
#define QC_CLASSIFY_2   10

//#define ADDRESS_TO_SEND   "http://www.personalsiter.com/c/index.php"
#define ADDRESS_TO_SEND   "http://pidder.int.ingv.it/qc/index.php"

//#define FILE_TO_PROCESS   "qc02.txt"


static char *trim ( char *s );
static int send_data(char * query_string);
static int parse_record (char * str);



char *trim ( char *s )
{
  int i = 0;
  int j = strlen(s) - 1;
  int k = 0;

  while ( isspace(s[i]) && s[i] != '\0' )
    i++;

  while ( isspace(s[j]) && j >= 0 )
    j--;

  while ( i <= j )
    s[k++] = s[i++];

  s[k] = '\0';

  return s;
}



int send_data(char * query_string)
{
	CURL *curl;
	CURLcode res;
	char str_to_send[200];

	printf("\n querystring: %s \n",query_string);
	curl = curl_easy_init();
	if(curl) {
		strcpy (str_to_send, ADDRESS_TO_SEND);
		strcat (str_to_send, query_string);
		curl_easy_setopt(curl, CURLOPT_URL, str_to_send);

	    //curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

		//curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1000);

		//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);  //Gets data to be written
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, wfp);  //Writes result

		res = curl_easy_perform(curl);

		/* Check errors */
		if(res != CURLE_OK)
			fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(res));
		else
			printf("\n curl OK \n");

		curl_easy_cleanup(curl);
	}
	return 0;
}

int parse_record (char * str)
{

	const char *stra[11] ;
	char * pch;

	char str_to_send[200];

	int i;

	pch = strtok (str,"[]_.");

	i = 0;
	while (pch != NULL)
	{
		stra[i] = trim(pch);
		pch = strtok (NULL, "[]_.");
		i++;
	}

	//if(atoi(stra[9]) > QC_CLASSIFY_1 || strcmp(stra[4],"GAP") == 0 )
	if(0 == 0 )
	{
		/* directory */
		strcpy (str_to_send, "?dir=");
		strcat (str_to_send, stra[0]);
		strcat (str_to_send, "/");
		strcat (str_to_send, stra[7]);
		strcat (str_to_send, "/");
		strcat (str_to_send, stra[5]);
		strcat (str_to_send, "/");
		strcat (str_to_send, stra[6]);
		strcat (str_to_send, ".D/");

		/* file */
		strcat (str_to_send, "&file=");
		strcat (str_to_send, stra[7]);
		strcat (str_to_send, ".");
		strcat (str_to_send, stra[5]);
		strcat (str_to_send, ".");
		strcat (str_to_send, stra[8]);
		strcat (str_to_send, ".");
		strcat (str_to_send, stra[6]);
		strcat (str_to_send, ".D");
		strcat (str_to_send, ".");
		strcat (str_to_send, stra[0]);
		strcat (str_to_send, ".");
		strcat (str_to_send, stra[1]);

		/* Quality meta */
		strcat (str_to_send, "&type=");
		strcat (str_to_send, stra[4]);
		strcat (str_to_send, "&tsec=");
		strcat (str_to_send, stra[9]);
		strcat (str_to_send, "&tmil=");
		strcat (str_to_send, stra[10]);
		strcat (str_to_send, "&ftime=");
		strcat (str_to_send, stra[2]);
		strcat (str_to_send, "&fmil=");
		strcat (str_to_send, stra[3]);

		/* Other meta */
		/*
		strcat (str_to_send, "&year=");
		strcat (str_to_send, stra[0]);
		strcat (str_to_send, "&doy=");
		strcat (str_to_send, stra[1]);
		strcat (str_to_send, "&sta=");
		strcat (str_to_send, stra[7]);
		strcat (str_to_send, "&cha=");
		strcat (str_to_send, stra[5]);
		strcat (str_to_send, "&net=");
		strcat (str_to_send, stra[8]);
		strcat (str_to_send, "&loc=");
		strcat (str_to_send, stra[6]);
	*/
		/* send */
		//printf("to send: %s \n",str_to_send);
		send_data(str_to_send);

	}


	return 0;
}


/*
 * pass the filename in argv1
 *
 */
int main (int argc, char *argv[])
{

	char str[BUFSIZE];
	FILE * file;
	fpos_t pos;

	file = fopen( argv[1] , "r");

	if (file) {

	    while (1)
	    {

			if (-1 == fgetpos(file, &pos)) {
				//fprintf(stderr,"Cannot read file: %s\n", argv[1]);
				printf("Cannot read file: %s\n", argv[1]);
				exit(EXIT_FAILURE);
			}

	    	if (feof(file)) {
	    		printf(" end of file found! at %i \n",&pos);
	    		fsetpos(file, &pos);
	    		//clearerr(file);

	    		// to remove remark for production
	    		sleep(3);
	    		clearerr(file);
	    		continue;

	    		// to remark for production
	    		//printf("\n***** eof found: sleep here for tail \n");
	    		//exit(EXIT_FAILURE);
	    	}
	    	else
	    	{
	    		fgets(str, BUFSIZE, file);
				printf(" file get: %s len: %d\n",str,strlen(str));

				if(strlen(str) >= 58)
				{

					parse_record (str);

				}



	    	}

	    	/*
	        fgets(str, BUFSIZE, file);
	        printf(" file get: %s \n",str);

	        parse_record (str);
	        */
	    }
	     fclose(file);
	}
	printf(" file not open! \n");

}




