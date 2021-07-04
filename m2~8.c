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

#define BENCHMARK_USE_NS
#define INTERVAL_BETWEEN_CALLBACKS_US 10LL
#define INTERVAL_BETWEEN_CALLBACKS_MS 1                                        //not use
#define INTERVAL_BETWEEN_CALLBACKS_NS (INTERVAL_BETWEEN_CALLBACKS_US * 1000LL) //100ms (scaled in ns)
#define NR_ITERATIONS 20
static struct hrtimer hr_timer;
static ktime_t ktime_interval;
static s64 starttime_ns;

//#define DEBUG_HRTIMER

#define calc_ns_to_us(ns_input) (ns_input / 1000)
#define do_posix_clock_monotonic_gettime(ts) ktime_get_ts(ts)
#define ktime_set_interval_us(interval) ktime_set(calc_ns_to_us(interval) / 1000000, (calc_ns_to_us(interval) % 1000000) * 1000)
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
#define MAX7219_delay_value 1000000

#define IRQ_NAME "button_1"
#define SERIAL_DEVICE "/dev/ttyS0"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("andythebreaker");
MODULE_DESCRIPTION("m2");
MODULE_VERSION("7.0");

static short int button_irq_id = 0;
static ktime_t last_time;
static char is_on = 0;
static char is_press = 0;

static int timeout_ms = INTERVAL_FAST_MS;
struct timer_list timer;
static ktime_t last_time;

static struct hrtimer vibe_timer;
//static struct work_struct vibe_work;
static int value = 1;

static short int screen_status = -5;
static uint8_t row_pattern[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

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
static enum hrtimer_restart my_hrtimer_callback(struct hrtimer *timer)
{
    static int n = 0;
    static int min = 1000000000, max = 0;

#ifdef BENCHMARK_USE_NS
    static s64 sum = 0;
    static s64 last_time_ns = 0;
    s64 time_diff_ns = 0;
#else
    static int sum = 0;
#endif

    int latency;
    s64 now_ns = ktime_to_ns(ktime_get());

    gpio_direction_output(UP_HAT_LED1, is_on);
    is_on ^= 0x01;

    hrtimer_forward(&hr_timer, hr_timer._softexpires, ktime_interval); //next call relative to expired timestamp
    // calculate some statistics values...
    n++;
    latency = now_ns - starttime_ns - n * INTERVAL_BETWEEN_CALLBACKS_NS;

#ifdef BENCHMARK_USE_NS
    sum += latency;
    time_diff_ns = now_ns - last_time_ns;
    last_time_ns = now_ns;
#else
    sum += latency / 1000;
#endif

    if (min > latency)
        min = latency;
    if (max < latency)
        max = latency;

#ifdef BENCHMARK_USE_NS
    printk("mod_hrtimer: my_hrtimer_callback called after %lldns. ( %lldns after last call)\n", (now_ns - starttime_ns), time_diff_ns);
#else
    printk("mod_hrtimer: my_hrtimer_callback called after %dus.\n", (int)(now_ns - starttime_ns) / 1000);
#endif

    //if (n < NR_ITERATIONS)
    return HRTIMER_RESTART;
    /*else
    {

#ifdef BENCHMARK_USE_NS
    printk("mod_hrtimer: my_hrtimer_callback: statistics latences over %d hrtimer callbacks: "
           "min=%dns, max=%dns, mean=%lldns\n",
           n, min, max, sum / n);
#else
    printk("mod_hrtimer: my_hrtimer_callback: statistics latences over %d hrtimer callbacks: "
           "min=%dus, max=%dus, mean=%dus\n",
           n, min / 1000, max / 1000, sum / n);
#endif

    return HRTIMER_NORESTART;
    }*/
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

    printk("mod_hrtimer: installing module...\n");
    //define a ktime variable with the interval time defined on top of this file
    ktime_interval = ktime_set(0, INTERVAL_BETWEEN_CALLBACKS_NS);
    //init a high resolution timer named 'hr_timer'
    hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    //set the callback function for this hr_timer
    hr_timer.function = &my_hrtimer_callback;
    //get the current time as high resolution timestamp, convert it to ns
    starttime_ns = ktime_to_ns(ktime_get());
    //activate the high resolution timer including callback function...
    hrtimer_start(&hr_timer, ktime_interval, HRTIMER_MODE_REL);
    printk("mod_hrtimer: started timer callback function to fire every %lldns (current jiffies=%ld, HZ=%d)\n",
           INTERVAL_BETWEEN_CALLBACKS_NS, jiffies, HZ);

    request_irq(button_irq_id, (irq_handler_t)isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, IRQ_NAME, NULL);

    return 0;
}

void cleanup_module(void)
{
    int ret;
    free_irq(button_irq_id, NULL);
    gpio_free(UP_HAT_SW1);
    gpio_free(UP_HAT_LED1);
    gpio_free(UP_HAT_LED5);
    gpio_free(UP_HAT_MAX7219_LOAD);
    gpio_free(UP_HAT_MAX7219_DIN);
    gpio_free(UP_HAT_MAX7219_CLK);

    del_timer(&timer);

    ret = hrtimer_cancel(&hr_timer);
    if (ret)
        printk("mod_hrtimer: The timer was still in use...\n");
    printk("mod_hrtimer: HR Timer module uninstalling\n");

    printk(KERN_DEBUG "\nGPIO unloaded !\n");
}
