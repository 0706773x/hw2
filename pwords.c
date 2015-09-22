//PHUMUDZO VUSANI NELUHENI 0706773X
  //HW2- MULTI-THREADED DICTIONARY

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define MAXWORD 1024
#define numOfThreads 4  

typedef struct dict {
  char *word;
  int count;
  struct dict *next;
} dict_t;
  dict_t *d;

char *make_word( char *word ) {
  return strcpy( malloc( strlen( word )+1 ), word );
}

pthread_mutex_t lock;

dict_t *make_dict(char *word) {
  dict_t *nd = (dict_t *) malloc( sizeof(dict_t) );
  nd->word = make_word( word );
  nd->count = 1;
  nd->next = NULL;
  return nd;
}


dict_t *insert_word( dict_t *d, char *word ) {
  dict_t *nd;
  dict_t *pd = NULL;		// prior to insertion point 
  dict_t *di = d;		// following insertion point

  while(di && ( strcmp(word, di->word ) >= 0) ) { 
    if( strcmp( word, di->word ) == 0 ) { 
      di->count++;		 
      return d;		
    }
    pd = di;			// advance ptr pair
    di = di->next;
  }
  nd = make_dict(word);		// not found, make entry 
  nd->next = di;		// entry bigger than word or tail 
  if (pd) {
    pd->next = nd;
    return d;			// insert beond head 
  }
  return nd;
}

void print_dict(dict_t *d) {
  while (d) {
    printf("[%d] %s\n", d->count, d->word);
    d = d->next;
  }
}

int get_word( char *buf, int n, FILE *infile) {

  int inword = 0;
  int c;  
  while( (c = fgetc(infile)) != EOF ) {
    if (inword && !isalpha(c)) {
      buf[inword] = '\0';	// terminate the word string
      return 1;
    } 
    if (isalpha(c)) {
      buf[inword++] = c;
    }
  }
  return 0;		
}

void *words( FILE *infile ) {
  d = NULL;
  char wordbuf[MAXWORD];

  pthread_mutex_lock(&lock);
  int have_words = get_word( wordbuf, MAXWORD, infile );
  pthread_mutex_unlock(&lock);


  while( have_words ) {
	 pthread_mutex_lock(&lock);
 	// add word to dictinonary
    	d = insert_word(d, wordbuf); 
  	pthread_mutex_unlock(&lock);
	
	pthread_mutex_lock(&lock);
	have_words = get_word( wordbuf, MAXWORD, infile );
	pthread_mutex_unlock(&lock);
  }
}

int main( int argc, char *argv[] ) {

pthread_t threads[numOfThreads];

	if(pthread_mutex_init(&lock,NULL)!=0)
	{
		printf("lock failed");
	}
	
  d = NULL;
  FILE *infile = stdin;
  
  	if (argc >= 2) {
    		infile = fopen (argv[1],"r");
  	}
  	if( !infile ) {
    		printf("Unable to open %s\n",argv[1]);
    		exit( EXIT_FAILURE );
  	}

int k;
for( k = 0; k <= numOfThreads; k++ ){
	pthread_create( &threads[k], NULL,&words, infile );
}

int j;
for( j = 0; j <= numOfThreads; j++ ){
	pthread_join( threads[j], NULL );
}
	print_dict(d);
	pthread_mutex_destroy(&lock);
  	fclose( infile );
}

//PHUMUDZO VUSANI NELUHENI 0706773X
//HW2- MULTI-THREADED DICTIONARY
