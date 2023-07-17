# Robocon2023-ER
Robocon2023 NCiST ER code
## 介绍
Robocon2023吴哥之花，风暴战队NCIST创新实验室大象机器人电控代码

## 文件目录结构&文件用途说明
| 文件夹    | 来源   | 内容                                |
| --------- | ------ | ----------------------------------- |
| Core | CubeMX | CubeMX生成源文件 |
| Drivers | CubeMX | stm32硬件驱动库 |
| MDK-ARM | CubeMX | MDK编译器配置编译文件 |
| Middlewares | CubeMX | 中间层第三方库文件 |
| User | 开发者 | 用户源代码 |

  User/Apps    系统应用层文件：负责系统任务，变量数据的使用和调配  
  User/BSP    板级支持包：主控板载资源底层驱动，无需更改  
  User/Drivers  外设驱动：外设模块的驱动文件
  User/Middlewares  中间层：包含各类算法文件，调试程序，机器人模块控制程序文件等  
  User/DOC  资料文件：包含硬件原理图，手册等  

## 使用说明
1. 开发依赖工具：Keil/VScode, CubeMX
2. 开发系统环境：Windows
3. 开发硬件环境：STM32F407
