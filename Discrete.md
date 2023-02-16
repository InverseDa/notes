# **网络流建模应用于匹配问题的求解**

<center><div style='height:2mm;'></div><div style="font-family:华文楷体;font-size:12pt;">缪克达，钟朗坤，庞栋文</div></center>
<center><div style='height:2mm;'></div><div style="font-family:华文楷体;font-size:10pt;">负责人邮箱：miaokeda@qq.com</div></center>
<center><span style="font-family:华文楷体;font-size:9pt;line-height:9mm">深圳大学，广东省，中国</span>
</center>
<div>
<div style="width:52px;float:left; font-family:方正公文黑体;"><b>摘　要：</b></div> 
<div style="overflow:hidden; font-family:华文楷体;">对于水路通路流量、论文审理分配、职员假日分配等问题，我们可以采用在离散数学中图论的相关知识来求解。本文将介绍求解以上问题的基本模型——网络流，给出其基本的理论。然后会介绍Ford-Fulkerson方法、Edmonds-Karp算法和Dinic算法。最后会利用最大流的算法来解决医生的假期分配问题，效果不错。</div>
</div>
<div>
  <div style="width:52px;float:left; font-family:方正公文黑体;"><b>组　员：</b></div> 
<div style="overflow:hidden; font-family:华文楷体;">2020274045缪克达——论文撰写与建立数学模型
  <br/>
  2020111037庞栋文——代码手与数据测试
  <br/>
  2021155033钟朗坤——论文撰写与审查
  </div>
</div>
<div>
<div>
  <div style="width:52px;float:left; font-family:方正公文黑体;"><b>关键词：</b></div> 
<div style="overflow:hidden; font-family:华文楷体;">计算机；离散数学；图论；网络流；最大流；</div>
</div>






## 引言

​		网络流在现实中有诸多应用，比如职位分配、假期分配等问题，其本质都是二分图匹配问题。而二分图匹配其实就是网络流问题。早在上世纪70至80年代，计算机科学家们就已经为解决此问题提出了多种方案，其中最有名的当属Ford-Fulkerson方法，其最基本的FORD-FULKERSON算法是网络流求解的第一战。但由于时间复杂度为$O(|E||f|)$（对于邻接表和链式前向星而言），复杂度较高，当$|E|$在上百级别的时候，该算法就无法在人类有限的时间内求解出来了。后来的学者对此问题进行优化，其中Edmonds-Karp和Dinic算法是较为简单且时间复杂度相对比较优的算法。

​		网络流问题是计算机科学界的研究方向之一，在几十年来，最优的算法——由Goldberg和Rao所提出的算法，其时间复杂度为$O(\min(|V|^{2/3},|E|^{1/2})|E|\lg(|V|^2/|E|+2)\lg C)$，其中$C=\max_{(u,v)\in E}[c(u,v)]$。他摒弃了传统的重贴标签方法，而是采用了基于对块流的寻找。$^{[1]}$不过这项纪录在2022年被Li Chen、Rasmus Kyng、Yang P. Liu、Richard Peng、Maximilian Probst Gutenberg和Sushant Sachdeva所突破，他们基于Min-Ratio Cycles方法提出了近似线性时间解的算法。网络流问题有了重大的突破。$^{[2]}$

​		本文将从最基本的网络模型讲起，然后讲解基于Ford-Fulkerson方法的FORD-FULKERSON、Edmonds-Karp和Dinic算法。

<div style="page-break-after:always"></div>

## 残存网络和网络流

### 网络和网络流

​		网络是一种有向图，即边具有方向。设网络为$G=(V,E)$，其中$V$为网络$G$的平面点集，$E$为网络$G$的边集。一般来说，$E\subseteq V\times V$。并且$\forall (u,v)\in E,c(u,v)\ne 0$，这里$c(u,v)$是一种二元映射，也就是边的权值，一般有$c(u,v)=w,\ w\in(u,v)$。为了解决网络流问题，我们引入了超级源点$S$和超级汇点$T$，如图1所示。

<center><strong>图 1  一种典型的网络流（流/边权）</strong></center>

<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119195043504.png" alt="image-20221119195043504" style="zoom: 33%;" />

​		表示网络的方法一般有邻接矩阵、邻接表和网上流传的链式前向星。在本文我们主要讨论前两种表示方法。一般而言，设$\bold{adjmat}$为邻接矩阵，$\rm{adjlist}$为邻接表。

​		为了引入网络流的概念，我们介绍网络流的流映射$f$。

### 流映射$f$

​		网络$G$对应的流$f$是一种映射，定义如下：
$$
f(u,v)=f(e),\ \bold{if(u,v)\in E}
$$
​		所以$f$一般来说是一种二元映射，但当$\exist (u,v)\in E\and e=(u,v),\ e\in E$，映射$f$就塌缩到一元映射了，即$f(u,v)=f(e)$。流映射$f$的概念是表示当前边$(u,v)$的流。为了简单期间，我们用$|f|$表示流的大小。$^{[3]}$

​		对于流映射$f$，必须满足以下定理：

**定理 1 容量限制：对于$G$，必须有**
$$
0\le f(u,v)\le c(u,v)
$$
​								**即流$f(u,v)$不能超过最大容量$c(u,v)$。**

**定理 2 流量守恒：对于$G$，必须有**
$$
\sum_{v\in V}f(v,u)=\sum_{v\in V}f(u,v)
$$
​								**即$\forall v\in V \and w \rightarrow v\rightarrow u, f(w,v)=f(v,u)$，流进等于流出。**

### 残存网络和增广路径

​		网络模型并不能完美解决网络流问题。这是因为网络的流在表现上并没有优良的可观性，即我们难以知道某条边$(u,v)$是否能通入最大流。为了提高可观性，我们引入残存网络的概念。

​		设$G=(V,E)$是网络，$G_f=(V,E_f)$为残存网络，其中$E_f$为残存边集合。残存网络是一种完全有向图，这就意味着$E_f=V\times V$。$G$和$G_f$本质上的区别就是$G_f$多了残存边。

<center><strong>图 2  网络和残存网络</strong></center>

![image-20221119014055439](/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014055439.png)

​		残存边是指一条已存在流量的边，比如边$e=(u,v)$的流量为6，容量为17，那么他的残存边就是$e^R=(v,u)$，其权值为6。其意义是，这条边还可以通过大小为6的流，其表达式如下：
$$
c_f(e)=\begin{cases}c(e)-f(e) & e\in E \\f(e) & e^R\in E\end{cases}
$$
​		基于残存边，就可以构造出增广路径，这是一种从超级源点$S$到超级汇点$T$的简单路径。

<center><strong>图 3  网络流和残存网络</strong></center>

<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014105813.png" alt="image-20221119014105813" style="zoom: 50%;" />

​		可以看出可观性增加了。

​		于是我们将计算网络流的问题就转换到了**寻找增广路径问题**。对于任意的网络，寻找路径的最基本的方法无非就是**Depth First Search**和**Breadth First Search**。

## Ford-Fulkerson方法

​		Ford-Fulkerson方法并不是一种算法，而是一种理论上的方法，但这种方法延伸出了很多性能不错的算法。

​		Ford-Fulkerson方法思路是从超级源点开始搜索路径，遇到超级汇点就结束。中间过程中会不断增加路径上的流量。这就是Ford-Fulkerson方法的精髓。常见的搜索增广路径的算法有经典的DFS搜索（也就是**FORD-FULKERSON算法**）、**Dinici算法**和**Edmonds-Karp算法**（优化后的FORD-FULKERSON算法，采用广度优先搜索BFS搜索增广路径）。

## FORD-FULKERSON算法

### 思路及伪代码

​		FORD-FULKERSON算法的基本思想，就是利用**深度优先搜索DFS**，寻找增广路径，然后尽可能增加增广路径上的流。所以我们不难得到它的伪代码，其思路就是，遍历每个边$(u,v)\in E$，更新流$(u,v)$来计算$G$的最大流。如果边$(u,v)\notin E$，则假设$f(u,v)=0$，即这条边的流为0。利用while循环，条件是存在一条从源点到汇点的增广路径。然后使得这条增广路径$p$的容量为这路径上边容量的最小值，这样才能是可行流：
$$
c_f(p)=\min \{c_f(u,v):(u,v)\in p\}
$$
​		简单描述下FORD-FULKERSON算法的过程，假设有这样的网络流：

<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014122924.png" alt="image-20221119014122924" style="zoom:50%;" />

​		其残存网络：

<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014129696.png" alt="image-20221119014129696" style="zoom:50%;" />

​		需要注意的是，邻接表要存储的是其残存网络。

​		第一步，DFS搜索，寻找一条增广路径，每次都更新最小容量的大小：

<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014250238.png" alt="image-20221119014250238" style="zoom:50%;" />

​		在这个增广路径上，最小容量为1，来自$(2,3)$这条边。因此我们通入大小为1的流，同时更新残存边的数据：

<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014256850.png" alt="image-20221119014256850" style="zoom:50%;" />

​		接着利用DFS搜索寻找第二条增广路径：

<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014706476.png" style="zoom:50%;" />

​		在这个增广路径上，最小容量为1，来自$(1,2)$这条边。因此我们通入大小为1的流，同时更新残存边的数据：

<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014657306.png" style="zoom:46%;" />

​		利用DFS搜索寻找第三条增广路径：

<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014649058.png" style="zoom:50%;" />

​		在这个增广路径上，最小容量为3，来自$(1,3)$这条边。因此我们通入大小为3的流，同时更新残存边的数据：

<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014642764.png" style="zoom:50%;" />

​		利用DFS搜索寻找第四条增广路径：

<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014631769.png" style="zoom:50%;" />

​		在这个增广路径上，最小容量为0，来自$(2,3)$这条边。因此不通入流。

​		利用DFS搜索寻找第五条增广路径：

<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014542934.png" alt="image-20221119014542934" style="zoom:50%;" />

​		在这个增广路径上，最小容量为2，来自$(2,4)$这条边。因此我们通入大小为2的流，同时更新残存边的数据：

<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014535424.png" alt="image-20221119014535424" style="zoom:50%;" />

​		至此我们所有的增广路径都找完了，因此最大流是4+3=7。

​		于是，FORD-FULKERSON算法的伪代码如下：

```c++
Ford-Fulkerson(G, s, t):
//遍历所有边，初始化他们的流量都为0
for edge(u,v) in EdgeSet
	(u,v).f = 0
//寻找增广路，当增广路存在的时候
while p is exist
    //设置增广路的最大容量，为路径上边容量的最小值
	cf(p)=min{cf(u,v) in p}
	//遍历在增广路上的边
	for edge(u,v) in p
        //如果这个边是属于边集的，那么通入流量
		if(u,v) in EdgeSet	(u,v).f=(u,v).f + cf(p)
        //残存边减少流量
		else (v,u).f = (v,u) - cf(p)
```

​		可以简单描述为，通过寻找增广路径，然后通入路上允许的最大容量的流，然后再次寻找下一个增广路...直到所有的增广路都找完了，循环结束，输出最大流。

### 时间复杂度

​		FORD-FULKERSON算法的运行时间，绝大部分取决于算法是如何寻找增广路的。并且，如果增广路寻找的不好，那么最终这个流的值会极慢的收敛于最大流，这就注定了FORD-FULKERSON算法在一般的情况下不太可能是多项式级别的运行时间。而如果采用的是邻接表的存图方式，DFS的搜索时间为$O(|V|+|E|)$。所以对于FF算法的时间复杂度，我们采用上界分析。

​		假设网络流$G=(V,E)$的最大流为$|f|$，那么循环中至多执行$|f|$次，这是因为每次流至少增加一个单位，上限就是$|f|$。又因为DFS的搜索时间为$O(|V|+|E|)$，所以FF算法的时间上界为：
$$
T(n)=O(|f|)\times O(|V|+|E|)=O(|V||f|+|E||f|)\sim O(|E||f|)
$$
​		至于为什么放缩至$O(|E||f|)$，这是因为残存图是一种典型的稠密图，边多点少，就会存在：
$$
\lim_{t\rightarrow +\infty} \frac{O(|V||f|+|E||f|)}{O(|E||f|)}=1
$$
​		所以，$O(|V||f|+|E||f|)$是$O(|E||f|)$的等价无穷大，可以用$O(|E||f|)$代替之。但从精度来讲，$O(|V||f|)$是不可以忽略的一部份。

​		也就是说，FORD-FULKERSON算法的时间复杂度为$O(|E||f|)$，由于$|f|$不定，于是这个算法不是多项式级别的。$^{[4]}$

> 《算法导论》中推导出了FORD-FULKERSON算法的时间复杂度为$O(|E||f|)$，但并未解释是采用何种搜索方式。这是错误的，因为不同的搜索方式，不同的存图方式得到的搜索时间必不相同。如果采用的是邻接矩阵存图，那么DFS的搜索时间为$O(|V|^2)$，那么整体的搜索时间就是：
> $$
> O(|V^2|)\times O(|f|)=O(|V|^2|f|)
> $$
> 所以不能断言FORD-FULKERSON算法的时间复杂度一定就是$O(|E||f|)$，要先说明存图方式以及搜索方式。
>
> 这里给出DFS和BFS对于不同图的存储方式所使用的搜索时间，可以看到邻接矩阵和邻接表大致相同：
>
> | 存图方式 | **DFS**      | **BFS**      |
> | -------- | ------------ | ------------ |
> | 邻接矩阵 | $O(|V|^2)$   | $O(|V|^2)$   |
> | 邻接表   | $O(|V|+|E|)$ | $O(|V|+|E|)$ |
>
> ## Edmonds-Karp算法
>
> ### 思路及伪代码
>
> ​		FORD-FULKERSON算法是直接用DFS暴力搜索，比较慢。并且它并没有按照最短路径的方式寻找增广路径（这里的最短路径并不是按照权值划分的，而是按照超级源点到超级汇点的最少边划分的），因此在很多时候都会出现超时的现象。所以我们有了第一种优化思路，也就是采用BFS搜索算法，以最短路径的方式搜索增广路径，然后增加流量。这种算法叫做Edmonds-Karp算法。大致思路与FORD-FULKERSON算法一样，只是从DFS改成了BFS，所以伪代码一致。但从递归模式到非递归模式，还是有很多不同的。最重要的是，Edmonds-Karp算法的时间是多项式级别的。
>
> ​		不用DFS而用BFS的原因是因为BFS可以在残存图中寻找一个最短的路。这取决于BFS独有的特性：广度优先搜索。他采取队列的方式，先遍历队头的顶点，再遍历队头顶点所连接的下一个顶点，周而复始，最先到$T$的这条路就是最短路。因此BFS默认搜索出来的增广路径就是最短的路。但需要注意的是，当每个边的代价相同的时候，BFS才能搜索到最短路。（明显符合，这是因为我们以增广路径上边的个数作为代价，每个边的个数明显为1）
>
> ​		在这个算法中我们要记录每个顶点的父亲信息，也就是顶点的上一个顶点是何者。然后还需要一个流量数组，模拟增广路径上的流量情况：
> $$
> flowArray[v]=\min\{flowArray[u], \bold{adjmat[u][v]}\}
> $$
> ​		$flowArray[v]$的含义是，从$S\leadsto v$的增广路径中，边流量的最小值。他应该等于$\min\{flowArray[u], \bold{adjMat[u][v]}\}$，也就是等于$S\leadsto u$的最小边流量与从$u\rightarrow v$的流量中的最小值。更直观的表示：
> $$
> (S\leadsto u \rightarrow v).f = \min\{(S\leadsto u).f \ , (u\rightarrow v).f \}
> $$
> ​		通过BFS，最终$flowArray[T]$就是一条增广路径中最小边流量的值。
>
> ​		因为Edmonds-Karp算法不能像FORD-FULKERSON算法中递归改变边的流量，所以在EK算法中记录的每一个点的匹配点，通过匹配点来改变边的流量。也就是定义一个$father[|V|]$的数组，使得$father[i]$为$i$的后继顶点。这样才能修改流量信息。也就是利用并查集回溯顶点，边回溯边修改他们的流量信息。
>
> ### 时间复杂度
>
> ​		Edmonds-Karp算法采用BFS搜索，因此找到的增广路径都符合最短路径，这是Edmonds-Karp算法中非常重要的一点。这一点足以使得从不定式复杂度降至多项式复杂度。这里给予证明：
>
> **命题：Edmonds-Karp算法的时间复杂度为$O(|V||E|^2)$**
>
> **证明：**对于$u,v\in V$，如果增广路径$p$上的残存容量等于$(u,v)$边上的容量（即$c_f(p)=c_f(u,v)$），那么边$(u,v)$就是增广路径$p\in P$的**关键边**。
>
> <center><strong>图 4  虚线中间有诸多个顶点</strong></center>
>
> <img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014811849.png" alt="image-20221119014811849" style="zoom:50%;" />
>
> ​		在沿着$p$增加对应的流之后，关键边一定会从残存图中消失。而且，任何一条增广路径中，至少会存在一条关键边。
>
> ​		由于增广路径都是最短路径，那么当边$(u,v)$成为关键边的时候，就有：
> $$
> \delta_f(s,v)=\delta_f(s,u)+1
> $$
> ​		其中$\delta_f(u,v)$为边$(u,v)$的最短路径，上式成立的原因在于，增广路为最短路，$v$是$u$的后继顶点，因此他们两者的路长相差1.
>
> ​		由于$(u,v)$为关键边，那么当对流增加之后，边$(u,v)$就会在残存图中消失，转而这条边的所有流都组成了$(v,u)$。即$f(u,v)=0$，$f'(v,u)=c(u,v)$。这个时候，增广路$p$就不连通了！
>
> <img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014822007.png" alt="image-20221119014822007" style="zoom:50%;" />
>
> ​		除非对流$(v,u)$减小，使得$(u,v)$重现在残存图中，否则$(u,v)$将不会出现在其他的增广路上。但注意的是，对流$(v,u)$有可能出现在其他的增广路径上。
>
> ​		设从$(u,v)$在残存图消失到其对流$(v,u)$出现在另外一条增广路上的这一事件发生之后流为$f'$，则：
> $$
> \delta_{f'}(s,u)=\delta_{f'}(s,v)+1
> $$
> ​		用图表示：
>
> <img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221214014006191.png" alt="image-20221214014006191" style="zoom: 40%;" />
>
> ​		存在这样的图，使得边$(v,u)\in P$，其中$P$是图的增广路径集合
>
> ​		从图中不难看出：
> $$
> \delta_{f'}(s,u)=\delta_{f'}(s,v)+1 \ge \delta_{f}(s,v) + 1
> $$
> ​		又因为
> $$
> \delta_{f}(s,v)=\delta_{f}(s,u) +1
> $$
> ​		则整理得到：
> $$
> \delta_{f'}(s,u)\ge\delta_{f}(s,u) +2
> $$
> ​		也就是说，从边$(u,v)$成为关键边到下一次$(u,v)$或者$(v,u)$成为关键边，$s$到$u$的距离至少增加了2个单位，而最小距离的上界是$|V|-1$，设$(u,v)$或者$(v,u)$成为关键边的次数为$n$，每次成为关键边至多使得最短距离增加2个单位，于是可以作这样的不等式：
> $$
> \delta(s,u) + 2n \le |V| - 1
> $$
> ​		化简得到：
> $$
> n\le\frac{|V|-1}{2}-\delta(s,u)\le|V|/2
> $$
> ​		即$n=O(|V|)$。
>
> ​		一共有$O(|E|)$对顶点可以在残存图中有点相连接，所以关键边的总数就是：
> $$
> O(|E|)\times O(|V|)=O(|V||E|)
> $$
> ​		Edmonds-Karp算法首先遍历了边集，因此这一部分花费的时间为$O(|E|)$，综上所述，最终的时间复杂度为：
> $$
> T(n)=O(|E|)\times O(|V||E|)=O(|V||E|^2)
> $$
>
> ## Dinic算法
>
> ### 思路及伪代码
>
> ​		Dinic算法是Edmonds-Karp算法的进一步优化，采取的策略是：BFS对图进行分层，DFS找增广路径。为什么要这么做呢？我们还是从一个简单的图来解释：
>
> <img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014848067.png" alt="image-20221119014848067" style="zoom: 50%;" />
>
> ​		在以往的策略中，我们都是一个个寻找增广路径的。但当一张图特别大的时候，这样做未免效率太低了。于是我们可以试着采用**同时搜索**的策略。我们试着对这个图分层：
>
> <img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014855967.png" alt="image-20221119014855967" style="zoom: 50%;" />
>
> ​		这样就可以在每一层同时进行搜索，达到多路增广的效果，效率就高起来了。
>
> ​		具体过程：
>
> 1. 通过BFS进行分层，但注意的是，只有能通入流的情况下才能分下去！
> 2. DFS搜索增广路，按照BFS所给出的分层顺序搜索
> 3. 找到增广路之后，修改残存边路上的流量
> 4. 继续重复1的步骤，当1执行不下去的时候，结束循环
>
> ​		我们用深度数组$deep[|V|]$表示层数关系，其含义如下：
> $$
> deep[v]=\begin{cases}
> deep[u]+1 &if\ deep[v]= -1且f(u,v)>0\\
> -1 &其他
> \end{cases}
> $$
> ​		如果$(u,v)$可以增加流，并且$deep[v]\ne -1$，那么$v$就是$u$的下一层。用BFS就可以实现分层，其原因于最短路类似，BFS采用的是广度优先，按照队列的形式先进先出，形式上遍历出的顶点就有一定的层次性。
>
> ​		其边界条件就是$deep[T]=-1$，如果等于-1，那么就说明分层不到汇点，残存图中存在不流通的情况。一般来说，这样就可以认为我们把所有的增广路找到了。
>
> ### 时间复杂度
>
> ​		Dinic算法和EK算法的时间复杂度分析很相似。
>
> ​		首先BFS建立分层图，因为遍历了边集，因此这一部分的用时为$O(|E|)$。
>
> ​		在分层图中，执行了单次的DFS，因此用时就是不是朴素DFS的$O(|V|^2)$，而是$O(|V|)$。
>
> ​		而最重要的就是建立分层图的次数有多少次，实际上，分层图的建立次数至多$O(|V|)$次。而证明方法我们在EK中已经强调过了，就是利用最短路径的不等式来证明。这是因为Dinic算法采用的是BFS生成分层图。
>
> ​		由前面的Edmonds-Karp算法时间复杂度证明可以知道：
> $$
> \delta_{f'}(s,u)\ge\delta_{f}(s,u) +2
> $$
> ​		于是分层次数：
> $$
> n\le\frac{|V|-1}{2}-\delta(s,u)\le|V|/2
> $$
> ​		即$n=O(|V|)$
>
> ​		综上所述，时间复杂度为：
> $$
> T(n)=O(|E|)\times O(|V|) \times n=O(|V|^2|E|)
> $$
>
> ## 网络流的应用—假期分配和调度
>
> ### 问题分析
>
> ​		常见的应用之一，就是假期的调度问题，可以适用于多个企业。
>
> ​		在网络流$G=(V,E)$中，存在一个医生的集合$Y$，满足$y_i\in Y$；存在假期集合$D_j$，满足$D=\bigcup_{j=1}^kD_j$，$G$中显然要表示出$Y\rightarrow D$的流关系。因此我们定义一个超级源点$S$，它将指向集合$Y$的所有元素，其边容量表示医生至多值班的天数，$Y$将指向$D$，其边容量表示医生在同个假期最多值班的天数，$D$将指向$D_j$的所有元素$C(D_j)$（即假日），其边容量为医生至多值班的天数，最终$D_j$都流到超级汇点$T$，因为每个医生都有值班的机会，所有边容量始终为1.
>
> | 超级源点$S$                                              | $D$                                                          | $Y$                                                          | The content of $D_j$                       | 超级汇点$T$    |
> | -------------------------------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------ | -------------- |
> | 抽象而来的顶点，边$S\rightarrow Y$表示医生至多值班的天数 | 假期集合，边$D\rightarrow C(D_j)$表示医生在同一个假期至多值班的天数 | 医生集合，边$Y\rightarrow D$表示医生在同一个假期至多值班的天数 | 假日，其边$C(D_j)\rightarrow T$的容量总为1 | 抽象出来的顶点 |
>
> ​		其$G$如图所示： 
>
> ![image-20221119014916550](/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014916550.png)
>
> ​		引入在Dinici算法中BFS的分层思想。超级源点为第一层，医生为第二层，假期为第三层，假日为第四层，超级汇点为第五层。
>
> ​		可以知道，最后的解与下列这几个变量有关：
>
> |                            |
> | :------------------------: |
> |   医生至多值班的天数$c$    |
> |     医生的个数$doctor$     |
> | 假期的个数$holiday*doctor$ |
> |    假日的个数$dateNum$     |
>
> ​		我们输入如下的数据简单验证算法的正确性：
>
> <img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014928307.png" alt="image-20221119014928307" style="zoom:50%;" />
>
> ​		其残存图如图
>
> <img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014937678.png" alt="image-20221119014937678" style="zoom:50%;" />
>
> ​		结果正确。
>
> ### 效率比较
>
> #### 固定医生100位，假期10个，假日10个，控制$c$值改变
>
> | $c$值 | FORD-FULKERSON算法 | Edmonds-Karp算法 | Dinic算法 |
> | ----- | ------------------ | ---------------- | --------- |
> | 5     | 1.03144ms          | 0.4473ms         | 0.15751ms |
> | 10    | 1.61015ms          | 0.27106ms        | 0.12818ms |
> | 15    | 1.43498ms          | 0.24822ms        | 0.11249ms |
> | 20    | 1.42684ms          | 0.25952ms        | 0.10864ms |
> | 25    | 1.55087ms          | 0.25779ms        | 0.12751ms |
> | 30    | 1.73115ms          | 0.27679ms        | 0.12797ms |
>
> 其顶点/边数的比值：
> $$
> \lambda = \frac{122}{220} =0.56
> $$
> 可以看出，$c$值的变化并不会显著影响这三个算法的时间，其数学期望较为稳定。
>
> <img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014947064.png" alt="image-20221119014947064" style="zoom:50%;" />
>
> #### 固定$c$值为5，改变医生的个数，假期和假日依然为10个
>
> | Worker | FORD-FULKERSON算法 | Edmonds-Karp算法 | Dinic算法 |
> | ------ | ------------------ | ---------------- | --------- |
> | 10     | 0.07039ms          | 0.0282ms         | 0.01216ms |
> | 50     | 0.37059ms          | 0.13586ms        | 0.0611ms  |
> | 100    | 0.85723ms          | 0.28545ms        | 0.11618ms |
> | 150    | 1.41952ms          | 0.43312ms        | 0.19876ms |
> | 200    | 2.23742ms          | 0.58542ms        | 0.25588ms |
>
> <img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119014956520.png" alt="image-20221119014956520" style="zoom:50%;" />
>
> #### 固定$c$值为5，固定医生的个数为100，改变假期与假日比
>
> | 假期/假日 | FORD-FULKERSON算法 | Edmonds-Karp算法 | Dinic算法 |
> | --------- | ------------------ | ---------------- | --------- |
> | 0.25      | 9.3616ms           | 0.74879ms        | 0.31917ms |
> | 0.5       | 10.5884ms          | 1.51565ms        | 0.60017ms |
> | 1         | 12.5005ms          | 2.83768ms        | 1.23248ms |
> | 1.5       | 15.9862ms          | 3.77311ms        | 1.75846ms |
> | 2         | 27.2151ms          | 4.77643ms        | 2.36156ms |
>
> <img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221119015003780.png" alt="image-20221119015003780" style="zoom:50%;" />
>
> ## 结论
>
> ​		通过上面的分析，我们总结了网络流的基本解法：FORD-FULKERSON算法、 Edmonds-Karp算法和Dinic算法。除了FORD-FULKERSON算法不具备多项式时间解，其余的算法都是多项式时间解。最后引入了假期调度问题，来对网络流应用的一个举例。
>
> ​		三种算法的时间复杂度为：
>
> |            | FORD-FULKERSON算法 | Edmonds-Karp算法 | Dinic算法     |
> | ---------- | ------------------ | ---------------- | ------------- |
> | 时间复杂度 | $O(|E||f|)$        | $O(|V||E|^2)$    | $O(|V|^2|E|)$ |
>
> ​		从应用的结果可以看出，FORD-FULKERSON在任何情况下都是效率最低的，而Dinic算法一般而言效率很快。
>
> 
>
> **参考文献:** 
>
> [ 1 ] [ 4 ]  算法导论.Thomas H.Cormen[j], Charles E.Leiserson[j]（第三版）2013.1，机械工业出版社
> [ 2 ]         Maximum Flow and Minimum-Cost Flow in Almost-Linear Time. Li Chen、Rasmus Kyng、Yang P. Liu、Richard Peng、Maximilian Probst Gutenberg and Sushant Sachdeva. rXiv:2203.00671v2 [cs.DS] 22 Apr 2022.
> [ 3 ]         Wikipedia contributors. Maximum flow problem Wikipedia, 4 August 2022, at 05:19 (UTC).[2022-08-04]. https://en.wikipedia.org/wiki/Maximum_flow_problem
>
> 
>
> <!--**自我评价**--> 
>
> ​		<!--这次大作业时间紧凑，再加上本来我们三个人的作业就特别多，所以这次的大作业对我们来说都是比较艰难的体验，但幸好我们都写完了。无论结果如何，我们都会觉得这次的大作业是非常有意义的。果然团队大于一切，我们的有序的分工才能使得作业在周五到周日之内完成。并且我们缪克达同学和庞栋文同学学过算法设计，因此对于这次大作业的选题还是颇有信心的！-->
>
> 
>
> <!--**诚信承诺**--> 
>
> <!--本组成员郑重承诺在项目实施的过程中不发生任何不诚信现象，一切不诚信所导致的后果均由本组成员承担。-->
>
> ​																																	<!--签名： <img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221120123648570.png" alt="image-20221120123648570" style="zoom:50%;" />-->
>
> ​                    																																<!--<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221120123716486.png" alt="image-20221120123716486" style="zoom:50%;" />-->
>
> ​																																		<!--<img src="/Users/miaokeda/Library/Application Support/typora-user-images/image-20221120123732299.png" alt="image-20221120123732299" style="zoom: 33%;" />-->