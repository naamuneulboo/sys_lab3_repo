#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

void print_directory(const char *dir, int depth) {
    DIR *pdir;
    struct dirent *pde;
    int i = 0;
    char path[1024];

    if ((pdir = opendir(dir)) == NULL) {
        perror("opendir");
        return;
    }

    while ((pde = readdir(pdir)) != NULL) {
        if (strcmp(pde->d_name, ".") == 0 || strcmp(pde->d_name, "..") == 0)
            continue;
        printf("%*s%s", depth*2, "", pde->d_name);
        if (pde->d_type == DT_DIR) {
            snprintf(path, sizeof(path), "%s/%s", dir, pde->d_name);
            print_directory(path, depth + 1);
        }
    }

    closedir(pdir);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: file_dir dirname");
        return 1;
    }

    print_directory(argv[1], 0);

    return 0;
}
