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

    unsigned int reminder = 0;                              //Used to contain the value of the reminder
    int sizeOfMessage = sizeof message / sizeof message[0]; //Length of the message in bytes
    int messageMinLength = 3;                               //Mininimum allowed length of the message according to the requiements, assuming appended zeroes/reminder bits
    int messageMaxLength = 16;                              //Maximum allowed length of the message according to the requiements, assuming appended zeroes/reminder bits
    int numberOfBitsPerByte = 8;                            //8 bits equal one byte
    int numberOfBits = sizeOfMessage * 8;                   //The total number of bits the message contains

    if (messageMinLength <= sizeOfMessage && sizeOfMessage <= messageMaxLength) //Check if the message is of the allowed length
    {
        int byte = 0; //Used to keep track on which byte the current bit belongs to

        //The loop caculate what the reminder should be before the message is sent
        for (int bit = 1; bit <= numberOfBits; bit++) //Iterate over all bits, one bit per iteration
        {
            reminder = (reminder << 1);                                   //Push the reminder bits one step to the left
            byte = (bit - 1) / numberOfBitsPerByte;                       //Get the bits in each byte from LSB to MSB from the first byte to the last byte
            if (message[byte] & (1 << ((bit - 1) % numberOfBitsPerByte))) //Set the reminder LSB bit to the value of the current bit
            {
                reminder = reminder | 1 << 0;
            }
            if (reminder & (1 << 15)) //If the reminder MSB bit is 1, do an XOR operation with the polynominal
            {
                reminder = reminder ^ POLYNOMIAL;
            }
        } //Loop ends here

        //Divide the reminder into two bytes
        uint8_t reminderFront = (reminder >> 8); //This is the part of the reminder that will go in the second last byte of the message
        uint8_t reminderBack = reminder;         //This is the part of the reminder that will go in the last byte of the message
        int secondLastByte = sizeOfMessage - 1;  //Position of the second last byte
        int lastByte = sizeOfMessage;            //Position of the last byte
        //Before the reminder can be added to the message the bits in the two bytes need to be reversed, so the LSB bit become the MSB and ect
        //This is done by the loop below which can take care of both bytes at the same time
        for (int bit = 1; bit <= 8; bit++)
        {
            //This part add in the reminder bits to the second last byte of the message
            message[secondLastByte] = (message[secondLastByte] << 1);
            if (reminderFront & 1)
            {
                message[secondLastByte] = message[secondLastByte] | 1;
            }
            reminderFront = (reminderFront >> 1);
            //This part add in the reminder bits to the last byte of the message
            message[lastByte] = (message[lastByte] << 1);
            if (reminderBack & 1)
            {
                message[lastByte] = message[lastByte] | 1;
            }
            reminderBack = (reminderBack >> 1);
        }

        //message[1] = 'a';
        //Here the loop that caculate the reminder is run again and should give a reminder of zero,
        //assuming the message have not been corrupted
        reminder = 0;
        for (int bit = 1; bit <= numberOfBits; bit++)
        {
            reminder = (reminder << 1);
            byte = (bit - 1) / numberOfBitsPerByte;
            if (message[byte] & (1 << ((bit - 1) % numberOfBitsPerByte)))
            {
                reminder = reminder | 1 << 0;
            }
            if (reminder & (1 << 15))
            {
                reminder = reminder ^ POLYNOMIAL;
            }
        }

        // Validate the message.
        // If the remainder is zero print "The data is OK\n";
        // otherwise print "The data is not OK\n"
        //If the reminder is zero the message have not been corrupted or atleast the likehood of corruption is minimal
        if (reminder == 0)
        {
            printf("The data is OK");
        }
        //Otherwise the message have been corrupted
        else
        {
            printf("The data not is OK");
        }
    }
    //This else is entered if the message is too short or too long
    else
    {
        //If message is too short
        if (!(messageMinLength <= sizeOfMessage))
        {
            printf("Error, the message is shorter than the allowed minimal length");
        }
        //If message is too long
        else if (!(sizeOfMessage <= messageMaxLength))
        {
            printf("Error, the message is longer than the allowed maximal length");
        }
    }
    return 0;
}
