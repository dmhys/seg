## SEG : Slam Easy Gui

![main](assets/main.png)



 [ SEG : SLAM Easy Gui ] is designed to be a beginner-friendly GUI tool. SEG aims to wrap all rendering needs for SLAM researchers, with minimum effort. For those who are unfamilliar with OpenGL, SEG wrapped its all rendering APIs inside. What you need to do is just a single line call, and SEG will do the job for you!

```c++
seg::initialize("my_window",seg::WindowSize(1000,600));
```

 Adding object is also very easy.

```c++
seg::addObject(seg::Object::Axis());
```

For more details, check SEG/examples.



### How To Build





### Getting Started



### Note

**Formatting**

SEG uses clang-format with Google style. See `.clang-format`.



### TODOs

- [ ] Shutdown guard: API calls after window close cause segfault (addObject on dead render thread)
- [ ] Wide line rendering: `glLineWidth` >1.0 unsupported on core profile. Need geometry shader or quad-based line
- [x] Embed default font as header (Roboto Regular, compressed in `seg/resources/roboto_regular.h`)
- [ ] Line/Pointcloud renderer deduplication (F6)
- [ ] Shader API consistency: some renderers use raw uniform strings instead of helpers (F7)
- [ ] `install()` target and interface library for downstream consumers
