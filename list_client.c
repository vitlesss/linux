#include <stdio.h>
#include <stdlib.h>

struct element
{
       char name[30];
       char symbol[3];
       int charge;
       float weight;
};

typedef struct element DataType;

struct List
{
       DataType data;
       struct List * next;
};

typedef struct List * list;

DataType input_element (void);
list read_file (char * filename);
list add (list, DataType);
int write_file (char * filename, list);
void delete_list (list);
void edit (list);
void show (list);
void search (list begin);
list del (list);

int main (void)
{
    char file[50];
    char menu;
    list elements = NULL;
    puts ("Enter the file name");
    gets (file);
    elements = read_file (file);
    do
    {
       system ("CLS");
       puts ("1. Add");
       puts ("2. Redact record");
       puts ("3. Show");
       puts ("4. Search max weight element");
       puts ("5. Delete record");
       puts ("6. Exit");
       menu = getchar(); getchar();
       switch (menu)
       {
              case '1': elements = add (elements, input_element()); break;
              case '2': edit (elements); break;
              case '3': show (elements); break;
              case '4': search (elements); break;
              case '5': elements = del (elements);
       }
    }
    while (menu!='6');
    if (write_file (file, elements))
        puts ("File saved");
    else
    	puts ("File not saved");
    delete_list (elements);
    return 0;
}

DataType input_element (void)
{
    DataType elementt;
    puts ("Name");
    gets (elementt.name);
    puts ("Symbol");
    gets (elementt.symbol);
    puts ("Charge");
    scanf ("%d", &elementt.charge);
    puts ("Weight");
    scanf ("%f", &elementt.weight);
	getchar();
    return elementt;
}

list add (list begin, DataType element)
{
    list temp = (list) malloc (sizeof(struct List));
    temp->data = element;
    temp->next = begin;
    return temp;
}

list read_file (char * filename)
{
    FILE * f;
    DataType elementt;
    list begin=NULL, cur;
    if ((f = fopen (filename, "rb")) == NULL)
    {
        perror ("Error open file");
        getchar();
        return begin;
    }
    if (fread(&elementt, sizeof(elementt), 1, f))
        begin = add (begin, elementt);
    else
        return NULL;
    cur = begin;
    while (fread(&elementt, sizeof(elementt), 1, f))
    {
        cur->next = add (cur->next, elementt);
        cur = cur->next;
	}
    fclose(f);
    return begin;
}

void delete_list (list begin)
{
    list temp = begin;
    while (temp)
    {
        begin = temp->next;
        free (temp);
        temp = begin;
    }
}

int write_file (char * filename, list begin)
{
    FILE * f;
    if ((f = fopen (filename, "wb")) == NULL)
    {
        perror ("Error create file");
        getchar();
        return 0;
    }
    while (begin)
    {
        if (fwrite (&begin->data, sizeof(DataType), 1, f))
            begin = begin->next;
        else
        	return 0;
    }
    return 1;
}

void print_data (struct element element)
{
    printf ("Name : %s\nSymbol : %s\nCharge : %d\nWeight : %.2f\n", element.name,
             element.symbol, element.charge, element.weight);
}

void edit (list cur)
{
     int n, k=1;
     char yes;
     system ("CLS");
     if (cur == NULL)
     {
          puts ("List is empty");
          getchar();
          return;
     }
     puts ("Number record for redact?");
     scanf ("%d", &n);
     getchar();
     if (n < 1)
     {
          puts ("Error");
          getchar();
          return;
     }
     while (cur && k < n)
     {
          cur = cur->next;
          k++;
     }
     if (cur == NULL)
     {
          puts ("Error");
          getchar();
          return;
     }
     print_data (cur->data);
     puts ("Redact? (y/n)");
     do
     {
          yes=getchar(); getchar();
	 }
	 while (yes!='y' && yes!='Y' && yes!='n' && yes!='N');
     if (yes=='y' || yes=='Y')
          cur->data = input_element();
}

void show (list cur)
{
     int k=0;
     system ("CLS");
     if (cur == NULL)
     {
          puts ("List is empty");
          getchar();
          return;
     }
     puts ("|  N |          Name                 |   Symbol   |  Charge |    Weight |");
     puts ("-------------------------------------------------------------------------");
     while (cur)
     {
        printf ("|%3d | %-29s |%11s |%8d |%10.2f |\n", ++k, cur->data.name,
                 cur->data.symbol, cur->data.charge, cur->data.weight);
        cur = cur->next;
     }
     puts ("-------------------------------------------------------------------------");
     getchar();
}

void search (list cur)
{
     float weight;
     int max_weight=0, k=-1;
     DataType elementt;
     system ("CLS");
     if (cur == NULL)
     {
	    puts ("List is empty");
	    getchar();
	    return;
     }
     puts ("Weight?");
     scanf ("%f",&weight);
     getchar();
     while (cur)
     {
        k++;
        if (cur->data.weight > weight && cur->data.weight > max_weight)
        {
           max_weight = cur->data.weight;
           elementt = cur->data;
        }
        cur = cur->next;
     }
     if (max_weight == 0) puts ("Not found");
     else print_data (elementt);
     getchar();
 }

list del (list begin)
{
     int n, k=1;
     char yes;
     list temp, temp1;
     system ("CLS");
     if (begin == NULL)
     {
          puts ("List is empty");
          getchar();
          return NULL;
     }
     puts ("Number record for delete?");
     scanf ("%d", &n);
     getchar();
     if (n < 1)
     {
          puts ("Error");
          getchar();
          return begin;
     }
     if (n == 1)
     {
          print_data (begin->data);
          puts ("Delete? (y/n)");
          do
          {
             yes=getchar(); getchar();
		  }
          while (yes!='y' && yes!='Y' && yes!='n' && yes!='N');
          if (yes=='y' || yes=='Y')
          {
               temp = begin->next;
               free (begin);
               return temp;
          }
          else return begin;
     }
     if (begin->next == NULL && n > 1)
     {
          puts ("Error");
          getchar();
          return begin;
     }
     temp = begin;
     temp1 = temp->next;
     while (temp1->next && k < n-1)
     {
          temp = temp1;
          temp1 = temp->next;
          k++;
     }
     if (k < n-1)
     {
          puts ("Error");
          getchar();
          return begin;
     }
     print_data (temp1->data);
     puts ("Delete? (y/n)");
     do
     {
          yes=getchar(); getchar();
	 }
     while (yes!='y' && yes!='Y' && yes!='n' && yes!='N');
     if (yes=='y' || yes=='Y')
     {
          temp->next = temp1->next;
          free (temp1);
     }
     return begin;
 }
