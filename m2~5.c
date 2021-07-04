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

#define DEBUG_HRTIMER

#define do_posix_clock_monotonic_gettime(ts) ktime_get_ts(ts)
#define ktime_set_interval_ns(interval) ktime_set(interval / 1000000000, interval % 1000000000)
#ifdef DEBUG_HRTIMER
#define KER_PRINT(fmt, ...) printk("<ker-driver>" fmt, ##__VA_ARGS__);
#endif

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

#define INTERVAL_SLOW_MS 1000
#define INTERVAL_FAST_MS 100

#define IRQ_NAME "button_1"
#define SERIAL_DEVICE "/dev/ttyS0"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("andythebreaker");
MODULE_DESCRIPTION("m3");
MODULE_VERSION("6.5");

static short int button_irq_id = 0;
static ktime_t last_time;
static char is_on = 0;
static char is_press = 0;

static int timeout_ms = INTERVAL_FAST_MS;
struct timer_list timer;
static ktime_t last_time;

static struct hrtimer vibe_timer;
static struct work_struct vibe_work;
static int value = 2000;

static enum hrtimer_restart vibrator_timer_func(struct hrtimer *timer)
{

    struct timespec uptime;

    do_posix_clock_monotonic_gettime(&uptime);
#ifdef DEBUG_HRTIMER
    KER_PRINT("Time:%lu.%02lu\n",

              (unsigned long)uptime.tv_sec,
              (uptime.tv_nsec / (NSEC_PER_SEC / 1000)));

    KER_PRINT("vibrator_timer_func\n");
#endif
    schedule_work(&vibe_work);
    return HRTIMER_NORESTART;
}
static void vibe_work_func(struct work_struct *work)
{
#ifdef DEBUG_HRTIMER
    KER_PRINT("'vibe_work_func'-->work\n");
#endif
    vibe_timer.function = vibrator_timer_func;
    hrtimer_start(&vibe_timer,

                  ktime_set_interval_ns(value), HRTIMER_MODE_REL);
}

static void ker_driver_init(void)
{

    struct timespec uptime;
#ifdef DEBUG_HRTIMER
    KER_PRINT("ker_driver_init\n");
    hrtimer_init(&vibe_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
#endif
    vibe_timer.function = vibrator_timer_func;
    hrtimer_start(&vibe_timer,

                  ktime_set_interval_ns(value), HRTIMER_MODE_REL);

    do_posix_clock_monotonic_gettime(&uptime);
#ifdef DEBUG_HRTIMER
    KER_PRINT("Time:%lu.%02lu\n",

              (unsigned long)uptime.tv_sec,
              (uptime.tv_nsec / (NSEC_PER_SEC / 1000)));
#endif
    INIT_WORK(&vibe_work, vibe_work_func);
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
            is_on ^= 0x01;
        }
    }
    last_time = this_time;
    return (irq_handler_t)IRQ_HANDLED;
}

int init_module()
{
    gpio_free(UP_HAT_SW1);
    gpio_free(UP_HAT_LED1);
    gpio_free(UP_HAT_LED5);
    if (gpio_request(UP_HAT_SW1, "UP_HAT_SW1") != 0)
        return -1;
    if (gpio_request(UP_HAT_LED1, "UP_HAT_LED1") != 0)
        return -1;
    if (gpio_request(UP_HAT_LED5, "UP_HAT_LED5") != 0)
        return -1;

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

    ker_driver_init();

    request_irq(button_irq_id, (irq_handler_t)isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, IRQ_NAME, NULL);

    return 0;
}

void cleanup_module(void)
{
    free_irq(button_irq_id, NULL);
    gpio_free(UP_HAT_SW1);
    gpio_free(UP_HAT_LED1);
    gpio_free(UP_HAT_LED5);

    hrtimer_cancel(&vibe_timer);
    del_timer(&timer);

    printk(KERN_DEBUG "\nGPIO unloaded !\n");
}
