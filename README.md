# GetWin10WallPaper
Add horizontal wall papers of Windows Foucus to user's picture folder.
+ `src` is source code  of Java.
+ `matlab` is source code of Matlab.

Win10锁屏界面的壁纸真好看，但是人工提取比较麻烦，于是就写个程序来做。

本来是实现了一个Matlab的，但是呢，启动Maltab本身太慢了。也不是不能写C++，但是C++列举文件夹中的文件太麻烦了，还是写Java吧。

主要包含以下步骤：
+ 从`%USERPROFILE%/AppData/Local/Packages/Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy/LocalState/Assets`读取横向壁纸，该文件夹下文件没有后缀名，而且并不是所有文件都是jpg格式的，需要进行识别：
    + JPEG文件以`0xFFD8`开头
    + `0xFFC0`之后移动3位，可以读取图片的高度和宽度
+ 利用MD5来标识图片，向`%USERPROFILE%/Pictures/Saved Pictures`添加新的图片。