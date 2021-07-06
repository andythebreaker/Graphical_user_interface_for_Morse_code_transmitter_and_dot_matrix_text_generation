#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/signal.h>
#include <asm/io.h>
#include <linux/timekeeping.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/dma-mapping.h>
#include <linux/hrtimer.h>
#include <linux/kthread.h>
#include <linux/delay.h>

//#define IF_TEST_ALL_CHAR_DISP
//#define IF_PRINT_STEP_ONE_TIME

#define MS_TO_NS(MS_INPUT) 1000000ll * (MS_INPUT)
#define NS_TO_MS(US_INPUT) (US_INPUT) / 1000000ll

#define JIFFIES_TIMER_GO(INPUT_STUFF_FOR_JIFFIES_TIMER_GO) \
    last_time = ktime_get();                               \
    if (timer_pending(&timer) == 1)                        \
    {                                                      \
        del_timer(&timer);                                 \
    }                                                      \
    timer_setup(&timer, timer_callback, 0);                \
    mod_timer(&timer, jiffies + msecs_to_jiffies(INPUT_STUFF_FOR_JIFFIES_TIMER_GO));

#define SCREEN_SHOW_FRAM(SCREEN_SHOW_FRAM_PATTERN)                                                                                        \
    row_pattern_obj = (row_pattern_foo){.row_pattern_foo_elem = SCREEN_SHOW_FRAM_PATTERN};                                                \
    for (SCREEN_SHOW_FRAM_for_loop_i = 0; SCREEN_SHOW_FRAM_for_loop_i < 49 * 8; SCREEN_SHOW_FRAM_for_loop_i++)                            \
    {                                                                                                                                     \
        screen_show_one_row(SCREEN_SHOW_FRAM_for_loop_i % 49, 0, SCREEN_SHOW_FRAM_for_loop_i / 49, row_pattern_obj.row_pattern_foo_elem); \
    }

#define ASCII88PATTERN_A                               \
    {                                                  \
        0x3C, 0x42, 0x81, 0x81, 0xFF, 0x81, 0x81, 0x81 \
    }
#define ASCII88PATTERN_B                               \
    {                                                  \
        0xFC, 0x82, 0x81, 0xFE, 0x82, 0x81, 0x82, 0xFC \
    }
#define ASCII88PATTERN_C                               \
    {                                                  \
        0x3C, 0x42, 0x81, 0x80, 0x80, 0x81, 0x42, 0x3C \
    }
#define ASCII88PATTERN_D                               \
    {                                                  \
        0xFC, 0x82, 0x81, 0x81, 0x81, 0x81, 0x82, 0xFC \
    }
#define ASCII88PATTERN_E                               \
    {                                                  \
        0xFF, 0x81, 0x80, 0xFC, 0x80, 0x80, 0x81, 0xFF \
    }
#define ASCII88PATTERN_F                               \
    {                                                  \
        0xFF, 0x81, 0x80, 0xFC, 0x80, 0x80, 0x80, 0x80 \
    }
#define ASCII88PATTERN_G                               \
    {                                                  \
        0xFF, 0x81, 0x80, 0x80, 0x87, 0x81, 0x81, 0xFF \
    }
#define ASCII88PATTERN_H                               \
    {                                                  \
        0x81, 0x81, 0x81, 0xFF, 0x81, 0x81, 0x81, 0x81 \
    }
#define ASCII88PATTERN_I                               \
    {                                                  \
        0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E \
    }
#define ASCII88PATTERN_J                               \
    {                                                  \
        0x3E, 0x08, 0x08, 0x08, 0x08, 0x48, 0x48, 0x30 \
    }
#define ASCII88PATTERN_K                               \
    {                                                  \
        0x42, 0x44, 0x48, 0x70, 0x48, 0x44, 0x42, 0x41 \
    }
#define ASCII88PATTERN_L                               \
    {                                                  \
        0x70, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7F \
    }
#define ASCII88PATTERN_M                               \
    {                                                  \
        0x81, 0xC3, 0xA5, 0x99, 0x81, 0x81, 0x81, 0x81 \
    }
#define ASCII88PATTERN_N                               \
    {                                                  \
        0x81, 0xC1, 0xA1, 0x91, 0x89, 0x85, 0x83, 0x81 \
    }
#define ASCII88PATTERN_O                               \
    {                                                  \
        0x3C, 0x42, 0x81, 0x81, 0x81, 0x81, 0x42, 0x3C \
    }
#define ASCII88PATTERN_P                               \
    {                                                  \
        0xFC, 0x42, 0x41, 0x42, 0x7C, 0x40, 0x40, 0xE0 \
    }
#define ASCII88PATTERN_Q                               \
    {                                                  \
        0x3C, 0x42, 0x42, 0x42, 0x4A, 0x46, 0x3C, 0x03 \
    }
#define ASCII88PATTERN_R                               \
    {                                                  \
        0xFC, 0x42, 0x42, 0x7C, 0x48, 0x44, 0x42, 0xC1 \
    }
#define ASCII88PATTERN_S                               \
    {                                                  \
        0x3C, 0x42, 0x40, 0x40, 0x3C, 0x02, 0x42, 0x3C \
    }
#define ASCII88PATTERN_T                               \
    {                                                  \
        0x7F, 0x49, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C \
    }
#define ASCII88PATTERN_U                               \
    {                                                  \
        0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x42, 0x3C \
    }
#define ASCII88PATTERN_V                               \
    {                                                  \
        0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x14, 0x08 \
    }
#define ASCII88PATTERN_W                               \
    {                                                  \
        0x82, 0x82, 0x82, 0x82, 0x82, 0x92, 0x54, 0x28 \
    }
#define ASCII88PATTERN_X                               \
    {                                                  \
        0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81 \
    }
#define ASCII88PATTERN_Y                               \
    {                                                  \
        0x41, 0x22, 0x14, 0x08, 0x08, 0x08, 0x08, 0x08 \
    }
#define ASCII88PATTERN_Z                               \
    {                                                  \
        0xFF, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0xFF \
    }
#define ASCII88PATTERN_0                               \
    {                                                  \
        0x3C, 0x42, 0x85, 0x89, 0x91, 0xA1, 0x42, 0x3C \
    }
#define ASCII88PATTERN_1                               \
    {                                                  \
        0x18, 0x28, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E \
    }
#define ASCII88PATTERN_2                               \
    {                                                  \
        0x3C, 0x42, 0x42, 0x04, 0x08, 0x10, 0x20, 0x7E \
    }
#define ASCII88PATTERN_3                               \
    {                                                  \
        0x3C, 0x42, 0x02, 0x04, 0x08, 0x04, 0x42, 0x3C \
    }
#define ASCII88PATTERN_4                               \
    {                                                  \
        0x04, 0x0C, 0x14, 0x24, 0x44, 0xFF, 0x04, 0x04 \
    }
#define ASCII88PATTERN_5                               \
    {                                                  \
        0x3E, 0x20, 0x20, 0x3C, 0x02, 0x02, 0x02, 0x3C \
    }
#define ASCII88PATTERN_6                               \
    {                                                  \
        0x1C, 0x22, 0x20, 0x3C, 0x22, 0x22, 0x22, 0x1C \
    }
#define ASCII88PATTERN_7                               \
    {                                                  \
        0x7E, 0x42, 0x04, 0x08, 0x10, 0x20, 0x20, 0x20 \
    }
#define ASCII88PATTERN_8                               \
    {                                                  \
        0x1C, 0x22, 0x22, 0x22, 0x1C, 0x22, 0x22, 0x1C \
    }
#define ASCII88PATTERN_9                               \
    {                                                  \
        0x1C, 0x22, 0x22, 0x22, 0x1E, 0x02, 0x22, 0x1C \
    }
#define ASCII88PATTERN_DOT                             \
    {                                                  \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18 \
    }
#define ASCII88PATTERN_LB                              \
    {                                                  \
        0x08, 0x10, 0x20, 0x20, 0x20, 0x20, 0x10, 0x08 \
    }
#define ASCII88PATTERN_RB                              \
    {                                                  \
        0x20, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x20 \
    }
#define ASCII88PATTERN_PLUS                            \
    {                                                  \
        0x08, 0x08, 0x08, 0x7F, 0x08, 0x08, 0x08, 0x00 \
    }
#define ASCII88PATTERN_SP                              \
    {                                                  \
        0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x08, 0x10 \
    }
#define ASCII88PATTERN_UQ                              \
    {                                                  \
        0x08, 0x00, 0x08, 0x0C, 0x02, 0x22, 0x14, 0x08 \
    }
#define ASCII88PATTERN_COMA                            \
    {                                                  \
        0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x08, 0x10 \
    }
#define ASCII88PATTERN_DASH                            \
    {                                                  \
        0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00 \
    }
#define ASCII88PATTERN_EQ                              \
    {                                                  \
        0x00, 0x00, 0x3C, 0x00, 0x00, 0x3C, 0x00, 0x00 \
    }
#define ASCII88PATTERN_UEXC                            \
    {                                                  \
        0x10, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10 \
    }
#define ASCII88PATTERN_QM                              \
    {                                                  \
        0x18, 0x24, 0x04, 0x04, 0x18, 0x10, 0x00, 0x10 \
    }
#define ASCII88PATTERN_AND                             \
    {                                                  \
        0x3C, 0x42, 0x42, 0x2C, 0x33, 0x4C, 0x44, 0x3B \
    }
#define ASCII88PATTERN_DD                              \
    {                                                  \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF \
    }
#define ASCII88PATTERN_MONY                            \
    {                                                  \
        0x10, 0x7C, 0x92, 0x90, 0x7C, 0x12, 0x92, 0x7C \
    }
#define ASCII88PATTERN_EXC                             \
    {                                                  \
        0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x10 \
    }
#define ASCII88PATTERN_SQUT                            \
    {                                                  \
        0x08, 0x10, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00 \
    }
#define ASCII88PATTERN_COLN                            \
    {                                                  \
        0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00 \
    }
#define ASCII88PATTERN_DQUT                            \
    {                                                  \
        0x09, 0x12, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00 \
    }
#define ASCII88PATTERN_AT                              \
    {                                                  \
        0x78, 0x8A, 0x9A, 0xAA, 0xAA, 0x9A, 0x82, 0x7C \
    }
#define ASCII88PATTERN_SLAH                            \
    {                                                  \
        0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 \
    }
#define ASCII88PATTERN_FFFF                            \
    {                                                  \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF \
    }

#define UP_HAT_SW1 25
#define UP_HAT_LED1 24
#define UP_HAT_74HC165_PL 19
#define UP_HAT_74HC165_Q7 26
#define UP_HAT_74HC165_CE 6
#define UP_HAT_74HC165_CP 5
#define UP_HAT_74HC595_DS 27
#define UP_HAT_74HC595_STCP 22
#define UP_HAT_74HC595_SHCP 23
#define UP_HAT_MAX7219_LOAD 8
#define UP_HAT_MAX7219_DIN 10
#define UP_HAT_MAX7219_CLK 11
#define UP_HAT_MCP3201_CLK 18
#define UP_HAT_MCP3201_DOUT 20
#define UP_HAT_MCP3201_CS 21
#define UP_HAT_LED5 12

#define INTERVAL_FAST_MS 100
#define INTERVAL_SLOW_MS 500
#define DEBOUNCE_BUFFER 50

#define TIME_BETWEEN_CHAR 3000
#define TIME_BETWEEN_PATTERN_SHORT 800
#define TIME_BETWEEN_PATTERN_STANDER 1000
#define TIME_BETWEEN_PATTERN_LONG 1200
#define TIME_DASH_SHORT 2400
#define TIME_DASH_STANDER 3000
#define TIME_DASH_LONG 3600
#define TIME_DOT_SHORT 800
#define TIME_DOT_STANDER 1000
#define TIME_DOT_LONG 1200

#define HRTIMER_MIN_TIME_INTERVAL 20
#define TIME_ERROR_BLINK_ALL 100
#define ERROR_BLINK_COUNTER_MAX_LIGHT_TIME 4

#define IRQ_NAME "button_1"
#define SERIAL_DEVICE "/dev/ttyS0"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("andythebreaker");
MODULE_DESCRIPTION("m2");
MODULE_VERSION("16.0");

static short int button_irq_id = 0;
//static char is_on = 0;
static char is_press = 0;

//static int timeout_ms = INTERVAL_SLOW_MS;
struct timer_list timer;
static ktime_t last_time;
static ktime_t last_relase;
static ktime_t last_press;

#ifdef IF_TEST_ALL_CHAR_DISP
static int TEST_ALL_CHAR_DISP_index = 0;
#endif
//static char bool_on_error = 0;
static uint8_t morse_pattern_status = 0;
static short int SCREEN_SHOW_FRAM_for_loop_i = 0;
typedef struct row_pattern_foo_struct
{
    uint8_t row_pattern_foo_elem[8];
} row_pattern_foo;
row_pattern_foo row_pattern_obj;
static short int loopi = 0;
static uint8_t led_status_3[8] = {1, 1, 1, 0, 0, 0, 0, 0};

static bool able_state_flag = true;
static bool able_press_flag = true;
static bool able_relase_flag = false;
static bool hrtimer_try_to_cancel_flag_hr_timer = false;
static bool target_morse_pattern_error_event_flag = false;
static bool target_input_length_error_event_flag = false;
static bool target_input_time_error_event_flag = false;
static bool infinite_flashing_input_time_error_event = false;
static short int morse_pattern_error_blink_counter = 0;
static short int input_length_error_blink_counter = 0;
static short int input_time_error_blink_counter = 0;

static struct hrtimer hr_timer;
static ktime_t ktime_interval;
static s64 starttime_ns;

typedef enum timer_callback_state_enum TCS;
enum timer_callback_state_enum
{
    timer_callback_state_ppt_blue0,
    timer_callback_state_ppt_blue1,
    timer_callback_state_ppt_blue2,
    timer_callback_state_ppt_blue3,
    timer_callback_state_big_if_else2,
    timer_callback_state_big_if_else5,
    timer_callback_state_set_timer_between_pattern_long_call_back_send_zero_set_var_last_relase,
    timer_callback_state_after_success
};
static TCS timer_callback_state = timer_callback_state_ppt_blue0;

//pre declare
static void call_back_fucn_n(void);
static void timer_callback(struct timer_list *arg);
static void morse_pattern_logic(char input_bool);

static void every_time_at_success(void)
{
    able_state_flag = false;
    timer_callback_state = timer_callback_state_after_success;
    JIFFIES_TIMER_GO(TIME_BETWEEN_CHAR)
}

static void set_timer_between_pattern_long_call_back_send_zero_set_var_last_relase(ktime_t last_relase_input)
{
    last_relase = last_relase_input;
    timer_callback_state = timer_callback_state_set_timer_between_pattern_long_call_back_send_zero_set_var_last_relase;
    JIFFIES_TIMER_GO(TIME_BETWEEN_PATTERN_LONG)
}

static void call_back_fucn_n(void)
{
    able_press_flag = true;
    if (hrtimer_try_to_cancel_flag_hr_timer)
    {
        hrtimer_try_to_cancel_flag_hr_timer = false;
    }
    else
    {
        gpio_direction_output(UP_HAT_LED5, 1);
        able_press_flag = false;
        timer_callback_state = timer_callback_state_ppt_blue2;
        JIFFIES_TIMER_GO(TIME_DASH_STANDER)
        able_relase_flag = true;
        last_press = ktime_get();
    }
}

static enum hrtimer_restart my_hrtimer_callback(struct hrtimer *timer)
{
    short int i = 0;
#ifdef IF_TEST_ALL_CHAR_DISP
    switch (TEST_ALL_CHAR_DISP_index)
    {
    case (0):
        morse_pattern_logic(1);
        break;
    case (1):
        morse_pattern_logic(2);
        break;
    case (2):
        morse_pattern_logic(0);
        break;
    case (3):
        morse_pattern_logic(2);
        break;
    case (4):
        morse_pattern_logic(1);
        break;
    case (5):
        morse_pattern_logic(1);
        break;
    case (6):
        morse_pattern_logic(1);
        break;
    case (7):
        morse_pattern_logic(0);
        break;
    case (8):
        morse_pattern_logic(2);
        break;
    case (9):
        morse_pattern_logic(1);
        break;
    case (10):
        morse_pattern_logic(2);
        break;
    case (11):
        morse_pattern_logic(1);
        break;
    case (12):
        morse_pattern_logic(0);
        break;
    case (13):
        morse_pattern_logic(2);
        break;
    case (14):
        morse_pattern_logic(1);
        break;
    case (15):
        morse_pattern_logic(1);
        break;
    case (16):
        morse_pattern_logic(0);
        break;
    case (17):
        morse_pattern_logic(1);
        break;
    case (18):
        morse_pattern_logic(0);
        break;
    case (19):
        morse_pattern_logic(1);
        break;
    case (20):
        morse_pattern_logic(1);
        break;
    case (21):
        morse_pattern_logic(2);
        break;
    case (22):
        morse_pattern_logic(1);
        break;
    case (23):
        morse_pattern_logic(0);
        break;
    case (24):
        morse_pattern_logic(2);
        break;
    case (25):
        morse_pattern_logic(2);
        break;
    case (26):
        morse_pattern_logic(1);
        break;
    case (27):
        morse_pattern_logic(0);
        break;
    case (28):
        morse_pattern_logic(1);
        break;
    case (29):
        morse_pattern_logic(1);
        break;
    case (30):
        morse_pattern_logic(1);
        break;
    case (31):
        morse_pattern_logic(1);
        break;
    case (32):
        morse_pattern_logic(0);
        break;
    case (33):
        morse_pattern_logic(1);
        break;
    case (34):
        morse_pattern_logic(1);
        break;
    case (35):
        morse_pattern_logic(0);
        break;
    case (36):
        morse_pattern_logic(1);
        break;
    case (37):
        morse_pattern_logic(2);
        break;
    case (38):
        morse_pattern_logic(2);
        break;
    case (39):
        morse_pattern_logic(2);
        break;
    case (40):
        morse_pattern_logic(0);
        break;
    case (41):
        morse_pattern_logic(2);
        break;
    case (42):
        morse_pattern_logic(1);
        break;
    case (43):
        morse_pattern_logic(2);
        break;
    case (44):
        morse_pattern_logic(0);
        break;
    case (45):
        morse_pattern_logic(1);
        break;
    case (46):
        morse_pattern_logic(2);
        break;
    case (47):
        morse_pattern_logic(1);
        break;
    case (48):
        morse_pattern_logic(1);
        break;
    case (49):
        morse_pattern_logic(0);
        break;
    case (50):
        morse_pattern_logic(2);
        break;
    case (51):
        morse_pattern_logic(2);
        break;
    case (52):
        morse_pattern_logic(0);
        break;
    case (53):
        morse_pattern_logic(2);
        break;
    case (54):
        morse_pattern_logic(1);
        break;
    case (55):
        morse_pattern_logic(0);
        break;
    case (56):
        morse_pattern_logic(2);
        break;
    case (57):
        morse_pattern_logic(2);
        break;
    case (58):
        morse_pattern_logic(2);
        break;
    case (59):
        morse_pattern_logic(0);
        break;
    case (60):
        morse_pattern_logic(1);
        break;
    case (61):
        morse_pattern_logic(2);
        break;
    case (62):
        morse_pattern_logic(2);
        break;
    case (63):
        morse_pattern_logic(1);
        break;
    case (64):
        morse_pattern_logic(0);
        break;
    case (65):
        morse_pattern_logic(2);
        break;
    case (66):
        morse_pattern_logic(2);
        break;
    case (67):
        morse_pattern_logic(1);
        break;
    case (68):
        morse_pattern_logic(2);
        break;
    case (69):
        morse_pattern_logic(0);
        break;
    case (70):
        morse_pattern_logic(1);
        break;
    case (71):
        morse_pattern_logic(2);
        break;
    case (72):
        morse_pattern_logic(1);
        break;
    case (73):
        morse_pattern_logic(0);
        break;
    case (74):
        morse_pattern_logic(1);
        break;
    case (75):
        morse_pattern_logic(1);
        break;
    case (76):
        morse_pattern_logic(1);
        break;
    case (77):
        morse_pattern_logic(0);
        break;
    case (78):
        morse_pattern_logic(2);
        break;
    case (79):
        morse_pattern_logic(0);
        break;
    case (80):
        morse_pattern_logic(1);
        break;
    case (81):
        morse_pattern_logic(1);
        break;
    case (82):
        morse_pattern_logic(2);
        break;
    case (83):
        morse_pattern_logic(0);
        break;
    case (84):
        morse_pattern_logic(1);
        break;
    case (85):
        morse_pattern_logic(1);
        break;
    case (86):
        morse_pattern_logic(1);
        break;
    case (87):
        morse_pattern_logic(2);
        break;
    case (88):
        morse_pattern_logic(0);
        break;
    case (89):
        morse_pattern_logic(1);
        break;
    case (90):
        morse_pattern_logic(2);
        break;
    case (91):
        morse_pattern_logic(2);
        break;
    case (92):
        morse_pattern_logic(0);
        break;
    case (93):
        morse_pattern_logic(2);
        break;
    case (94):
        morse_pattern_logic(1);
        break;
    case (95):
        morse_pattern_logic(1);
        break;
    case (96):
        morse_pattern_logic(2);
        break;
    case (97):
        morse_pattern_logic(0);
        break;
    case (98):
        morse_pattern_logic(2);
        break;
    case (99):
        morse_pattern_logic(1);
        break;
    case (100):
        morse_pattern_logic(2);
        break;
    case (101):
        morse_pattern_logic(2);
        break;
    case (102):
        morse_pattern_logic(0);
        break;
    case (103):
        morse_pattern_logic(2);
        break;
    case (104):
        morse_pattern_logic(2);
        break;
    case (105):
        morse_pattern_logic(1);
        break;
    case (106):
        morse_pattern_logic(1);
        break;
    case (107):
        morse_pattern_logic(0);
        break;
    case (108):
        morse_pattern_logic(2);
        break;
    case (109):
        morse_pattern_logic(2);
        break;
    case (110):
        morse_pattern_logic(2);
        break;
    case (111):
        morse_pattern_logic(2);
        break;
    case (112):
        morse_pattern_logic(2);
        break;
    case (113):
        morse_pattern_logic(0);
        break;
    case (114):
        morse_pattern_logic(1);
        break;
    case (115):
        morse_pattern_logic(2);
        break;
    case (116):
        morse_pattern_logic(2);
        break;
    case (117):
        morse_pattern_logic(2);
        break;
    case (118):
        morse_pattern_logic(2);
        break;
    case (119):
        morse_pattern_logic(0);
        break;
    case (120):
        morse_pattern_logic(1);
        break;
    case (121):
        morse_pattern_logic(1);
        break;
    case (122):
        morse_pattern_logic(2);
        break;
    case (123):
        morse_pattern_logic(2);
        break;
    case (124):
        morse_pattern_logic(2);
        break;
    case (125):
        morse_pattern_logic(0);
        break;
    case (126):
        morse_pattern_logic(1);
        break;
    case (127):
        morse_pattern_logic(1);
        break;
    case (128):
        morse_pattern_logic(1);
        break;
    case (129):
        morse_pattern_logic(2);
        break;
    case (130):
        morse_pattern_logic(2);
        break;
    case (131):
        morse_pattern_logic(0);
        break;
    case (132):
        morse_pattern_logic(1);
        break;
    case (133):
        morse_pattern_logic(1);
        break;
    case (134):
        morse_pattern_logic(1);
        break;
    case (135):
        morse_pattern_logic(1);
        break;
    case (136):
        morse_pattern_logic(2);
        break;
    case (137):
        morse_pattern_logic(0);
        break;
    case (138):
        morse_pattern_logic(1);
        break;
    case (139):
        morse_pattern_logic(1);
        break;
    case (140):
        morse_pattern_logic(1);
        break;
    case (141):
        morse_pattern_logic(1);
        break;
    case (142):
        morse_pattern_logic(1);
        break;
    case (143):
        morse_pattern_logic(0);
        break;
    case (144):
        morse_pattern_logic(2);
        break;
    case (145):
        morse_pattern_logic(1);
        break;
    case (146):
        morse_pattern_logic(1);
        break;
    case (147):
        morse_pattern_logic(1);
        break;
    case (148):
        morse_pattern_logic(1);
        break;
    case (149):
        morse_pattern_logic(0);
        break;
    case (150):
        morse_pattern_logic(2);
        break;
    case (151):
        morse_pattern_logic(2);
        break;
    case (152):
        morse_pattern_logic(1);
        break;
    case (153):
        morse_pattern_logic(1);
        break;
    case (154):
        morse_pattern_logic(1);
        break;
    case (155):
        morse_pattern_logic(0);
        break;
    case (156):
        morse_pattern_logic(2);
        break;
    case (157):
        morse_pattern_logic(2);
        break;
    case (158):
        morse_pattern_logic(2);
        break;
    case (159):
        morse_pattern_logic(1);
        break;
    case (160):
        morse_pattern_logic(1);
        break;
    case (161):
        morse_pattern_logic(0);
        break;
    case (162):
        morse_pattern_logic(2);
        break;
    case (163):
        morse_pattern_logic(2);
        break;
    case (164):
        morse_pattern_logic(2);
        break;
    case (165):
        morse_pattern_logic(2);
        break;
    case (166):
        morse_pattern_logic(1);
        break;
    case (167):
        morse_pattern_logic(0);
        break;
    case (168):
        morse_pattern_logic(1);
        break;
    case (169):
        morse_pattern_logic(2);
        break;
    case (170):
        morse_pattern_logic(1);
        break;
    case (171):
        morse_pattern_logic(2);
        break;
    case (172):
        morse_pattern_logic(1);
        break;
    case (173):
        morse_pattern_logic(2);
        break;
    case (174):
        morse_pattern_logic(0);
        break;
    case (175):
        morse_pattern_logic(2);
        break;
    case (176):
        morse_pattern_logic(1);
        break;
    case (177):
        morse_pattern_logic(2);
        break;
    case (178):
        morse_pattern_logic(2);
        break;
    case (179):
        morse_pattern_logic(1);
        break;
    case (180):
        morse_pattern_logic(0);
        break;
    case (181):
        morse_pattern_logic(2);
        break;
    case (182):
        morse_pattern_logic(1);
        break;
    case (183):
        morse_pattern_logic(2);
        break;
    case (184):
        morse_pattern_logic(2);
        break;
    case (185):
        morse_pattern_logic(1);
        break;
    case (186):
        morse_pattern_logic(2);
        break;
    case (187):
        morse_pattern_logic(0);
        break;
    case (188):
        morse_pattern_logic(1);
        break;
    case (189):
        morse_pattern_logic(2);
        break;
    case (190):
        morse_pattern_logic(1);
        break;
    case (191):
        morse_pattern_logic(2);
        break;
    case (192):
        morse_pattern_logic(1);
        break;
    case (193):
        morse_pattern_logic(0);
        break;
    case (194):
        morse_pattern_logic(2);
        break;
    case (195):
        morse_pattern_logic(1);
        break;
    case (196):
        morse_pattern_logic(2);
        break;
    case (197):
        morse_pattern_logic(1);
        break;
    case (198):
        morse_pattern_logic(2);
        break;
    case (199):
        morse_pattern_logic(1);
        break;
    case (200):
        morse_pattern_logic(0);
        break;
    case (201):
        morse_pattern_logic(1);
        break;
    case (202):
        morse_pattern_logic(1);
        break;
    case (203):
        morse_pattern_logic(2);
        break;
    case (204):
        morse_pattern_logic(1);
        break;
    case (205):
        morse_pattern_logic(2);
        break;
    case (206):
        morse_pattern_logic(0);
        break;
    case (207):
        morse_pattern_logic(2);
        break;
    case (208):
        morse_pattern_logic(2);
        break;
    case (209):
        morse_pattern_logic(1);
        break;
    case (210):
        morse_pattern_logic(1);
        break;
    case (211):
        morse_pattern_logic(2);
        break;
    case (212):
        morse_pattern_logic(2);
        break;
    case (213):
        morse_pattern_logic(0);
        break;
    case (214):
        morse_pattern_logic(2);
        break;
    case (215):
        morse_pattern_logic(1);
        break;
    case (216):
        morse_pattern_logic(1);
        break;
    case (217):
        morse_pattern_logic(1);
        break;
    case (218):
        morse_pattern_logic(1);
        break;
    case (219):
        morse_pattern_logic(2);
        break;
    case (220):
        morse_pattern_logic(0);
        break;
    case (221):
        morse_pattern_logic(2);
        break;
    case (222):
        morse_pattern_logic(1);
        break;
    case (223):
        morse_pattern_logic(1);
        break;
    case (224):
        morse_pattern_logic(1);
        break;
    case (225):
        morse_pattern_logic(2);
        break;
    case (226):
        morse_pattern_logic(0);
        break;
    case (227):
        morse_pattern_logic(2);
        break;
    case (228):
        morse_pattern_logic(2);
        break;
    case (229):
        morse_pattern_logic(1);
        break;
    case (230):
        morse_pattern_logic(1);
        break;
    case (231):
        morse_pattern_logic(1);
        break;
    case (232):
        morse_pattern_logic(2);
        break;
    case (233):
        morse_pattern_logic(0);
        break;
    case (234):
        morse_pattern_logic(1);
        break;
    case (235):
        morse_pattern_logic(1);
        break;
    case (236):
        morse_pattern_logic(2);
        break;
    case (237):
        morse_pattern_logic(2);
        break;
    case (238):
        morse_pattern_logic(1);
        break;
    case (239):
        morse_pattern_logic(1);
        break;
    case (240):
        morse_pattern_logic(0);
        break;
    case (241):
        morse_pattern_logic(1);
        break;
    case (242):
        morse_pattern_logic(2);
        break;
    case (243):
        morse_pattern_logic(1);
        break;
    case (244):
        morse_pattern_logic(1);
        break;
    case (245):
        morse_pattern_logic(1);
        break;
    case (246):
        morse_pattern_logic(0);
        break;
    case (247):
        morse_pattern_logic(1);
        break;
    case (248):
        morse_pattern_logic(1);
        break;
    case (249):
        morse_pattern_logic(2);
        break;
    case (250):
        morse_pattern_logic(2);
        break;
    case (251):
        morse_pattern_logic(1);
        break;
    case (252):
        morse_pattern_logic(2);
        break;
    case (253):
        morse_pattern_logic(0);
        break;
    case (254):
        morse_pattern_logic(1);
        break;
    case (255):
        morse_pattern_logic(1);
        break;
    case (256):
        morse_pattern_logic(1);
        break;
    case (257):
        morse_pattern_logic(2);
        break;
    case (258):
        morse_pattern_logic(1);
        break;
    case (259):
        morse_pattern_logic(1);
        break;
    case (260):
        morse_pattern_logic(2);
        break;
    case (261):
        morse_pattern_logic(0);
        break;
    case (262):
        morse_pattern_logic(2);
        break;
    case (263):
        morse_pattern_logic(1);
        break;
    case (264):
        morse_pattern_logic(2);
        break;
    case (265):
        morse_pattern_logic(1);
        break;
    case (266):
        morse_pattern_logic(2);
        break;
    case (267):
        morse_pattern_logic(2);
        break;
    case (268):
        morse_pattern_logic(0);
        break;
    case (269):
        morse_pattern_logic(1);
        break;
    case (270):
        morse_pattern_logic(2);
        break;
    case (271):
        morse_pattern_logic(2);
        break;
    case (272):
        morse_pattern_logic(2);
        break;
    case (273):
        morse_pattern_logic(2);
        break;
    case (274):
        morse_pattern_logic(1);
        break;
    case (275):
        morse_pattern_logic(0);
        break;
    case (276):
        morse_pattern_logic(2);
        break;
    case (277):
        morse_pattern_logic(2);
        break;
    case (278):
        morse_pattern_logic(2);
        break;
    case (279):
        morse_pattern_logic(1);
        break;
    case (280):
        morse_pattern_logic(1);
        break;
    case (281):
        morse_pattern_logic(1);
        break;
    case (282):
        morse_pattern_logic(0);
        break;
    case (283):
        morse_pattern_logic(1);
        break;
    case (284):
        morse_pattern_logic(2);
        break;
    case (285):
        morse_pattern_logic(1);
        break;
    case (286):
        morse_pattern_logic(1);
        break;
    case (287):
        morse_pattern_logic(2);
        break;
    case (288):
        morse_pattern_logic(1);
        break;
    case (289):
        morse_pattern_logic(0);
        break;
    case (290):
        morse_pattern_logic(1);
        break;
    case (291):
        morse_pattern_logic(2);
        break;
    case (292):
        morse_pattern_logic(2);
        break;
    case (293):
        morse_pattern_logic(1);
        break;
    case (294):
        morse_pattern_logic(2);
        break;
    case (295):
        morse_pattern_logic(1);
        break;
    case (296):
        morse_pattern_logic(0);
        break;
    case (297):
        morse_pattern_logic(2);
        break;
    case (298):
        morse_pattern_logic(1);
        break;
    case (299):
        morse_pattern_logic(1);
        break;
    case (300):
        morse_pattern_logic(2);
        break;
    case (301):
        morse_pattern_logic(1);
        break;
    case (302):
        morse_pattern_logic(0);
        break;

    default:
        TEST_ALL_CHAR_DISP_index = 0;
        break;
    }
    TEST_ALL_CHAR_DISP_index++;
#endif

    hrtimer_forward(&hr_timer, hr_timer._softexpires, ktime_interval);

    if (target_morse_pattern_error_event_flag)
    {
        if (morse_pattern_error_blink_counter < ERROR_BLINK_COUNTER_MAX_LIGHT_TIME * 2)
        {
            led_status_3[0] = !led_status_3[0];
            morse_pattern_error_blink_counter++;
        }
        else
        {
            morse_pattern_error_blink_counter = 0;
            led_status_3[0] = 0;
            target_morse_pattern_error_event_flag = false;
        }
    }
    else
    {
        //no move
    }
    if (target_input_length_error_event_flag)
    {
        if (input_length_error_blink_counter < ERROR_BLINK_COUNTER_MAX_LIGHT_TIME * 2)
        {
            led_status_3[1] = !led_status_3[1];
            input_length_error_blink_counter++;
        }
        else
        {
            input_length_error_blink_counter = 0;
            led_status_3[1] = 0;
            target_input_length_error_event_flag = false;
        }
    }
    else
    {
        //no move
    }
    if (target_input_time_error_event_flag)
    {
        if (input_time_error_blink_counter < ERROR_BLINK_COUNTER_MAX_LIGHT_TIME * 2)
        {
            led_status_3[2] = !led_status_3[2];
            input_time_error_blink_counter++;
        }
        else
        {
            input_time_error_blink_counter = 0;
            led_status_3[2] = 0;
            target_input_time_error_event_flag = false;
        }
    }
    else
    {
        //no move
    }

    gpio_direction_output(UP_HAT_74HC595_STCP, 0);
    for (i = 7; i >= 0; i--)
    {
        gpio_direction_output(UP_HAT_74HC595_SHCP, 0);
        gpio_direction_output(UP_HAT_74HC595_DS, led_status_3[i]);
        gpio_direction_output(UP_HAT_74HC595_SHCP, 1);
    }
    gpio_direction_output(UP_HAT_74HC595_STCP, 1);

    return HRTIMER_RESTART;
}

static void init_error_3_led(void)
{
    short int i = 0;
    gpio_direction_output(UP_HAT_74HC595_STCP, 0);
    for (i = 7; i >= 0; i--)
    {
        gpio_direction_output(UP_HAT_74HC595_SHCP, 0);
        gpio_direction_output(UP_HAT_74HC595_DS, led_status_3[i]);
        gpio_direction_output(UP_HAT_74HC595_SHCP, 1);
    }
    gpio_direction_output(UP_HAT_74HC595_STCP, 1);
}

static void screen_show_one_row(short int screen_status_pa_49, uint8_t bool_setting, uint8_t row_pattern_index, uint8_t *row_pattern)
{
    //printk(KERN_DEBUG "\nvoid screen_show_one_row(short int screen_status_pa_49, uint8_t bool_setting, uint8_t row_pattern_index, uint8_t *row_pattern)\n");
    switch (screen_status_pa_49)
    {
    case 0:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        gpio_direction_output(UP_HAT_MAX7219_LOAD, 0);
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 1:
        gpio_direction_output(UP_HAT_MAX7219_DIN, ((row_pattern_index + 1) >> 7) & 1u);
        break;
    case 2:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 3:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 4:
        gpio_direction_output(UP_HAT_MAX7219_DIN, ((row_pattern_index + 1) >> 6) & 1u);
        break;
    case 5:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 6:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 7:
        gpio_direction_output(UP_HAT_MAX7219_DIN, ((row_pattern_index + 1) >> 5) & 1u);
        break;
    case 8:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 9:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 10:
        gpio_direction_output(UP_HAT_MAX7219_DIN, ((row_pattern_index + 1) >> 4) & 1u);
        break;
    case 11:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 12:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 13:
        gpio_direction_output(UP_HAT_MAX7219_DIN, ((row_pattern_index + 1) >> 3) & 1u);
        break;
    case 14:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 15:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 16:
        gpio_direction_output(UP_HAT_MAX7219_DIN, ((row_pattern_index + 1) >> 2) & 1u);
        break;
    case 17:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 18:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 19:
        gpio_direction_output(UP_HAT_MAX7219_DIN, ((row_pattern_index + 1) >> 1) & 1u);
        break;
    case 20:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 21:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 22:
        gpio_direction_output(UP_HAT_MAX7219_DIN, ((row_pattern_index + 1) >> 0) & 1u);
        break;
    case 23:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 24:

        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 25:
        gpio_direction_output(UP_HAT_MAX7219_DIN, (row_pattern[(bool_setting == 1) ? 0 : row_pattern_index] >> 7) & 1u);
        break;
    case 26:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 27:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 28:
        gpio_direction_output(UP_HAT_MAX7219_DIN, (row_pattern[(bool_setting == 1) ? 0 : row_pattern_index] >> 6) & 1u);
        break;
    case 29:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 30:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 31:
        gpio_direction_output(UP_HAT_MAX7219_DIN, (row_pattern[(bool_setting == 1) ? 0 : row_pattern_index] >> 5) & 1u);
        break;
    case 32:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 33:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 34:
        gpio_direction_output(UP_HAT_MAX7219_DIN, (row_pattern[(bool_setting == 1) ? 0 : row_pattern_index] >> 4) & 1u);
        break;
    case 35:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 36:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 37:
        gpio_direction_output(UP_HAT_MAX7219_DIN, (row_pattern[(bool_setting == 1) ? 0 : row_pattern_index] >> 3) & 1u);
        break;
    case 38:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 39:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 40:
        gpio_direction_output(UP_HAT_MAX7219_DIN, (row_pattern[(bool_setting == 1) ? 0 : row_pattern_index] >> 2) & 1u);
        break;
    case 41:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 42:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 43:
        gpio_direction_output(UP_HAT_MAX7219_DIN, (row_pattern[(bool_setting == 1) ? 0 : row_pattern_index] >> 1) & 1u);
        break;
    case 44:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 45:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        break;
    case 46:
        gpio_direction_output(UP_HAT_MAX7219_DIN, (row_pattern[(bool_setting == 1) ? 0 : row_pattern_index] >> 0) & 1u);
        break;
    case 47:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 1);
        break;
    case 48:
        gpio_direction_output(UP_HAT_MAX7219_CLK, 0);
        gpio_direction_output(UP_HAT_MAX7219_LOAD, 1);
        break;
    default:
        printk(KERN_ERR "\non fucn. 'screen_show_one_row' went into default !!!\n");
        break;
    }
}

static void all_error_parrent_event(void)
{
    gpio_direction_output(UP_HAT_LED1, 1);
    gpio_direction_output(UP_HAT_LED5, 1);
}

static void target_morse_pattern_error_event(void)
{
    //SCREEN_SHOW_FRAM(ASCII88PATTERN_A)
    target_morse_pattern_error_event_flag = true;
    all_error_parrent_event();
}
static void target_input_length_error_event(void)
{
    printk(KERN_DEBUG "\nkey word : target_input_length_error_event(void) => morse_pattern_logic(0);\n");
    morse_pattern_logic(0);
    //SCREEN_SHOW_FRAM(ASCII88PATTERN_B)
    target_input_length_error_event_flag = true;
    all_error_parrent_event();
}
static void target_input_time_error_event(void)
{
    printk(KERN_DEBUG "\nkey word : target_input_time_error_event(void) => morse_pattern_logic(0);\n");
    morse_pattern_logic(0);
    //SCREEN_SHOW_FRAM(ASCII88PATTERN_C)
    target_input_time_error_event_flag = true;
    all_error_parrent_event();
}
static void morse_pattern_logic(char input_bool)
{

    switch (morse_pattern_status)
    {
    case 0:
        if (input_bool == 1)
        {
            morse_pattern_status = 1;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 2;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 1:
        if (input_bool == 1)
        {
            morse_pattern_status = 3;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 4;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_E)
        }
        break;
    case 2:
        if (input_bool == 1)
        {
            morse_pattern_status = 5;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 6;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_T)
        }
        break;
    case 3:
        if (input_bool == 1)
        {
            morse_pattern_status = 7;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 8;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_I)
        }
        break;
    case 4:
        if (input_bool == 1)
        {
            morse_pattern_status = 9;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 10;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_A)
        }
        break;
    case 5:
        if (input_bool == 1)
        {
            morse_pattern_status = 11;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 12;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_N)
        }
        break;
    case 6:
        if (input_bool == 1)
        {
            morse_pattern_status = 13;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 14;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_M)
        }
        break;
    case 7:
        if (input_bool == 1)
        {
            morse_pattern_status = 15;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 16;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_S)
        }
        break;
    case 8:
        if (input_bool == 1)
        {
            morse_pattern_status = 17;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 18;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_U)
        }
        break;
    case 9:
        if (input_bool == 1)
        {
            morse_pattern_status = 19;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 20;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_R)
        }
        break;
    case 10:
        if (input_bool == 1)
        {
            morse_pattern_status = 21;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 22;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_W)
        }
        break;
    case 11:
        if (input_bool == 1)
        {
            morse_pattern_status = 23;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 24;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_D)
        }
        break;
    case 12:
        if (input_bool == 1)
        {
            morse_pattern_status = 25;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 26;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_K)
        }
        break;
    case 13:
        if (input_bool == 1)
        {
            morse_pattern_status = 27;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 28;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_G)
        }
        break;
    case 14:
        if (input_bool == 1)
        {
            morse_pattern_status = 29;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 30;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_O)
        }
        break;
    case 15:
        if (input_bool == 1)
        {
            morse_pattern_status = 31;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 32;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_H)
        }
        break;
    case 16:
        if (input_bool == 1)
        {
            morse_pattern_status = 33;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 34;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_V)
        }
        break;
    case 17:
        if (input_bool == 1)
        {
            morse_pattern_status = 35;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 36;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_F)
        }
        break;
    case 18:
        if (input_bool == 1)
        {
            morse_pattern_status = 37;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 38;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 19:
        if (input_bool == 1)
        {
            morse_pattern_status = 39;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 40;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_L)
        }
        break;
    case 20:
        if (input_bool == 1)
        {
            morse_pattern_status = 41;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 42;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 21:
        if (input_bool == 1)
        {
            morse_pattern_status = 43;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 44;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_P)
        }
        break;
    case 22:
        if (input_bool == 1)
        {
            morse_pattern_status = 45;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 46;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_J)
        }
        break;
    case 23:
        if (input_bool == 1)
        {
            morse_pattern_status = 47;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 48;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_B)
        }
        break;
    case 24:
        if (input_bool == 1)
        {
            morse_pattern_status = 49;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 50;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_X)
        }
        break;
    case 25:
        if (input_bool == 1)
        {
            morse_pattern_status = 51;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 52;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_C)
        }
        break;
    case 26:
        if (input_bool == 1)
        {
            morse_pattern_status = 53;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 54;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_Y)
        }
        break;
    case 27:
        if (input_bool == 1)
        {
            morse_pattern_status = 55;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 56;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_Z)
        }
        break;
    case 28:
        if (input_bool == 1)
        {
            morse_pattern_status = 57;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 58;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_Q)
        }
        break;
    case 29:
        if (input_bool == 1)
        {
            morse_pattern_status = 59;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 60;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 30:
        if (input_bool == 1)
        {
            morse_pattern_status = 61;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 62;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 31:
        if (input_bool == 1)
        {
            morse_pattern_status = 63;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 64;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_5)
        }
        break;
    case 32:
        if (input_bool == 1)
        {
            morse_pattern_status = 65;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 66;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_4)
        }
        break;
    case 33:
        if (input_bool == 1)
        {
            morse_pattern_status = 67;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 68;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 34:
        if (input_bool == 1)
        {
            morse_pattern_status = 69;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 70;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_3)
        }
        break;
    case 35:
        if (input_bool == 1)
        {
            morse_pattern_status = 71;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 72;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 36:
        if (input_bool == 1)
        {
            morse_pattern_status = 73;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 74;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_UQ)
        }
        break;
    case 37:
        if (input_bool == 1)
        {
            morse_pattern_status = 75;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 76;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 38:
        if (input_bool == 1)
        {
            morse_pattern_status = 77;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 78;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_2)
        }
        break;
    case 39:
        if (input_bool == 1)
        {
            morse_pattern_status = 79;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 80;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_AND)
        }
        break;
    case 40:
        if (input_bool == 1)
        {
            morse_pattern_status = 81;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 82;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 41:
        if (input_bool == 1)
        {
            morse_pattern_status = 83;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 84;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_PLUS)
        }
        break;
    case 42:
        if (input_bool == 1)
        {
            morse_pattern_status = 85;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 86;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 43:
        if (input_bool == 1)
        {
            morse_pattern_status = 87;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 88;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 44:
        if (input_bool == 1)
        {
            morse_pattern_status = 89;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 90;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 45:
        if (input_bool == 1)
        {
            morse_pattern_status = 91;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 92;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 46:
        if (input_bool == 1)
        {
            morse_pattern_status = 93;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 94;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_1)
        }
        break;
    case 47:
        if (input_bool == 1)
        {
            morse_pattern_status = 95;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 96;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_6)
        }
        break;
    case 48:
        if (input_bool == 1)
        {
            morse_pattern_status = 97;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 98;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_EQ)
        }
        break;
    case 49:
        if (input_bool == 1)
        {
            morse_pattern_status = 99;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 100;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_SLAH)
        }
        break;
    case 50:
        if (input_bool == 1)
        {
            morse_pattern_status = 101;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 102;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 51:
        if (input_bool == 1)
        {
            morse_pattern_status = 103;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 104;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 52:
        if (input_bool == 1)
        {
            morse_pattern_status = 105;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 106;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 53:
        if (input_bool == 1)
        {
            morse_pattern_status = 107;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 108;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_LB)
        }
        break;
    case 54:
        if (input_bool == 1)
        {
            morse_pattern_status = 109;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 110;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 55:
        if (input_bool == 1)
        {
            morse_pattern_status = 111;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 112;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_7)
        }
        break;
    case 56:
        if (input_bool == 1)
        {
            morse_pattern_status = 113;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 114;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 57:
        if (input_bool == 1)
        {
            morse_pattern_status = 115;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 116;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 58:
        if (input_bool == 1)
        {
            morse_pattern_status = 117;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 118;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 59:
        if (input_bool == 1)
        {
            morse_pattern_status = 119;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 120;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_8)
        }
        break;
    case 60:
        if (input_bool == 1)
        {
            morse_pattern_status = 121;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 122;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 61:
        if (input_bool == 1)
        {
            morse_pattern_status = 123;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 124;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_9)
        }
        break;
    case 62:
        if (input_bool == 1)
        {
            morse_pattern_status = 125;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 126;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_0)
        }
        break;
    case 63:
        if (input_bool == 1)
        {
            morse_pattern_status = 127;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 128;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 64:
        if (input_bool == 1)
        {
            morse_pattern_status = 129;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 130;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 65:
        if (input_bool == 1)
        {
            morse_pattern_status = 131;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 132;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 66:
        if (input_bool == 1)
        {
            morse_pattern_status = 133;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 134;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 67:
        if (input_bool == 1)
        {
            morse_pattern_status = 135;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 136;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 68:
        if (input_bool == 1)
        {
            morse_pattern_status = 137;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 138;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 69:
        if (input_bool == 1)
        {
            morse_pattern_status = 139;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 140;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 70:
        if (input_bool == 1)
        {
            morse_pattern_status = 141;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 142;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 71:
        if (input_bool == 1)
        {
            morse_pattern_status = 143;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 144;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 72:
        if (input_bool == 1)
        {
            morse_pattern_status = 145;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 146;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 73:
        if (input_bool == 1)
        {
            morse_pattern_status = 147;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 148;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 74:
        if (input_bool == 1)
        {
            morse_pattern_status = 149;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 150;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 75:
        if (input_bool == 1)
        {
            morse_pattern_status = 151;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 152;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_QM)
        }
        break;
    case 76:
        if (input_bool == 1)
        {
            morse_pattern_status = 153;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 154;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_DD)
        }
        break;
    case 77:
        if (input_bool == 1)
        {
            morse_pattern_status = 155;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 156;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 78:
        if (input_bool == 1)
        {
            morse_pattern_status = 157;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 158;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 79:
        if (input_bool == 1)
        {
            morse_pattern_status = 159;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 160;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 80:
        if (input_bool == 1)
        {
            morse_pattern_status = 161;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 162;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 81:
        if (input_bool == 1)
        {
            morse_pattern_status = 163;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 164;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_DQUT)
        }
        break;
    case 82:
        if (input_bool == 1)
        {
            morse_pattern_status = 165;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 166;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 83:
        if (input_bool == 1)
        {
            morse_pattern_status = 167;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 168;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 84:
        if (input_bool == 1)
        {
            morse_pattern_status = 169;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 170;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_DOT)
        }
        break;
    case 85:
        if (input_bool == 1)
        {
            morse_pattern_status = 171;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 172;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 86:
        if (input_bool == 1)
        {
            morse_pattern_status = 173;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 174;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 87:
        if (input_bool == 1)
        {
            morse_pattern_status = 175;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 176;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 88:
        if (input_bool == 1)
        {
            morse_pattern_status = 177;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 178;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 89:
        if (input_bool == 1)
        {
            morse_pattern_status = 179;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 180;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_AT)
        }
        break;
    case 90:
        if (input_bool == 1)
        {
            morse_pattern_status = 181;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 182;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 91:
        if (input_bool == 1)
        {
            morse_pattern_status = 183;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 184;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 92:
        if (input_bool == 1)
        {
            morse_pattern_status = 185;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 186;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 93:
        if (input_bool == 1)
        {
            morse_pattern_status = 187;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 188;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_SQUT)
        }
        break;
    case 94:
        if (input_bool == 1)
        {
            morse_pattern_status = 189;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 190;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 95:
        if (input_bool == 1)
        {
            morse_pattern_status = 191;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 192;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 96:
        if (input_bool == 1)
        {
            morse_pattern_status = 193;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 194;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_DASH)
        }
        break;
    case 97:
        if (input_bool == 1)
        {
            morse_pattern_status = 195;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 196;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 98:
        if (input_bool == 1)
        {
            morse_pattern_status = 197;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 198;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 99:
        if (input_bool == 1)
        {
            morse_pattern_status = 199;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 200;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 100:
        if (input_bool == 1)
        {
            morse_pattern_status = 201;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 202;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 101:
        if (input_bool == 1)
        {
            morse_pattern_status = 203;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 204;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 102:
        if (input_bool == 1)
        {
            morse_pattern_status = 205;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 206;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 103:
        if (input_bool == 1)
        {
            morse_pattern_status = 207;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 208;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 104:
        if (input_bool == 1)
        {
            morse_pattern_status = 209;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 210;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 105:
        if (input_bool == 1)
        {
            morse_pattern_status = 211;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 212;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_SP)
        }
        break;
    case 106:
        if (input_bool == 1)
        {
            morse_pattern_status = 213;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 214;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_EXC)
        }
        break;
    case 107:
        if (input_bool == 1)
        {
            morse_pattern_status = 215;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 216;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 108:
        if (input_bool == 1)
        {
            morse_pattern_status = 217;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 218;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_RB)
        }
        break;
    case 109:
        if (input_bool == 1)
        {
            morse_pattern_status = 219;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 220;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 110:
        if (input_bool == 1)
        {
            morse_pattern_status = 221;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 222;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 111:
        if (input_bool == 1)
        {
            morse_pattern_status = 223;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 224;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 112:
        if (input_bool == 1)
        {
            morse_pattern_status = 225;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 226;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_UEXC)
        }
        break;
    case 113:
        if (input_bool == 1)
        {
            morse_pattern_status = 227;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 228;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 114:
        if (input_bool == 1)
        {
            morse_pattern_status = 229;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 230;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_COMA)
        }
        break;
    case 115:
        if (input_bool == 1)
        {
            morse_pattern_status = 231;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 232;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 116:
        if (input_bool == 1)
        {
            morse_pattern_status = 233;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 234;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 117:
        if (input_bool == 1)
        {
            morse_pattern_status = 235;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 236;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 118:
        if (input_bool == 1)
        {
            morse_pattern_status = 237;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 238;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 119:
        if (input_bool == 1)
        {
            morse_pattern_status = 239;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 240;
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_COLN)
        }
        break;
    case 120:
        if (input_bool == 1)
        {
            morse_pattern_status = 241;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 242;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 121:
        if (input_bool == 1)
        {
            morse_pattern_status = 243;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 244;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 122:
        if (input_bool == 1)
        {
            morse_pattern_status = 245;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 246;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 123:
        if (input_bool == 1)
        {
            morse_pattern_status = 247;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 248;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 124:
        if (input_bool == 1)
        {
            morse_pattern_status = 249;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 250;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 125:
        if (input_bool == 1)
        {
            morse_pattern_status = 251;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 252;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 126:
        if (input_bool == 1)
        {
            morse_pattern_status = 253;
        }
        else if (input_bool == 2)
        {
            morse_pattern_status = 254;
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 127:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 128:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 129:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 130:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 131:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 132:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 133:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 134:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 135:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 136:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            morse_pattern_status = 0;
            every_time_at_success();
            SCREEN_SHOW_FRAM(ASCII88PATTERN_MONY)
        }
        break;
    case 137:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 138:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 139:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 140:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 141:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 142:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 143:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 144:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 145:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 146:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 147:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 148:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 149:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 150:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 151:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 152:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 153:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 154:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 155:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 156:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 157:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 158:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 159:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 160:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 161:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 162:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 163:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 164:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 165:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 166:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 167:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 168:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 169:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 170:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 171:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 172:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 173:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 174:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 175:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 176:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 177:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 178:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 179:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 180:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 181:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 182:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 183:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 184:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 185:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 186:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 187:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 188:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 189:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 190:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 191:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 192:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 193:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 194:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 195:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 196:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 197:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 198:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 199:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 200:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 201:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 202:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 203:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 204:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 205:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 206:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 207:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 208:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 209:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 210:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 211:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 212:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 213:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 214:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 215:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 216:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 217:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 218:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 219:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 220:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 221:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 222:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 223:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 224:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 225:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 226:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 227:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 228:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 229:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 230:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 231:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 232:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 233:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 234:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 235:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 236:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 237:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 238:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 239:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 240:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 241:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 242:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 243:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 244:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 245:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 246:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 247:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 248:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 249:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 250:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 251:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 252:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 253:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;
    case 254:
        if (input_bool == 1)
        {
            target_morse_pattern_error_event();
        }
        else if (input_bool == 2)
        {
            target_morse_pattern_error_event();
        }
        else
        {
            target_morse_pattern_error_event();
        }
        break;

    default:
        target_morse_pattern_error_event();
        break;
    }
}

static void timer_callback(struct timer_list *arg)
{
    //gpio_direction_output(UP_HAT_LED5, !__gpio_get_value(UP_HAT_LED5));

    switch (timer_callback_state)
    {
    case timer_callback_state_ppt_blue1:
        call_back_fucn_n();
        break;
    case timer_callback_state_ppt_blue2:
        gpio_direction_output(UP_HAT_LED5, 0);
        timer_callback_state = timer_callback_state_ppt_blue3;
        JIFFIES_TIMER_GO(TIME_DASH_LONG - TIME_DASH_STANDER)
        break;
    case timer_callback_state_ppt_blue3:
        infinite_flashing_input_time_error_event = true;
        printk(KERN_DEBUG "\nkey word : timer_callback_state_ppt_blue3\n");
        target_input_length_error_event();
        break;
    case timer_callback_state_big_if_else2:
        gpio_direction_output(UP_HAT_LED5, 0);
        morse_pattern_logic(1);
        set_timer_between_pattern_long_call_back_send_zero_set_var_last_relase(ktime_get());
        break;
    case timer_callback_state_big_if_else5:
        gpio_direction_output(UP_HAT_LED5, 0);
        morse_pattern_logic(2);
        set_timer_between_pattern_long_call_back_send_zero_set_var_last_relase(ktime_get());
        break;
    case timer_callback_state_set_timer_between_pattern_long_call_back_send_zero_set_var_last_relase:
        printk(KERN_DEBUG "\nkey word : timer_callback_state_set_timer_between_pattern_long_call_back_send_zero_set_var_last_relase => morse_pattern_logic(0);\n");
        morse_pattern_logic(0);
        break;
    case timer_callback_state_after_success:
        able_state_flag = true;
        gpio_direction_output(UP_HAT_LED1, 1);
        break;
    default:
        printk(KERN_ERR "\ntimer_callback (switch - case @ jiffies) \nUnusual entry into the default area.\n");
        break;
    }

    //mod_timer(&timer, jiffies + msecs_to_jiffies(timeout_ms));
}

irq_handler_t isr(int irq, void *data)
{
    ktime_t this_time = ktime_get();
    if (this_time - last_time > MS_TO_NS(DEBOUNCE_BUFFER))
    {
        if (able_state_flag)
        {
            //disable_clock_B();
            is_press ^= 0x01;
            if (is_press)
            {
                if (able_press_flag)
                {
                    long long target_delay_time = ktime_to_ns(last_relase) + MS_TO_NS(TIME_BETWEEN_PATTERN_STANDER) - ktime_to_ns(this_time);
                    long long target_delay_time_ms = NS_TO_MS(ktime_to_ns(last_relase) + MS_TO_NS(TIME_BETWEEN_PATTERN_STANDER) - ktime_to_ns(this_time));
                    gpio_direction_output(UP_HAT_LED1, 0);
                    if (this_time - last_relase < MS_TO_NS(TIME_BETWEEN_PATTERN_SHORT))
                    {

#ifdef IF_PRINT_STEP_ONE_TIME
                        printk("\nthis time - last relase:%lld\n", this_time - last_relase);
#endif
                        target_input_time_error_event();
                    }
                    else if (this_time - last_relase < MS_TO_NS(TIME_BETWEEN_PATTERN_STANDER) && target_delay_time > MS_TO_NS(HRTIMER_MIN_TIME_INTERVAL))
                    {
                        able_press_flag = false;
                        timer_callback_state = timer_callback_state_ppt_blue1;

#ifdef IF_PRINT_STEP_ONE_TIME
                        printk("\nthis time - last relase:\n\t%lld\ndelay time:\n\t%lld\ndelay time:\t\n%lld...\n", this_time - last_relase, target_delay_time, target_delay_time_ms);
#endif
                        JIFFIES_TIMER_GO(target_delay_time_ms);
                    }
                    else
                    {
                        call_back_fucn_n();
                    }
                }
                else
                {
                    hrtimer_try_to_cancel_flag_hr_timer = true;
                    //SCREEN_SHOW_FRAM(ASCII88PATTERN_B)
                    printk(KERN_DEBUG "\nkey word : if (able_press_flag) => false\n");
                    target_input_length_error_event();
                }
            }
            else
            { //relase
                infinite_flashing_input_time_error_event = false;
                if (able_relase_flag)
                {
                    long long var_switch_case = this_time - last_press;
                    long long target_delay_time_dot = ktime_to_ns(last_press) + MS_TO_NS(TIME_DOT_STANDER) - ktime_to_ns(this_time);
                    long long target_delay_time_ms_dot = NS_TO_MS(target_delay_time_dot);
                    long long target_delay_time_dash = ktime_to_ns(last_press) + MS_TO_NS(TIME_DASH_STANDER) - ktime_to_ns(this_time);
                    long long target_delay_time_ms_dash = NS_TO_MS(target_delay_time_dot);
                    if (var_switch_case < TIME_DOT_SHORT)
                    { //1
                        printk(KERN_DEBUG "\nkey word : if (var_switch_case __this_time - last_press__ < TIME_DOT_SHORT) => @ place relase\n");
                        target_input_length_error_event();
                        able_relase_flag = false;
                        set_timer_between_pattern_long_call_back_send_zero_set_var_last_relase(this_time);
                    }
                    else if (var_switch_case < TIME_DOT_STANDER)
                    { //2
                        if (target_delay_time_dot > MS_TO_NS(HRTIMER_MIN_TIME_INTERVAL))
                        {
                            timer_callback_state = timer_callback_state_big_if_else2;
                            JIFFIES_TIMER_GO(target_delay_time_ms_dot)
                        }
                        else
                        {
                            /*copy from case 3*/
                            gpio_direction_output(UP_HAT_LED5, 0);
                            morse_pattern_logic(1);
                            set_timer_between_pattern_long_call_back_send_zero_set_var_last_relase(this_time);
                        }
                    }
                    else if (var_switch_case < TIME_DOT_LONG)
                    { //3
                        gpio_direction_output(UP_HAT_LED5, 0);
                        morse_pattern_logic(1);
                        set_timer_between_pattern_long_call_back_send_zero_set_var_last_relase(this_time);
                    }
                    else if (var_switch_case < TIME_DASH_SHORT)
                    { //4
                        target_input_length_error_event();
                        able_relase_flag = false;
                        set_timer_between_pattern_long_call_back_send_zero_set_var_last_relase(this_time);
                    }
                    else if (var_switch_case < TIME_DOT_STANDER)
                    { //5
                        if (target_delay_time_dash > MS_TO_NS(HRTIMER_MIN_TIME_INTERVAL))
                        {
                            timer_callback_state = timer_callback_state_big_if_else5;
                            JIFFIES_TIMER_GO(target_delay_time_ms_dash)
                        }
                        else
                        {
                            /*copy from case 6*/
                            gpio_direction_output(UP_HAT_LED5, 0);
                            morse_pattern_logic(2);
                            set_timer_between_pattern_long_call_back_send_zero_set_var_last_relase(this_time);
                        }
                    }
                    else if (var_switch_case < TIME_DOT_LONG)
                    { //6
                        gpio_direction_output(UP_HAT_LED5, 0);
                        morse_pattern_logic(2);
                        set_timer_between_pattern_long_call_back_send_zero_set_var_last_relase(this_time);
                    }
                    else
                    { //7
                        able_relase_flag = false;
                    }
                }
                else
                {
                    target_input_length_error_event();
                }
            }
        }
        else
        {
            target_input_time_error_event();
            if (timer_pending(&timer) == 1)
            {
                del_timer(&timer);
            }
        }
    }
    last_time = this_time;
    return (irq_handler_t)IRQ_HANDLED;
}

int init_module()
{
    uint8_t screen_setting_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    gpio_free(UP_HAT_SW1);
    gpio_free(UP_HAT_LED1);
    gpio_free(UP_HAT_LED5);
    gpio_free(UP_HAT_MAX7219_LOAD);
    gpio_free(UP_HAT_MAX7219_DIN);
    gpio_free(UP_HAT_MAX7219_CLK);
    gpio_free(UP_HAT_74HC595_DS);
    gpio_free(UP_HAT_74HC595_STCP);
    gpio_free(UP_HAT_74HC595_SHCP);
    if (gpio_request(UP_HAT_SW1, "UP_HAT_SW1") != 0)
    {
        printk(KERN_ERR "\nif (gpio_request(UP_HAT_SW1, UP_HAT_SW1) != 0)\n");
        return -1;
    }
    if (gpio_request(UP_HAT_LED1, "UP_HAT_LED1") != 0)
    {
        printk(KERN_ERR "\nif (gpio_request(UP_HAT_SW1, UP_HAT_LED1) != 0)\n");
        return -1;
    }
    if (gpio_request(UP_HAT_LED5, "UP_HAT_LED5") != 0)
    {
        printk(KERN_ERR "\nif (gpio_request(UP_HAT_SW1, UP_HAT_LED5) != 0)\n");
        return -1;
    }
    if (gpio_request(UP_HAT_MAX7219_LOAD, "UP_HAT_MAX7219_LOAD") != 0)
    {
        printk(KERN_ERR "\nif (gpio_request(UP_HAT_SW1, UP_HAT_MAX7219_LOAD) != 0)\n");
        return -1;
    }
    if (gpio_request(UP_HAT_MAX7219_DIN, "UP_HAT_MAX7219_DIN") != 0)
    {
        printk(KERN_ERR "\nif (gpio_request(UP_HAT_SW1, UP_HAT_MAX7219_DIN) != 0)\n");
        return -1;
    }
    if (gpio_request(UP_HAT_MAX7219_CLK, "UP_HAT_MAX7219_CLK") != 0)
    {
        printk(KERN_ERR "\nif (gpio_request(UP_HAT_SW1, UP_HAT_MAX7219_CLK) != 0)\n");
        return -1;
    }
    if (gpio_request(UP_HAT_74HC595_STCP, "UP_HAT_74HC595_STCP") != 0)
    {
        printk(KERN_ERR "\nif (gpio_request(UP_HAT_74HC595_STCP, UP_HAT_74HC595_STCP) != 0)\n");
        return -1;
    }
    if (gpio_request(UP_HAT_74HC595_SHCP, "UP_HAT_74HC595_SHCP") != 0)
    {
        printk(KERN_ERR "\nif (gpio_request(UP_HAT_74HC595_SHCP, UP_HAT_74HC595_SHCP) != 0)\n");
        return -1;
    }
    if (gpio_request(UP_HAT_74HC595_DS, "UP_HAT_74HC595_DS") != 0)
    {
        printk(KERN_ERR "\nif (gpio_request(UP_HAT_74HC595_DS, UP_HAT_74HC595_DS) != 0)\n");
        return -1;
    }
    gpio_direction_output(UP_HAT_LED5, 0);

    init_error_3_led();

    /*gpio_direction_output(UP_HAT_74HC595_STCP, 0);
    for (loopi = 7; loopi >= 0; loopi--)
    {
        gpio_direction_output(UP_HAT_74HC595_SHCP, 0);
        gpio_direction_output(UP_HAT_74HC595_DS, led_status_3[loopi]);
        gpio_direction_output(UP_HAT_74HC595_SHCP, 1);
    }
    gpio_direction_output(UP_HAT_74HC595_STCP, 1);*/

    printk(KERN_DEBUG "\nGPIO loaded !\n");
    //is_on ^= 0x01;

    if ((button_irq_id = gpio_to_irq(UP_HAT_SW1)) < 0)
    {
        return -1;
    }

    screen_setting_data[0] = 0x01;
    for (loopi = 0; loopi < 49 * 8; loopi++)
    {
        screen_show_one_row(loopi % 49, 1, 0x0c - 1, screen_setting_data);
    }
    usleep_range(20000ul, 30000ul);
    screen_setting_data[0] = 0x00;
    for (loopi = 0; loopi < 49 * 8; loopi++)
    {
        screen_show_one_row(loopi % 49, 1, 0x0f - 1, screen_setting_data);
    }
    usleep_range(20000ul, 30000ul);
    screen_setting_data[0] = 0x00;
    for (loopi = 0; loopi < 49 * 8; loopi++)
    {
        screen_show_one_row(loopi % 49, 1, 0x09 - 1, screen_setting_data);
    }
    usleep_range(20000ul, 30000ul);
    screen_setting_data[0] = 0x07;
    for (loopi = 0; loopi < 49 * 8; loopi++)
    {
        screen_show_one_row(loopi % 49, 1, 0x0b - 1, screen_setting_data);
    }
    usleep_range(20000ul, 30000ul);
    screen_setting_data[0] = 0x01;
    for (loopi = 0; loopi < 49 * 8; loopi++)
    {
        screen_show_one_row(loopi % 49, 1, 0x0a - 1, screen_setting_data);
    }
    usleep_range(20000ul, 30000ul);

    request_irq(button_irq_id, (irq_handler_t)isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, IRQ_NAME, NULL);
    //printk("mod_hrtimer: installing module...\n");
    //define a ktime variable with the interval time defined on top of this file
    ktime_interval = ktime_set(0, MS_TO_NS(TIME_ERROR_BLINK_ALL));
    //init a high resolution timer named 'hr_timer'
    hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    //set the callback function for this hr_timer
    hr_timer.function = &my_hrtimer_callback;
    //get the current time as high resolution timestamp, convert it to ns
    starttime_ns = ktime_to_ns(ktime_get());
    //activate the high resolution timer including callback function...
    hrtimer_start(&hr_timer, ktime_interval, HRTIMER_MODE_REL);
    /*printk( "mod_hrtimer: started timer callback function to fire every %lldns (current jiffies=%ld, HZ=%d)\n", 
		INTERVAL_BETWEEN_CALLBACKS, jiffies, HZ );*/
    SCREEN_SHOW_FRAM(ASCII88PATTERN_FFFF)
    gpio_direction_output(UP_HAT_LED1, 1);
    led_status_3[0] = 0;
    led_status_3[1] = 0;
    led_status_3[2] = 0;
    init_error_3_led();
    return 0;
}

void cleanup_module(void)
{
    int ret;
    ret = hrtimer_cancel(&hr_timer);
    if (ret)
        printk("mod_hrtimer: The timer was still in use...\n");
    printk("mod_hrtimer: HR Timer module uninstalling\n");

    free_irq(button_irq_id, NULL);
    gpio_free(UP_HAT_SW1);
    gpio_free(UP_HAT_LED1);
    gpio_free(UP_HAT_LED5);
    gpio_free(UP_HAT_MAX7219_LOAD);
    gpio_free(UP_HAT_MAX7219_DIN);
    gpio_free(UP_HAT_MAX7219_CLK);
    gpio_direction_output(UP_HAT_74HC595_STCP, 0);
    for (loopi = 7; loopi >= 0; loopi--)
    {
        gpio_direction_output(UP_HAT_74HC595_SHCP, 0);
        gpio_direction_output(UP_HAT_74HC595_DS, 0);
        gpio_direction_output(UP_HAT_74HC595_SHCP, 1);
    }
    gpio_direction_output(UP_HAT_74HC595_STCP, 1);
    gpio_free(UP_HAT_74HC595_STCP);
    gpio_free(UP_HAT_74HC595_SHCP);
    gpio_free(UP_HAT_74HC595_DS);

    del_timer(&timer);

    printk(KERN_DEBUG "\nGPIO unloaded !\n");
}