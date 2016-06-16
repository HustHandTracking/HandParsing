#HandParsing

  Hand Parsing 是一种描述手各部分的方式，将Finger Tracking问题转化为pixel分类问题。本项目主要基于H. Liang, J. Yuan and D. Thalmann的 Parsing the Hand in Depth Images, in IEEE Trans. Multimedia, Aug. 2014【1】. 
  
##说明

  在【1】中作者将手分为12个部分(左图)，为实现简单，本项目中将手分为6个部分（右图）,下图中不同的颜色代表不同的分类。
  
  ![image](https://github.com/HustHandTracking/HandParsing/blob/master/img/HandParsing_12.jpg)![image](https://github.com/HustHandTracking/HandParsing/blob/master/img/HandParsing_6.jpg)



##参考文献：
[1] H. Liang, J. Yuan and D. Thalmann, Parsing the Hand in Depth Images, in IEEE Trans. Multimedia, Aug. 2014.

##运行：
opencv 文件夹中为相应的opencv依赖（opencv源码已经被修改）

exe 文件夹中为可执行文件，在windows 10 x64环境下测试成功，相关依赖在opencv文件中。

src 文件夹中为相应源文件


训练模型在：http://pan.baidu.com/s/1i5AoA4t 

运行环境(Windows 10 x64)：

    硬件：Intel RealSense F200/SR300

    软件：opencv2.4.11（其中Random Forest 已经被修改过，请直接利用已经上传的opencv相关文件）
        
有任何问题发邮件至 chenxihust@hust.edu.cn

##组员：
陈希（chenxihust@hust.edu.cn）

王健（1014887480@qq.com）

杨健（703769185@qq.com）

赵峰（zhaofeng_hust@163.com）
