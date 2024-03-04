#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024


struct Product {
    int id;
    float price;
    char* name;
    int stores;
    int* storeStock; // the array holds the number of products of this type in each store
};

struct Product* createProductFromConsole() {

    struct Product* newProduct = malloc(sizeof(struct Product));

    char buffer[BUFFER_SIZE];

    printf("New product ID: ");
    scanf_s("%d", &newProduct->id);

    // Other way of reading input data (*)
    /*fgets(buffer, BUFFER_SIZE, stdin);
    char* endptr = NULL;
    newProduct->id = (int)strtol(buffer, endptr, 10);*/

    printf("\nNew product price: ");
    scanf_s("%f", &newProduct->price);

    // (*)
    /*fgets(buffer, BUFFER_SIZE, stdin);
    newProduct->price = strtof(buffer, endptr);*/

    printf("\nNew product name: ");
    scanf_s("%s", buffer, BUFFER_SIZE);

    // (*)
    //fgets(buffer, BUFFER_SIZE, stdin);

    newProduct->name = malloc((strlen(buffer) + 1) * sizeof(char));
    strcpy(newProduct->name, buffer);

    printf("\nNew product stores number: ");

    // (*)
    /*fgets(buffer, BUFFER_SIZE, stdin);
    int storesNumber = (int)strtol(buffer, endptr, 10);*/

    int storesNumber;
    scanf_s("%d", &storesNumber);

    newProduct->stores = storesNumber;

    newProduct->storeStock = malloc(storesNumber * sizeof(int));

    for (int i = 0; i < storesNumber; i++) {

        char storeSizeText[BUFFER_SIZE];
        sprintf_s(storeSizeText, BUFFER_SIZE, "Store %d stock size: ", i + 1);
        printf("\n%s", storeSizeText);
        
        int stock;
        scanf_s("%d", &newProduct->storeStock[i]);

        // (*)
        /*fgets(buffer, BUFFER_SIZE, stdin);
        int stock = (int)strtol(buffer, endptr, 10);*/

    }

    printf("\n");

    return newProduct;

}

void printProductToConsole(struct Product* product) {

    printf("Product ID: %d", product->id);
    printf("\nProduct price: %.2f", product->price);
    printf("\nProduct name: %s", product->name);

    printf("\nStores: %d", product->stores);
    printf("\nProduct stock in store: ");

    for (int i = 0; i < product->stores; i++) {
        printf("%d ", product->storeStock[i]);
    }

    printf("\n\n");
}

int main(void)
{
    struct Product** products;

    products = (struct Product**)malloc(3 * sizeof(struct Product*));


    for (int i = 0; i < 3; i++) {
        struct Product* product = (struct Product*)malloc(sizeof(struct Product));

        if (product == NULL) {
            printf("Error allocating memory");
            return(-1);
        }

        product->id = i + 1;

        product->price = 90 * (float)rand() / (float)RAND_MAX + 10;
        
        product->name = (char*)malloc(9 * sizeof(char));
        
        strcpy(product->name, "Product");
        char c[2];
        _itoa_s(i + 1, &c, 2, 10);
        strcat(product->name, c);


        product->stores = i + 2;
        product->storeStock = (int*)malloc(product->stores * sizeof(int));

        for (int j = 0; j < product->stores; j++) {
            product->storeStock[j] = 9 * rand() / RAND_MAX + 1;
        }
        products[i] = product;
    }

    printProductToConsole(products[0]);
    printProductToConsole(products[1]);
    printProductToConsole(products[2]);

    struct Product* product = createProductFromConsole();
    printProductToConsole(product);

    free(product->name);
    free(product->storeStock);
    free(product);


    for (int i = 0; i < 3; i++) {
        free(products[i]->name);
        free(products[i]->storeStock);
        free(products[i]);
    }

    free(products);

    return 0;
}
