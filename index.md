
# 1. Quick demo
``` c++
int a = 17;
print(a*a+a);
```
output:

306

# 2. Fundamentals
# 2.1 variables
Variable declaration: 

syntax: int name;

Array declaration:

syntax: int[] name;
``` c++
int a;
int[] b;
```
Declaration with initialization:

syntax: int name = value;

syntax: int[] b = value;
```c++
int a = 17;
int[] b = foo();
```
Array operations:

name.push_back(value) method extends array with value.

name.size() method returns array size.


name[k] retrieves the value in k-th cell of the array or used in assignment name[k] = value.

```c++
	int[] b;
	b.push_back(1);
	print(b[0]) 	     	 //retrieve value in 0-th cell of array b
	b[b.size()] = b.size(); //assign value b.size() (here '1') to b.size()-th cell (here '1')
			       //equivalent to b.push_back(1)
	print(b)
```
output:

1

1 1


# 2.2 IF
if( condition ) { stmts; } 

(optionally) else { stmts; }
``` c++
int a = 5;
if(a != 5)
{
	print(a);
}
else
{
	print(a+1);
}

if(a == 5)
{
	print(a+2);
}
```
output: 

6

7

# 2.3 WHILE
while( condition ) { stmts; }
``` c++
int a = 5;
while(a < 10)
{
	print(a);
	a = a+1;
}
```
output:

5

6

7

8

9


# 2.4 functions
int name(variable/array declaration, ...,)
{
	stmts;
}

``` c++
int foo(int a, int[] b)
{
	b.push_back(a*a);
	return b;
}
int[] c;
c.push_back(2);
c = foo(3, c);
print(c);
```
output:

2 9

# 2.5 See examples at
https://github.com/rafpacut/uC/tree/master/programs/showcase
