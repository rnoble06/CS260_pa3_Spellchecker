#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256
/**********************************************************************/

// linked list -------------------------------------------------------
  // int value will be char word
typedef struct node
{
  char word[BUFSIZE];
  struct node *next;
}Node;

Node *initNode(char *word, Node *next)
{
  Node *newNode = malloc(sizeof(Node));
  strcpy(newNode->word, word);
  newNode->next = next;
  return newNode;
}

typedef struct linkedList
{
  Node *head;
}LinkedList;

LinkedList *init()
{
  LinkedList *newList = malloc(sizeof(LinkedList));
  newList->head = NULL;
  return newList;
}

void insertToHead(LinkedList *myList, char *word)
{
  Node *newNode=initNode(word, myList->head);
  myList->head = newNode;
}

Node *locate(LinkedList *myList, char *word)
{
  Node *ptr = myList->head;
  while(ptr!=NULL)
  {
    if(strcmp(word,ptr->word)==0)
      return ptr;
    else
      ptr = ptr->next;
  }
  return NULL;
}

void deleteFromHead(LinkedList *myList)
{
  if(myList->head!=NULL)
  {
    Node *ptr = myList->head;
    myList->head=myList->head->next;
    free(ptr);
  }
}

void deleteSublist(Node *mySublist)
{
  if(mySublist->next!=NULL)
  {
    deleteSublist(mySublist->next);
  }
  free(mySublist);
}

void deleteList(LinkedList *myList)
{
  if(myList->head!=NULL)
    {
      deleteSublist(myList->head);
    }
  myList->head=NULL;
}
void printList(LinkedList *myList)
{
  if(myList->head==NULL)
  {
    printf("empty list!!!\n");
    return;
  }
  Node *ptr = myList->head;
  while(ptr!=NULL)
  {
    printf("%s\n",ptr->word);
    ptr=ptr->next;
  }
}

// open hash----------------------------------------------------------
  // int value will be char word

int hashWord(char *word, int size)
{
  unsigned int total=0;
  for(int i=0; i<strlen(word); i++)
  {
    total = total + word[i];
    total = total * 101;
  }
  return (total % size);
}

typedef struct openHashTable
{
  int size;
  Node **table;
}OpenHashTable;

OpenHashTable *initHashTable(int size)
{
  OpenHashTable *hashTable = malloc(sizeof(OpenHashTable));
  hashTable->size=size;
  hashTable->table = malloc(sizeof(Node *)*size);
  for(int i=0; i<size; i++)
  {
    LinkedList *myList = init();
    hashTable->table[i] = myList->head;
  }
  return hashTable;
}

void insert(OpenHashTable *hashTable, char *word,int size)
{
  int position = hashWord(word,size);
  Node *ptr = hashTable->table[position];
  while(ptr!=NULL)
  {
    if(ptr->word == word)
      return;
    else
      ptr=ptr->next;
  }
  hashTable->table[position] = initNode(word, hashTable->table[position]);
}

// open hash isMember--------------------------------------------------

int isMember(OpenHashTable *hashTable, char *word, int size)
{
  int position = hashWord(word, size);
  Node *ptr = hashTable->table[position];
  while(ptr!=NULL)
  {
    if(!strcmp(word,ptr->word))
      return 1;
    else
      ptr = ptr->next;
  }
  return 0;
}

void printHashTable(OpenHashTable *hashTable, int hashSize)
{
for(int k=0; k < hashSize; k++)
  {
    printf("Row %d: [",k);
    Node *ptr = hashTable->table[k];
    while(ptr!=NULL)
    {
      printf(" %s",ptr->word);
      ptr=ptr->next;
    }
    printf(" ]\n");
  }
}

/**********************************************************************/
int main(int argc, char **argv)
{
	char *dictionaryFilePath = argv[1]; //this keeps the path to the dictionary file file
	char *inputFilePath = argv[2]; //this keeps the path to the input text file
	char *check = argv[3]; // this keeps the flag to whether we should insert mistyped words into dictionary or ignore
	int numOfWords=0; //this variable will tell us how much memory to allocate
  int hashSize=0;

	int insertToDictionary;
  if(strcmp(check,"add")==0)
		insertToDictionary = 1;
	else
		insertToDictionary = 0;
    
	////////////////////////////////////////////////////////////////////
	//read dictionary file
  FILE *fp = fopen(dictionaryFilePath, "r");
  char *line = NULL; //variable to be used for line counting
  size_t lineBuffSize = 0; //variable to be used for line counting
  ssize_t lineSize; //variable to be used for line counting

  //check if the file is accessible, just to make sure...
  if(fp == NULL)
  {
      fprintf(stderr, "Error opening file\n");
      exit(1);
  }

  //First, let's count number of words in the dictionary.
  //This will help us know how much memory to allocate for our hash table
  while((lineSize = getline(&line,&lineBuffSize,fp)) !=-1)
      numOfWords++;

  //Printing line count for debugging purposes.
  //You can remove this part from your submission.
  //printf("Number of words in words.txt %d\n",numOfWords);
  
  //HINT: You can initialize your hash table here, since you know the size of the dictionary

  /**********************************************************************/
  /**********************************************************************/

  hashSize = numOfWords + 7;
  OpenHashTable *myHash;
  myHash = initHashTable(hashSize);

  /**********************************************************************/
  /**********************************************************************/

  //rewind file pointer to the beginning of the file, to be able to read it line by line.
  fseek(fp, 0, SEEK_SET);

  char wrd[BUFSIZE];
  for (int i = 0; i < numOfWords; i++)
  {
    fscanf(fp, "%s \n", wrd);
    //You can print the words for Debug purposes, just to make sure you are loading the dictionary as intended
    //printf("%d: %s\n",i,wrd);
    
    //HINT: here is a good place to insert the words into your hash table

    /**********************************************************************/
    /**********************************************************************/

    insert(myHash, wrd, hashSize);

    /**********************************************************************/
    /**********************************************************************/
  }
  fclose(fp);

	////////////////////////////////////////////////////////////////////
	//read the input text file word by word
  fp = fopen(inputFilePath, "r");
	
	//check if the file is accessible, just to make sure...
	if(fp == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return -1;
	}

    //HINT: You can use a flag to indicate if there is a misspleed word or not, which is initially set to 1
	int noTypo=1;

	//read a line from the input file
	while((lineSize = getline(&line,&lineBuffSize,fp)) !=-1)
	{
		char *word;
    //These are the delimiters you are expected to check for. Nothing else is needed here.
		const char delimiter[]= " ,.:;!\n";

		//split the buffer by delimiters to read a single word
		word = strtok(line,delimiter); 

		//read the line word by word
		while(word!=NULL)
		{
      // You can print the words of the inpit file for Debug purposes, just to make sure you are loading the input text as intended
			//printf("%s\n",word);

      // HINT: Since this nested while loop will keep reading the input text word by word, here is a good place to check for misspelled words
                  
      // INPUT/OUTPUT SPECS: use the following line for printing a "word" that is misspelled.
      //printf("Misspelled word: %s\n",word);
                  
      // INPUT/OUTPUT SPECS: use the following line for printing suggestions, each of which will be separated by a comma and whitespace.
      //printf("Suggestions: "); //the suggested words should follow
      
      if(isMember(myHash, word, hashSize)!=1)
      {
        int i=0;
        char sugWord[strlen(word)-1];
        //-------------suggestions here-------------
        //-----------place each in a list-----------
        //--------print list for each suggestion------
        // create suggestionList
        LinkedList *suggestionList;
        suggestionList = init();
        // if extra at beginning
          // hash for 2 to n chars of word. exact match
          // if match add to list else continue
        while(word[i+1]!='\0')
        {
          sugWord[i]=word[i+1];
          i++;
        }
        sugWord[strlen(word)-1]=word[strlen(word)];
        sugWord[strlen(word)]='\0';

        if(isMember(myHash, sugWord, hashSize)==1)
        {
          insertToHead(suggestionList, sugWord);
        }
        // if extra at end
          // hash for 1 to n-1 chars of word. exact match
          // if match add to list else continue
        strncpy(sugWord,word,strlen(word)-1);
        sugWord[strlen(word)-1]='\0';
        if(isMember(myHash, sugWord, hashSize)==1)
        {
          insertToHead(suggestionList, sugWord);
        }
        char sugWordM1[strlen(word)+1];
        // if missing beginning
          // test for each case single letter a-z before word. exact match needed. may have more than 1
        for(int a=97; a<=122; a++)
        {
          // In front
          int i=0;
          sugWord[0]=a;
          while(word[i]!='\0')
          {
            sugWord[i+1]=word[i];
            i++;
          }
          sugWord[strlen(word)+1]='\0';
          if(isMember(myHash, sugWord, hashSize)==1)
          {
            insertToHead(suggestionList, sugWord);
          }

          // At end
          i=0;
          while(word[i]!='\0')
          {
            sugWord[i]=word[i];
            i++;
          }
          sugWord[strlen(word)]=a;
          sugWord[strlen(word)+1]='\0';
          if(isMember(myHash, sugWord, hashSize)==1)
          {
            insertToHead(suggestionList, sugWord);
          }
        }

        // if chars are swapped
          // hash should be the same, look for same # of chars and same chars in each. may have more than 1
        


        printf("Misspelled word: %s\n",word);
        printf("Suggestions: \n");
        noTypo=-1;

        // if add is specified
        if(insertToDictionary==1)
        {
          insert(myHash, word, hashSize);
        }
        //printList(suggestionList);
        // release suggestionList
        deleteList(suggestionList);
      }
			word = strtok(NULL,delimiter); 
		}
	} 

	fclose(fp);
    
  //HINT: If the flag noTypo is not altered (which you should do in the loop above if there exists a word not in the dictionary), then you should print "No typo!"
  if(noTypo==1)
    printf("No typo!\n");

  // DON'T FORGET to free the memory that you allocated
    
	return 0;
}
