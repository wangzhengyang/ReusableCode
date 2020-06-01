# arm-tcpdump编译

1. 解决依赖

`sudo apt-get install flex bison`

2. 编译`libpcap`

[the-tcpdump-group/libpcap](https://github.com/the-tcpdump-group/libpcap)

`cd libpcap`

`mkdir install`

`./configure --prefix=安装路径 --host=arm-linux CC=arm-linux-gnueabi-gcc`

`make && make install`

3. 编译`tcpdump`

[the-tcpdump-group/tcpdump](https://github.com/the-tcpdump-group/tcpdump)

`cd tcpdump`

`mkdir install`

`./configure --prefix=安装路径 --host=arm-linux CC=arm-linux-gnueabi-gcc`

`make && make install`



运行需要依赖`libcrypto.so.1.0.0`库文件

使用[openssl1.0.0](https://www.openssl.org/source/old/1.0.0/)来编译`libcrypto.so.1.0.0动态库`

`tar -zxvf openssl1.0.0.tar.gz`

`./config no-asm shared --prefix=$(pwd)/install`

修改`Makefile`

```makefile
CC = arm-linux-gnueabi-gcc
删除 CFLAGS= 中的 -m64
AR = arm-linux-gnueabi-ar $(ARFLAGS) r
RANLIB = arm-linux-gnueabi-ranlib
NM = arm-linux-gnueabi-nm
SHARED_LDFLAGS = 
```

`make && make install`