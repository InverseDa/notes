1. 隐藏活动窗口

```bash
bspc node --flag hidden=off
```
2. 显示已隐藏的窗口（显示窗口的内存地址）
```bash
bspc query -N -d focused -n .hidden.window
```