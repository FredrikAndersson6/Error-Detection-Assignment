#include <stdio.h>
#include <stdint.h>

#define POLYNOMIAL 0xC599U // The CAN protocol uses the CRC-15 with this polynomial
// Polynomial is equal to binary number 11000101 10011001 which is 16 in length

#define RPOLYNOMIAL 0x99A3U
#define RRPOLYNOMIAL 0xA399U
#define RRRPOLYNOMIAL 0x99C5U

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
    //printf("%i\n", reminder ^ message[0]);
    int sizeOfMessage = sizeof message / sizeof message[0];
    //printf("%i\n", sizeOfMessage);

    //byte = message[i] //next byte

    //lastBit = byte & 0x0001U; //Get  bit
    //reminder = (reminder << 1) | lastBit //add last bit at the end of reminder

    int CRCLength = 15;
    int numberOfBitsPerByte = 8;
    int numberOfBits = sizeOfMessage * 8 - (numberOfBitsPerByte - numberOfBitsPerByte % CRCLength);
    //printf("%i\n", CRCLength % numberOfBitsPerByte);
    int byte = 0;

    for (int bit = 1; bit < numberOfBits + 1; bit++)
    {
        reminder = (reminder << 1);
        byte = (bit - 1) / numberOfBitsPerByte;
        //printf("%i\n", (bit % numberOfBitsPerByte));
        if (message[byte] & (1 << ((bit - 1) % numberOfBitsPerByte)))
        {
            reminder = reminder | 1 << 0;
        }
        //reminder = reminder & 0x7FFFU;

        //printf("%i\n", (1 + bit % numberOfBitsPerByte));
        /*for (int i = 15; i > -1; i--)
        {
            printf("%i", (reminder >> i) & 1);
        }
        printf("\n");*/
        //if (CRCLength <= bit && CRCLength <= numberOfBits - bit)
        //{
        //printf("%i %i\n", byte, bit);
        if (reminder & (1 << 15))
        {
            reminder = reminder ^ POLYNOMIAL;
            /*printf("%i %i\n", byte, bit);
            for (int i = 15; i > -1; i--)
            {
                printf("%i", (reminder >> i) & 1);
            }
            printf("\n");*/
        }

        //}
        //printf("%i %i\n", byte, bit);
        //printf("%i\n", reminder);
    }

    //0000101100110101 2869

    /*int next_input_bit = 0;
    int multiple = 0;
    for (int bit = 1; bit < numberOfBits; bit++)
    {
        byte = (bit - 1) / numberOfBitsPerByte;
        next_input_bit = (message[byte] & 1 << (bit - 1) % numberOfBitsPerByte) << (14 - ((bit - 1) % numberOfBitsPerByte - 1));
        //printf("%i\n", next_input_bit);
        reminder ^= next_input_bit;
        multiple = (reminder & 1) ? RRRPOLYNOMIAL : 0;
        reminder = reminder ^ multiple;
        for (int i = 15; i > -1; i--)
        {
            printf("%i", (reminder >> i) & 1);
        }
        reminder = (reminder >> 1);
        printf("\n");
    }*/
    /*for (int i = 15; i > -1; i--)
    {
        printf("%i", (reminder >> i) & 1);
    }
    printf("\n");*/

    /*for (int byte = 0; byte < sizeOfMessage; byte++)
    {
        reminder = reminder ^ message[byte];
        for (int bit = 0; bit < 8; bit++)
        { // Assuming 8 bits per byte
            if (reminder & 0x0001U)
            { // if rightmost (most significant) bit is set
                reminder = (reminder >> 1) ^ RRRPOLYNOMIAL;
            }
            else
            {
                reminder = (reminder >> 1);
            }
        }
        //printf("%i\n", reminder);
        for (int i = 15; i > -1; i--)
        {
            printf("%i", (reminder >> i) & 1);
        }
        printf("\n");
    }*/
    //reminder = (reminder << 1);
    /*for (int i = 15; i > -1; i--)
    {
        printf("%i", (reminder >> i) & 1);
    }
    printf("\n");*/
    //reminder = reminder & 0x7FFFU;

    uint8_t reminderFront = (reminder >> 8);
    uint8_t reminderBack = reminder;

    /*for (int i = 15; i > -1; i--)
    {
        printf("%i", (reminderFront >> i) & 1);
    }
    printf("\n");
    for (int i = 15; i > -1; i--)
    {
        printf("%i", (reminderBack >> i) & 1);
    }
    printf("\n");

    printf("%i, %i, %i\n", reminder, reminderFront, reminderBack);*/
    for (int bit = 1; bit <= 8; bit++)
    {
        message[12] = (message[12] << 1);
        if (reminderFront & 1)
        {
            message[12] = message[12] | 1;
        }
        reminderFront = (reminderFront >> 1);
        message[13] = (message[13] << 1);
        if (reminderBack & 1)
        {
            message[13] = message[13] | 1;
        }
        reminderBack = (reminderBack >> 1);
    }
    /*for (int i = 15; i > -1; i--)
    {
        printf("%i", (message[12] >> i) & 1);
    }
    printf("\n");
    for (int i = 15; i > -1; i--)
    {
        printf("%i", (message[13] >> i) & 1);
    }
    printf("\n");
    printf("%i, %i\n", reminderFront, reminderBack);*/
    //message[12] = reminderFront;
    //message[12] = 208;
    //message[13] = reminderBack;
    //message[13] = 172;

    /*for (int i = 0; i < sizeOfMessage; i++)
    {
        printf("%c\n", (char)message[i]);
    }*/

    //message[1] = 'a';

    // Validate the message.
    // If the remainder is zero print "The data is OK\n";
    // otherwise print "The data is not OK\n"

    /*reminder = 0;
    //printf("%i\n", reminder);
    for (int byte = 0; byte < sizeOfMessage; byte++)
    {
        reminder = reminder ^ message[byte];
        for (int bit = 0; bit < 8; bit++)
        { // Assuming 8 bits per byte
            if (reminder & 0x0001U)
            { // if rightmost (most significant) bit is set
                reminder = (reminder >> 1) ^ RRRPOLYNOMIAL;
            }
            else
            {
                reminder = (reminder >> 1);
            }
        }
        //printf("%i\n", reminder);
    }

    printf("%i\n", reminder);*/

    /*reminderFront = (reminder >> 8);
    reminderBack = reminder;

    printf("%i , %i\n", reminderFront, reminderBack);

    message[12] = reminderFront;
    message[13] = reminderBack;

    for (int i = 0; i < sizeOfMessage; i++)
    {
        printf("%c\n", (char)message[i]);
    }*/
    reminder = 0;
    for (int bit = 1; bit < numberOfBits + 1; bit++)
    {
        reminder = (reminder << 1);
        byte = (bit - 1) / numberOfBitsPerByte;
        //printf("%i\n", (bit % numberOfBitsPerByte));
        if (message[byte] & (1 << ((bit - 1) % numberOfBitsPerByte)))
        {
            reminder = reminder | 1 << 0;
        }
        //reminder = reminder & 0x7FFFU;

        //printf("%i\n", (1 + bit % numberOfBitsPerByte));
        /*for (int i = 15; i > -1; i--)
        {
            printf("%i", (reminder >> i) & 1);
        }
        printf("\n");*/
        //if (CRCLength <= bit && CRCLength <= numberOfBits - bit)
        //{
        //printf("%i %i\n", byte, bit);
        if (reminder & (1 << 15))
        {
            reminder = reminder ^ POLYNOMIAL;
            /*printf("%i %i\n", byte, bit);
            for (int i = 15; i > -1; i--)
            {
                printf("%i", (reminder >> i) & 1);
            }
            printf("\n");*/
        }

        //}
        //printf("%i %i\n", byte, bit);
        //printf("%i\n", reminder);
    }
    printf("%i\n", reminder);
    return 0;
}
