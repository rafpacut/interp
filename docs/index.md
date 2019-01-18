
# 1. Quick demo
``` c++
int a = 17;
print(a*a+a);
```
output:

306

# 2. Fundamentals
# 2.1 variables
``` c++
int a = 5*10;
int[] b;
b.push_back(7+7*7);
b[b.size()] = 3;
print(a);
print(b);
```
output: 

50 

56 3

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

