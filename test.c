
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    // Allocate integer on the heap and set it to 42
    int *p = malloc(sizeof(int));
    if (!p) {
        perror("malloc");
        return 1;
    }
    *p = 42;

    printf("[Parent] Before fork, p=%d at address %p\n", *p, (void*)p);

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        free(p);
        return 1;
    } 
    else if (pid == 0) {
        // Child process
        printf("[Child ] After fork, sees p=%d at address %p\n", *p, (void*)p);

        // Child modifies the value
        *p = 1234;
        printf("[Child ] Modified p to %d (child only)\n", *p);

        // Exit child
        free(p);
        return 0;
    } 
    else {
        // Parent process
        // Wait for child to finish
        wait(NULL);

        // Check the parent's value after the child's modification
        printf("[Parent] After child, still sees p=%d\n", *p);

        free(p);
        return 0;
    }
}
