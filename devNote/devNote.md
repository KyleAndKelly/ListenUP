## issue1:exited with code -1073741515

### Background

- After copying the libvlc `lib` and `include` directories into the project directory and modifying the project's `.pro` file, the build was successful, but an error occurred during runtime.

### Root Cause

- The error code `-1073741515` typically corresponds to the Windows system error **`0xC0000135`**, which is a common **"DLL not found"** or **"missing dependency"** issue. It indicates that the program cannot find the required dynamic link libraries (DLLs) during execution.

### Solution

In addition to copying the `sdk/lib` and `sdk/include` directories from libvlc, you must also copy the following files from the libvlc directory:

- `plugins`
- `axvlc.dll`
- `libvlc.dll`
- `libvlccore.dll`
- `npvlc.dll`

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



- The path separator on Windows is `\`, not `/`.
- File paths containing non-ASCII characters (such as Chinese characters) may not be correctly handled by some `libVLC` APIs. Since `libVLC` uses C-style strings (`char*`), which have limited support for Unicode, file paths need to be converted to UTF-8 encoding.





### solution

```c++
QString normalizedPath = QDir::toNativeSeparators(filePath);
QByteArray filePathUtf8 = normalizedPath.toUtf8();
media = libvlc_media_new_path(vlcInstance, filePathUtf8.constData());

```



