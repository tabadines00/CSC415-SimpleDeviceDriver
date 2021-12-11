/**************************************************************
* Class:  CSC-415-0# Fall 2021
* Name: Thomas Abadines
* Student ID: 918674489
* GitHub ID: tabadines00
* Project: Assignment 6 – Device Driver
*
* File: hashbrown.c
*
* Description: This program is a software device driver that
*              uses the 'write' functionality to accept a string
*              by the user into the device buffer, creates a hash
*              using basic arithmetic operations and functions,
*              and lets a user 'read' the result.
*              Full functionality is not complete, however.
*
**************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/slab.h>
MODULE_LICENSE("Dual BSD/GPL");

#define DEVICE_NAME "hashbrown"
#define MAJOR_NUM 111

// When the device file is successfully opened, log in the kernel and return 0
static int dev_open(struct inode* inode, struct file* fp) {
    printk("Hashbrown device opened\n");
    return 0;
}

// When the device file is successfully closed, log in the kernel and return 0
static int dev_release(struct inode* inode, struct file* fp) {
    printk("Hashbrown device closed\n");
    return 0;
}

// When a user "reads" the device, copy the last hash into their buffer
static ssize_t dev_read (struct file* fp, char __user* buffer, size_t count, loff_t* offset) {

    // This message was a placeholder for the last created hash
    static const char msg[] = "Hello from the kernel\0";
    static const int msgLen = sizeof(msg);

    printk("Hashbrown device was read from\n");

    // Check if the offset is more than the length of what we are sending
    if(*offset >= msgLen) {
        return 0;
    }

    // Only send the amount of bytes that we have
    if(*offset + count > msgLen) {
        count = msgLen - *offset;
    }

    // Copy the data to the user space using copy_to_user
    if(copy_to_user(buffer, msg + *offset, count) != 0) {
        return -EFAULT;    
    }
    
    // Update the offset
    *offset += count;
    printk("Hashbrown device says %s with count %ld\n", msg, count);
    return count;
}

static ssize_t dev_write (struct file* fp, const char __user* buffer, size_t count, loff_t* offset) {
    //return a nonnegative value, possibly the number of bytes written
    int retval = 0;

    // TODO: Accept input from user char buffer using copy_from_user
    char* message = kmalloc(sizeof(char) * 32, GFP_KERNEL);
    if (!message) {
        printk("Hashbrown device unable to allocate memory.\n");
        return -1;
    } else {
        printk("Hashbrown devic allocated memory successfully\n");
    }

    // Take input from 'write' using copy_from_user
    if (copy_from_user(message, buffer, count)) {
        retval = -EFAULT;
    }

    printk("Hashbrown device successfully captured %s\n", message);
    retval = count;
    
    // TODO: Hash the input string using my homemade hash function

    // TODO: Overwrite previous hash and store the new hash until 'read' by any user

    return retval;
}

// IOCTL would be used to extend functionality of this device driver
static long dev_ioctl (struct file* fp, unsigned int i, unsigned long offset) {
    return 0;
}

// This structure holds the operations 
static struct file_operations dev_fops = {
    .open = dev_open,
    .release = dev_release,
    .owner = THIS_MODULE,
    .read = dev_read,
    .write = dev_write,
    .unlocked_ioctl = dev_ioctl
};

// This function initializes the device driver using register_chrdev and logs to the kernel
// NOTE: This device has to be registered using the command 'sudo mknod /dev/hashbrown c 111 0'
static int __init hashbrown_init(void) {
    int ret;
    printk("Hashbrown module has been loaded\n");

    if((ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &dev_fops)) < 0) {
        printk("register_chrdev: %d\n", ret);
    }
    return ret;
}

// This function unregisters the device driver using unregister_chrdev and logs to the kernel
// NOTE: This device has to finish unregistering by using 'sudo rm /dev/hashbrown'
static void __exit hashbrown_exit(void) {
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    printk("Hashbrown module has been unloaded\n");
}

module_init(hashbrown_init);
module_exit(hashbrown_exit);