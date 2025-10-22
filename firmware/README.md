### 1. pcan_canable_hw.bin
PCAN转USB适配器的固件文件，用于四路PCAN板。
####需要一个一个下载口下载

### 2. ST-LINK Utility
ST-LINK调试器的固件下载工具。

**主要功能：**
- 烧录固件到STM32芯片
- 读取/擦除Flash内存
- 调试程序

**使用方法：**
1. 运行 `ST-LINK Utility/STM32 ST-LINK Utility.exe`
2. 连接ST-LINK到目标板
3. 选择固件文件（.bin/.hex）
4. 点击 "Program & Verify" 烧录

### 3. ST-LINK_USB_V2_1_Driver
ST-LINK V2.1调试器的USB驱动程序。

**安装方法：**
1. 运行 `ST-LINK_USB_V2_1_Driver/stlink_winusb_install.bat`
2. 或手动运行 `dpinst_amd64.exe`（64位系统）/ `dpinst_x86.exe`（32位系统）
3. 重新插拔ST-LINK设备

## 快速开始

### 烧录PCAN固件到四路PCAN板

1. 安装ST-LINK驱动（如果未安装）
2. 使用ST-LINK连接四路PCAN板
3. 打开ST-LINK Utility
4. 加载 `pcan_canable_hw.bin` 文件
5. 设置起始地址为 `0x08000000`
6. 点击烧录

### 烧录电源管理板固件

电源管理板的源码和编译后的固件在 `../code/Power/` 目录。

1. 编译后生成 `.bin` 或 `.hex` 文件
2. 使用ST-LINK Utility烧录到STM32F103C8T6芯片

## 注意事项

- 烧录前请确认目标芯片型号和固件匹配
- 烧录时不要断开连接
- 首次使用需要安装ST-LINK驱动