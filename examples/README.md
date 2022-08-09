## Note

是纯的单独界面（spr create delete 分块实现  每个ino 为一页）

coder: 0 coded by team 1 by myself
Status: ✅✅ finished ✅ doing  ❌ todo

| Code | Content | Status| Coder|
| :------------ |:---------------:| -----:| -----:|
|New_Network| 是纯的单独界面（spr create delete 分块实现  每个ino 为一页） |✅ |0|
|New_Sense| 是纯的单独界面（spr create delete 分块实现  每个ino 为一页） |✅✅ |0|
|Process| 已做出单独函数 完全解耦 直接依次调用就可以完成 Process 界面绘制 title 处轻微闪烁 因为视窗有部分重叠的地方  对于刷新背景为黑色 可以直接可以 tft.fillScreen(TFT_BLACK);|✅✅|1|
|...| ...| ...|

## Color

蓝色 tft.color565(135, 206, 235)
红色 tft.color565(160, 34, 34)
灰色 tft.color565(100, 100, 100)
绿色 tft.color565(0, 139, 0)
橙黄色 tft.color565(255, 165, 0)

常规字体：FSS9
加粗字体：FSSB9
如果要使用更大号字体，可以选择FSS12、FSS18、FSS24
如果要使用小号字体，请使用FSS9，将字号参数GFXFF改为2