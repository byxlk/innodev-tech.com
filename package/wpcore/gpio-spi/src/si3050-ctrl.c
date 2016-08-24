#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mman.h>
#include <linux/random.h>
#include <linux/init.h>
#include <linux/raw.h>
#include <linux/tty.h>
#include <linux/capability.h>
#include <linux/ptrace.h>
#include <linux/device.h>
#include <linux/highmem.h>
#include <linux/crash_dump.h>
#include <linux/backing-dev.h>
#include <linux/bootmem.h>
#include <linux/splice.h>
#include <linux/pfn.h>
#include <linux/export.h>
#include <linux/io.h>
#include <linux/aio.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/version.h>

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/list.h>
#include <asm/delay.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <asm/bitops.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>



#include <asm/uaccess.h>

#include "si3050-ctrl.h"
#include "gpio-spi.h"


#define GPIOMODE_BASE 0x10000000
#define GPIOMODE_OFFSET 0x0060
#define GPIO_REG_BASE 0x10000600
#define GPIO21_00_INT 0x0000
#define GPIO21_00_EDGE 0x0004
#define GPIO21_00_RENA 0x0008
#define GPIO21_00_FENA 0x000c
#define GPIO21_00_POL 0x0028
#define GPIO21_00_SET 0x002c
#define GPIO21_00_RESET 0x0030
#define GPIO21_00_TOG 0x0034
#define GPIO21_00_DIR 0x0024
#define GPIO21_00_DATA 0x0020

#define GPIO27_22_INT 0x0060
#define GPIO27_22_EDGE 0x0064
#define GPIO27_22_RENA 0x0068
#define GPIO27_22_FENA 0x006c
#define GPIO27_22_POL 0x0078
#define GPIO27_22_SET 0x007c
#define GPIO27_22_RESET 0x0080
#define GPIO27_22_TOG 0x0084
#define GPIO27_22_DIR 0x0074
#define GPIO27_22_DATA 0x0070

static struct class *si3050_drv_class;

static inline uint32_t rt5350_reg_read(unsigned int reg_addr)
{
        return __raw_readl(ioremap(reg_addr, 4));
}

static inline void rt5350_reg_write(	unsigned int reg_addr, uint32_t value)
{
        __raw_writel(value, ioremap(reg_addr, 4));
}


static int si3050_drv_open(struct inode *inode, struct file *file)
{
    
    return 0;
}

static int si3050_drv_close(struct inode *inode, struct file *file)
{

    return 0;
}

static ssize_t si3050_drv_write(struct file *file, const char __user *buf, 
size_t size, loff_t *ppos)
{
    char val;

    copy_from_user(&val, buf, 1);

    return 1;
}

static ssize_t si3050_drv_read(struct file *file, const char __user *buf, 
size_t size, loff_t *ppos)
{

    return 0;
}

static long si3050_drv_unlocked_ioctl(struct file *file, unsigned int cmd, 
unsigned long arg)
{
    unsigned int reset_value; 
     
    switch(cmd)
    {
        case SEND_SI3050_RESET_SIGNAL: //
           //if (copy_from_user(&reset_value, arg, sizeof(reset_value)))
           //     return -1;

           //如果reset_value != 0则发送复位信号
           //if(reset_value){

           //} else {

           //}
            break;
        default:
            break;
    }

    return 0;
}

/* 1.分配、设置一个file_operations结构体 */
static struct file_operations si3050_drv_fops = {
    .owner              = THIS_MODULE,                  
    .open               = si3050_drv_open,
    .write               = si3050_drv_write,
//    .read                = si3050_drv_read,
    .unlocked_ioctl = si3050_drv_unlocked_ioctl,   
    .release            = si3050_drv_close,
};

int major;
static int __init si3050_drv_init(void)
{
    unsigned int gpio_cfg;
    
    printk("[%s: %d]++\n",__FUNCTION__,__LINE__);
    /* 2.注册 */
    major = register_chrdev(0, DEV_NODE_NAME, &si3050_drv_fops);

    /* 3.自动创建设备节点 */ /* 创建类 */   
    si3050_drv_class = class_create(THIS_MODULE, DEV_NODE_NAME);
    
    /* 类下面创建设备节点 */
    device_create(si3050_drv_class, NULL, MKDEV(major, 0), NULL, DEV_NODE_NAME);

    /* 4.硬件相关的操作 */
    /* 映射寄存器的地址 */
    //getAllGpioRegValue();

    // Config GPIO MODE register
    gpio_cfg = rt5350_reg_read(GPIOMODE_BASE + GPIOMODE_OFFSET);   
    gpio_cfg |= (0x1 << 6); // JTAG_GPIO_MODE     
    gpio_cfg |= (0x4 << 2);// UARTF_SHARE_MODE  PCM + GPIO = b100
    rt5350_reg_write(GPIOMODE_BASE + GPIOMODE_OFFSET, gpio_cfg);

    //Config GPIO DIR register
    gpio_cfg = rt5350_reg_read(GPIO_REG_BASE + GPIO21_00_DIR);    
    gpio_cfg &= ~((0x1 << 10) | (0x1 << 21));// Set GPIO#10 & GPIO#21 as input
    gpio_cfg |= (1 << 8)|(1 << 17)|(1 << 18); //Set GPIO#8,17,18 as output mode
    gpio_cfg |= (1 << 19)|(1 << 20)|(1 << 21);//Set GPIO#19,20,21 as output mode
    rt5350_reg_write(GPIO_REG_BASE + GPIO21_00_DIR, gpio_cfg);
    
    /* RESET(GPIO#8) Default value is output 1, Others output 0 */
    gpio_cfg = rt5350_reg_read(GPIO_REG_BASE + GPIO21_00_DATA); 
    gpio_cfg |= (1 << 8)|(1 << 19);
    gpio_cfg &= ~((1 << 17) | (1 << 18) | (1 << 20));
    rt5350_reg_write(GPIO_REG_BASE + GPIO21_00_DIR, gpio_cfg);

    printk("[%s: %d]--\n",__FUNCTION__,__LINE__);
    return 0;
}

static void __exit si3050_drv_exit(void)
{

    unregister_chrdev(major,DEV_NODE_NAME);
    device_destroy(si3050_drv_class, MKDEV(major, 0));
    class_destroy(si3050_drv_class);

}

module_init(si3050_drv_init);
module_exit(si3050_drv_exit);

MODULE_DESCRIPTION("ASoC ES9038 driver");
MODULE_AUTHOR("Derek Quan");
MODULE_LICENSE("GPL");
