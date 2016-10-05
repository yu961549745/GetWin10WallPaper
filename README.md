# GetWin10WallPaper
Add horizontal wall papers of Windows Foucus to user's picture folder.
+ `src` is source code  of Java.
+ `java/AddWin10Pic.jar` is the runnable file of Java.
+ `matlab` is source code of Matlab.

## Usage

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
+ 也不是不能写C++，但是C++列举文件夹中的文件太麻烦了，正则表达式是的实现也不是特别好。
+ 还是写Java吧。

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