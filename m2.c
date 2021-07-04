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

#define IRQ_NAME "button_1"
#define SERIAL_DEVICE "/dev/ttyS0"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("andythebreaker");
MODULE_DESCRIPTION("m2");
MODULE_VERSION("8.5");

static short int button_irq_id = 0;
static ktime_t last_time;
static char is_on = 0;
static char is_press = 0;

static int timeout_ms = INTERVAL_FAST_MS;
struct timer_list timer;
static ktime_t last_time;

static uint8_t row_pattern[8] = ASCII88PATTERN_5;

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
        break;
    }
}

static void timer_callback(struct timer_list *arg)
{
    gpio_direction_output(UP_HAT_LED5, !__gpio_get_value(UP_HAT_LED5));
    mod_timer(&timer, jiffies + msecs_to_jiffies(timeout_ms));
}

irq_handler_t isr(int irq, void *data)
{
    ktime_t this_time = ktime_get();
    if (this_time - last_time > 100000000)
    {
        is_press ^= 0x01;
        if (is_press)
        {

            gpio_direction_output(UP_HAT_LED1, is_on);
            //printk(KERN_DEBUG "\ngpio_direction_output(UP_HAT_LED1, is_on);\n");
            is_on ^= 0x01;
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

    gpio_direction_output(UP_HAT_LED5, is_on);

    gpio_direction_output(UP_HAT_LED1, is_on);
    is_on ^= 0x01;

    last_time = ktime_get();
    if ((button_irq_id = gpio_to_irq(UP_HAT_SW1)) < 0)
    {
        return -1;
    }

    printk(KERN_DEBUG "\nGPIO loaded !\n");

    last_time = ktime_get();
    timer_setup(&timer, timer_callback, 0);
    mod_timer(&timer, jiffies + msecs_to_jiffies(timeout_ms));

    screen_setting_data[0] = 0x01;
    screen_show_one_row(-5, 1, 0x0c, screen_setting_data);
    screen_setting_data[0] = 0x00;
    screen_show_one_row(-4, 1, 0x0f, screen_setting_data);
    screen_setting_data[0] = 0xFF;
    screen_show_one_row(-3, 1, 0x09, screen_setting_data);
    screen_setting_data[0] = 0x07;
    screen_show_one_row(-2, 1, 0x0b, screen_setting_data);
    screen_setting_data[0] = 0x01;
    screen_show_one_row(-1, 1, 0x0a, screen_setting_data);
    request_irq(button_irq_id, (irq_handler_t)isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, IRQ_NAME, NULL);
    short int i = 0;
    for (i = 0; i < 49 * 8; i++)
    {
        screen_show_one_row(i % 49, 0, i / 49, row_pattern);
    }

    return 0;
}

void cleanup_module(void)
{

    free_irq(button_irq_id, NULL);
    gpio_free(UP_HAT_SW1);
    gpio_free(UP_HAT_LED1);
    gpio_free(UP_HAT_LED5);
    gpio_free(UP_HAT_MAX7219_LOAD);
    gpio_free(UP_HAT_MAX7219_DIN);
    gpio_free(UP_HAT_MAX7219_CLK);

    del_timer(&timer);

    printk(KERN_DEBUG "\nGPIO unloaded !\n");
}
