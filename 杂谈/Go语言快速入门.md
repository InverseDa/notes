## Hello Golang

```go
package main
import "fmt"

func main() {
    fmt.Print("Hello Golang")
}
```

这是最简单的Go程序，可以看到我们处于main包，并且导入了fmt包，里面有基本的函数比如Print函数，用于输出到控制台。

## 变量的定义

用var修饰定义一个变量，可以同时赋值也可以不初始化：

```go
var variable1 type = value
var variable type
```

这样写其实有些麻烦，但可以直接用:=运算符，这样就可以直接创建变量并赋值：

```go
variable := value
```

比如说：

```go
package main
import "fmt"

func main() {
    var sum int = 0
    var sum1 int
    
    pos := 123
    //共创建三个变量
}
```

一般来说，也可以用var()创建一系列的变量：

```go
var(
	sum int = 0
    sum1 int
)
```

## 常量的定义

利用const关键字即可：

```go
const constant type = value
```



## 基本数据类型

| 类型           | type    | 注释                                                         |
| -------------- | ------- | ------------------------------------------------------------ |
| 整数类型       | int     | 包括uintD和intD，D可取8、16、32、64。例如uint8、int32        |
| 单精度浮点类型 | float32 | 支持复数                                                     |
| 双精度浮点类型 | float64 | 支持复数                                                     |
| 字符串类型     | string  | 没有char类型，但比如有str这个字符串变量，那么str[pos]就是单个字节类型 |
| 指针           | *type   | 这语言居然有指针...                                          |

## 运算符

与C语言一样

## 条件语句

注意，一定要有大括号！

``` go
if condition {
    ...
}

if condition {
    ...
} else {
    ...
}

if condition1 {
    ...
} else if condition2 {
    ...
} else {
    ...
}
```

## 循环语句

只有for循环，但for循环有时候可以当作foreach循环和while循环使用：

```go
//将for当作while使用
for condition {
	...
}
//for的正统用法
for initial; condition; post {
    
}
//foreach循环，对数组、切片、字符串、map等结构可用。这里举map的例子
for key, value := range aMap {
    ...
}
//这是数组的例子
for value := range array {
    ...
}
```

## 函数

函数的定义：

```go
//函数关键字、函数名、参数...、返回值类型
func functionName(var1 type, ...) type {
	...
}
```

返回值类型可以不给，这样就是void的了

Go语言有个神奇的特性，那就是函数可以返回多个值（其实相当于C++里return了{}而已，说不上神奇，但C++的做法会麻烦些）

```go
package main

import "fmt"

func swap(x, y string) (string, string) {
   return y, x
}

func main() {
   a, b := swap("Google", "Runoob")
   fmt.Println(a, b)
}
```

这里简单介绍引用传递，其实就是C语言的指针传递。在Go语言中，没有引用这个东西了，只有指针。

```go
/* 定义交换值函数*/
func swap(x *int, y *int) {
   var temp int
   temp = *x    /* 保持 x 地址上的值 */
   *x = *y      /* 将 y 值赋给 x */
   *y = temp    /* 将 temp 值赋给 y */
}
```

至于这个函数带来的后果都知道。

## 指针

跟C语言类似：

```go
var ptr *int	//int指针
val := 32
ptr = &val		//&获取地址，ptr指向val的地址

var ptr2 *int
//用new分配内存空间（make也可以，但它只为map、切片等类型服务。基本数据类型就老实用new吧）
ptr2 = new(int)	//new(type)
*ptr2 = 24
```

不同的是，我们用nil关键字表示空指针（而不是nullptr了）

```
if ptr == nil {
	...
}
if ptr != nil {
	...
}
```

Go支持多级指针，最常见的是二级指针：

```go
package main

import "fmt"

func main() {

   var a int
   var ptr *int
   var pptr **int

   a = 3000

   /* 指针 ptr 地址 */
   ptr = &a

   /* 指向指针 ptr 地址 */
   pptr = &ptr

   /* 获取 pptr 的值 */
   fmt.Printf("变量 a = %d\n", a )
   fmt.Printf("指针变量 *ptr = %d\n", *ptr )
   fmt.Printf("指向指针的指针变量 **pptr = %d\n", **pptr)
}
```

## 数组

定义一维数组：

```go
var variable_name [SIZE] variable_type
```

```go
package main

import "fmt"

func main() {
   var n [10]int /* n 是一个长度为 10 的数组 */
   var i,j int

   /* 为数组 n 初始化元素 */        
   for i = 0; i < 10; i++ {
      n[i] = i + 100 /* 设置元素为 i + 100 */
   }

   /* 输出每个数组元素的值 */
   for j = 0; j < 10; j++ {
      fmt.Printf("Element[%d] = %d\n", j, n[j] )
   }
}
```

```go
package main

import "fmt"

func main() {
   var i,j,k int
   // 声明数组的同时快速初始化数组
   balance := [5]float32{1000.0, 2.0, 3.4, 7.0, 50.0}

   /* 输出数组元素 */         ...
   for i = 0; i < 5; i++ {
      fmt.Printf("balance[%d] = %f\n", i, balance[i] )
   }
   
   balance2 := [...]float32{1000.0, 2.0, 3.4, 7.0, 50.0}
   /* 输出每个数组元素的值 */
   for j = 0; j < 5; j++ {
      fmt.Printf("balance2[%d] = %f\n", j, balance2[j] )
   }

   //  将索引为 1 和 3 的元素初始化
   balance3 := [5]float32{1:2.0,3:7.0}  
   for k = 0; k < 5; k++ {
      fmt.Printf("balance3[%d] = %f\n", k, balance3[k] )
   }
}
```

二维数组也可以：

```go
var arrayName [ x ][ y ] variable_type
```

## 切片

当数组大小初始化为空的时候，这个数组就叫做切片（很像vector的赶脚）。其实就是个动态数组而已！

```go
var identifier []type
```

