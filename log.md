#HandParsing

  Hand Parsing 是一种描述手各部分的方式，将Finger Tracking问题转化为pixel分类问题。本项目主要基于H. Liang, J. Yuan and D. Thalmann的 Parsing the Hand in Depth Images, in IEEE Trans. Multimedia, Aug. 2014【1】. 
  
##说明

  在【1】中作者将手分为12个部分(左图)，为实现简单，本项目中将手分为6个部分（右图）,下图中不同的颜色代表不同的分类。
  
  ![image](https://github.com/HustHandTracking/HandParsing/blob/master/img/HandParsing_12.jpg)![image](https://github.com/HustHandTracking/HandParsing/blob/master/img/HandParsing_6.jpg)

#算法流程
![image](https://github.com/HustHandTracking/HandParsing/blob/master/img/%E7%AE%97%E6%B3%95%E6%B5%81%E7%A8%8B.jpg)

###训练集
由于真实数据难以获取，我们选择利用合成的样本来模拟深度图。模型渲染基于QtOpenGL，渲染时将手的不同关节标上不同颜色来区分，然后投影到二维平面上。投影时，focal length 为202.0mm，近投影面距离400.0mm，远投影面距离1200.0，水平张角73.0度。最终我们生成了30k张样本。

![image](https://github.com/HustHandTracking/HandParsing/blob/master/img/HandParsing_6.jpg)
