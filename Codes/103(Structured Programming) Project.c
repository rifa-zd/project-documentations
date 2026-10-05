//header files
#include<stdio.h>
#include<string.h>
#include<stdlib.h>//needed for exit()
#define MAX 11
/* Simple Phone directory */
//these define is used so that in future if needed
//can change the sizes easily rather than finding in all over the code
typedef struct phonebook
{
    //done
    int no;
    char name[40];
    char number[11];//number would be 10 digits
    char mail[30];
    int code;
} book; //with this can declare in functions


void store()
{
    //done
    book *p;//as don't know the amount of input
    int n, i;

    FILE *fp=NULL;
    fp = fopen("DIRECTORY.txt", "a+");
    if (fp == NULL)
    {
        printf("!!Something went wrong!!\n");
        exit (1);//terminates program
    }

    printf("Enter the amount of input: ");
    scanf("%d", &n);

    p = (book*)calloc(n, sizeof(book));
//calloc used to allocated memory of *p, as it has multiple blocks

    for (i=0; i<n; i++)
    {
        p[i].no = i ;
        printf("\nEnter name: ");
        fflush(stdin);//to clear/flush the buffer of output.
        gets(p[i].name);

        printf("Enter telephone number: ");
        fflush(stdin);
        gets(p[i].number);

        printf("Enter mail address: ");
        fflush(stdin);
        gets(p[i].mail);

        printf("Enter area code: ");
        fflush(stdin);
        scanf("%d", &p[i].code);

//printing the inputs in the file
        fprintf(fp, "%d %s %s %s %d\n", p[i+1].no, p[i].name, p[i].number, p[i].mail, p[i].code);
    }

    printf("\n You have entered %d contact details.\n", i);

    fclose(fp);
}


void load()
{
    //done
    book p;

    FILE *fp=NULL, *fp2=NULL;
    char ch;
    fp = fopen("DIRECTORY.txt", "r");
    if (fp == NULL)
    {
        printf("!!Something went wrong!!\n");
        exit (1);
    }

    fp2 = fopen("Copy of DIRECTORY.txt", "w+");
    if (fp2 == NULL)
    {
        printf("!!Something went wrong!!\n");
        exit (1);
    }

    while((ch = fgetc(fp))!= EOF)
        putc(ch, fp2);

    fclose(fp);
    fclose(fp2);
    printf("\n");
    printf("\t*Loading DONE*\n");
}


void Insert()
{
    //done
    book p[MAX];
    char ch;

    int serial, count_line=1;

    FILE *fp;
    fp = fopen("DIRECTORY.txt", "a+");
    if (fp == NULL)
    {
        printf("!!Something went wrong!!\n");
        exit (1);//terminates program
    }

    while((ch=fgetc(fp))!= EOF)
    {
        if(ch == '\n')
            count_line++;
    }
    serial = count_line-1;
//lines are initialized 1 as even the cursor is at the begining of the
//files show 1 line
    for (int i=0; ; i++)
    {
        if(serial==MAX)
        {
            printf("\n\t!!!Directory Full!!! Delete some data to insert more\n\n");
            break;
        }
        serial++;
        printf("%d.", serial);

        printf("\nEnter name: ");
        fflush(stdin);
        gets(p[i].name);

        printf("Enter telephone number: ");
        fflush(stdin);
        gets(p[i].number);

        printf("Enter mail address: ");
        fflush(stdin);
        gets(p[i].mail);

        printf("Enter area code: ");
        fflush(stdin);
        scanf("%d", &p[i].code);

        //printing the inputs in the file
        fprintf(fp, "%d %s %s %s %d\n", serial, p[i].name, p[i].number, p[i].mail, p[i].code);
        /*fprintf(fp, "%d. %s\n", serial, people[i].name);
        fprintf(fp, "\t%s\n", people[i].number);
        fprintf(fp, "\t%s\n", people[i].mail);
        fprintf(fp, "\t%d\n", people[i].code);*/

        printf("\n\tInsertion Complete.\n");
        break;

    }
    fclose(fp);
}


void Delete()
{
    book p1;
    char ch;
    int n, serial, found=0, look ;

    FILE *fp=NULL, *fp1=NULL;
    fp = fopen("DIRECTORY.txt", "r");
    if (fp == NULL)
    {
        printf("!!Something went wrong!!\n");
        exit (1);
    }

    fp1 = fopen("TEMPORARY.txt", "r");
    if (fp1 == NULL)
    {
        printf("!!Something went wrong!!\n");
        exit (1);
    }

    printf("\nEnter the serial want to delete: ");
    scanf("%d", &look);

    //printf("%s\n, p.name");
    while(fread(&p1, sizeof(book), 1, fp))
    {
        if(p1.no == look)
        {
            found = 1;
        }
        else
            fwrite(&p1, sizeof(book), 1, fp1);
    }
    fclose(fp);
    fclose(fp1);
    if(found == 0)
        fp1 = fopen("TEMPORARY.txt", "r");
    fp = fopen("DIRECTORY.txt", "w+");

    while(fread(&p1, sizeof(book), 1, fp1))
        fwrite(&p1, sizeof(book), 1, fp);

    fclose(fp);
    fclose(fp1);
}


void show()
{
    //done
    book p;
    char ch;

    FILE *fp = NULL;
    fp = fopen("DIRECTORY.txt", "r");
    if (fp == NULL)
    {
        printf("!!Something went wrong!!\n");
        exit (1);
    }

    printf("\n");
    while((ch=fgetc(fp)) != EOF)
    {
        putchar(ch);
    }

    /*not showing output
    int serial, count_line=0;
    char ch;

    FILE *fp=NULL;
    fp = fopen("DIRECTORY.txt", "r");
    if (fp == NULL)
    {
        printf("!!Something went wrong!!\n");
        exit (1);
    }

    while((ch=fgetc(fp))!= EOF)
    {
        if(ch == '\n')
            count_line++;
    }
    serial = count_line;

    book p[MAX];

    while(fread(&p, sizeof(book), 1, fp))
    {
        for(int i=0; i<serial; i++)
        {
          printf("\n%d%-15s%s%-10s%d", p[i].no, p[i].name, p[i].number, p[i].mail, p[i].code);
    //as name and mail lenght may vary so used %-15s/%-10s to fill up emtpty spaces.
        }
    }*/
    fclose(fp);
}


void search()
{
    book p;
    char ch, look[100];
    int n, serial, found=0, count_line =1;
//lines are initialized 1 as even the cursor is at the begining of the
//files show 1 line
    FILE *fp=NULL;
    fp = fopen("DIRECTORY.txt", "r");
    if (fp == NULL)
    {
        printf("!!Something went wrong!!\n");
        exit (1);
    }

    while((ch=fgetc(fp))!= EOF)
    {
        if(ch == '\n')
            count_line++;
    }
    serial = count_line-1;

    /*fseek(fp, 0, SEEK_END);
    n = ftell(fp)/sizeof(book);
    //to get the number of record saved but didn't work*/

    printf("\n\t**You have %d data saved.**\n", serial);

    printf("\nEnter the serial want to search: ");
    scanf("%d", look);

    //printf("%s\n, p.name"); shows weird value

    /*char line[100];
    while(fgets(line, sizeof(line), fp))
    {
        if(strcmp(line, look)==0)
        {
            found = 1;
            printf("%s\n",line);
        }
    }
    if(found == 0)
        printf("\n!! NOT FOUND!!\n");*/

    fclose(fp);
}


void edit()
{
    book p1;
    char ch;
    int n, found=0, look;

    FILE *fp=NULL, *fp1=NULL;
    fp = fopen("DIRECTORY.txt", "r");
    if (fp == NULL)
    {
        printf("!!Something went wrong!!\n");
        exit (1);
    }

    fp1 = fopen("TEMPORARY.txt", "r");
    if (fp1 == NULL)
    {
        printf("!!Something went wrong!!\n");
        exit (1);
    }

    printf("\nEnter the serial want to edit: ");
    scanf("%d", &look);//4 no input want to edit

    //printf("%s\n, p.name");
    while(fread(&p1, sizeof(book), 1, fp))
    {
        if(p1.no == look)
        {
            found = 1;//found 4 no input
            //if ntot then writing in temp file
            //taking inputs for 4no output
            printf("%d.", look);

            printf("\nEnter new name: ");
            fflush(stdin);
            gets(p1.name);

            printf("Enter new telephone number: ");
            fflush(stdin);
            gets(p1.number);

            printf("Enter  new mail address: ");
            fflush(stdin);
            gets(p1.mail);

            printf("Enter new area code: ");
            fflush(stdin);
            scanf("%d", &p1.code);

            //printing the inputs in the file
            fprintf(fp, "%d %s %s %s %d\n", look, p1.name, p1.number, p1.mail, p1.code);
            //new inputs added in 4no serial
        }
//until not found then will print those values in fp1
        fwrite(&p1, sizeof(book), 1, fp1);
    }
    fclose(fp);
    fclose(fp1);
    if(found == 0)
    {
        fp1 = fopen("TEMPORARY.txt", "r");
        fp = fopen("DIRECTORY.txt", "w+");
        while(fread(&p1, sizeof(book), 1, fp1))
            fwrite(&p1, sizeof(book), 1, fp);
    }

    fclose(fp);
    fclose(fp1);

}


void menu()
{
    int entry;
    while(1)
    {
        printf("\n");
        printf("Press 1 to Load\n");
        printf("Press 2 to Store\n");
        printf("Press 3 to Insert\n");
        printf("Press 4 to Delete\n");
        printf("Press 5 to Edit\n");
        printf("Press 6 to Search\n");
        printf("Press 7 to Show\n");
        printf("Press 8 to Quit\n");

        printf("\nMenu Input:  ");
        scanf("%d", &entry);
        switch(entry)
        {
        //depending on entry, functions will be called
        case 1:
            load();
            break;
        case 2:
            store();
            break;
        case 3:
            Insert();
            break;
        case 4:
            Delete();
            break;
        case 5:
            edit();
            break;
        case 6:
            search();
            break;
        case 7:
            show();
            break;
        case 8:
            printf("\n***Directory closed***\n\n");
            exit(1);
            break;
        }
//end of switch
    }
}


//Start of Program
int main(void)
{
    //all implentation will be done in various functions separatley
    // that's why main() does not have much content
    printf("\n\t || TELEPHONE DIRECTORY || \n\n");
    menu();
    return 0;
}
