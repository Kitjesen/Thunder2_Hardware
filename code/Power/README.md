# Machine_Dog


## 项目简介
电源和通信控制板

基于STM32F103的机械狗底层硬件控制项目，包含电源管理、CAN通信、ADC、LED、蜂鸣器、UART等模块。

## 主要功能
- 电源管理
- CAN总线通信
- ADC采集
- LED控制
- 蜂鸣器控制
- UART串口通信

## 目录结构
- `Power/` 主要固件工程目录（STM32F103）
  - `Core/` STM32主代码（Src/源码，Inc/头文件）
  - `Drivers/` STM32 HAL库及CMSIS
  - `User/` 用户自定义模块（ADC、CAN、LED、BUZZER、UART）
  - `Config/` 调试/下载配置文件
  - `Power.ioc` STM32CubeMX 工程文件
- `pcan_canable_hw.bin` 相关固件
- `ProPrj_Dog-电源+通信_2025-07-25.epro` 项目文档

## 编译与使用
- 推荐使用 STM32CubeIDE 或 Keil 打开 `Power/Power.ioc` 工程进行开发。
- 或使用 CMake 进行命令行编译（见 `Power/cmake-build-debug/` 相关文件）。
- 需安装 STM32 HAL 库及相关工具链。

## 依赖
- STM32 HAL 库
- CMake（如用命令行编译）
- STM32CubeIDE 或 Keil（推荐）