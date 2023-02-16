### C++的输入与输出

头文件为<iostream>，一般用cout流输出、cin流输入，他们都在标准名称空间std内，所以要加上using namespace std。

```c++
#include <iostream>
using namespace std;

int main(){
    //endl是回车符号的意思，也可以用cout << "HelloWorld!\n"实现同样的效果
	cout << "HelloWorld!" << endl;
    return 0;
}
```

用cin输入，cout输出的样例：

```c++
#include <iostream>
using namespace std;

int main(){
    int a;
	cin >> a;
    //可以直接输出变量a
    cout << "You input a number a = " << a << endl;
    return 0;
}
```

### C++ 类 对象

类和C语言的结构体很像，只是多了public和private的字段（还有protect等，但在数据结构中不怎么用到），里面的函数、变量都叫做成员。

这样定义一个类：

```c++
class Student {
//私有成员，一般是变量
private:
	int id;
	string name;
//公有成员，一般为函数的定义，可以在类外具体实现函数的内容
public:
    //构造函数，在创建对象的时候执行，作用是初始化变量
    Student();
    Student(int id, string name);
    //析构函数，在对象删除的时候执行
    ~Student();
    //输出学生信息
	void printStudent();
    //设置学生id
	void setID(int id);
    //设置学生姓名
	void setName(string name);
    //获得学生id
	int getID();
    //获得学生姓名
	int getName();
}
//类外补充函数的内容，但需要有作用域符号::，原因如下：
//如果Student类和People都有setID()的公有函数，那补充定义的话就会出现二义性：即不确定你补充的setID函数是Student类还是People类。所以要用::符号确定函数是哪个类里面的。
Student::Student(){
    this->id = -1;
    this->name = "";
}
Student::Student(int id, string name){
    //this是指针，指向类内的成员，这里将参数赋值给类内的成员
    this->id = id;
    this->name = name;
}
Student::~Student(){
    
}
void Student::printStudent(){
    cout << "Name: " << this->name << endl;
    cout << "ID: " << this->id << endl;
}
void Student::setID(int id){
    this->id = id;
}
void Student::setName(string name){
    this->name = name;
}
int Student::getID(){
    return this->id;
}
int Student::getName(){
    return this->name;
}
```

在主函数中一般这样创建类的对象：

```c++
int main(){
    //默认构造，这样得到的stu1的id=-1、name=""
	Student stu1;
    //报错，因为id和name是私有成员，无法直接用点运算符获取数据
	cout << stu1.id << " " << stu1.name << endl;
    //正确，因为getID函数和getName函数是公有成员，这两个函数都返回了对应的id和name，输出-1和空的字符串
	cout << stu1.getID() << " " << stu1.getName() << endl;
    
    //含餐构造，就会执行Student(int id, string name)的构造函数
    Student stu2(123, "Jack");
    //输出123 Jack
    cout << stu1.getID() << " " << stu1.getName() << endl;
    
    return 0;
    //函数结束的时候，析构函数开始执行。
}
```

### string字符串

string是一种类，里面集成了字符串的各种用法。

使用string可以基本上抛弃char了。string是字符串，其基本用法与char大同小异

```c++
using namespace std;	//string在std空间内

...
    
string name;	//创建了string对象name
name = "Jack";	//字符串赋值
cout << name.size(); 	//输出4，name的长度
string id = "1234";

//string支持加法运算，其意义是字符串的拼接
cout << name + id << endl;	//输出Jack1234
cout << id + name << endl;	//输出1234Jack
string tmp = name + id; 	//tmp的内容就是Jack1234

//string支持大小比较，其标准是字典序
//比如abc与BDF的比较，"abc">"BDF"，这是因为'a'的ASCII大于'B'，那么后面的字符大小就不重要了
//又如"abcd"和"accd"，答案是"accd"大，这是因为第一个字符是相同的值，那么就比较第二个字符，谁的ASCII值大字符串就大。 具体可以百度一下string的字符串比较

if("abc" > "BDF") cout << "true";
if("abcd" > "accd") cout << "true";

//可以用string代替char[]，可以用string[]代替char[][]
string str;	//对应char str[] string的字符串是动态的，长度可以变化
string arr[10];	//对应char arr[10][]

//可以用数组的下标方法访问字符串中某一位置的元素
for(int i = 0; i < str.size(); i++){
    cout << str[i];
}
```

### vector动态数组

C++中使用new关键字创建动态数组，但并不是真正的动态。我们不能随意进行长度的变更。可以考虑std中的vector类，它是一个动态数组，操作起来非常方便，而且不需要我们进行析构。

```c++
int main(){
    //定义了一个空的动态数组，长度为0
	vector<int> arr;
    arr.push_back(0);
    arr.push_back(1);
    arr.push_back(2);
    //现在arr有了三个元素：0、1、2，长度为3
    cout << arr.size() << endl;	//输出长度
    
    //也可以定义一个已经有一定长度的数组，比如
    vector<int> brr(10);	//长度为10，元素全为0的数组
    vector<int> crr(10, -1);	//长度为10，元素全为-1的数组
    
    //利用动态数组创建矩阵（二维数组）
    int n = 3, m = 4;
    vector<vector<int>> mat(n, vector<int>(m));	//创建一个n*m的矩阵
}
```

### unordered_set 乱序集合

可以定义一个集合，集合中元素具有唯一性



### sort函数

sort函数基于快速排序，只要给定数组的首尾，就可以获得有序的序列

```c++
int main(){
	int arr[10];
	
	//对数组排序，这里数组长度是10
	sort(arr, arr+10);
    //这样得到的数组就是有序的
    
    vector<int> brr;
    //对于vector动态数组，同样也可以sort排序
    sort(brr.begin(), brr.end());
}
```

默认为升序，如果要使用降序排序，就得手写比较器：

```c++
bool cmp(int a, int b){
	return a > b;
}
int main(){
	int arr[10];
	
	//对数组排序，这里数组长度是10
	sort(arr, arr+10, cmp);
    //这样得到的数组就是有序的
    
    vector<int> brr;
    //对于vector动态数组，同样也可以sort排序
    sort(brr.begin(), brr.end(), cmp);
}
```

可以使用lambda匿名函数的技巧，将cmp“内嵌”到sort里：

```c++
int main(){
	int arr[10];
	
	//对数组排序，这里数组长度是10
	sort(arr, arr+10, [](int a, int b) -> bool {
        return a > b;
    });
    //这样得到的数组就是有序的
    
    vector<int> brr;
    //对于vector动态数组，同样也可以sort排序
    sort(brr.begin(), brr.end(), [](int a, int b){
        return a > b;
    });//也可以不提供推断类型，这样自动推导返回类型
}
```

sort函数可以基于结构体或者类排序，比如要按照Student结构体中的age从大到小排序：

```c++
struct Student{
	string name;
    int age;
};

int main(){
    Student stu[10];
    
    sort(stu, stu+10, [](Student a, Student b){
        return a.age > b.age;
    });
}
```

也可以按照如下规则排序：如果age不相等，按照age从大到小，如果age相等，按照name的字典序从大到小排序：

```c++
struct Student{
	string name;
    int age;
};

int main(){
    Student stu[10];
    
    sort(stu, stu+10, [](Student a, Student b){
        if(a.age == b.age) return a.name > b.name;
        return a.age > b.age;
    });
}
```

