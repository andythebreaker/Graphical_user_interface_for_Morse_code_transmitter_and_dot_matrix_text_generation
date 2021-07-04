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

#include <errno.h>
#include <mraa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define BUTTON_1 25
#define LED_1 24

#define IRQ_NAME "button_1"
#define SERIAL_DEVICE "/dev/ttyS0"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RTES LAB");
MODULE_DESCRIPTION("irq demo for RTES lab");
MODULE_VERSION("0.3");

static short int button_irq_id = 0;
static ktime_t last_time;
static char is_on = 0;
static char is_press = 0;

void setup_terminal(int fd, struct termios *old_term, int echo)
{
    if (tcgetattr(fd, old_term) != 0)
    {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }
    struct termios new_term = *old_term;
    new_term.c_lflag |= (echo) ? ECHO : ~ECHO | ECHOE | ~ICANON | ICRNL;

    cfsetspeed(&new_term, B115200);
    if (tcsetattr(fd, TCSANOW, &new_term) != 0)
    {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void restore_terminal(int fd, struct termios *old_term)
{
    if (tcsetattr(fd, TCSANOW, old_term) != 0)
    {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

char getch_(int echo, FILE *filestream_pointer, int fd, struct termios *old_term)
{
    char ch;
    setup_terminal(fd, old_term, echo);
    ch = getc(filestream_pointer);
    restore_terminal(fd, old_term);
    return ch;
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

int main()
{
    int fd = open(SERIAL_DEVICE, O_RDWR | O_NOCTTY);
    if (fd == -1)
    {
        printk(KERN_ALERT "open_port: Unable to open " SERIAL_DEVICE " - \n");
    }
    FILE *file = fdopen(fd, "r+");
    if (!file)
    {
        printk(KERN_ALERT "fdopen\n");
    }

    struct termios old_term;
    int no_feed_back_flag = 0;
    dprintf(fd, "\033[4;h");

    while (1)
    {
        char c;
        c = getch_(READ1CHARACTER_WItHOUT_ECHO, file, fd, &old_term);
        dprintf(fd, "%c", c);
        if (no_feed_back_flag == 2)
        {
            no_feed_back_flag = 0;
            switch (c)
            {
            case 65:
                //printf("\t\t[arrow U]");
                break;
            case 66:
                //printf("\t\t[arrow D]");
                break;
            case 67:
                //printf("\t\t[arrow R]");
                break;
            case 68:
                //printf("\t\t[arrow L]");
                break;
            default:
                //printf("\t\t[hum...]");
                break;
            }
        }
        else
        {
            if (no_feed_back_flag == 1)
            {
                if (c == 91)
                {
                    no_feed_back_flag = 2;
                }
            }
            else
            {
                if (c == 27)
                {
                    //printf("\t\t[esc]");
                    no_feed_back_flag = 1;
                }
                else
                {
                    //empty
                }
            }
        }
    }
    close(fd);
}

int init_module()
{
    gpio_free(BUTTON_1); //in case BUTTON_1 pin is occupy
    gpio_free(LED_1);    //in case LED_1 pin is occupy
    if (gpio_request(BUTTON_1, "BUTTON_1") != 0)
    {              //sign up for using BUTTON_1
        return -1; //sign up fail, exit with -1
    }
    if (gpio_request(LED_1, "LED") != 0)
    {              //sign up for using LED_1
        return -1; //sign up fail, exit with -1
    }

    gpio_direction_output(LED_1, is_on); //tell system we will use LED_1 as output port
    is_on ^= 0x01;                       //initialize

    last_time = ktime_get(); //get current time
    if ((button_irq_id = gpio_to_irq(BUTTON_1)) < 0)
    { //translate gpio pin id to irq id
        return -1;
    }

    printk(KERN_ALERT "\nbutton_isr loaded !\n"); //debug information, check with dmesg
    //go google dmesg if you don't know what it is

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
{                                                     //triggered when rmmod use
    free_irq(button_irq_id, NULL);                    //free IRQ
    gpio_free(BUTTON_1);                              //free BUTTON_1 gpio pin
    gpio_free(LED_1);                                 //free LED_1 gpio pin
    printk(KERN_ALERT "\n1 button_isr unloaded !\n"); //debug information, check with dmesg
}
