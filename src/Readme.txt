本程序主要希望解决的是：
采用shell语言实现的脚本，不希望被看到具体的实现。想将其变为一种不能直接读懂且不易逆向的可执行程序。

具体的实现：
程序分为两部分：转化程序和调用程序，调用程序的源码作为数据保存在转化程序的可执行文件中；
调用程序作为最后生成的可执行程序的一部分存在。

转化程序sh2bin
1.  将读取脚本，将脚本作为程序中的数据。
2.  对数据进行加密，将密文作为数据，并生成头文件。
3.  从数据区中提取调用程序的源码， 和生成的头文件一起编译，得到最后的可执行程序。

调用程序(最终得到的可执行程序的主线）
1. 对存放在数据区中的脚本密文进行解密
2. 调用脚本。

限制：
运行的环境中必须装有相应平台的编译环境
脚本的大小限制

安全性考虑：
1. 采用不易破解的加密算法
2.调用部分应该使用混淆技术使其不易被逆向分析



实现
1. init.c   对应实现的功能是生成data.h   用于将common.h、 
main_scripts.h、Makefile、放置在data文件中以数组保存
2.main.c
编译生成sh2bin主程序


源码的编译
How to compile source.
unzip  source to sh2bin
chdir to sh2bin
gcc  init.c  -o  init
./init     #for  get data.h
gcc  main.c   -o sh2bin

done

使用方法
使用的前 :
 对于目标脚本运行的平台是linux 平台需安装编译器gcc
 对于目标脚本运行的平台是android平台需要安装ndk
sh2bin  [ -p  android/linux ] -i  inputSh   [ -o outputBin]
中括号[]中的有默认项 
 -p选择平台  默认是linux平台
 -i  输入文件
 -o  输出文件   默认输出文件名是a.out
 


