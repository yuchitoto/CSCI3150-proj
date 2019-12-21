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

## Preprocessor Using Examples

[Best Abuse of the C Preprocessor 1993](http://www.de.ioccc.org/years.html#1993_dgibson)

[Best Abuse of the C Preprocessor 1990](http://www.de.ioccc.org/years.html#1990_dg)

[Best Abuse of the C Preprocessor 2001](http://www.de.ioccc.org/years.html#2001_herrmann1)

[Worst abuse of the C preprocessor 1986](http://www.de.ioccc.org/years.html#1986_hague)

[Worst abuse of the C preprocessor 1992](http://www.de.ioccc.org/years.html#1992_lush)

[Worst abuse of the C preprocessor 1995](http://www.de.ioccc.org/years.html#1995_vanschnitz)

[Worst abuse of the C preprocessor 1996](http://www.de.ioccc.org/years.html#1996_schweikh1)

[Worst abuse of the C preprocessor 1985](http://www.de.ioccc.org/years.html#1985_sicherman)

[Worst abuse of the C preprocessor 1995](http://www.de.ioccc.org/years.html#1995_vanschnitz)

[Worst abuse of the C preprocessor 1994](http://www.de.ioccc.org/years.html#1994_westley)
