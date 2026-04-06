# 运行指南

## 操作步骤

1. **生成 C++ 相关文件**  
   CMakeLists.txt 会自动生成 proto 的 C++ 相关文件。

2. **生成 Python 相关文件**  
   打开 `assistant` 文件夹，找到 `py编译指令.txt`，运行其中的指令生成 proto 的 Python 相关文件。  
   *注：proto 生成的相关文件会存放于 `proto/generated` 目录下。*

3. **启动服务器**  
   打开 `assistant` 文件夹，找到 `py编译指令.txt`，运行启动服务器的指令。

4. **运行客户端**  
   运行 `send.cpp`。

---

## 常见提示说明

运行 `server.py` 时，IDE 可能会提示以下信息，**无需理会**：  
> 无法解析导入"message_pb2"  
> 无法解析导入"message_pb2_grpc"  

*原因说明：* 模块在运行时才被生成并加入到搜索路径，而编译器的搜索路径是静态的，因此无法识别这两个文件，但不影响实际运行。
