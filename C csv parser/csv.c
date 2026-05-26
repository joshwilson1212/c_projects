#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LINE 1000

typedef struct values{
    char *data;
    struct values *next;
}values;

typedef struct{
        char *key;
        values *value;
} hashtable;

values* create_node(const char *data){
    values *new_node = (values*)malloc(sizeof(values));
    if (new_node == NULL) {
        printf("MEMORY ALLOCATION FAILED\n");
        exit(1);
    }
    new_node->data = strdup(data);
    new_node->next = NULL;
    return new_node;
}


void parse_csv(FILE *file){
    int ch;
    int line = 0;
    bool inQuotes = false;
    char line_buffer[MAX_LINE];

    while ((ch = fgetc(file))!=EOF){
        int i = 0;
        if(line >= 2){break;}

        while(ch != '\n' && ch != EOF && i < MAX_LINE - 1){
            if((ch == ',') && !inQuotes){
                line_buffer[i] = '\0';
                printf("%s\n", line_buffer);

                i = 0;
                ch = fgetc(file);
                continue;
            }
            else{
            line_buffer[i] = ch;
            i++;
            if(ch == '\"'){ inQuotes = !inQuotes; }
            ch = fgetc(file);
            }

        }
            //wrapping up the last field
            line_buffer[i] = '\0';
            printf("%s\n", line_buffer);

        printf("\n");  

    }
}

int main(){

    FILE *file = fopen("test.csv","r");
    if(file == NULL){
        printf("The file is not opened.\n");
        return 1;
    }
    printf("successfully opened file\n");
   
    hashtable col;
    col.key = "DATE";
    col.value = NULL;

    col.value = create_node("5/18/2026");
    col.value->next = create_node("5/16/2026");
    col.value->next->next = create_node("5/16/2026");

    printf("parsing key:%s",col.key);
    values *current = col.value;
    while(current != NULL){
        printf("[%s]->",current->data);
        current = current->next;
    }
    printf("NULL\n");

    current = col.value;
    while(current != NULL){
        values *temp = current;
        current = current->next;
        
        free(temp->data);
        free(temp);
    }
    //parse_csv(file);
    
    fclose(file);
    return 0;

}