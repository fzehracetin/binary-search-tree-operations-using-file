#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 100

typedef struct node 
{
    int id; 
    char name[MAX]; 
    int* friends; 
    struct node* right;  
    struct node* left; 
} node;

node* users = NULL;  //head node

//string to integer
int strToInt (char str[]) 
{ 
    int i = 0, num = 0; 
    while(i < strlen(str)) 
    {
        num += pow(10, strlen(str)-1-i)*(str[i] - '0'); 
        i++;
    }
    return num;
}

node* findLine(char data[], int id, int select) {
    int i = 0, j = 0, k = 0;// i datayi char char okumayi saglar, j datayi str'a atmak icin str indisi, k friends dizisinin indisi
    char str[MAX]; // datayı okurken parcalamayi saglar
    node* user; // yeni kullanicinin node u
    user = (node*) malloc(sizeof(node));
    user->friends = (int*) malloc(sizeof(int));
    user->friends[0] = -1;
    
    while( !((data[i] >= '0') && (data[i]<= '9'))){ //bazen okunan satirin basinda garip karakterler oluyor bu durumda hatayi engellemek icin
            i++;
    }
    
    while (data[i] != ',') { //kullanicinin id'si dugum yapisina koyulur
            str[j] = data [i];
            j++; i++;
    }
    str[j] = '\0';
    
    if (((id == strToInt(str)) && (select == 0)) || (select == 1))  { //aranan kullanici bulundu
        user->id = strToInt(str);
        
        i++; j = 0; // virgulu atla
        while ((data[i] != ',') && (data[i] != '\n')) { //kullanicinin adi dugum yapisina koyulur
            str[j] = data[i];
            i++; j++;
        }
        str[j] = '\0';
        strcpy(user->name, str);
        //kullanicinin arkadaslari dugum yapisinda diziye eklenir
        while(data[i] != '\n') { 
            i++; j=0; //virgulu veya kisa cizgiyi atla 
            while ( (data[i] != '-') && (data[i] != '\n') ) { // hic arkadasi yoksa buraya girmez
                str[j] = data[i];
                i++; j++;
            }
            str[j] = '\0';
            user->friends[k] = strToInt(str);
            user->friends = realloc(user->friends, (++k+1)*sizeof(int));
            user->friends[k] = -1;
        }
        user->left = NULL;
        user->right = NULL;
        return user;
    } 
    else
        return NULL;
    
}

node* findUser(int id, FILE* f1) {
    char data[MAX]; // okunan satirin atilacagi char dizisi
    node* user = NULL; //yeni kullanicinin atanacagi dugum
    
    fseek(f1, 0, SEEK_SET);
    while ( (user == NULL) && (fgets(data, MAX, f1) != NULL) && (data[0] != '\n') ) {
        user = findLine(data, id, 0);
    }
    return user;
}

void insertNewUser(int id, FILE* f1, node* newNode) {
    node * current, *parent, *newUser; // current, linkli listede hareketi saglar; parent, currentin parent dugumu; newUser yeni kullanicin atandigi dugum
    int stop = 0; // whiledan cikis sarti icin tutulan int
    
    if (id != -1)
        newUser = findUser(id, f1);
    else
        newUser = newNode;
    
    if (newUser == NULL) 
        printf("Idsi verilen kullanici dosyada yok !! \n");
    else if (users == NULL) {
        users = newUser;
        printf("Kullanici basariyla eklendi. \n");
    }
    else {
        current = users;
        while (!stop) {
            parent = current;
            if (newUser->id < current->id) {
                current = current->left;
                if( current == NULL) {
                    parent->left = (node*) malloc(sizeof(node));
                    parent->left = newUser;
                    stop = 1;
                    printf("Kullanici basariyla eklendi. \n");
                }
            } 
            else {
                current = current->right;
                if (current == NULL) {
                    parent->right = (node*) malloc(sizeof(node));
                    parent->right = newUser;
                    stop = 1;
                    printf("Kullanici basariyla eklendi. \n");
                }
            }
        }
    }
}

void addAll(FILE *f1) {
    char data[100];
    node* user = NULL;
    fseek(f1, 0, SEEK_SET);
    
    while (( fgets(data, MAX, f1) != NULL) && (data[0] != '\n') ) {
        printf("data : %s \n", data);
        user = findLine(data, -1, 1);
        insertNewUser(-1, f1, user);
    }
}


int findChildNumber(node* root) {
    if ( (root->right == NULL) && (root->left == NULL) ) {
        return -1;
    } else if ( (root->right != NULL) && (root->left != NULL)){
        return 2;
    } else {
        if (root->right != NULL) 
            return 1;
        else
            return 0;
    }
}

node* findMinimum(node* root) {
    if(root != NULL){
        node* parent, *current = root; // current linkli listede gezmeyi saglar; parent, currentin parent dugumu
        while (current != NULL) {
            parent = current;
            current = current->left;
        }
        return parent;
    }
    return NULL;
}

node* findMaximum(node* root) {
    if (root != NULL) {
        node* parent, *current = root;
        while (current != NULL) {
            parent = current;
            current = current->right;
        }
        return parent;
    }
    return NULL;
}

node* deleteUser(node* myusers, int id) {
    node* current; //linkli listede gezinmek icin
	
    if (myusers == NULL) 
        return NULL;
    if (id < myusers->id)
        myusers->left = deleteUser(myusers->left, id);
    else if (id > myusers->id)
        myusers->right = deleteUser(myusers->right, id);
    else {
        if (findChildNumber(myusers) == -1) {
            free(myusers);
            myusers = NULL;
        } 
        else if (findChildNumber(myusers) == 0) { // tek cocuk var ve left
            current = findMaximum(myusers->left);
            myusers->id = current->id;
            strcpy(myusers->name, current->name);
            myusers->friends = current->friends;
            myusers->left = deleteUser(myusers->left, current->id);
        } 
        else if (findChildNumber(myusers) == 1) { //tek cocuk var ve right
            current = findMinimum(myusers->right);
            myusers->id = current->id;
            strcpy(myusers->name, current->name);
            myusers->friends = current->friends;
            myusers->right = deleteUser(myusers->right, current->id);
        }
        else {
            current = findMinimum(myusers->right);// right subtreenin min elemani
            myusers->id = current->id;
            strcpy(myusers->name, current->name);
            myusers->friends = current->friends;
            myusers->right = deleteUser(myusers->right, current->id);
        }
    }
    return myusers;
}

void contains ( int id) {
    node* current = NULL; //linkli listede gezinmek icin
    if (users == NULL) 
        printf("The tree is empty \n");
    else {
        current = users;
        while( (current != NULL) && (current->id != id) ) {
            //printf("%d ile karsilastiriyor. . .", current->id);
            if ( current->id < id) 
                current = current->right;
            else 
                current = current->left;
            //printf("\n");
        }
    }
    if (current == NULL) 
        printf("This person is not in the tree. \n");
    else 
        printf("Name-Surname : %s \n", current->name);
}

node* isInTree(node* myusers, int id) {
    node* current = NULL; //linkli listede gezinmek icin
    if (myusers != NULL) {
        current = myusers;
        while( (current != NULL) && (current->id != id) ) {
            if ( current->id < id) 
                current = current->right;
            else 
                current = current->left;
        }
    }
    return current;
}

void friends (int id) {
    if (isInTree(users, id) != NULL) {
        node* user = isInTree(users, id); //idsi verilen kullanici agacta ise dugumu doner
        int i = 0; //friends dizisinin iteratoru
        while(user->friends[i] != -1) {
            contains(user->friends[i]);
            i++;
        }
        if (i == 0) 
            printf("This person has no friends \n");
    } 
    else 
        printf("This person is not in the tree. \n");
}

void printInOrder(node* myusers) {
    if (myusers != NULL) {
        printInOrder(myusers->left);
        printf("%d ", myusers->id);
        printf("%s \n", myusers->name);
        printInOrder(myusers->right);
    }
}

void treeTraversal(node* myusers, int* size) {
    if (myusers != NULL) {
        treeTraversal(myusers->left, size);
        treeTraversal(myusers->right, size);
        *size= *size + 1; //tum dugumleri sayar
    }
}

void size() {
    int size = 0; // dugum sayisinin tutulacagi degisken
    treeTraversal(users, &size); //dugumleri sayar
    printf("Size : %d \n", size);
}

void printNext(node* myusers, int id) {
    node* current = isInTree(myusers, id);// verilen kullanicin dugumune doner
    if (current != NULL){ // eger boyle bir kullanici varsa 
        printInOrder(current->left); //sol subtreesi yazdirilir
        printInOrder(current->right); //sag subtreesi yazdirilir
    }
}

void printGreater (node* myusers, int id) {
    if (myusers != NULL) {
        printGreater(myusers->left, id);
        if (id < myusers->id) { //kullanicinin idsinden buyukse yazdirilir
            printf("%d ", myusers->id);
            printf("%s \n", myusers->name);
        }
        printGreater(myusers->right, id);
    }
}

int main(int argc, char **argv)
{
    FILE *f1; //dosyanin pointeri
    int id, fonk, out = 1; //menu yapisi icin kullanilacak degiskenler
    f1 = fopen("D:\\Input.txt", "r");
    
    if (f1 == NULL) {
        printf("File could not opened\n");
    }
    
   do
    {
        system("cls");
        printf("_________IKILI ARAMA AGACI UZERINDE ISLEMLER__________ \n");
        printf("0 - addAll() \n");
        printf("1 - insertNewUser() \n");
        printf("2 - deleteUser() \n");
        printf("3 - contains() \n");
        printf("4 - friends() \n");
        printf("5 - size() \n");
        printf("6 - printNext() \n");
        printf("7 - printGreater() \n");
        printf("8 - printInOrder() \n");
        printf("9 - EXIT \n");
        printf("Islem yapmak istediginiz fonksiyonu seciniz : ");
        out = 1; //do-while cikis sarti olacak ic kisimlarda
        scanf("%d", &fonk);
        system("cls");
        switch(fonk) {
            case 0:
                do {
                    printf(" 0 - EXIT from function \n");
                    printf(" 1 - addAll() \n");
                    printf("Islemi seciniz : ");
                    scanf("%d", &out);
                    switch(out) {
                        case 0:
                            break;
                        case 1:
                            addAll(f1);
                            break;
                    }
                } while (out != 0);
                break;
            case 1: 
                do {
                    printf(" 0 - EXIT from function \n");
                    printf(" 1 - insertNewUser() \n");
                    printf("Islemi seciniz : ");
                    scanf("%d", &out);
                    switch(out) {
                        case 0:
                            break;
                        case 1:
                            printf("Eklenecek elemanin idsini girin : ");
                            scanf("%d", &id);
                            insertNewUser(id, f1, 0);
                            break;
                    }
                } while (out != 0);
                break;
            case 2:
                do {
                    printf(" 0 - EXIT from function \n");
                    printf(" 1 - deleteUser() \n");
                    printf("Islemi seciniz : ");
                    scanf("%d", &out);
                    switch(out) {
                        case 0:
                            break;
                        case 1:
                            printf("Silinecek kullanicinin idsini girin : ");
                            scanf("%d", &id);
                            deleteUser(users, id);
                            printf("Kullanici silindi. \n");
                            break;
                    }
                } while (out != 0);
                break;
            case 3:
                 do {
                    printf(" 0 - EXIT from function \n");
                    printf(" 1 - contains() \n");
                    printf("Islemi seciniz : ");
                    scanf("%d", &out);
                    switch(out) {
                        case 0:
                            break;
                        case 1:
                            printf("Aranacak kullanicinin idsini girin : ");
                            scanf("%d", &id);
                            contains(id);
                            break;
                    }
                } while (out != 0);
                break;
            case 4:
                do {
                    printf(" 0 - EXIT from function \n");
                    printf(" 1 - friends() \n");
                    printf("Islemi seciniz : ");
                    scanf("%d", &out);
                    switch(out) {
                        case 0:
                            break;
                        case 1:
                            printf("Arkadaslari yazdirilacak kullanicinin idsini girin : ");
                            scanf("%d", &id);
                            friends(id);
                            break;
                    }
                } while (out != 0);
                break;
            case 5:
                 do {
                    printf(" 0 - EXIT from function \n");
                    printf(" 1 - size() \n");
                    printf("Islemi seciniz : ");
                    scanf("%d", &out);
                    switch(out) {
                        case 0:
                            break;
                        case 1:
                            size();
                            break;
                    }
                } while (out != 0);
                break;
            case 6:
                do {
                    printf(" 0 - EXIT from function \n");
                    printf(" 1 - printNext() \n");
                    printf("Islemi seciniz : ");
                    scanf("%d", &out);
                    switch(out) {
                        case 0:
                            break;
                        case 1:
                            printf("Alt agacindaki elemanlari yazdirmak istediginiz kullanicinin idsini girin: ");
                            scanf("%d", &id);
                            printNext(users, id);
                            break;
                    }
                } while (out != 0);
                break;
            case 7:
                do {
                    printf(" 0 - EXIT from function \n");
                    printf(" 1 - printGreater() \n");
                    printf("Islemi seciniz : ");
                    scanf("%d", &out);
                    switch(out) {
                        case 0:
                            break;
                        case 1:
                            printf("Kendisinden daha buyuk elemanlarin bulunacagi kullanicinin idsini girin : ");
                            scanf("%d", &id);
                            printGreater(users, id);
                            break;
                    }
                } while (out != 0);
                break;
            case 8:
                do {
                    printf(" 0 - EXIT from function \n");
                    printf(" 1 - printInOrder() \n");
                    printf("Islemi seciniz : ");
                    scanf("%d", &out);
                    switch(out) {
                        case 0:
                            break;
                        case 1:
                            printInOrder(users);
                            break;
                    }
                } while (out != 0);
                break;
            case 9:
                printf("Program kapatiliyor. . . \n");
                exit(0);
                break;
            default: 
                printf("Hatali giris yaptiniz! \n");
                break;
            
        }
    } while (fonk != 9);
	return 0;
}
