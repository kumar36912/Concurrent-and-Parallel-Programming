#include <fcntl.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    const char* fifoName = "./myPipe1";
    int num;
    int fd = open(fifoName, O_RDONLY);              /** line 1 **/

    while (read(fd, &num, sizeof(num)) > 0)         /** line 2 **/ 
      printf("%i\n", num);
    close(fd);                                      /** line 3 **/
    unlink(fifoName);                               /** line 4 **/
    return 0;
}

