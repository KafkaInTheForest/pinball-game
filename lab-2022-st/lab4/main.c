#include <stdio.h>
#include<math.h>
#include "../common/common.h"

#define COLOR_BACKGROUND	FB_COLOR(0xff,0xff,0xff)
static struct pre_point{
	int last_x;
	int last_y;
}last_point[5];


static int touch_fd;
static void touch_event_cb(int fd)
{
	int type,x,y,finger;
	
	type = touch_read(fd, &x,&y,&finger);
	switch(type){
	case TOUCH_PRESS:
		printf("TOUCH_PRESS：x=%d,y=%d,finger=%d\n",x,y,finger);
		if(x>=800 && x<=980 && y>=500 && y<= 580){
			fb_draw_rect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,COLOR_BACKGROUND);
			fb_draw_rect(800,500,180,80,FB_COLOR(0,0,0));
			//fb_update();
			fb_draw_text(840,550,"clear",32,FB_COLOR(250,240,230));
			fb_update();
			return;
		}
		switch(finger){
			case 0:
				fb_draw_circle(x,y,8,FB_COLOR(250,235,215));
				break;
			case 1:
				fb_draw_circle(x,y,8,FB_COLOR(30,144,255));
				break;
			case 2:
				fb_draw_circle(x,y,8,FB_COLOR(255,227,132));
				break;
			case 3:
				fb_draw_circle(x,y,8,FB_COLOR(189,252,201));
				break;
			case 4:
				fb_draw_circle(x,y,8,FB_COLOR(255,127,80));
				break;

		}
		last_point[finger].last_x = x;
		last_point[finger].last_y = y;
		printf("last_x=%d,last_y=%d\n",last_point[finger].last_x,last_point[finger].last_y = y);
		break;
	case TOUCH_MOVE:
		printf("TOUCH_MOVE：x=%d,y=%d,finger=%d\n",x,y,finger);
		switch(finger){
			case 0:
				fb_draw_circle(x,y,8,FB_COLOR(250,235,215));
				fb_draw_line_circle(last_point[finger].last_x,last_point[finger].last_y,x,y,8,FB_COLOR(250,235,215));
				last_point[finger].last_x = x;
				last_point[finger].last_y = y;
				break;
			case 1:
				fb_draw_circle(x,y,8,FB_COLOR(30,144,255));
				fb_draw_line_circle(last_point[finger].last_x,last_point[finger].last_y,x,y,8,FB_COLOR(30,144,255));
				last_point[finger].last_x = x;
				last_point[finger].last_y = y;
				break;
			case 2:
				fb_draw_circle(x,y,8,FB_COLOR(255,227,132));
				fb_draw_line_circle(last_point[finger].last_x,last_point[finger].last_y,x,y,8,FB_COLOR(255,227,132));
				last_point[finger].last_x = x;
				last_point[finger].last_y = y;
				break;
			case 3:
				fb_draw_circle(x,y,8,FB_COLOR(189,252,201));
				fb_draw_line_circle(last_point[finger].last_x,last_point[finger].last_y,x,y,8,FB_COLOR(189,252,201));
				last_point[finger].last_x = x;
				last_point[finger].last_y = y;
				break;
			case 4:
				fb_draw_circle(x,y,8,FB_COLOR(255,127,80));
				fb_draw_line_circle(last_point[finger].last_x,last_point[finger].last_y,x,y,8,FB_COLOR(255,127,80));
				last_point[finger].last_x = x;
				last_point[finger].last_y = y;
				break;
		}
		break;
	case TOUCH_RELEASE:
		printf("TOUCH_RELEASE：x=%d,y=%d,finger=%d\n",x,y,finger);
		break;
	case TOUCH_ERROR:
		printf("close touch fd\n");
		close(fd);
		task_delete_file(fd);
		break;
	default:
		return;
	}
	fb_update();
	return;
}

int main(int argc, char *argv[])
{
	fb_init("/dev/fb0");
	font_init("./font.ttc");
	fb_draw_rect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,COLOR_BACKGROUND);
	fb_update();
	//打开多点触摸设备文件, 返回文件fd
	touch_fd = touch_init("/dev/input/event2");
	//添加任务, 当touch_fd文件可读时, 会自动调用touch_event_cb函数
	fb_draw_rect(800,500,180,80,FB_COLOR(0,0,0));
	fb_update();
	fb_draw_text(840,550,"clear",32,FB_COLOR(250,240,230));
	fb_update();
	task_add_file(touch_fd, touch_event_cb);
	
	task_loop(); //进入任务循环
	return 0;
}
