#ifndef __MAIN_H__
#define __MAIN_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <intrins.h>
#include <math.h>
#include <REGX52.H>
/*---------------------------------------------------------------------------*/
#define __enable_irq()        EA = 1///<使能全局中断
#define __disable_irq()       EA = 0///<失能全局中断

///定义true/false
#ifndef true
#define	true	      ( 1 )
#endif

#ifndef false
#define	false	      ( 0 )
#endif


///对已有变量进行重命名
typedef unsigned char uint8_t;
typedef char int8_t;

typedef unsigned int uint16_t;
typedef int int16_t;

typedef unsigned long int uint32_t;
typedef long int int32_t;

typedef bit bool;

typedef uint16_t clock_time_t;

/*---------------------------------------------------------------------------*/
///常用宏定义
#define   INC_LIMIT(VAL,LIMIT_VAL)    (VAL) < (LIMIT_VAL) ? (++VAL): (VAL = (LIMIT_VAL))
#define   DEC_LIMIT(VAL,LIMIT_VAL)    (VAL) > (LIMIT_VAL) ? (--VAL): (VAL = (LIMIT_VAL))
#define   ENUM_TO_STR(e)              (#e)
/*---------------------------------------------------------------------------*/
#define TIMER0_DEV_DRIVER
#define KEY_DEV_DRIVER
#define LCD12864_DEV_DRIVER
/*---------------------------------------------------------------------------*/
#ifdef TIMER0_DEV_DRIVER
extern volatile clock_time_t clock_ticks;
#define HAL_GetTick() clock_ticks

#define time_event_define(name) \
  clock_time_t time_event_base##name;\
  bool is_start_time_event##name = false

#define time_event_declare(name) \
  extern clock_time_t time_event_base##name;\
  extern bool is_start_time_event##name

#define time_event_start(name) \
  do{\
      time_event_base##name = HAL_GetTick();\
      is_start_time_event##name = true;\
    }while(0)

#define time_event_loop(name, time_event_timeout_handle, timeout) \
  do{\
    if(is_start_time_event##name && (clock_time_t)(HAL_GetTick() - time_event_base##name) > timeout){ \
        is_start_time_event##name = false;\
        time_event_timeout_handle();\
      }\
    }while(0)

void timer0_mode1_init(void);
void timer0_irq(void);
void timer0_mode2_init(void);
void HAL_Delay(uint32_t Delay);
#endif
/*---------------------------------------------------------------------------*/
#ifdef LCD12864_DEV_DRIVER
typedef enum{
  LCD12864_GRAPHIC_CLEAR = 0x01,///<檫除点
  LCD12864_GRAPHIC_NOT = 0x02,///<反相点
  LCD12864_GRAPHIC_DRAW = 0X03///<画点
}lcd12864_graphic_e;

void lcd12864_init( void );
void lcd12864_show_one_chinese(uint8_t ucPage,uint8_t ucLine, const uint8_t * str);
void lcd12864_show_line_chinese(uint8_t ucPage,uint8_t ucLine, const uint8_t * str);
void lcd12864_show_one_char(uint8_t ucPage,uint8_t ucLine,uint8_t ch);
void lcd12864_show_string(uint8_t ucPage,uint8_t ucLine, uint8_t * str);
void lcd12864_show_graph(uint8_t ucPage,uint8_t ucLine,uint8_t ucWidth,uint8_t ucHigh,uint8_t * ucaGraph);
/*---------------------------------------------------------------------------*/
///画图相关函数
void lcd12864_clear_display(uint8_t clear_color);
void lcd12864_draw_point(uint8_t X, uint8_t Y, lcd12864_graphic_e Type);
void lcd12864_draw_line(uint8_t x1, uint8_t y1, uint8_t x2,uint8_t y2);
void lcd12864_draw_level(void);
void lcd12864_draw_sine_wave(void);
void lcd12864_draw_circle(uint8_t x0,uint8_t y0,uint8_t r);
void lcd12864_draw_retic(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1);
void lcd12864_draw_trigle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t x3,uint8_t y3);
void lcd12864_draw_dotted_line(uint8_t x1, uint8_t y1, uint8_t x2,uint8_t y2);
#endif

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __MAIN_H__ */




