# TinyRenderer

这是一个基于C++的CPU软件渲染器，支持obj模型加载、光栅化、法线可视化输出等功能，目前正在开发中。

## 已实现

- OBJ模型加载
- 根据模型输入进行光栅化渲染
- 深度测试
- 输出法线贴图

## 待实现

- 纹理映射
- 光照计算
- 以及其他尚在学习的操作……

## 如何运行这份项目

1. 确保你已经安装了g++(built by MinGW-W64)和CMake

2. 克隆本仓库到本地

3. 进入项目目录并创建构建目录
```bash
cmake -S . -B build
cmake --build build
```

注意：所有命令应当在项目根目录执行，否则模型加载可能找不到文件。

4. 执行

```bash
./build/TinyRenderer.exe
```

输出framebuffer.tga即为渲染图。

### 如果有更多的模型需要加载

请将main函数中 
```c++
Model model("obj\\african_head\\african_head.obj");
```

修改为对应的模型obj路径即可。