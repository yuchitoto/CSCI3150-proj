# CSCI3150 2019 Final Project
You are not allowed to use auxillary functions in the macro, though you can still implement functions using preprocessors as wrappers.

## Implementation Concepts
### open_t()
You have pathname of /XXX/XXX this kind of things,

what you should do is handle each part between "/" one by one.

You can cut the first "/" out and use strcspn() to locate the remaining substring and use open_t recursively, in the way demonstrated below

```c
#define macro(a,b,c) func(a,b,c)

int func(int fd, int inode, char *pathname)
{
  /*code to find the dir from pathname*/
  return macro(fd, next_inode, remaining_pathname);
}

int open_t(char *pathname)
{
  /*some code to find current inode*/
  return func(fd, 0, pathname)
}
```

Or you can simply use strtok() and update inode repeatedly

Declaration: I did not use recursive algorithm to handle it

### read_t()
Simple arithmetic on file_size(inode->i_size)
