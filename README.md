# Introduction
#### 工程中的所有速度测试均未启用O2优化。

- speed_rgb2gray_sse.cpp 使用sse加速RGB和灰度图转换算法，相比于OpenCV系统函数有2-3倍加速。算法原理：https://blog.csdn.net/just_sort/article/details/94456945 。速度测试结果如下：

|优化方式|图像分辨率|速度|
|---------|----------|----|
|C语言实现+1线程|4032*3024||
|4次循环展开+1线程|4032*3024||
|SSE优化+1线程|4032*3024||

- speed_gaussian_filter_sse.cpp 使用sse加速高斯滤波算法，但很遗憾因为未知原因没有带来加速，需要进一步研究。算法原理：https://blog.csdn.net/just_sort/article/details/95212099

- speed_integral_graph_sse.cpp 使用SSE加速积分图运算，算法原理请看：https://www.cnblogs.com/Imageshop/p/6897233.html 。速度测试结果如下：

|优化方式|图像分辨率 |速度|
|---------|----------|-------|
|C语言实现+1线程|4032*3024||
|C语言实现+4线程|4032*3024||
|SSE优化+1线程|4032*3024||
|SSE优化+4线程|4032*3024||
