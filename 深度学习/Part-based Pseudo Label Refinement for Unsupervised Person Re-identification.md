# **Part-based Pseudo Label Refinement for Unsupervised Person Re-identification**

## 1. Abstract

无监督的人再识别re-ID的目的是为了学习从未标记的数据中检索到人的区别表征。在现有的技术中，我们通过使用伪标签来完成这个任务，但是这种方法的标签本身就有噪声，这样会降低准确性。

为了解决这样的问题，人们提出了几种伪标签细化的方法，但他们忽略了re-ID所需要的细粒度局部上下文。

所以我们提出了一种全新的基于部分的伪标签细化（PPLR）框架，通过利用全局特征和部分特征两个集合的互补关系来降低标签的噪声。

即设计了一个cross agreement score作为特征空间之间的k-邻近相似性。基于这个理论，我们整合了部分特征的预测来细化全局特征的伪标签，从而共同缓解了全局特征聚类中的噪声。

![image-20220823183450957](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/image-20220823183450957.png)

## 2. Method

### 2.1 **Part-based Unsupervised re-ID Framework**

设$D=\{x_i\}_{i=1}^{N_D}$表示未训练的数据集，其中$x_i$是图像，$N_D$是图像的数量，显然$D=\bigcup_{i=1}^{N_D} x_i$。

首先提取$F_\theta (x_i)\in \mathbb{R}^{C\times H\times W}$，其中C、H、W分别表示的是通道的大小、特征图的高度和宽度。