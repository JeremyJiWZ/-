//
//  main.c
//  mysql数据库编程
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <time.h>

MYSQL mysql,*sock;
char a_id[15];

void Connector(char* i_query,int function);
void insert_book1();
int In_book(char id[]);
void Menu();
char* itoa(int num,char* str,int radix);
void Update_book_num(char id[],int n, int flag);
int In_ad_id(char a_id[]);
int In_ad_pw(char a_pw[]);
void query_menu();
void Bo_Re_menu();
void Add_bo();
void delete_bo();
void show_book(char card_id[]);
void borrow(char card_id[], char book_id[]);
void borrow_book();
int book_num(char book_id[]);
int borrow_id_int;
int In_card_id(char card_id[]);
void return_b(char card_id[], char book_id[]);
int In_borrow(char card_id[], char book_id[]);
void return_book();
void insert_book_file();
void insert_book(char id[], char type[], char title[], char press[],
                 char year[], char writer[], char price[], char number[]);

int main(int argc, const char * argv[])
{
    Menu();
    return 0;
}




void Menu()
{
    const char* host="localhost";
    const char* user="root";
    const char* password="";
    const char* db="library";
    unsigned int port=0;
    const char* unix_socket=NULL;
    unsigned long client_flag=0;
    mysql_init(&mysql);
    sock = mysql_real_connect(&mysql,host,user,password,db,port,unix_socket,client_flag);
    mysql_set_character_set(&mysql,"GBK");
    
    
    char ad_password[25];
    while (1) {
        printf("Please input the administer id: ");
        scanf("%s",a_id);
        if (!In_ad_id(a_id))
            printf("Wrong id！\n");
        else{
            printf("Please input the password: ");
            scanf("%s",ad_password);
            if (!In_ad_pw(ad_password))
                printf("Wrong password!\n");
            else
            {
                printf("log in successfully!\n");
                break;
            }
        }
    }

    
    int cmd;
    printf("Welcome to the Library Management System!\n");
    while (1){
        printf("command: \n");
        printf("1: insert books one by one\n");
        printf("2: insert books in a file\n");
        printf("3: query books' imformation\n");
        printf("4: borrow and return books\n");
        printf("0: exit\n");
        printf("Enter your command: \n");
        scanf("%d",&cmd);
        switch (cmd) {
            case 1: //单本插入
                insert_book1();
                break;
            case 2:
                insert_book_file();
                break;
            case 3: //查询图书信息
                query_menu();
                break;
            case 4:
                Bo_Re_menu();
                break;
            default:
                break;
        }
        if (cmd==0){
            mysql_close(sock);
            break;
        }
    }
}
void Connector(char* i_query,int function)
{
    
    MYSQL_RES* result;
    MYSQL_ROW row;
    int i=0;
    switch(function){
        case 1://单本插入
            mysql_query(sock,i_query);
            printf("insert successfully!\n");
            break;
        case 2://查询图书信息
            mysql_query(&mysql,i_query);
            result=mysql_store_result(&mysql);
            while((row=mysql_fetch_row(result))!=NULL)
            {
                printf("id:\t%s\n",row[0]);
                printf("type:\t%s\n",row[1]);
                printf("Title:\t%s\n",row[2]);
                printf("Press:\t%s\n",row[3]);
                printf("year:\t%s\n",row[4]);
                printf("writer:\t%s\n",row[5]);
                printf("price:\t%s\n",row[6]);
                printf("totle number:\t%s\n",row[7]);
                printf("current number:\t%s\n",row[8]);
                i++;
                if (i>=50)
                    break;
            }
            mysql_free_result(result);
            break;
        case 3://查询借书时间
            mysql_query(&mysql,i_query);
            result=mysql_store_result(&mysql);
            while(!!result&&(row=mysql_fetch_row(result))!=NULL)
            {
                printf("return time:\t%s\n",row[0]);
            }
            break;
    }
}
char* itoa(int num,char* str,int radix)
{/*索引表*/
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned unum;/*中间变量*/
    int i=0,j,k;
    /*确定unum的值*/
    if(radix==10&&num<0)/*十进制负数*/
    {
        unum=(unsigned)-num;
        str[i++]='-';
    }
    else unum=(unsigned)num;/*其他情况*/
    /*转换*/
    do{
        str[i++]=index[unum%(unsigned)radix];
        unum/=radix;
    }while(unum);
    str[i]='\0';
    /*逆序*/
    if(str[0]=='-')k=1;/*十进制负数*/
    else k=0;
    char temp;
    for(j=k;j<=(i-1)/2;j++)
    {
        temp=str[j];
        str[j]=str[i-1+k-j];
        str[i-1+k-j]=temp;
    }
    return str;
}
void Update_book_num(char id[],int n,int flag)//flag = 1,then tot_num change;flag = 0 tot_num doesn't change
{
    char query[400]="update book set cur_num = cur_num + \"";
    char query1[400]="update book set tol_num = tol_num + \"";

    char sign_n[10];
    itoa(n, sign_n, 10);
    strcat(query, sign_n);
    strcat(query, "\" where book_id = \"");
    strcat(query, id);
    strcat(query, "\"");
    strcat(query1, sign_n);
    strcat(query1, "\" where book_id = \"");
    strcat(query1, id);
    strcat(query1, "\"");
    Connector(query, 3);
    if (flag)
        Connector(query1, 3);
    
}

int In_ad_id(char a_id[])
{
    char query[300]="select * from administer";
    
    MYSQL_RES* result;
    MYSQL_ROW row;
    mysql_query(&mysql,query);
    result=mysql_store_result(&mysql);
    while((row=mysql_fetch_row(result))!=NULL){
        if (strcmp(a_id, row[0])==0) {
            mysql_free_result(result);
            return 1;
        }
    }
    mysql_free_result(result);
    return 0;
}

int In_ad_pw(char a_pw[])
{
    char query[300]="select * from administer";
    
    MYSQL_RES* result;
    MYSQL_ROW row;
    mysql_query(&mysql,query);
    result=mysql_store_result(&mysql);
    while((row=mysql_fetch_row(result))!=NULL){
        if (strcmp(a_pw, row[1])==0) {
            mysql_free_result(result);
            return 1;
        }
    }
    mysql_free_result(result);
    return 0;
}
void insert_book1()
{
    char query1[300]="insert into book values (\"";
    char id[12];
    char type[22];
    char title[42],press[42],writer[22];
    char year[10],price[10],tot_num[10],cur_num[10];
    char n[5];
    int number;
    printf("Please input the book id: ");
    scanf("%s",id);
    printf("Please input the book type: ");
    scanf("%s",type);
    printf("Please input the book title: ");
    scanf("%s",title);
    printf("Please input the press: ");
    scanf("%s",press);
    printf("Please input the year: ");
    scanf("%s",year);
    printf("Please input the book writer: ");
    scanf("%s",writer);
    printf("Please input the book price: ");
    scanf("%s",price);
    printf("Please input number: ");
    scanf("%d",&number);
    if (In_book(id))
        Update_book_num(id,number,1);
    else
    {
        itoa(number,n,10);
        strcpy(tot_num, n);
        strcpy(cur_num, n);
        strcat(query1, id);
        strcat(query1, "\",\"");
        strcat(query1, type);
        strcat(query1, "\",\"");
        strcat(query1, title);
        strcat(query1, "\",\"");
        strcat(query1, press);
        strcat(query1, "\",\"");
        strcat(query1, year);
        strcat(query1, "\",\"");
        strcat(query1, writer);
        strcat(query1, "\",\"");
        strcat(query1, price);
        strcat(query1, "\",\"");
        strcat(query1, tot_num);
        strcat(query1, "\",\"");
        strcat(query1, cur_num);
        strcat(query1, "\")");
        Connector(query1, 1);
    }
    
}

int In_book(char id[])
{
    char query[300]="select * from book where book_id=\"";
    strcat(query, id);
    strcat(query, "\"");
    
    MYSQL_RES* result;
    MYSQL_ROW row;
    mysql_query(&mysql,query);
    result=mysql_store_result(&mysql);
    while((row=mysql_fetch_row(result))!=NULL)
    {
        if (strcmp(id, row[0])==0) {
            mysql_free_result(result);
            return 1;
        }
    }
    mysql_free_result(result);
    return 0;
}

void query_menu()
{
    int cmd;
    char type[25];
    char title[45];
    char press[45];
    char year1[6],year2[6];
    char price1[6],price2[6];
    char writer[25];
    char query[200]="select * from book where ";
    printf("Command: \n");
    printf("1: query by book type\n");
    printf("2: query by book title\n");
    printf("3: query by book press\n");
    printf("4: query by year\n");
    printf("5: query by book price\n");
    printf("6: query by book author\n");
    printf("0: back to the main menu\n");
    while (1){
        printf("input your command: \n");
        scanf("%d",&cmd);
        switch (cmd) {
            case 1:
                printf("input type: ");
                scanf("%s",type);
                strcat(strcat(strcat(query, "book_type = \""),type),"\"");
                Connector(query, 2);
                break;
            case 2:
                printf("input title: ");
                scanf("%s",title);
                strcat(strcat(strcat(query, "title = \""),title),"\"");
                Connector(query, 2);
                break;
            case 3:
                printf("input press: ");
                scanf("%s",press);
                strcat(strcat(strcat(query, "press = \""),press),"\"");
                Connector(query, 2);
                break;
            case 4:
                printf("input lower year: ");
                scanf("%s",year1);
                printf("input higher year: ");
                scanf("%s",year2);
                strcat(strcat(strcat(query, "year >= \""),year1),"\"");
                strcat(strcat(strcat(query, " and year <= \""),year2),"\"");
                Connector(query, 2);
                break;
            case 5:
                printf("input lower price: ");
                scanf("%s",price1);
                printf("input higher price: ");
                scanf("%s",price2);
                strcat(strcat(strcat(query, "price >= \""),price1),"\"");
                strcat(strcat(strcat(query, " and price <= \""),price2),"\"");
                Connector(query, 2);
                break;
            case 6:
                printf("input writer: ");
                scanf("%s",writer);
                strcat(strcat(strcat(query, "writer = \""),writer),"\"");
                Connector(query, 2);
                break;
            case 0:
                break;
            default:
                printf("Invalid cmd!\n");
                break;
    }
        if (cmd==0) {
            break;
        }
    }
}

void Bo_Re_menu()
{
    int cmd;
    
    printf("Command: \n");
    printf("1: add a borrower's card\n");
    printf("2: delete a borrower's card\n");
    printf("3: borrow a book\n");
    printf("4: return a book\n");
    printf("0: back to the main menu\n");
    
    while (1) {
        printf("input your command: ");
        scanf("%d",&cmd);
        switch (cmd) {
            case 1:
                Add_bo();
                break;
            case 2:
                delete_bo();
                break;
            case 3:
                borrow_book();
                break;
            case 4:
                return_book();
                break;
            default:
                printf("Invalid command!\n");
                break;
        }
        if (cmd==0) {
            break;
        }
        
    }
}

void Add_bo()
{
    char card_id[25];
    char name[25];
    char deparment[25];
    char card_type[25];
    char query1[300]="insert into borrow_card values (\"";

    printf("input the card_id: ");
    scanf("%s",card_id);
    printf("input the name: ");
    scanf("%s",name);
    printf("input the deparment: ");
    scanf("%s",deparment);
    printf("input the card type: ");
    scanf("%s",card_type);
    
    strcat(query1, card_id);
    strcat(query1, "\",\"");
    strcat(query1, name);
    strcat(query1, "\",\"");
    strcat(query1, deparment);
    strcat(query1, "\",\"");
    strcat(query1, card_type);
    strcat(query1, "\")");
    mysql_query(sock,query1);
    printf("OK\n");
}

void delete_bo()
{
    char card_id[25];
    char query1[300]="delete from borrow_card where card_id = \"";
    printf("input the card id: ");
    scanf("%s",card_id);
    strcat(query1, card_id);
    strcat(query1, "\"");
    mysql_query(sock, query1);
    printf("OK\n");
}

void borrow_book()
{
    char card_id[25];
    char book_id[15];
    
    printf("input the card id: ");
    scanf("%s",card_id);
    if (!In_card_id(card_id)){
        printf("card id not exist!\n");
        return;
    }
    show_book(card_id);
    printf("input the book id: ");
    scanf("%s",book_id);
    if (!In_book(book_id)){
        printf("book id not exist!\n");
        return;
    }
    borrow(card_id, book_id);
    
    
}
void show_book(char card_id[])
{
    MYSQL_RES* result;
    MYSQL_ROW row;
    char query[200]="select book_id, book_type, title, press, year, writer, price, tot_num, cur_num from borrow_record natural join book where card_id= \"";
    strcat(query, card_id);
    strcat(query, "\"");
    mysql_query(&mysql,query);
    result=mysql_store_result(&mysql);
    while((row=mysql_fetch_row(result))!=NULL)
    {
        printf("id:\t%s\n",row[0]);
        printf("type:\t%s\n",row[1]);
        printf("Title:\t%s\n",row[2]);
        printf("Press:\t%s\n",row[3]);
        printf("year:\t%s\n",row[4]);
        printf("writer:\t%s\n",row[5]);
        printf("price:\t%s\n",row[6]);
        printf("totle number:\t%s\n",row[7]);
        printf("current number:\t%s\n",row[8]);
    }
    mysql_free_result(result);
    
}
int book_num(char book_id[])
{
    MYSQL_RES* result;
    MYSQL_ROW row;
    char query[200]="select cur_num from book where book_id = \"";
    int n;
    strcat(query, book_id);
    strcat(query, "\"");
    mysql_query(&mysql, query);
    result=mysql_store_result(&mysql);
    if ((row=mysql_fetch_row(result))!=NULL)
        n=atoi(row[0]);
    return n;
}
void borrow(char card_id[], char book_id[])
{
    char query1[200]="select borrow_time from borrow_record where book_id = \"";
    char query2[200]="insert into borrow_record values (\"";
    time_t rawtime;
    struct tm * timeinfo;
    char time_s[15];
    int y,m,d,hou,min,sec;
    char ys[5],ms[5],ds[5],hous[5],mins[5],secs[5];
    char mss[5]="0";
    char secss[5]="0";
    char minss[5]="0";
    strcat(query1, book_id);
    strcat(query1, "\"");
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    y=2015;
    m=4;
    d=timeinfo->tm_mday;
    hou=timeinfo->tm_hour;
    min=timeinfo->tm_min;
    sec=timeinfo->tm_sec;
    itoa(hou, hous, 10);
    itoa(m, ms, 10);
    itoa(d, ds, 10);
    itoa(y, ys, 10);
    itoa(min, mins, 10);
    itoa(sec, secs, 10);
    if (m<10) strcat(mss, ms);
    if (min<10) strcat(minss, mins);
    if (sec<10) strcat(secss, secs);
    strcpy(time_s, ys);
    strcat(time_s, mss);
    strcat(time_s, ds);
    strcat(time_s, hous);
    if (min<10)
        strcat(time_s, minss);
    else
        strcat(time_s, mins);
    if (sec<10)
        strcat(time_s, secss);
    else
        strcat(time_s, secs);
    
    strcat(query2, card_id);
    strcat(query2, "\",\"");
    strcat(query2, time_s);
    strcat(query2, "\",NULL,\"");
    strcat(query2, a_id);
    strcat(query2, "\",\"");
    strcat(query2, book_id);
    strcat(query2, "\")");
    if (book_num(book_id)<=0) {//库存书籍不够
        printf("The books are all borrowed!\n");
        Connector(query1, 3);//显示借书时间
    }
    else //库存书籍足够
    {
        Update_book_num(book_id, -1,0);
        printf("borrow success!\n");
        mysql_query(sock, query2);
    }
}
int In_card_id(char card_id[])
{
    char query[300]="select * from borrow_card where card_id=\"";
    strcat(query, card_id);
    strcat(query, "\"");
    
    MYSQL_RES* result;
    MYSQL_ROW row;
    mysql_query(&mysql,query);
    result=mysql_store_result(&mysql);
    while((row=mysql_fetch_row(result))!=NULL)
    {
        if (strcmp(card_id, row[0])==0) {
            mysql_free_result(result);
            return 1;
        }
    }
    mysql_free_result(result);
    return 0;
}
void return_book()
{
    char card_id[25];
    char book_id[15];
    
    printf("input the card id: ");
    scanf("%s",card_id);
    if (!In_card_id(card_id)){
        printf("card id not exist!\n");
        return;
    }
    show_book(card_id);
    printf("input the book id: ");
    scanf("%s",book_id);
    if (!In_borrow(card_id,book_id)){
        printf("You have not borrowed this book!\n");
        return;
    }
    return_b(card_id, book_id);
}
int In_borrow(char card_id[], char book_id[])
{
    char query[300]="select book_id from borrow_record where card_id=\"";
    strcat(query, card_id);
    strcat(query, "\"");
    
    MYSQL_RES* result;
    MYSQL_ROW row;
    mysql_query(&mysql,query);
    result=mysql_store_result(&mysql);
    while((row=mysql_fetch_row(result))!=NULL)
    {
        if (strcmp(book_id, row[0])==0) {
            mysql_free_result(result);
            return 1;
        }
    }
    mysql_free_result(result);
    return 0;
}
void return_b(char card_id[], char book_id[])
{
    char query[300]="update borrow_record set return_time = \"";
    char query1[100]="where card_id = \"";
    char query2[100]="and book_id = \"";
    time_t rawtime;
    struct tm * timeinfo;
    char time_s[15];
    int y,m,d,hou,min,sec;
    char ys[5],ms[5],ds[5],hous[5],mins[5],secs[5];
    char mss[5]="0";
    char secss[5]="0";
    char minss[5]="0";
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    y=2015;
    m=4;
    d=timeinfo->tm_mday;
    hou=timeinfo->tm_hour;
    min=timeinfo->tm_min;
    sec=timeinfo->tm_sec;
    itoa(hou, hous, 10);
    itoa(m, ms, 10);
    itoa(d, ds, 10);
    itoa(y, ys, 10);
    itoa(min, mins, 10);
    itoa(sec, secs, 10);
    if (m<10) strcat(mss, ms);
    if (min<10) strcat(minss, mins);
    if (sec<10) strcat(secss, secs);
    strcpy(time_s, ys);
    strcat(time_s, mss);
    strcat(time_s, ds);
    strcat(time_s, hous);
    if (min<10)
        strcat(time_s, minss);
    else
        strcat(time_s, mins);
    if (sec<10)
        strcat(time_s, secss);
    else
        strcat(time_s, secs);
    
    strcat(query, time_s);
    strcat(query, "\"");
    strcat(query, query1);
    strcat(query, card_id);
    strcat(query, "\" ");
    strcat(query, query2);
    strcat(query, book_id);
    strcat(query, "\"");
    
    Update_book_num(book_id, 1,0);
    printf("return success!\n");
    mysql_query(sock, query);
}

void insert_book_file()
{
    char file_n[50];
    FILE *fp;
    char book_id[22];
    char type[22];
    char title[42],press[42],writer[22];
    char year[10],price[10];
    char n[6];
    printf("input the file directory: \n");
    scanf("%s",file_n);
    fp=fopen(file_n, "r");
    if(!fp)
    {
        printf("can't open file: %s\n",file_n);
        return ;
    }
    while (!feof(fp)) {
        fscanf(fp, "%s",book_id);
        fscanf(fp, "%s",type);
        fscanf(fp, "%s",title);
        fscanf(fp, "%s",press);
        fscanf(fp, "%s",year);
        fscanf(fp, "%s",writer);
        fscanf(fp, "%s",price);
        fscanf(fp, "%s",n);
        insert_book(book_id,type,title,press,year,writer,price,n);
    }
    printf("Success!\n");
    fclose(fp);
}
void insert_book(char id[], char type[], char title[], char press[],
                 char year[], char writer[], char price[], char number[])
{
    char query1[300]="insert into book values (\"";
    char tot_num[6],cur_num[6];
    int num;
    num=atoi(number);
    if (In_book(id))
        Update_book_num(id,num,1);
    else
    {
        strcpy(tot_num, number);
        strcpy(cur_num, number);
        strcat(query1, id);
        strcat(query1, "\",\"");
        strcat(query1, type);
        strcat(query1, "\",\"");
        strcat(query1, title);
        strcat(query1, "\",\"");
        strcat(query1, press);
        strcat(query1, "\",\"");
        strcat(query1, year);
        strcat(query1, "\",\"");
        strcat(query1, writer);
        strcat(query1, "\",\"");
        strcat(query1, price);
        strcat(query1, "\",\"");
        strcat(query1, tot_num);
        strcat(query1, "\",\"");
        strcat(query1, cur_num);
        strcat(query1, "\")");
        mysql_query(sock,query1);
    }
    
}




