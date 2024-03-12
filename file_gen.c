#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


#define MAX_LENGTH 256
#define KEY_COUNT 10
#define KEY_LENGTH 11

/*
    Created by Peter Vanát 2024
    Ako pouzit tento program: 
        - v priecinku kde bude executable, musi byt vytvoreny subor info.txt kde na kazdom riadku bude <meno:heslo> (nech je prazdny riadok po poslednom riadku)
        - do funckie hash_string doplnte logiku hashovania, ktore chcete pouzit
        - upravte flagy v MakeFile (LDLIBS) aby sa program skompiloval
        - zavolajte "make gen"
        - executable bude mat nazov "generator"
        - vygenerovany subor bude mat nazov "hesla.csv"
*/

struct file_gen {
    char name[MAX_LENGTH];
    char hashed_password[MAX_LENGTH];
    char **keys;
};

/**
 * @brief Function to hash input string 
 * 
 * @param str string to hash
 * @param outputBuffer return argument, place for hashed string
 */
void hash_string(const char *str, char * outputBuffer){
    
    //strcpy(outputBuffer, hashed_string);
}
/**
 * @brief generate number between min and max
 * 
 * @param min 
 * @param max 
 * @return int 
 */
int generate_random_in_range(int min, int max){
    return (rand() % (max-min+1)) + min;
}

/**
 * @brief Function to generate KEY_COUNT keys, with lenght of KEY_LENGTH
 * Keys are generated from a-zA-Z0-9 
 * 
 * @param output output struct, generated keys are stored in output->keys 
 */
void generateKeys(struct file_gen *output){
    char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789";
    for(int keyNum = 0; keyNum < KEY_COUNT; keyNum++){
        char *key = calloc(KEY_LENGTH+1, sizeof(char));
        if(key == NULL){
            printf("hmmm\n");
            exit(1);
        }
        for(int j = 0; j < KEY_LENGTH-1; j++){
            int idx = generate_random_in_range(0, strlen(alphabet)-1);
            key[j] = alphabet[idx];
        }
        key[KEY_LENGTH] = '\0';
        strcpy(output->keys[keyNum], key);
        free(key);
    }
}

/**
 * @brief Initialize FILE_GEN struct with given parameters
 * 
 * @param output struct to initialize
 * @param name name to be put into output->name
 * @param hashed_password password to be put to output->hashed_password
 */
void file_gen_init(struct file_gen *output, char name[], char hashed_password[]){
    strcpy(output->name, name);
    strcpy(output->hashed_password, hashed_password);
    output->keys = malloc(KEY_COUNT*sizeof(char*));
    for(int i = 0; i < KEY_COUNT; i++){
        output->keys[i] = calloc(KEY_LENGTH+1, sizeof(char));
    }
}

/**
 * @brief Function to free all memory used by FILE_GEN struct
 * If you are not scared of memory leaks, feel free to not use this function
 * @param input FILE_GEN struct to free
 */
void file_gen_destroy(struct file_gen *input){
    for(int i = 0; i < KEY_COUNT; i++){
        free(input->keys[i]);
    }
    free(input->keys);
}

/**
 * @brief Function to write FILE_GEN struct into file "hesla.csv"
 * Each written line has <name:hashedPassword:keys> format, where keys are delimited by ,
 * 
 * @param input FILE_GEN struct to write into file
 */
void write_result_file(struct file_gen *input){
    FILE *output = fopen("hesla.csv", "a");
    fprintf(output, "%s:%s:", input->name, input->hashed_password);
    for(int i = 0; i < KEY_COUNT; i++){
        if(i + 1 == KEY_COUNT){
            fprintf(output, "%s", input->keys[i]);    
        }
        else{
            fprintf(output, "%s,", input->keys[i]);
        }
    }
    fprintf(output, "\n");
    fclose(output);
}

int main() {
    srand(time(NULL));
    

    FILE *file = fopen("info.txt", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while((read = getline(&line, &len, file)) != -1){
        char name[MAX_LENGTH];
        char password[MAX_LENGTH];
        char hashed_password[MAX_LENGTH];
        char *token = strtok(line, ":");
        strcpy(name, token);
        token = strtok(NULL, ":");
        strcpy(password, token);
        password[strlen(token)-1] = '\0';
        hash_string((const char *) password, hashed_password);
        
        struct file_gen output;   
        file_gen_init(&output, name, hashed_password);
        generateKeys(&output);
        write_result_file(&output);
        file_gen_destroy(&output); 
        
    }


   
    if(line)
        free(line);
    fclose(file);
    return 0;
}