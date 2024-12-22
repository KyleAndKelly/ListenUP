## issue1:exited with code -1073741515

### background 

- 将libvlc的lib和include拷贝到项目的目录下，并且更改了项目的pro文件后 能成功build，但是运行时报错

### rootcause

- 错误代码 `-1073741515` 通常对应于 Windows 系统的错误 **`0xC0000135`**，这是一个典型的 **"DLL not found"** 或 **"依赖库缺失"** 的问题。它表明程序运行时无法找到所需的动态链接库（DLL）。

### solution

在libvlc的目录中,除了要把sdk/lib  sdk/include拷贝过去 

还要把plugins , axvlc.dll,  libvlc.dll,libvlccore.dll, npvlc.dll拷贝过去

![image-20241222101837302](C:\Users\Kyle\AppData\Roaming\Typora\typora-user-images\image-20241222101837302.png)

![1734830150609](D:\Project\cppProject\ListenUp\devNote\1734830150609.png)

## issue2: Failed to create VLC media for file



### background 

```c++
   // Create a new libVLC media object
    media = libvlc_media_new_path(vlcInstance, filePath.toStdString().c_str());
    if (!media) {
        std::cerr << "Failed to create VLC media for file: " << filePath.toStdString() << std::endl;
        return;
    }
```



```
Failed to create VLC media for file: D:/Project/cppProject/ListenUp/testvideos/test.mp4
```



### rootcause



- Windows 上的路径分隔符是 `\`，而不是 `/`
- 文件路径中包含非 ASCII 字符（例如中文字符），而 `libVLC` 的某些 API 可能无法正确处理这些字符。`libVLC` 使用的是 C 风格的字符串（`char*`），它对 Unicode 支持有限，因此需要将文件路径转换为 UTF-8 编码。





### solution

```c++
QString normalizedPath = QDir::toNativeSeparators(filePath);
QByteArray filePathUtf8 = normalizedPath.toUtf8();
media = libvlc_media_new_path(vlcInstance, filePathUtf8.constData());

```



