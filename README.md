
SenseCAP K1100 Firmware

# K1100 for Wio Terminal

## setup & build

```
pip3 install platformio
```

```
pio run -v
```

## Document

[https://modao.cc/app/htu5pizCrfz4ahrMjab5x4#screen=sl6brj77635mqit](https://modao.cc/app/htu5pizCrfz4ahrMjab5x4#screen=sl6brj77635mqit)

## Description

根据产品经理的设计 UI部分实现 主要分为三个部分。

- Sense界面
- [Process界面](#Process部分)
- Network界面

### Structure

```
+-- examples
|   +-- Sense
|       +-- Page_tf
|   +-- Process
|       +-- Process_ino
|   +-- Network
|   +-- Page_TEMPLE
|   +-- Page_WELCOME
+-- src
|   +-- *.cpp
|   +-- *.h
-- *.md
```

### Todo

✅  UI部分实现中

✅✅  UI部分初步实现

❌ 与文档内容无关或暂未开始

| Code | Content | Status|
| :------------ |:---------------:| -----:|
|Sense| Sence |✅ |
|Process| Process |✅✅ |
|Network| Network|❌|
|Page_tf| tf_card (Sence 四级界面 ) | ✅✅ |
|Page_WELCOME| Welcome & Configure Page | ❌ |
|Page_TEMPLE| 模板界面 基于此界面 加入 其他界面内容| ❌|
|...| ...| ...|

# Process部分

页面根据 modao 文档要求实现 如下。 函数解耦，彼此可以独立调用。 作为测试的 ino 在 example/Process_main 下 烧录点击 A 可以检查左下角网络状态显示 点击B 可以查看 Process内各界面。在Ai vision Log 部分 因为不熟悉 ardiuno  能力有限，uint8_t 与 int 的队列没有构建出 目前显示的时间数据 Time 列 进位是有问题的但是可以作为测试界面 供逻辑部分的设计调用。

Process_Display();  //
Process_TinyML_ENTER(); //
Vision_AI_real_time_analysis(); //
Network_state(int s_key); // 改变网络状态 1 显示 ON 0 显示 OFF 可以导入 Process_Display 或者任意位置

| Code | Content | Status|
| :------------ |:---------------:| -----:|
|Vision AI log| Vision_AI_real_time_analysis() |✅ |
|TinyML已进入| Process_TinyML_ENTER() |✅✅ |
|Network| Network_state(int s_key)|✅✅|
|Proccsee首页| Network_state(int s_key)|✅✅|

## Process_Display()

绘制 顶部三个按键 背景颜色 默认的底层图层

## Process_main(int page)

!!!Note
    此处加入 page 0;  page3 为了方便测试 可以调用此函数 也可以独立调用

- case 0   Vision AI log
- case 1  Proccsee首页
- case 2  TinyML示例
- case 3  TinyML已进入

## Network_state(int s_key)

网络状态显示函数 左下角的 Network。  1 ON 0 OFF

## Vision_AI_real_time_analysis(void)

 Vision AI log

## Process_TinyML_ENTER()

这个是 TinyML已进入的界面 为了呈现二维码 引用了
<https://www.arduino.cc/reference/en/libraries/qrcode/> 这个[库](https://www.arduino.cc/reference/en/libraries/qrcode/)

## Licence

[MIT](LICENSE.txt)
