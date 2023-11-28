#include <stdio.h>

int main(){
    FILE *file;
    char c;
    
    file = fopen("testfile.txt", "r");
    if(file == NULL){
        printf("cannot open file! \n");
        return 1;
    }
    
    while((c=fgetc(file))!=EOF) {
        printf("%c", c);
    }
    
    fclose(file);
    
    return 0;
}
