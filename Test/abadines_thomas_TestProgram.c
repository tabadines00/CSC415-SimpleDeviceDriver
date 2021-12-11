/**************************************************************
* Class:  CSC-415-0# Fall 2021
* Name: Thomas Abadines
* Student ID: 918674489
* GitHub ID: tabadines00
* Project: Assignment 6 – Device Driver
*
* File: abadines_thomas_TestProgram.c
*
* Description: This program is the test application for
*              the device driver. It will test the functionality
*              of 'writing' a message to the device, then
*              'reading' the resulting hash and comparing it
*              like the back end of a login.
*
**************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define BUFFERSIZE 32

int main() {
    printf("Welcome to the Hashbrown hashing function Test Program\n");

    // Initialize buffer
    // For the first test, set the user buffer to "hello world"
    char buffer[BUFFERSIZE] = "hello world";

    // Open the device driver like a file
    FILE* fp = fopen("/dev/hashbrown", "rw");

    // If the file descriptor is 0, then an error has occured
    if(fp < 0) {
        printf("Error finding device\n");
        exit(1);
    } else {
        printf("Device was accessed\n");
    }

    // 'Write' our message in the buffer to the device driver
    // It will create a hash that we can access later
    int count = fwrite(buffer, sizeof(char), 11, fp);
    if(count == -1){
        printf("Error writing to device\n");
        perror("perror output:");
        exit(1);
    } else {
        printf("Wrote to device successfully\n");
    }

    // Read from the device driver
    // This will copy the hash of our 'write' to our buffer
    count = fread(buffer, sizeof(char), BUFFERSIZE, fp);
    if(count < 0) {
        printf("Error reading from device\n");
        perror("perror output:");
        exit(1);
    } else {
        printf("Read from device successfully\n");
    }

    // Print out our hashed message
    printf("Our hashed message is: %s\n", buffer);

    // Make sure to close the file descriptor
    fclose(fp);

    return 0;
}