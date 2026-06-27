# 2.8inch RGB Round Touch LCD Demo

> 基于 ESP32-S3-LCD-Driver-Board + 2.8inch RGB 圆形触摸屏

| 项目 | 规格 |
|------|------|
| 主控 | ESP32-S3（Octal PSRAM） |
| 开发板 | ESP32-S3-LCD-Driver-Board |
| 屏幕 | 2.8inch RGB Round Touch LCD |
| 屏幕型号 | JY280-022-02A0 / BL-HD276001 |
| LCD 驱动 IC | ST7701S |
| 触摸 IC | GT911 |
| 分辨率 | 480 × 480（圆形） |
| 接口 | RGB 565（16-bit） |
| 图形库 | LVGL |
| ESP-IDF 版本 | v5.5.4 |

---

## 硬件引脚连接

### RGB LCD 数据引脚

| 信号 | GPIO | 颜色位 |
|------|------|--------|
| DATA0 | GPIO 5 | B0 |
| DATA1 | GPIO 45 | B1 |
| DATA2 | GPIO 48 | B2 |
| DATA3 | GPIO 47 | B3 |
| DATA4 | GPIO 21 | B4 |
| DATA5 | GPIO 14 | G0 |
| DATA6 | GPIO 13 | G1 |
| DATA7 | GPIO 12 | G2 |
| DATA8 | GPIO 11 | G3 |
| DATA9 | GPIO 10 | G4 |
| DATA10 | GPIO 9 | G5 |
| DATA11 | GPIO 46 | R0 |
| DATA12 | GPIO 3 | R1 |
| DATA13 | GPIO 8 | R2 |
| DATA14 | GPIO 18 | R3 |
| DATA15 | GPIO 17 | R4 |

### 控制信号

| 信号 | GPIO |
|------|------|
| HSYNC | GPIO 38 |
| VSYNC | GPIO 39 |
| DE | GPIO 40 |
| PCLK | GPIO 41 |
| 背光 PWM | GPIO 6 |

### LCD 初始化 SPI

| 信号 | GPIO |
|------|------|
| SDA (MOSI) | GPIO 1 |
| SCL (SCK) | GPIO 2 |
| CS | GPIO 42 |

### 触摸 I2C（GT911）

| 信号 | GPIO |
|------|------|
| SDA | GPIO 15 |
| SCL | GPIO 7 |
| INT | GPIO 16 |
| RST | -1（未使用） |

### IO 扩展芯片

| 芯片 | I2C 地址 |
|------|----------|
| TCA9554PWR | 0x20 |

---

## 快速开始

### 环境准备
```batch
call C:\esp\v5.5.4\esp-idf\export.bat
```

### 编译 & 烧录
```batch
cd /d d:\LStudy\ws_demo\2.8inch\2.8inch_RGB_LCD
idf.py build flash monitor -p COM5
```

首次编译会自动下载 LVGL 等依赖组件，需要 2-5 分钟。按 `Ctrl+]` 退出监视器。

---

## 项目结构

```
├── main/
│   ├── main.c                 # 主程序入口
│   ├── LCD_Driver/
│   │   ├── ST7701S.c/.h       # ST7701S LCD 驱动
│   ├── Touch/
│   │   ├── GT911.c/.h         # GT911 触摸驱动
│   │   └── esp_lcd_touch/     # ESP LCD Touch 通用接口
│   └── TCA9554PWR/
│       ├── TCA9554PWR.c/.h    # I2C GPIO 扩展芯片驱动
├── components/
│   └── lvgl__lvgl/            # LVGL 图形库
├── sdkconfig                  # ESP-IDF 配置
├── CMakeLists.txt
└── README.md
```

---

## 常见问题

- **屏幕不亮？** 检查背光 PWM 输出（GPIO 6）
- **花屏/闪烁？** 降低 PCLK 频率（`EXAMPLE_LCD_PIXEL_CLOCK_HZ`）
- **触摸无反应？** 检查 I2C 连接（GPIO 7, 15），GT911 地址 0x5D
- **内存不足？** 确保使能 Octal PSRAM
