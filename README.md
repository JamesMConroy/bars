# bars

Print a histogram to your terminal.

## Example

```
$ cat example
100 label
9 2nd
31 something
20 foo-bar

$ ./bars -c 80 < example
     label | ############################################################ | 100 
       2nd | ######                                                       | 9 
 something | ###################                                          | 31 
   foo-bar | ############                                                 | 20 
```

Inspired by [jez/bars](https://github.com/jez/bars)/[jez/barchart](https://github.com/jez/barchart), but made in c11.

# Issues

Plenty

- little to no input checking
- the program freezes if the input doesn't match what `scanf` is expecting
- doesn't handle floats

