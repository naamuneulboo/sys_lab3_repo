#include <stdio.h>

int main(){
    FILE *file;
    
    file = fopen("testfile.txt","w");
    if(file==NULL){
        printf("cannot open file!\n");
        return 1;
    }
    
    fprintf(file, "Hello, World! \n");
    
    fclose(file);
    
    return 0;
}
