#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

/* ### For reference, the structure declaration:
struct stat {
   mode_t   st_mode;     // File mode (type, perms) 
   ino_t    st_ino;      // Inode number 
   dev_t    st_dev;      // containing device ID 
   dev_t    st_rdev;     // device ID 
   nlink_t  st_nlink;    // Number of links 
   uid_t    st_uid;      // User ID of the file's owner 
   gid_t    st_gid;      // Group ID of the file's group 
   off_t    st_size;     // File size in bytes 
   time_t   st_atime;    // Time of last access 
   time_t   st_mtime;    // Time of last data modification 
   time_t   st_ctime;    // Time of last file status change
                         // Times measured in seconds since 
                         // 00:00:00 UTC, Jan. 1, 1970 
   long     st_blksize;  // Preferred I/O block size 
   blkcnt_t st_blocks;   // Number of 512 byte blocks allocated
} 
*/

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: status <path to file>\n");
    return -1;
  }
  struct stat status;
  memset(&status, 0, sizeof(struct stat));
  printf("File name: %s\n", argv[1]);
  stat(argv[1], &status); /* file name, pointer to stat struct */    /** line 1 **/

  /* Get time of last access -- in several steps for emphasis */
  time_t my_time = status.st_atime;
  char buffer[1024];
  ctime_r(&my_time, buffer);

  /* Get status: directory or regular */
  char* type;
  if (S_ISREG(status.st_mode)) type = "regular";
  else if (S_ISDIR(status.st_mode)) type = "directory";
  else type = "other (neither regular nor directory)";

  /* ower read/write status */
  int readable = status.st_mode & S_IRUSR;
  int writable = status.st_mode & S_IWUSR;
  char* readO  = (readable) ? "Owner readable" : "Not owner readable";
  char* writeO = (writable) ? "Owner writable" : "Not owner writable";

  /** Print information. **/
  printf("\nInformation about file %s", argv[1]);
  printf(":\n\n");

  printf("Owner ID:    %lu\n", (unsigned long) status.st_uid);
  printf("Group ID:    %lu\n", (unsigned long) status.st_gid);
  printf("Byte size:   %lu\n", (unsigned long) status.st_size);
  printf("Last access: %s",    buffer);
  printf("File type:   %s\n",  type);
  printf("Owner read:  %s\n",  readO);
  printf("Owner write: %s\n",  writeO);

  return 0;
}
