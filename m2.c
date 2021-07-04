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

static uint8_t row_pattern[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0x18, 0xFF, 0xFF, 0xFF};

static void screen_show_one_row(short int screen_status_pa_49, uint8_t bool_setting, uint8_t row_pattern_index, uint8_t *row_pattern)
{
    printk(KERN_DEBUG "\nvoid screen_show_one_row(short int screen_status_pa_49, uint8_t bool_setting, uint8_t row_pattern_index, uint8_t *row_pattern)\n");
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
            short int i = 0;
            gpio_direction_output(UP_HAT_LED1, is_on);
            is_on ^= 0x01;
            for (i = 0; i < 49 * 8; i++)
            {
                screen_show_one_row(i % 49, 0, i / 49, row_pattern);
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
