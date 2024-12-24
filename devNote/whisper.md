### Introduction
- Recently, I have been developing a speech recognition tool software using C++/Qt, and as part of this, I have tried OpenAI's whisper.cpp.
- whisper.cpp is an open-source project that is a C/C++ port of OpenAI's Whisper model.
- OpenAI's Whisper is an automatic speech recognition (ASR) system trained with large amounts of multilingual and multitask supervised data, capable of performing tasks such as multilingual speech recognition, speech translation, and language identification. The whisper.cpp project allows this model to run locally on various platforms, including Windows, Mac OS, Linux, Android, and WebAssembly.
- The main advantages of whisper.cpp are as follows:

  - **No dependencies**: whisper.cpp does not require installing any external libraries or frameworks, as it can be compiled and run with just a C/C++ compiler. This reduces the dependency on external libraries.
  - **Lightweight**: The whisper.cpp library is very compact, consisting of only one source file, making it easy to integrate into any project without adding significant overhead.
  - **High performance**: By using template metaprogramming and C++17 features, whisper.cpp incurs almost no extra overhead at runtime. It uses type-safe methods to ensure efficient event handling.
  - **Simple and easy to use**: The design is simple and clear, and the API is user-friendly, allowing developers to quickly start writing event-driven code.

### Steps

- **Step 1: Download the code**
```
git clone https://github.com/ggerganov/whisper.cpp
```

- **Step 2: Download the model**
There is a "models" folder in the root directory of the whisper.cpp project. Navigate to that folder and use the command to download the model. Below is the CMD command to run:
```
download-ggml-model.cmd small
```
Alternatively, there is a shell command:
```
download-ggml-model.sh medium
```
If neither of these methods works, you can manually download the model from the following URL:
```
https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-small.bin
```

- **Step 3: Compile**
After successfully completing the above two steps, generate the `main.exe` file in the "build/bin" directory of the Whisper.cpp project by running the following commands:
```
cmake .
mingw32-make
```

- **Step 4: Run**
Navigate to the root directory and run the following command to see the recognized subtitles in the terminal:
```
.build/bin/whisper-cli.exe -m ./models/ggml-small.bin -f ./samples/jfk.wav
```

![image.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/3916068/39ab54dd-1d4b-1215-3717-4a073874a174.png)

---

Here is the translated and organized English document based on the provided content. Feel free to copy or modify it as needed.