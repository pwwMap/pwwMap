Title:       The core of the big data solutions -- Map.
Author:      pengwenwei
address:     No.17-18 of XiangGangbatang Community, Xiangtan City of Hunan Province, China.   
Language:    c++
Platform:    Windows, linux
Technology:  Perfect hash algorithm
Level:       Advanced
Description: A high performance map algorithm
Section      MFC c++ map stl
SubSection   c++ algorithm
License:     (GPLv3)


Map is widely used in c++ programs. Its performance is critical to programs' performance. Especially in big data  and the scenarios which can't realize data distribution and parallel processing.

I have been working on big data analysis for many years in telecommunition and information security industry. The data analysis is so complicated that they can't work without map. Especially in information security industry, the data is much more complicated than others. For example, ip table, mac table, telephone numbers table, dns table etc.


Currently, the STL map and Google's hash map are the most popular maps. But they have some disadvantages. The STL map is based on binary chop, which causes a bad performance. Google Hash map has the best performance at present, but it has probability of collision. For big data analysis, the collision probability is unacceptable.

Now I would like to publish pwwMap. It includes three different maps for different scenarios:
1. Memory Map(memMap): It has a good access speed. But its size is limited by memory size.
2. Harddisk Map(diskMap): It utilizes hard disk to store data. So it could accept much more data than memory map.
3. Hashmap(hashMap): It has the best performance and a great lookup speed, but it doesn't have 'insert' and 'delete' functionality.

MemMap and diskMap could be converted to hashMap by function memMap2HashMap and diskMap2HashMap. According to the test result, my algorithms' collision probability is zero. About performance, memMap has a comparable performance with google, and hashMap's performance is 100 times better than Google's hashmap.

In summary, pwwhash are perfect hash algorithms with zero collision probability. You can refer to following artical to find the key index and compress algorithm theory:
http://blog.csdn.net/chixinmuzi/article/details/1727195

Source code and documents:
https://sourceforge.net/projects/pwwhashmap/files/?source=navbar

I would like to transfer my technique with my immigration to the United States as the condition.

Please do not contact me with email, qq or telephone. We can have a face to face talk.

My email is pww71@sina.com and password is 8622507. after delivery. Please make sure your email is received. Otherwise it's the national security agency to restrict me to receive your mail, you can only talk with me.

My address is as follows:
No.17-18 of XiangGangbatang Community, Xiangtan City of Hunan Province, China.