#include <stdio.h>
#include <stdint.h>

#define POLYNOMIAL 0xC599U // The CAN protocol uses the CRC-15 with this polynomial
// Polynomial is equal to binary number 11000101 10011001 which is 16 in length

#define RPOLYNOMIAL 0x99A3U

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

    // Least significant bit first (little-endian)
    // x^16+x^12+x^5+1 = 1000 0100 0000 1000 (1) = 0x8408

    unsigned int reminder = 0;
    printf("%i\n", reminder ^ message[0]);
    int sizeOfMessage = sizeof message / sizeof message[0];
    printf("%i\n", sizeOfMessage);

    for (int i = 0; i < sizeOfMessage; i++)
    {
        reminder = reminder ^ message[i];
        for (int j = 1; j <= 8; j++)
        { // Assuming 8 bits per byte
            if (reminder & 0x0001)
            { // if rightmost (most significant) bit is set
                reminder = (reminder >> 1) ^ POLYNOMIAL;
            }
            else
            {
                reminder = (reminder >> 1);
            }
        }
    }

    uint8_t reminderFront = (reminder >> 8);
    uint8_t reminderBack = reminder;

    printf("%i, %i, %i\n", reminder, reminderFront, reminderBack);

    message[12] = reminderFront;
    message[13] = reminderBack;

    for (int i = 0; i < sizeOfMessage; i++)
    {
        printf("%c\n", (char)message[i]);
    }

    //message[1] = 'a';

    // Validate the message.
    // If the remainder is zero print "The data is OK\n";
    // otherwise print "The data is not OK\n"

    reminder = 0;
    printf("%i\n", reminder);
    for (int i = 0; i < sizeOfMessage; i++)
    {
        reminder = reminder ^ message[i];
        for (int j = 1; j <= 8; j++)
        { // Assuming 8 bits per byte
            if (reminder & 0x0001)
            { // if rightmost (most significant) bit is set
                reminder = (reminder >> 1) ^ POLYNOMIAL;
            }
            else
            {
                reminder = (reminder >> 1);
            }
        }
    }

    printf("%i\n", reminder);

    reminderFront = (reminder >> 8);
    reminderBack = reminder;

    printf("%i , %i\n", reminderFront, reminderBack);

    message[12] = reminderFront;
    message[13] = reminderBack;

    for (int i = 0; i < sizeOfMessage; i++)
    {
        printf("%c\n", (char)message[i]);
    }

    return 0;
}
