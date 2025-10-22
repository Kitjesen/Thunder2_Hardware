目录说明
code/

分电板（Power Distribution Board, PDB）的源代码。
负责电源通道管理、电流检测与过流保护等逻辑，可通过 STM32 平台编译运行。

firmware/

存放与固件烧录相关的工具与驱动，包括：

ST-LINK V2 驱动程序（用于 STM32 下载与调试）；

编译与下载工具（用于固件编译、烧录与验证）；

PCAN 固件与调试工具（用于 CAN 通信测试与验证）。

PCB/

电路板的硬件设计文件，包含：

原理图（.sch）

PCB 布局文件（.pcb / .brd / .gerber）

加工文件与 BOM 物料清单
