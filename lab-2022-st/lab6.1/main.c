#include "func.h"

#define COLOR_BACKGROUND	FB_COLOR(0,0,0)

static int touch_fd;
static ball_info ball,*p_ball;
static status_info status,*p_status;
static position board,*p_board;

static void touch_event_cb(int fd)
{
	int type,x,y,finger;
	//printf("enter touch_event\n");
	type = touch_read(fd, &x,&y,&finger);
	switch(type){
	case TOUCH_PRESS:
		printf("TOUCH_PRESS：x=%d,y=%d,finger=%d\n",x,y,finger);
		if(!status.start && x>= 437 && x<587 && y>=270 && y <=330){
				status.start = 1;
				fb_draw_rect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,COLOR_BACKGROUND);
    			//画出边框
				fb_draw_rect(0,0,SCREEN_WIDTH,10,FB_COLOR(255,255,255));
				fb_draw_rect(0,0,10,SCREEN_HEIGHT,FB_COLOR(255,255,255));
				fb_draw_rect(1014,0,10,SCREEN_HEIGHT,FB_COLOR(255,255,255));
				//fb_draw_rect(437,270,150,60,COLOR_BACKGROUND);
				init_ball(p_ball);
				init_board(p_board);
		}
		else{
			fb_draw_rect(p_board->x-board_len/2,board_height,board_len,10,FB_COLOR(0,0,0)); //清除上次的
			if(x<=62) x=62;
			else if (x + 60> SCREEN_WIDTH) x = 962;
			fb_draw_rect(x-board_len/2,board_height,board_len,10,FB_COLOR(255,255,255));
			p_board->x = x;
		}
		break;
	case TOUCH_MOVE:
		printf("TOUCH_MOVE：x=%d,y=%d,finger=%d\n",x,y,finger);
		fb_draw_rect(p_board->x-board_len/2,board_height,board_len,10,FB_COLOR(0,0,0)); //清除上次的
		if(x<=62) x=62;
		else if (x + 60> SCREEN_WIDTH) x = 962;
		fb_draw_rect(x-board_len/2,board_height,board_len,10,FB_COLOR(255,255,255));
		p_board->x = x;
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

static void move_event_cb(int fd){
		if(status.start){ 
			fb_draw_circle(p_ball->x,p_ball->y,p_ball->radius,FB_COLOR(0,0,0));
			printf("moving\n");
			move(p_ball,p_status,p_board);
			fb_draw_circle(p_ball->x,p_ball->y,p_ball->radius,FB_COLOR(255,255,255));
			if(!status.start){
				fb_draw_border(437,270,150,60,FB_COLOR(255,255,255));
				//fb_draw_rect(462,270,100,60,FB_COLOR(255,255,255));
				fb_draw_text(445,305,"重新开始",32,FB_COLOR(250,240,230));
			}
			fb_update();
		}
}


int main(int argc, char *argv[])
{
	fb_init("/dev/fb0");
	font_init("./font.ttc");
	status.start = 0;
	status.count = 0;
	p_ball = &ball;
	p_status = &status;
	p_board =&board;

	fb_draw_rect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,COLOR_BACKGROUND);
    //画出边框
	//fb_draw_rect(0,0,SCREEN_WIDTH,10,FB_COLOR(255,255,255));
	//fb_draw_rect(0,0,10,SCREEN_HEIGHT,FB_COLOR(255,255,255));
	//fb_draw_rect(1014,0,10,SCREEN_HEIGHT,FB_COLOR(255,255,255));

	//开始游戏界面
	fb_draw_border(437,270,150,60,FB_COLOR(255,255,255));
	fb_draw_text(445,305,"开始游戏",32,FB_COLOR(250,240,230));
	fb_update();
	//打开多点触摸设备文件, 返回文件fd
	touch_fd = touch_init("/dev/input/event2");
	//添加任务, 当touch_fd文件可读时, 会自动调用touch_event_cb函数
	printf("%d\n",touch_fd);
	task_add_file(touch_fd, touch_event_cb);
	 task_add_timer(30,move_event_cb);

	task_loop(); //进入任务循环
	return 0;
}