# Three-axis gimbal

Status: In progress
Sort: Prj

- [x]  改进云台结构，4310电机承重2025-12-15 22:16:56
- [x]  等元器件，焊接2025-12-23 23:06:42
- [x]  改PCB sample LSB问题 dcdc啸叫2025-12-24 17:22:35
- [x]  等待PCB Ver1.12025-12-28 10:10:11
- [x]  焊接第二版
- [ ]  开始打印结构件

# Structure

Ver1.0：

![ExportBlock/9a7429444ead9815cb6c9cb01ef3c920_720.png](9a7429444ead9815cb6c9cb01ef3c920_720.png)

Ver1.1：底部加入支撑转盘

![image.png](image.png)

# Hardware

### Mini_Drv Ver1.0：

![image.png](image%201.png)

![image.png](image%202.png)

![image.png](image%203.png)

### Mini_Drv Ver1.1：

![image.png](image%204.png)

![image.png](image%205.png)

![image.png](image%206.png)

# Software

pid参数：

![image.png](image%207.png)

# Problems

- DCDC部分，错误地将芯片BS引脚与输出连在一起。
- MOS啸叫，未找到原因。纠正：MOS不会啸叫，压电类器件更易啸叫。

![Snipaste_2025-12-21_15-04-18.png](Snipaste_2025-12-21_15-04-18.png)
