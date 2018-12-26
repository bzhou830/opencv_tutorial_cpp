# <OpenCV轻松学>C++版

## 1. 编译OpenCV

linux平台下：

1. 首先需要安装构建OpenCV的相关工具. 安装build-essential、cmake、git和pkg-config

```shell
sudo apt-get install build-essential cmake git pkg-config 
```

2. 安装常用图像格式的工具包，用于支持各种格式的图像

```
// 安装jpeg格式图像工具包
sudo apt-get install libjpeg8-dev 
// 安装tif格式图像工具包 
sudo apt-get install libtiff5-dev 
// 安装JPEG-2000图像工具包 
sudo apt-get install libjasper-dev 
// 安装png图像工具包 
sudo apt-get install libpng12-dev 
```

3. 安装视频读写相关工具包

```shell
sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
```

```shell
sudo apt-get install libgtk2.0-dev
sudo apt-get install libatlas-base-dev gfortran
```

4. 下载OpenCV源码和OpenCV contrib源码

使用wget下载OpenCV源码。

```shell
wget -O opencv-3.4.1.zip https://github.com/Itseez/opencv/archive/3.4.1.zip
wget -O opencv_contrib-3.4.1.zip https://github.com/Itseez/opencv_contrib/archive/3.4.1.zip
```

下载完成之后，解压压缩包。cd到opencv源码目录。创建一个文件夹用来保存cmake生成的文件。

```
cd opencv-3.4.1
// 新建release文件夹
mkdir release
// 进入release文件夹
cd release
```
设置cmake编译参数，安装目录默认为/usr/local ，注意参数名、等号和参数值之间不能有空格，但每行末尾“\”之前有空格，参数值最后是两个英文的点：
```
/** CMAKE_BUILD_TYPE是编译方式 
* CMAKE_INSTALL_PREFIX是安装目录 
* OPENCV_EXTRA_MODULES_PATH是加载额外模块 
* INSTALL_PYTHON_EXAMPLES是安装官方python例程 
* BUILD_EXAMPLES是编译例程（这两个可以不加，不加编译稍微快一点点，想要C语言的例程的话，在最后一行前加参数INSTALL_C_EXAMPLES=ON \） 
**/ 

sudo cmake -D CMAKE_BUILD_TYPE=RELEASE \ -D CMAKE_INSTALL_PREFIX=/usr/local \
-D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib-3.4.1/modules \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D BUILD_EXAMPLES=ON ..
```

```
// 编译
sudo make
// 安装
sudo make install
// 更新动态链接库
sudo ldconfig
```
