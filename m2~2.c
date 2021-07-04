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

// https://hackmd.io/WUQIe9uySQKZNWRAzuQI0g?view#GPIO
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
// end of gpio pin def. ==============================

#define BUTTON_1 UP_HAT_SW1
#define LED_1 UP_HAT_LED1

#define INTERVAL_SLOW_MS 1000
#define INTERVAL_FAST_MS 100

#define IRQ_NAME "button_1"
#define SERIAL_DEVICE "/dev/ttyS0"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("andythebreaker");
MODULE_DESCRIPTION("m3");
MODULE_VERSION("4.5");

static short int button_irq_id = 0;
static ktime_t last_time;
static char is_on = 0;
static char is_press = 0;

static int timeout_ms = INTERVAL_FAST_MS;
struct timer_list timer;
static ktime_t last_time;

static void timer_callback(struct timer_list *arg)
{
    gpio_direction_output(UP_HAT_LED5, !__gpio_get_value(UP_HAT_LED5));
    mod_timer(&timer, jiffies + msecs_to_jiffies(timeout_ms));
}

irq_handler_t isr(int irq, void *data)
{
    ktime_t this_time = ktime_get(); //get current time
    if (this_time - last_time > 100000000)
    {                     //debounce, minimum interval time = 0.1s
        is_press ^= 0x01; //1 when button press, 0 when button relese
        if (is_press)
        { //if button press, change the state of LED_1
            gpio_direction_output(LED_1, is_on);
            is_on ^= 0x01; //1 for LED_1 on, 0 for LED_1 off
        }
    }
    last_time = this_time;             //update the last trigger time of ISR
    return (irq_handler_t)IRQ_HANDLED; //tell system that this ISR is handled
}

int init_module()
{
    gpio_free(BUTTON_1); //in case BUTTON_1 pin is occupy


    gpio_free(LED_1);    //in case LED_1 pin is occupy

    gpio_free(UP_HAT_LED5);    //in case LED_1 pin is occupy

if (gpio_request(UP_HAT_LED5, "LED") != 0)
    {              //sign up for using LED_1
        return -1; //sign up fail, exit with -1
    }

    if (gpio_request(BUTTON_1, "BUTTON_1") != 0)
    {              //sign up for using BUTTON_1
        return -1; //sign up fail, exit with -1
    }
    if (gpio_request(LED_1, "LED") != 0)
    {              //sign up for using LED_1
        return -1; //sign up fail, exit with -1
    }

        gpio_direction_output(UP_HAT_LED5, is_on); //tell system we will use LED_1 as output port


    gpio_direction_output(LED_1, is_on); //tell system we will use LED_1 as output port
    is_on ^= 0x01;                       //initialize

    last_time = ktime_get(); //get current time
    if ((button_irq_id = gpio_to_irq(BUTTON_1)) < 0)
    { //translate gpio pin id to irq id
        return -1;
    }

    printk(KERN_DEBUG "\nGPIO loaded !\n");
    //go google dmesg if you don't know what it is

    last_time = ktime_get(); //get current time
    timer_setup(&timer, timer_callback, 0);
    mod_timer(&timer, jiffies + msecs_to_jiffies(timeout_ms));

    request_irq(button_irq_id, (irq_handler_t)isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, IRQ_NAME, NULL);
    //sign up IRQ
    //let's break this up
    //button_irq_id : irq id of BUTTON_1, got this with gpio_to_irq()
    //isr           : the name of your ISR, in this case, the function name is "isr"
    //IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING  : bit mask(google it if you don't know what is bit mask),
    //                                            we are using "|" to tell system to trigger isr
    //                                            when rising edge AND falling edge event happen
    //IRQ_NAME      : the name of this IRQ, you could check this in "/proc/interrupts"
    //NULL          : the address of varible passed to handler function
    return 0;
}

void cleanup_module(void)
{                                  //triggered when rmmod use
    free_irq(button_irq_id, NULL); //free IRQ
    gpio_free(BUTTON_1);           //free BUTTON_1 gpio pin
    gpio_free(LED_1);              //free LED_1 gpio pin
    gpio_free(UP_HAT_LED5);              //free LED_1 gpio pin

    del_timer(&timer);

    printk(KERN_DEBUG "\nGPIO unloaded !\n"); //debug information, check with dmesg
}
