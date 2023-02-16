本想着那么多基于Linux内核的系统都差不多，所以一直都没装Arch Linux。有一次看到一个WSL2里居然有Arch Linux发行版。那个时候我才发现原来WSL2还可以装Arch Linux，于是我第一次装Arch Linux就在WSL2里安装的......后来经常需要在Linux环境下完成项目，WSL2满足不了我了，所以就安装了Arch Linux。
若有疑问可以参考：
- 官方Wiki的Installation Guide: https://wiki.archlinux.org/title/installation_guide
- 由Arch Linux写的安装指南: https://arch.icekylin.online/

## 下载Arch Linux镜像
首先是要下载镜像，官网就有。一般来说要装在x86_64架构的机器上,，址：https://archlinux.org/download/
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201220343.png)
但对于MacOS M1用户，由于M1是ARM的AARCH64架构，在官网是下载是找不到这个版本的镜像的，但是在Wiki可以：https://wiki.archlinux.org/title/Parallels_Desktop
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201224824.png)
在Wiki中找到1.2节，点击archboot（因为我们要用最朴素的安装方式hhh），进入archboot的wiki（https://pkgbuild.com/~tpowa/archboot/web/archboot.html）：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201224938.png)
下载镜像即可：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201225041.png)


## （双系统）制作启动盘
制作启动盘的软件有很多，但首先确保要有至少4~8GB的的U盘。我们要利用制作启动盘的软件来生成关于要安装的系统的引导文件，以便本机在启动的时候能扫到U盘的引导。
在Windows平台上，可以用`Rufus`制作启动盘，虽然看起来界面简陋，但是实际上很好用很稳定：
![image-20221207022201066](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/image-20221207022201066.png)
但对于跨平台来说，我更加推荐用`BalenaEtcher`这款软件，Windows、MacOS和Linux都可以用（本质是Electron），并且不需要很多人为的配置，一键即可构建属于你的启动盘，强烈推荐（美中不足的是，若要恢复启动盘到原本的U盘，需要你用某些工具来重建这个U盘的分区表，如`DiskGenius`）
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201220751.png)

## （双系统）为Arch Linux创建空余磁盘空间
为了给新系统腾出空间，我们需要创建一些空余分区。这里推荐用`DiskGenius`创建分区。
待更新......

## （双系统）进入BIOS设置引导选项
这一步主要是为了能让你的电脑能够引导启动盘启动，毕竟要从启动盘里安装Arch Linux。至于为什么是可选的？这是因为有些计算机他的引导选项本来就是优先U盘启动的，这样就不需要进入BIOS了。
首先按照厂商的BIOS按键进入BIOS（我这里是F2）：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201222011.png)
一般在Boot选项里，有引导顺序的调整，找到U盘的即可咯（这里不多介绍，因为不同厂商的主板，BIOS长得都不太一样，我这个是最原始的版本）

## Arch Linux 基础安装
为什么要基础安装？我们在一部份将安装最基础的Linux Kernel，划分分区表（这一部份我认为最重要，也是最危险的一部份，注意要看清楚是否是Windows分区）
首先进入Arch（一般第一个选项）：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201232239.png)
待一系列的输出完毕后，就会出现一个Welcome to Arch Linux...的字样，我们的大部份操作都是在终端操作的：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201232404.png)
接下来请按照步骤进行。

### 再次确认是否为UEFI引导模式
一定要UEFI引导！
```bash
ls /sys/firmware/efi/efivars
```
如果出现一大串的输出，那就image/20230201233150.png)

### 连接网络
如果是虚拟机或者网线LAN连接的网络，那就不需要配置这方面的内容。
但如果是无线连接的话，你需要`iwctl`连接
```bash
iwctl # 进入iwctl交互式命令行
device list # 列出无线网卡设备名，比如无线网卡看到叫 wlan0
station wlan0 scan # 扫描网络
station wlan0 get-networks # 列出所有 wifi 网络
station wlan0 connect wifi-name # 进行连接，注意这里无法输入中文。回车后输入密码即可
exit # 连接成功后退出
```
连接之后，可以用`ping -c5 www.baidu.com`测试网络的连通性：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201233519.png)

### 更新系统时钟
这一步可选：
```bash
timedatectl set-ntp true
timedatectl status
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201233900.png)

### 更换国内镜像源
这一步也是可选的，如果你是在国内（应该大部份都是在国内），需要配置镜像源：
```bash
vim /etc/pacman.d/mirrorlist
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201234031.png)
在注释中找到China的选项，并取消相关的注释（根据你的地区，选择最优的镜像源，我这里选择清华源）。
另外，如果你的mirrorlist中没有相关的源，那么可以复制粘贴以下的镜像源：
```bash
Server = https://mirrors.ustc.edu.cn/archlinux/$repo/os/$arch # 中国科学技术大学开源镜像站
Server = https://mirrors.tuna.tsinghua.edu.cn/archlinux/$repo/os/$arch # 清华大学开源软件镜像站
Server = https://repo.huaweicloud.com/archlinux/$repo/os/$arch # 华为开源镜像站
Server = http://mirror.lzu.edu.cn/archlinux/$repo/os/$arch # 兰州大学开源镜像站
```

### 分区
本节非常重要，建议在执行操作之前先查看自己的Windows分区信息，不然格式化你的Windows分区那就糟糕了。
我们将采用比较先进的btrfs文件系统，其文件结构如下：
-   `/` 根目录：`>= 128GB`（和用户主目录在同一个 `Btrfs` 文件系统上）
-   `/home` 用户主目录：`>= 128GB`（和根目录在同一个 `Btrfs` 文件系统上）
-   `/boot/efi` EFI 分区：`256MB`（由电脑厂商或 Windows 决定，无需再次创建）
-   Swap 分区：`>= 电脑实际运行内存（RAM）的 60%`（设置这个大小是为了配置休眠准备）

#### 1. 通过 `lsblk` 命令显示磁盘的分区信息：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201235059.png)

> sda表示SATA磁盘，如果你这里显示的是nvme，那么说明你的磁盘是nvme接口的，这无关紧要。

虽然笔者是在虚拟机环境下安装的，为了模拟双系统环境，还是说明下sda的分区结构吧：
- sda1：EFI
- sda2：Windows的C盘
- sda3：Windows的D盘

#### 2. 通过`cfdisk`命令分区并格式化：
进入cfdisk环境，选择gpt：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201235152.png)
可以看到我们这个sda的环境，看得出来还剩下52.7G，这一部份我们分配Arch Linux。
在Free space新建一个Linux swap分区（类型在Type里修改），笔者给了$[8\times 0.6]=5GB$的Linux swap，实际大小按照你们的RAM分配：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201235621.png)
剩下的创建Linux filesystem分区：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201235809.png)
接下来选择Write选项，应用以上的修改：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201235901.png)
最后Quit。
我们确认下分区的结果：
```bash
fdisk -l
```
![](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201235949.png)

### 格式化
#### 1. 格式化Swap分区
通过以下命令格式化对应的 Swap 分区，请按照实际情况替换 `x` 和 `n`（下同）：
```bash
mkswap /dev/sdxn
```
笔者的Swap分区是在/dev/sda4，所以：
```bash
mkswap /dev/sda4
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202000308.png)

#### 2. 格式化Btrfs分区
我们需要将整一个分区格式化为 `Btrfs` 文件系统。使用如下命令进行格式化：
```bash
mkfs.btrfs -L <yourdiskname> /dev/sdxn
```
笔者这里想要在/dev/sda5构建一个Btrfs文件系统，磁盘名为myarch，所以：
```bash
mkfs.btrfs -L myarch /dev/sda5
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202000616.png)
然后将 `Btrfs` 分区挂载到 `/mnt` 下：
```bash
mount -t btrfs -o compress=zstd /dev/sdxn /mnt
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202000809.png)
使用`df -h`复查情况：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202000844.png)

#### 3. 创建Btrfs子卷
```bash
btrfs subvolume create /mnt/@ # 创建 / 目录子卷
btrfs subvolume create /mnt/@home # 创建 /home 目录子卷
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202001028.png)
然后复查情况：
```bash
btrfs subvolume list -p /mnt
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202001114.png)

#### 4. 卸载/mnt，以挂载子卷
```bash
umount /mnt
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202001212.png)

### 挂载
挂载是有顺序的，需要从根目录开始挂载。使用如下命令挂载子卷：
```bash
mount -t btrfs -o subvol=/@,compress=zstd /dev/sdxn /mnt # 挂载 / 目录
mkdir /mnt/home # 创建 /home 目录
mount -t btrfs -o subvol=/@home,compress=zstd /dev/sdxn /mnt/home # 挂载 /home 目录，这里是挂载文件系统，注意sdxn
mkdir -p /mnt/boot/efi # 创建 /boot/efi 目录
mount /dev/sdxn /mnt/boot/efi # 挂载 /boot/efi 目录，这里是挂载EFI系统，注意sdxn
swapon /dev/sdxn # 挂载交换分区，这里是挂载交换分区，注意sdxn
```
笔者的Btrfs文件系统在sda5，Swap分区在sda4，EFI系统在sda1，所以：
```bash
mount -t btrfs -o subvol=/@,compress=zstd /dev/sda5 /mnt
mkdir /mnt/home
mount -t btrfs -o subvol=/@home,compress=zstd /dev/sda5 /mnt/home 
mkdir -p /mnt/boot/efi
mount /dev/sda1 /mnt/boot/efi
swapon /dev/sda4
```

> ⚠️注意：如果遇到如下错误
> ![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202002041.png)
> 这是因为你的EFI分区的文件格式不符合，所以只需要修改下就好了，根据Arch Wiki的指示：
> ![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202002134.png)
> 我们只需要执行如下的语句即可：
> ```bash
> mkfs.fat -F 32 /dev/sdxn
> ```
> 如果报错：`WARNING: Not enough clusters for a 32 bit FAT!`，换用：
> ```bash
> mkfs.fat -F 12 /dev/sdxn
> ```
> 这里的sdxn是指EFI分区

使用`df -h`复查挂载情况：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202004217.png)

使用`free -h`复查Swap分区挂载情况：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202004313.png)

### 安装系统
使用`pacstrap`安装`base`，`base-devel`，`linux`，`linux-firmware`，`btrfs-progs`：
```bash
pacstrap /mnt base base-devel linux linux-firmware btrfs-progs
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202005135.png)

使用`pacstrap`安装`dhcpcd`，`iwd`，`vim`，`sudo`，`zsh`，`zsh-completions`：
```bash
pacstrap /mnt dhcpcd iwd vim sudo zsh zsh-completions
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202005437.png)

### 生成fstab文件
`fstab` 用来定义磁盘分区。它是 Linux 系统中重要的文件之一。使用 `genfstab` 自动根据当前挂载情况生成并写入 `fstab` 文件：
```bash
genfstab -U /mnt > /mnt/etc/fstab
```
复查一下 `/mnt/etc/fstab` 确保没有错误：
```bash
cat /mnt/etc/fstab
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202005608.png)

### change root
切换到新系统：
```bash
arch-chroot /mnt
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202005659.png)

### 设置主机名和时区
1.  首先在 `/etc/hostname` 设置主机名：
```bash
vim /etc/hostname
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202005833.png)

2.  接下来在 `/etc/hosts` 设置与其匹配的条目：
```bash
vim /etc/hosts
```
加入如下内容：
```bash
127.0.0.1   localhost
::1         localhost
127.0.1.1   myarch.localdomain	myarch
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202010012.png)
3.  随后设置时区，在 `/etc/localtime` 下用 `/usr` 中合适的时区创建符号链接：
```bash
ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
```
![](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202010119.png)

### 硬件时间设置
使用如下命令将系统时间同步到硬件时间：
```bash
hwclock --systohc
```

### 设置 Locale
`Locale` 决定了软件使用的语言、书写习惯和字符集。

1.  编辑 `/etc/locale.gen`，去掉 `en_US.UTF-8 UTF-8` 以及 `zh_CN.UTF-8 UTF-8` 行前的注释符：

```bash
vim /etc/locale.gen
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202010330.png)
![](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202010330.png)
2.  然后使用如下命令生成 `locale`：

```bash
locale-gen
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202010411.png)

3.  向 `/etc/locale.conf` 输入内容：

```bash
echo 'LANG=en_US.UTF-8'  > /etc/locale.conf
```

### 为用户root创建密码
```bash
passwd root
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202010544.png)

### 安装微码
利用pacman安装：
```bash
pacman -S intel-ucode
```
如果是amd的cpu，用：
```bash
pacman -S amd-ucode
```

### 安装GRUB引导程序
1.  安装相应的包：
```bash
pacman -S grub efibootmgr os-prober
```
2. 安装 GRUB 到 EFI 分区：
```bash
grub-install --target=x86_64-efi --efi-directory=/boot/efi --bootloader-id=ARCH
```
我是用M1虚拟机安装的，所以`target`是`arm64-efi`：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202011211.png)
3.  接下来使用 `vim` 编辑 `/etc/default/grub` 文件：
```bash
vim /etc/default/grub
```
进行如下修改：
-   去掉 `GRUB_CMDLINE_LINUX_DEFAULT` 一行中最后的 `quiet` 参数
-   把 `loglevel` 的数值从 `3` 改成 `5`。这样是为了后续如果出现系统错误，方便排错
-   加入 `nowatchdog` 参数，这可以显著提高开关机速度
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202011326.png)
如果要添加Windows的引导，需要添加新的一行 `GRUB_DISABLE_OS_PROBER=false`（或者取消最后一行的注释）：
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202011537.png)

4.  最后生成 `GRUB` 所需的配置文件：
```bash
grub-mkconfig -o /boot/grub/grub.cfg
```
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202011620.png)

### 完成安装
1.  输入以下命令：
```bash
exit # 退回安装环境
umount -R /mnt # 卸载新分区
reboot # 重启
```
2. 重启，输入你的用户名和密码（可能会有多余输出，不用管他）
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230202011900.png)
现在你有了一个最基础的Arch Linux，但目前并没有图形界面。为了能够投入日常使用，我们需要安装Display Manager和Windows Manager，这两者统称为Desktop Manager（简称DM）。
常见的DM有Gnome、KDE、Xfce等等。当然，很多人也不喜欢安装这些DM，因为有些臃肿。他们（包括我）更喜欢分开来装。常见的组合有lightdm作为Display Manager，bspwm作为Windows Manager。这里为了照顾两派人，我们分开来讲解。（二选一）
