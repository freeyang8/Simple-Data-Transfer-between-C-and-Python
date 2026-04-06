#include <iostream>
#include <memory>
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include <opencv2/opencv.hpp>
#include <vector>

// cv::imencode(format, img, buf);: 这是核心的编码步骤，调用了 OpenCV 的 imencode 函数。
// format: 指定编码格式，例如 ".jpg" 或 ".png"。
// img: 传入的需要被编码的源图像。
// buf: 编码后的二进制数据会被写入这个缓冲区
std::vector<uchar> encodeImage(const cv::Mat& img, const std::string& format = ".jpg") {
    std::vector<uchar> buf;
    cv::imencode(format, img, buf);
    return buf;
}

int main() {
    // 连接服务器
    //grpc::CreateChannel，这是 gRPC 库提供的工厂函数，用来建立连接
    //std::unique_ptr<...>，“独占拥有权”，意思是这个 stub 只有当前这个作用域能用，出了作用域它会自动销毁（自动释放内存），防止内存泄漏
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    std::unique_ptr<SearchService::Stub> stub = SearchService::NewStub(channel);

    //获取图片
    cv::Mat image = cv::imread("./image/myima.jpg");
    if(image.empty()){
        std::cerr<< "Unable to read image"<<std::endl;
        return -1;
    }
    //编码图片
    auto img_buf = encodeImage(image, ".jpg");

    // 构造请求
    SearchRequest request;
    request.set_query("hello gRPC");
    request.set_page_number(100);
    request.set_success(true);
    //img_buf.data(),获取原始数据的内存首地址
    request.set_image_data(img_buf.data(),img_buf.size());
    request.set_image_format("jpg");

    // 发送请求
    //response是一块没有内容的内存地址，服务器返回的数据会放进这里
    //context是客服端向服务器发送数据时额外添加的信息，这里不需要额外的信息，所以没有内容
    //注：context 的内容会被转换成一种网络传输标准（也就是 HTTP/2 的头部信息），只是给服务器看的，不参与后续的数据处理
    SearchResponse response;
    grpc::ClientContext context;
    
    //这段代码做了五件事，①request序列化，②发送数据给服务器，③阻塞等待，④接收数据并反序列化，装进response，⑤返回状态
    grpc::Status status = stub->Search(&context, request, &response);

    //.ok() 是一个判断函数。如果服务器顺利处理完了请求，并且没有报错，它会返回 true
    if (status.ok()) {
        std::cout << "收到响应: " << response.result() << std::endl;
    } else {
        std::cout << "RPC 失败: " << status.error_message() << std::endl;
    }
    return 0;
}