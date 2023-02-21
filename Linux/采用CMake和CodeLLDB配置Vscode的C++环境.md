## 为什么要选择CMake + Clangd + CodeLLDB？

Microsoft的C++插件补全比较垃圾，并且采用LLDB更好一些。这个配方是跨平台的（Windows要使用WSL2），所以大家可以随便cv配置。

需要你对系统比较熟悉，并且会一点点CMake的语法，反正不难。

## 安装插件

需要安装CMake、CMake Tools、CodeLLDB和Clangd的插件。

![image-20230216231520425](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/image-20230216231520425.png)

## CMake语法

在项目文件夹中编写CMakeLists.txt文件，这里给出最基本的语法：

```cmake
# 设置cmake的最低版本
cmake_minimum_required(VERSION 版本号)

## ===================================== set =====================================
# set语句用于赋值变量，可自定义变量也可以定义cmake变量，
# cmake变量可在官网查询。这里定义C++的版本为17版本
set(CMAKE_CXX_STANDARD 17)
# 定义变量ENV_VAL，其中ENV_VAL为系统变量ENV_VAL1的值
set(ENV_VAL $ENV{ENV_VAL_1})
# 定义变量VAL，值为123
set(VAL 123)

# Google Sanitizer用于内存检测，建议开启，防止UB找不到错误
# fsanitize的参数有address（ASAN检测内存）、undefined（检测
# UB）、leaks（检测内存未完全释放）
set(CMAKE_CXX_FLAGS"${CMAKE_CXX_FLAGS} 
					-fsanitize=address  
					-fno-omit-frame-pointer 
					-g3"
)

## ==================================== Project ====================================
# 定义项目的文件名称
project(Project)

## ================================= 头文件目录设置 =================================
# 一般来说，默认的头文件目录是系统的头文件目录或者编译器所在地头文件目录
# （比如Windows下的MSVC和MingGW），下面的指令可以添加自定义头文件目录
include_directories("${PROJECT_SOURCE_DIR}/include") #添加项目文件家内的include头文件夹

## ================================== 可执行程序 ===================================
# 这一部分生成可执行程序
add_executable(Project main.cpp)

## ================================= 添加第三方库 ==================================
# 需要你提前下载好第三方库的包，如果是Windows平台，推荐使用MSYS2，它集成了
# ArchLinux的包管理器pacman，下载库很方便
# CONFIG添加参数REQUIRED（将在找不到包的时候终止，推荐加上）
find_package(fmt CONFIG REQUIRED) # 寻找fmt库
target_link_libraries(Project PRIVATE fmt::fmt) #链接fmt到项目

```

下面是最简单的CMakeLists，只有一个main.cpp：

```cmake
cmake_minimum_required(VERSION 3.24.1)
project(Project)

add_executable(Project main.cpp)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(Project PRIVATE fmt::fmt)
```

文件树：
├── CMakeLists.txt
└── main.cpp

## 使用CMake构建并运行项目

写好CMakeLists之后，就可以构建项目了。使用`Ctrl + Shift + P`，输入CMake，选择`CMake 快速入门`：

![image-20230217002646714](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/image-20230217002646714.png)

然后按需要选择编译器：

![image-20230217002721871](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/image-20230217002721871.png)

在底部点击Build构建：

![image-20230217003131253](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/image-20230217003131253.png)

构建好后，可以直接运行，在底部点击▶️：

![image-20230217003414570](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/image-20230217003414570.png)

## 使用LLDB调试

我们有时候需要调试程序，所以需要用到LLDB。按`F5`，会自动创建`./vscode/launch.json`文件，在这个文件中配置启动LLDB的参数：

![image-20230217003611989](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/image-20230217003611989.png)

由vscode的cmake文档（https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/debug-launch.md）和codelldb的文档（https://github.com/vadimcn/vscode-lldb/blob/v1.8.1/MANUAL.md）可以知道，我们需要修改`program`为`${command:cmake.launchTargetPath}`和`type`为`lldb`：

```json
{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "type": "lldb",
            "request": "launch",
            "name": "Debug",
            "program": "${command:cmake.launchTargetPath}",
            "args": [],
            "cwd": "${workspaceFolder}"
        }
    ]
}
```

最后按`F5`来启动LLDB：

![image-20230217004123227](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/image-20230217004123227.png)

可以调试了。

