/* <DESC>
 * Very simple HTTP GET
 * </DESC>
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <curl/curl.h>
#include <time.h>
#include <unistd.h>

//#define DEBUG 1
//#define EXAMPLE 1

struct request {
    char *event_input;
    size_t event_length;
    int numHeaders;
    char *headers[50];
    char *headerValues[50];
} request1,request2;

char handlerFilename[300];
char handlerFunction[300];


int getNextRequest();
char *getheader(const char* headerName,struct request *r);
int sendResponse(char *invocationId,char *response);
void debugPrintf(const char* format);
void cleanupRequest(struct request *s);
char *callphp(char *event);


//int main2(int argc, char **argv, char **envp)
//{
//  for (char **env = envp; *env != 0; env++)
//  {
//    char *thisEnv = *env;
//    printf("%s\n", thisEnv);
//  }
//  return 0;
//}

//
//void
//write_data (FILE * stream)
//{
//  int i;
//
////  for (i = 0; i < 100; i++)
////    fprintf (stream, "%d\n", i);
//  fprintf(stream,"<?php \n");
//  fprintf(stream,"$object=json_decode('{\"hello\":\"world\"}',true);\n");
//  fprintf(stream,"print_r($object);\n");
//  fprintf(stream,"echo \"hello\";\n");
//
//  if (ferror (stream))
//    {
//      fprintf (stderr, "Output to stream failed.\n");
//      exit (EXIT_FAILURE);
//    }
//}
//
//int php(const char *filename,const char *function,const char *event)
//{
//    FILE *fp;
//    char buffer[3000];
//
//    printf("EVENT: %s\n",event);
//    fp = popen ("echo '<?php echo \"hello\";' | /opt/bin/php-bin", "r");
//    if (fp == NULL) {
//        fprintf (stderr, "incorrect parameters or too many files.\n");
//        return EXIT_FAILURE;
//    }
//
//    //      write_data (fp);
//    while (fgets(buffer,3000,fp) != NULL) {
//        printf("PHP: [%s]",buffer);
//    }
//
//    if (pclose (fp) != 0) {
//        fprintf (stderr,"Could not run.\n");
//    }
//    return EXIT_SUCCESS;
//}

#define BILLION 1000000000L
double tick_start_time_seconds = 0.0;

double tick_now() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t);
/*
    CLOCK_REALTIME - clock on the wall time (adjusted by ntp)
    CLOCK_MONOTONIC - time since last boot (affected by ntp)
    CLOCK_MONOTONIC_RAW  - time since last boot (not affected by ntp)
    CLOCK_BOOTTIME - time includes time when hardware was suspended. (affected by ntp)
    CLOCK_PROCESS_CPUTIME_ID - time consumed by CPU (so doesn't include sleep time)
*/
//    printf("t.tv_sec  =%10ld\n",t.tv_sec);
//    printf("t.tv_nsec =%10ld\n",t.tv_nsec);
    double d = ((double)t.tv_sec + (double)((double)t.tv_nsec/(double)BILLION)) * (1000.0);
//    printf("d         =%10.8lf\n",d);
//    printf("fraction  =%10.8lf\n",(double)((double)t.tv_nsec/(double)BILLION)  );
    return d;
}
void tick_start()
{
    tick_start_time_seconds = tick_now();
}
double tick_since()
{
    double now = tick_now();
    return now-tick_start_time_seconds;
}

double showTicks()
{
    double d = tick_since();
    printf("TICK_COUNT: %lf\n",d);
    return d;
}

int main(int argc, char **argv, char **envp)
{
    int i,VERSION;
    char *ptr,*token;
    char buffer[300];
    char requestID[40];

	uint64_t diff;
	struct timespec start, end;
	double t1,t2;

//    t1=time(NULL);
//    double tk1,tk2;
	/* measure monotonic time */
//	tk1=tick_now();
//	tick_start();
//	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);	/* mark start time */
//	sleep(1);	/* do stuff */
//	showTicks();
//	tk2=tick_now();
//	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);	/* mark the end time */
//    t2=time(NULL);


//
//	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
//	printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) diff);

//    printf("time: \nt1=%10ld, \nt2=%10ld\n  =%10ld\n",t1,t2,t2-t1);

//    exit(1);

    for (i=0;i<50;i++) {
        request1.headers[i]=NULL;
        request1.headerValues[i]=NULL;
        request2.headers[i]=NULL;
        request2.headerValues[i]=NULL;
    }
    request1.event_input=NULL;
    request1.event_length=0;
    request1.numHeaders=0;
    request2.event_input=NULL;
    request2.event_length=0;
    request2.numHeaders=0;
    FILE *f;
    f = fopen("src/VERSION","rt");
    if (f) {
        fgets(buffer,200,f);
        VERSION = atoi(buffer);
        fclose(f);
    }

    for (char **env = envp; *env != 0; env++) {
        char *thisEnv = *env;
        printf("ENV: %s\n", thisEnv);
    }

    ptr=getenv("_HANDLER");
    if (ptr == NULL) {
        printf("_HANDLER env not found\n");
        return 1;
    } else {
        fprintf(stderr,"_HANDLER: %s\n",ptr);
        token=strtok(ptr,".");
        sprintf(handlerFilename,"%s.php",token);
        strcpy(handlerFunction,strtok(NULL,"."));
        printf("HANDLER_FILENAME: %s\n",handlerFilename);
        printf("HANDLER_FUNCTION: %s\n",handlerFunction);
    }

    char *response;
//
//    char *response = callphp("{\"hel'lo\":\"world\"}");
//
//    printf("RESPONSE: %s\n",response);
//    free(response);

//    php("filename","function","{\"hel'lo\":\"world\"}");


//    return 1;

    while (1) {
        tick_start();
        getNextRequest();
        printf("GET_REQUEST_");showTicks();

        ptr=getheader("Lambda-Runtime-Aws-Request-Id",&request1);
        if (ptr != NULL) {
            strcpy(requestID,ptr);
        } else {
            printf("REQUEST_ID: NULL\n");
            return 1;
        }

        printf("BEFORE_PHP_");t1=showTicks();
        response = callphp(request1.event_input);
        printf("AFTER_PHP_");t2=showTicks();
        printf("PHP_TOTAL: %lf\n",t2-t1);

        printf("RESPONSE: %s\n",response);
//        free(response);


//        if (requestID != NULL) {
//            printf("REQUEST_ID: %s\n",requestID);
//        } else {
//            printf("REQUEST_ID: NULL\n");
//            return 1;
//        }



//        printf("------\n");
//        char *home = getenv("HOME");
//        if (home != NULL) {
//            printf("HOME:%s\n",home);
//        } else {
//            printf("HOME:null\n",home);
//        }

    //    Lambda-Runtime-Aws-Request-Id

        sprintf(buffer,"{\"hello\":\"world\",\"time\":%d,\"version\":%d}",time(NULL),VERSION);
//        printf("%s\n",buffer);



        printf("BEFORE_RESPONSE_");showTicks();
        sendResponse(requestID,response);
        printf("AFTER_RESPONSE_");showTicks();
        free(response);

        cleanupRequest(&request1);
        cleanupRequest(&request2);
        fflush(stdout);

        printf("END_OF_LINE_");showTicks();
    }
    return 0;
}

char *getheader(const char* headerName,struct request *r) {
    int i;
    for (i=0;i<r->numHeaders;i++) {
        if (strncasecmp(r->headers[i],headerName,strlen(headerName)) == 0) {
            return r->headerValues[i];
        }
    }
    return NULL;
}

/*
HEADER: HTTP/1.1 200 OK
HEADER: Age: 482346
HEADER: Cache-Control: max-age=604800
HEADER: Content-Type: text/html; charset=UTF-8
HEADER: Date: Wed, 11 Nov 2020 22:11:22 GMT
HEADER: Etag: "3147526947+ident"
HEADER: Expires: Wed, 18 Nov 2020 22:11:22 GMT
HEADER: Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT
HEADER: Server: ECS (oxr/8305)
HEADER: Vary: Accept-Encoding
HEADER: X-Cache: HIT
HEADER: Content-Length: 1256
HEADER:
*/

static size_t header_callback(char *buffer, size_t size,
                              size_t nitems, void *userdata)
{
    /* received header is nitems * size long in 'buffer' NOT ZERO TERMINATED */
    /* 'userdata' is set with CURLOPT_HEADERDATA */

    int i,len;
    struct request *mem = (struct request *)userdata;

//    printf("last few digits: %d %d %d\n",buffer[size*nitems-1],buffer[size*nitems-2],buffer[size*nitems-3]);
    char *tmp_buffer = malloc(size*nitems+1);

    memcpy(tmp_buffer,buffer,size*nitems);
    i=size*nitems-1; // this should be = 10
    while (tmp_buffer[i]==10 || tmp_buffer[i]==13) {
        i-=1;
    };
    tmp_buffer[i+1] = 0;
    len = strlen(tmp_buffer);

//    printf("CALLBACK: %s\n",tmp_buffer);
//Cache-Control: max-age=604800
    for (i=0;i<size*nitems;i++) { // look for the ":" in the header string... example: Cache-Control: max-age=604800
        if (buffer[i] == ':') {
            int sizeA = i;
            int sizeB = strlen(tmp_buffer) - (sizeA+2); // remove the colon/space ": "

//            printf("  i     = %ld\n",i);
//            printf("  size  = %ld\n",strlen(tmp_buffer));
//            printf("  sizeA = %ld\n",sizeA);
//            printf("  sizeB = %ld\n",sizeB);

//            h[numHeaders] = malloc(len+1);
//            strcpy(h[numHeaders],tmp_buffer);

            tmp_buffer[i]=0; // zero terminate at the colon

            mem->headers[mem->numHeaders] = malloc(sizeA+1); // allocate space for the header name
            strcpy(mem->headers[mem->numHeaders],tmp_buffer);

            mem->headerValues[mem->numHeaders] = malloc(sizeB+1); // allocate space for the header name
            strcpy(mem->headerValues[mem->numHeaders],&tmp_buffer[i+2]);

//            printf("  buf    = %s\n",tmp_buffer); //h[numHeaders]);
//            printf("  header = %s\n",headers[numHeaders]);
//            printf("  value  = %s\n",headerValues[numHeaders]);

            mem->numHeaders++;
            i = size*nitems;
        }
    }
    free(tmp_buffer);

////  printf("debug1\n");
//  headers[numHeaders] = malloc(size*nitems);
////  printf("debug2\n");
//  memcpy(headers[numHeaders],buffer,size*nitems);
////  printf("debug3\n");
////  printf("callback_debug2\n");
//  headers[numHeaders][size*nitems-1] = 0;
//  printf("debug4\n");

//  printf("callback_debug1\n");
//  memcpy(header,buffer,size*nitems);
//  printf("callback_debug2\n");
//  header[size*nitems-1] = 0;
//  printf("callback_debug3\n");
//    printf("HEADER: [%ld/%ld/%ld]: %s\n",nitems,size,nitems*size,headers[numHeaders]);
//  printf("callback_debug4\n");
//    numHeaders++;
    return nitems * size;
}




static size_t curl_write_function(void *data, size_t size, size_t nmemb, void *userp)
{
    debugPrintf("curl_write_debug1\n");
    size_t realsize = size * nmemb;
    struct request *mem = (struct request *)userp;
    debugPrintf("curl_write_debug2\n");

    char *ptr = realloc(mem->event_input, mem->event_length + realsize + 1);
    debugPrintf("curl_write_debug3\n");
    if(ptr == NULL) {
        debugPrintf("curl_write_outofmemory\n");
        return 0;  /* out of memory! */
    }
    debugPrintf("curl_write_debug4\n");

    mem->event_input = ptr;
    debugPrintf("curl_write_debug5\n");
    memcpy(&(mem->event_input[mem->event_length]), data, realsize);
    debugPrintf("curl_write_debug6\n");
    mem->event_length += realsize;
    debugPrintf("curl_write_debug7\n");
    mem->event_input[mem->event_length] = 0;

    debugPrintf("curl_write_debug99\n");
    return realsize;
}

int getNextRequest() {
    CURL *curl;
    CURLcode res;
    char url[1000];

#ifndef EXAMPLE
    char *runtimeAPI = getenv("AWS_LAMBDA_RUNTIME_API");
    if (runtimeAPI == NULL) {
        printf("environment variable: AWS_LAMBDA_RUNTIME_API doesn't exist.\n");
        return 1;
    }

    sprintf(url,"http://%s/2018-06-01/runtime/invocation/next",runtimeAPI);
#else
    sprintf(url,"https://example.com");
#endif
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&request1);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_function);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&request1);

//        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//        curl_easy_setopt(curl, CURLOPT_HEADER, 1L);dl

        /* Perform the request, res will get the return code */
        debugPrintf("curl_debug1\n");
        res = curl_easy_perform(curl);
        debugPrintf("curl_debug2\n");

        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
        printf("-----------\nEVENT: %s\n-----------\n",request1.event_input);
    }
}


int sendResponse(char *invocationId,char *response) {
    CURL *curl;
    CURLcode res;
    char url[1000];
    char *runtimeAPI = getenv("AWS_LAMBDA_RUNTIME_API");
    if (runtimeAPI == NULL) {
        printf("sendResponse(): environment variable: AWS_LAMBDA_RUNTIME_API doesn't exist.\n");
        return 1;
    }

    sprintf(url,"http://%s/2018-06-01/runtime/invocation/%s/response",runtimeAPI,invocationId);

//    printf("RESPONSE_URL: %s\n",url);

//    sprintf(url,"https://example.com");
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_function);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&request2);

//        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
//        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//        curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}

void debugPrintf(const char* format) {
#ifdef DEBUG
    printf(format);
#endif
}


void cleanupRequest(struct request *s) {
    int i;
    debugPrintf("cleanup_debug1\n");
    for (i=0;i<s->numHeaders;i++) {
        free(s->headers[i]);
        free(s->headerValues[i]);
        s->headers[i]=NULL;
        s->headerValues[i]=NULL;
    }
    debugPrintf("cleanup_debug2\n");
    s->numHeaders=0;
    debugPrintf("cleanup_debug3\n");
    if (s->event_input != NULL) {
        s->event_length=0;
        free(s->event_input);
        s->event_input=NULL;
    }
    debugPrintf("cleanup_debug4\n");
}

/*
HTTP/1.1 200 OK
Content-Type: application/json
Lambda-Runtime-Aws-Request-Id: 8489a0d1-7c6a-41fc-8e0e-47769198577e
Lambda-Runtime-Deadline-Ms: 1605129957906
Lambda-Runtime-Invoked-Function-Arn: arn:aws:lambda:us-west-2:686403235852:function:php-example-bootstrap
Lambda-Runtime-Trace-Id: Root=1-5fac56e2-1bd7bfc32f57f2826844aea0;Parent=621f262d6a28b8e0;Sampled=0
Date: Wed, 11 Nov 2020 21:25:54 GMT
Content-Length: 49

{"key1":"value1","key2":"value2","key3":"value3"}
*/

/*
ENV: LAMBDA_RUNTIME_DIR=/var/runtime
ENV: AWS_LAMBDA_FUNCTION_MEMORY_SIZE=128
ENV: AWS_DEFAULT_REGION=us-west-2
ENV: PATH=/usr/local/bin:/usr/bin/:/bin:/opt/bin
ENV: LANG=en_US.UTF-8
ENV: AWS_ACCESS_KEY_ID=ASIAZ7UGSGAGJ5UE53MU
ENV: AWS_SESSION_TOKEN=IQoJb3JpZ2luX2VjEC0aCXVzLXdlc3QtMiJGMEQCIHlOERZevS+CqIeJeeQmKaa+yCtcAFn8K59TX81Xa53LAiBTiZH294YpxzltaIXc25jZ3IMy2pinDBakXqGoMCfcoirdAQiW//////////8BEAAaDDY4NjQwMzIzNTg1MiIMgx1Hd5gUZm1tiQA6KrEBcsxsRaZxFQlIqJWGN85tlZ9WJVCxXyf+b9PKOmEo2KvqEtPC6PHoWMWP6OpmvSj7T5eWlF3KEb7JuQPWrllRJLbHYC75leO1VbN7WItij/yHBvZakJ7HcqnieN9F/NRVAG0WlO4vtzT+c4bEw1bfCDC1c/2MprbT21co2HK+s+kBq64Z1PGZZRwczKJY8jNxP2PmU8KRrtLaOWV0f5VoQJnoXqxYDUI9zyNXE9f7nhRiMJGisf0FOuEBj9a0RzZuZGlgYG0WLJMJkmc710s2j0TNtMVPlPqVNCghvG5rPJdjzyOmH5GfrxjZlcSuRPdAu/rty7i8iIS3LEVC0WCi1QSEub4yQ1rJUY4HBdYeSF8utT1kJ6o6sjeGeGkmaE3pj2yYbsG64RDgbjC1Di7u+BeUadIQCjhOygzTclpCOdD6SFUXZyyR19lR9VU8qbw2QU5SOrs4g2KvYqDSaT/HfDZ+vJtp92TtcZ/wt7JIaaE6n/zUZgjMF8yOtrFhxAMCvQbUFWH15qw2OIpEBMS9pkuUlBzFV2GKP0WQ
ENV: LAMBDA_TASK_ROOT=/var/task
ENV: _AWS_XRAY_DAEMON_PORT=2000
ENV: AWS_LAMBDA_LOG_STREAM_NAME=2020/11/11/[$LATEST]44899547c916421b9d67ec21a62777bd
ENV: AWS_REGION=us-west-2
ENV: _AWS_XRAY_DAEMON_ADDRESS=169.254.79.2
ENV: AWS_SECRET_ACCESS_KEY=59KQC3sKI5dfEuAJE+E4/+ccD3cUMRMXhaupAdiP
ENV: AWS_LAMBDA_LOG_GROUP_NAME=/aws/lambda/php-example-bootstrap
ENV: AWS_LAMBDA_FUNCTION_NAME=php-example-bootstrap
ENV: AWS_LAMBDA_RUNTIME_API=127.0.0.1:9001
ENV: TZ=:UTC
ENV: LD_LIBRARY_PATH=/lib64:/usr/lib64:/var/runtime:/var/runtime/lib:/var/task:/var/task/lib:/opt/lib
ENV: AWS_XRAY_CONTEXT_MISSING=LOG_ERROR
ENV: AWS_XRAY_DAEMON_ADDRESS=169.254.79.2:2000
ENV: _HANDLER=main.handler
ENV: AWS_LAMBDA_FUNCTION_VERSION=$LATEST
 */



char *escapeString(char *string)
{
    int len=strlen(string),i,n;
    char *esacapedString = malloc(len*2);
    for (i=0,n=0;i<len;i++) {
        switch (string[i]) { // escape ' and \
            case '{':
            case '\\':
            case '\'':
                esacapedString[n++]='\\';
                break;
        }
        esacapedString[n++]=string[i];
    }
    esacapedString[n]=0;
    return esacapedString;
}

char *escapeCMD(char *string)
{
    int len=strlen(string),i,n;
    char *esacapedString = malloc(len*2);
    for (i=0,n=0;i<len;i++) {
//        printf("%d:%c\n",string[i],string[i]);
        switch (string[i]) { // escape ' and \
            case 10:
            case '\n':
//                esacapedString[n++]='\\';
//                esacapedString[n++]='n';
                break;
            case '\\':
            case '$':
            case '"':
                esacapedString[n++]='\\';
                esacapedString[n++]=string[i];
                break;
            default:
                esacapedString[n++]=string[i];
                break;
        }
    }
    esacapedString[n]=0;
    return esacapedString;
}

char *callphp(char *event) {
    FILE *fp;
    char cmd[300];
    char buffer[3000];
    int count,len;
    char template[120] =
        "<?php \n"
        "$_EVENT=json_decode('%s',true);\n"
        "require 'src/%s';\n"
        "$rv=%s($_EVENT);\n"
        "echo \"\\n-=-=-=-=RV=-=-=-=-\\n\";\n"
        "echo \"$rv\\n\";\n";

//    printf("TEMPLATE: \n%s\n==============\n",template);


    char *eventEsacaped = escapeString(event);
    sprintf(cmd,template,eventEsacaped,handlerFilename,handlerFunction);
//    sprintf(cmd,"echo $'<?php $_EVENT = json_decode(\\\'%s\\\',true);print_r($_EVENT);' | /opt/bin/php-bin",eventEsacaped);
//    printf("CMD: %s\n",cmd);

//    fp=fopen("tmp9999.php","w");
//    if (fp) {
//        fprintf(fp,cmd);
//        fclose(fp);
//        system("cat tmp9999.php");
//    }

    char *escapedCmd = escapeCMD(cmd);
    char *callPHP = malloc(strlen(escapedCmd)+100);
    sprintf(callPHP,"echo \"%s\" | /opt/bin/php-bin",escapedCmd);
//    sprintf(callPHP,"/opt/bin/php-bin --version");
    printf("===========\n%s\n",callPHP);

    char *response=NULL;
    size_t response_size=0;
    int in_response=0;

    fp = popen(callPHP,"r");
    if (fp) {
        while (fgets(buffer,300,fp) != NULL) {
            if (in_response) {
                len = strlen(buffer);
                response = realloc(response,response_size+len+1);
                if (response == NULL) {
                    fprintf(stderr,"Out of Memory\n");
                    exit(1);
                }
                sprintf(&response[response_size],"%s",buffer);
                response_size+=len;
//                response[response_size] = 0; // zero terminate
            } else {
                if (strstr(buffer,"-=-=-=-=RV=-=-=-=-") != NULL) {
                    in_response = 1;
                } else {
                    printf("%s",buffer);
                }
            }
        }
        fclose(fp);
    }

    free(eventEsacaped);
    free(escapedCmd);
    free(callPHP);
    return response;
}


//echo -e '<?php \n$_EVENT=json_decode('{"hel\'lo":"world"}',true);\nrequire 'main.php';\n$rv=handler($_EVENT);\necho "\n-=-=-=-=RV=-=-=-=-\n";\necho "$rv\n";'

