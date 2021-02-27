#include <stdio.h>
#include <stdint.h>

#define POLYNOMIAL 0xC599U // The CAN protocol uses the CRC-15 with this polynomial
// Polynomial is equal to binary number 1100 0101 1001 1001 which is 16 in length

int main(void)
{
    uint8_t message[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', 0, 0}; // 15 zeros have been appended to the message

    // Calculate the CRC and Checksum the message

    //Append 15 zeroes at the end, already done for message
    //XOR the first 16 bits in the message with Polynomial
    //Remove any 0 from the result that come before the first 1 to the left,
    //example CRC-8 0000 1100 = 1100
    //Fill out the result by getting the next bits in the message by appending them to the right
    //to it again is 16 bit long, from last example 1100 filled out to 1100 1001 and again
    //do XOR with the Polynomial.
    //Keep repeating filling out and XOR with Polynomial to it is no longer possible to keep filling
    //out with 16 bits, the bits left at that point is the reminder.
    //Pad the reminder with 0 to the left so it is 15 bits long, this is the CRC checksum, then replaced the appened 15 zeroes
    //added to the message with the padded reminder.
    //So the message should look something like {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', r, r};
    //with r,r representing the reminder bits.

    message[1] = 'a';

    // Validate the message.
    // If the remainder is zero print "The data is OK\n";
    // otherwise print "The data is not OK\n"

    return 0;
}
