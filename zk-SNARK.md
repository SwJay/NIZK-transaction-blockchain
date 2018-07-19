# zk-SNARK

- it's a proof method, not applied to computational problems

- how to operate zk_SNARK: convert a computational problem into a quadratic arithmetic program (QAP), zk-SNARK can works on QAP

![1531883692397](C:\Users\Sunwe\AppData\Local\Temp\1531883692397.png)



##QAP

### eg. prove u know the answer to $x^3+x+5==35$ (which is 3)

#### step 1: flattening

function can be`func(x){return x^3 + x + 5}`

flatten it into statements like logic gates in a circuit:

```c
tmp1 = x * x
y = tmp1 * x // x^3
tmp2 = y + x
~out = tmp2 + 5
```

#### step 2: Gates to R1CS

##### bg: rank-1 constraint system (R1CS)

- an R1CS: a sequence of groups of three vectors`(a, b, c)`
- a solution to an R1CS: a vector `s` which satisfies the equation `s . a * s . b - s . c = 0`
  - eg.![1531884663698](C:\Users\Sunwe\AppData\Local\Temp\1531884663698.png)

##### gates to R1CS

- the length of vector is equal to the total number of variables used above, including:
  -  a dummy variable `~one` at the first index; 
  - a dummy variable `~out` for output;
  -  input variable `x`; 
  - intermediate variable `tmp1`, `tmp2`, `y`.
- vector: `'~one', 'x', '~out', 'tmp1', 'y', 'tmp2'`
- 1st gate:

```
a = [0, 1, 0, 0, 0, 0]
b = [0, 1, 0, 0, 0, 0]
c = [0, 0, 0, 1, 0, 0]
```

- 2nd gate:

```
a = [0, 0, 0, 1, 0, 0]
b = [0, 1, 0, 0, 0, 0]
c = [0, 0, 0, 0, 1, 0]
```

- 3rd gate:

```
a = [0, 1, 0, 0, 1, 0]
b = [1, 0, 0, 0, 0, 0]
c = [0, 0, 0, 0, 0, 1]
```

here checks `(x + y) * 1 == tmp2`

- 4th gate:

```
a = [5, 0, 0, 0, 0, 1]
b = [1, 0, 0, 0, 0, 0]
c = [0, 0, 1, 0, 0, 0]
```

- solution:
  - 带入x=3计算出一个对应的vector`[1, 3, 35, 9, 27, 30]`
- complete R1CS

```
A
[0, 1, 0, 0, 0, 0]
[0, 0, 0, 1, 0, 0]
[0, 1, 0, 0, 1, 0]
[5, 0, 0, 0, 0, 1]

B
[0, 1, 0, 0, 0, 0]
[0, 1, 0, 0, 0, 0]
[1, 0, 0, 0, 0, 0]
[1, 0, 0, 0, 0, 0]

C
[0, 0, 0, 1, 0, 0]
[0, 0, 0, 0, 1, 0]
[0, 0, 0, 0, 0, 1]
[0, 0, 1, 0, 0, 0]
```

#### step3: RICS to QAP

对a,b,c三组，取每一列作为y坐标，x坐标为1，2，3，4，做拉格朗日插值

eg. A第一列0, 0, 0, 5 -> 对(1,0) (2,0) (3, 0) (4, 5)进行插值

answer ( 系数升序排列，eg.第一个$-5+9.166x-5x^2+0.833x^3$ )

```
A polynomials
[-5.0, 9.166, -5.0, 0.833]
[8.0, -11.333, 5.0, -0.666]
[0.0, 0.0, 0.0, 0.0]
[-6.0, 9.5, -4.0, 0.5]
[4.0, -7.0, 3.5, -0.5]
[-1.0, 1.833, -1.0, 0.166]
B polynomials
[3.0, -5.166, 2.5, -0.333]
[-2.0, 5.166, -2.5, 0.333]
[0.0, 0.0, 0.0, 0.0]
[0.0, 0.0, 0.0, 0.0]
[0.0, 0.0, 0.0, 0.0]
[0.0, 0.0, 0.0, 0.0]
C polynomials
[0.0, 0.0, 0.0, 0.0]
[0.0, 0.0, 0.0, 0.0]
[-1.0, 1.833, -1.0, 0.166]
[4.0, -4.333, 1.5, -0.166]
[-6.0, 9.5, -4.0, 0.5]
[4.0, -7.0, 3.5, -0.5]
```

- sets above are QAP parameters, once generated, they can be reused for the same original function.
- evaluate at x -> f(x)
- 转化如下图![1531892045812](C:\Users\Sunwe\AppData\Local\Temp\1531892045812.png)
- x仅取点在给定的点，我们引入一个基本函数`z=(x-1)*(x-2)*...`，`t(x) = A(x)*B(x)-C(x)`若在给定点为零，则能表示成`t(x)=H*z(x)`的形式，即判断`t/z`能否整除

