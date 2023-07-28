#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
    //node array of linked lists
    struct node* hashArray;
    int size;

};

//adding in node structure for next and record
struct node{
    struct RecordType record;
    struct node* next;
};

// Compute the hash function
int hash(int x, int hashSz)
{
    return x%hashSz;

}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen("lab9.inputtxt.txt.txt", "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
        struct node* current = pHashArray->hashArray[i].next;
        if(current != NULL)
        {
            printf("index %d -> ", i);
            while(current != NULL)
            {
                printf("%d %c %d", current->record.id,current->record.name, current->record.order);
                current = current->next;
                if(current != NULL)
                {
                    printf(" -> ");
                }
                
            }
            printf("\n");

        }
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("lab9.inputtxt.txt.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation

    //create hash table
    int hashSz = 2*recordSz;
    struct HashType hashTable;
    hashTable.hashArray = (struct node *)malloc(sizeof(struct node)*hashSz);
    if(hashTable.hashArray == NULL)
    {
        printf("Cannot allocate memory for the hash table.\n");
        exit(-1);
    }
    hashTable.size = 15;

    //Initializing the hash table
    int i;
    for(i = 0; i < 15; i++)
    {
        struct RecordType *record = &pRecords[i];
        int index = hash(record->id, hashSz);
        struct node* newNode = (struct node*)malloc(sizeof(struct node));
        if (newNode == NULL)
        {
            printf("Cannot allocate memory for a new node\n");
            exit(-1);
        }
        newNode->record = *record;
        newNode->next = hashTable.hashArray[index].next;
        hashTable.hashArray[index].next = newNode;
    }

    // Displaying records in the hash structure
    displayRecordsInHash(&hashTable, hashSz);

    // Cleanup - free the memory used by the hash table
    for (i = 0; i < hashSz; ++i)
    {
        struct node* currNode = hashTable.hashArray[i].next;
        while (currNode != NULL)
        {
            struct node* temp = currNode;
            currNode = currNode->next;
            free(temp);
        }
    }

    free(hashTable.hashArray);
    free(pRecords);
    return 0;
}
        
    
