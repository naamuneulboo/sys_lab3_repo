#include <stdio.h>

int main(){
    FILE *file;
    long pos;
    
    file=fopen("testfile.txt","r+");
    if(file == NULL){
        printf("cannot open file! \n");
        return 1;
    }
    
    fseek(file, 5, SEEK_SET);
    fprintf(file, "Everyone! \n");
    
    fclose(file);
    
    return 0;
}
