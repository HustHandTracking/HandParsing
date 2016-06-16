#HandParsing

  Hand Parsing 是一种描述手各部分的方式，将Finger Tracking问题转化为pixel分类问题。本项目主要基于H. Liang, J. Yuan and D. Thalmann的 Parsing the Hand in Depth Images, in IEEE Trans. Multimedia, Aug. 2014【1】. 
  
##1 说明

  在【1】中作者将手分为12个部分(左图)，为实现简单，本项目中将手分为6个部分（右图）,下图中不同的颜色代表不同的分类，且本项目中没有实现【1】中的Superpixel-Markov Randm Field(SMRF)部分。
  
  ![image](https://github.com/HustHandTracking/HandParsing/blob/master/img/HandParsing_12.jpg)![image](https://github.com/HustHandTracking/HandParsing/blob/master/img/HandParsing_6.jpg)

#2 算法流程
![image](https://github.com/HustHandTracking/HandParsing/blob/master/img/%E7%AE%97%E6%B3%95%E6%B5%81%E7%A8%8B.jpg)

###2.1 训练集
由于真实数据难以获取，我们选择利用合成的样本来模拟深度图。模型渲染基于QtOpenGL，渲染时将手的不同关节标上不同颜色来区分，然后投影到二维平面上。投影时，focal length 为202.0mm，近投影面距离400.0mm，远投影面距离1200.0，水平张角73.0度。最终我们生成了30k张样本。

![image](https://github.com/HustHandTracking/HandParsing/blob/master/img/HandParsing_6.jpg)

###2.2 特征提取
特征提取利用【1】中的差分特征-已当前点为中心，在其邻域内选择若干差分点，求差分点与当前点的深度值的差做为该点的特征。考虑到越近的点描述能力越强，差分点的选择是越靠近中心点越密集，下图中，左图为差分点选择示意（取100个查分点），M为距离；右为某一维度上差分点分布概率。

![image](https://github.com/HustHandTracking/HandParsing/blob/master/img/%E5%B7%AE%E5%88%86%E7%82%B9%E9%80%89%E6%8B%A9.jpg)![image](https://github.com/HustHandTracking/HandParsing/blob/master/img/%E5%B7%AE%E5%88%86%E7%82%B9%E6%A6%82%E7%8E%87.jpg)

在训练时，并不是手上每个区域的每个点都要提取特征，这样做训练集过大，容易过拟合。最终我们选择对每个部分随机提取20个点（手掌提取40个）。
