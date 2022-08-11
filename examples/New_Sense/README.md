## ALL in Sence First

examples\New_Sense\Sense_first\Sense_first.ino
## Change Line Chart color

如果需要修改坐标轴文字和坐标线条颜色 需要修改库  seeed_line_chart.h
221-245 行

颜色定义在 seeed_graphics_define.h

```bash

public:
    xpositionx(
        line_chart,
        xlist(pix_t width, pix_t height), {
            _height = height;
            _width = width; }, {
            _x_max_tick_count = 10;
            _x_min_tick_count = 3;
            _y_max_tick_count = 8;
            _y_min_tick_count = 3;
            _x_skip_tick = 0;
            _tick = 8;
            _x_auxi_role = dash_line().color(gray);
            _x_role_color = pink;
            _x_tick_color = green;
            _x_role_thickness = pan_thickness;
            _y_role_color = pink;  // 线
            _y_tick_color = green; // 坐标轴上文字
            _y_role_thickness = pan_thickness;
            _backgroud = transparent;
            _format = "%g";
            _color = classic_colors;
            _show_circle = std::initializer_list<bool>{ true };
            _based_on = 0;
            _max_size = 0; });
    xprop(pix_t, height);
    xprop(pix_t, width);
    xprop(pix_t, x_max_tick_count);
    xprop(pix_t, x_min_tick_count);
    xprop(pix_t, y_max_tick_count);

```