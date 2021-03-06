# GetWin10WallPaper
Add horizontal wall papers from Win 10 lock screen to user's picture folder.
+ pick horizontal wall papers from the folder of Win 10 lock screen wall papers. People can't see the pictures directly, because files of this folder doesn't have extension names. It's tired for people to rename files one by one and select the valid files.
+ add new pictures to user's picture folder that different from the exist pictures. It's tired for people to check if the new picture is already exist.

## Usage
+ `src` is source code  of Java.
+ `java/AddWin10Pic.jar` is the runnable file of Java.
+ `matlab` is source code of Matlab.
+ `win32` contains C++ source code and executable file `AddWin10Pic.exe`.

### C++
Just run `AddWin10Pic.exe`.

The source code `main.cpp` can complie with `Visual Studio 2013`.

### Java
This is an Eclipse project, can import into Eclipse directly.

### Maltab
Just run `getWin10Pic.m`.

The `*.mexw64` files is for 64-bit Matlab. For 32-bit Matlab, you should [setup mex complie enviroment](https://www.mathworks.com/help/matlab/ref/mex.html) first, and run following code in Matlab Command Window
```
mex mexIsJpg.cpp
mex mexGetJpgSize.cpp
mex mMD5.cpp
```
For 64-bit Maltab that earlier than R2016a, if there is any problem, you  can try above method first.

## 说明
Win10锁屏界面的壁纸真好看，但是人工提取比较麻烦，于是就写个程序来做。
+ 本来是实现了一个Matlab的，但是呢，启动Maltab本身太慢了。
+ 然后写了个Java吧。
+ 方便大家不用安装Java环境，就干脆写个C++吧。

主要包含以下步骤：
+ 从`%USERPROFILE%/AppData/Local/Packages/Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy/LocalState/Assets`读取横向壁纸，该文件夹下文件没有后缀名，而且并不是所有文件都是jpg格式的，需要进行识别：
    + JPEG文件以`0xFFD8`开头
    + `0xFFC0`之后移动3个字节，可以读取图片的高度和宽度
+ 利用MD5来标识图片，向`%USERPROFILE%/Pictures/Saved Pictures`添加新的图片。

## 可重用部分
这个项目的可重用部分有：
+ 在不完全加载文件的情况下，判断一个文件是否是JPEG文件。
+ 在不完全加载文件的情况下，获取JPEG图片的高度和宽度。
+ 获取文件的MD5。