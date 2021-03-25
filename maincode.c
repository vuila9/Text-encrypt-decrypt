#include <stdio.h>
#include <string.h>

#define MAX_BUF  256

// Initialize functions
unsigned char getBit(unsigned char, int);
unsigned char setBit(unsigned char, int);
unsigned char clearBit(unsigned char, int);
unsigned char xOr (unsigned char p1, unsigned char p2);
unsigned char turnBitIntoThis (unsigned char c, int n, unsigned char oneOrZero);
unsigned char updateCounter(unsigned char counter, unsigned char key);
int encryptOrDecrypt (unsigned char currentElement, unsigned char key, unsigned char counter);

/*
  Function: main
  Purpose:  ask the user to encrypt/decrypt a string they input, 
  			then print the result corresponding to the user's selection
  To use:	compile using GCC
  Author:	John Nguyen
*/
int main() {

	int  choice, ind, forDecrypt;
	char userStr[MAX_BUF];
	unsigned char key		= 0b10110010;
	unsigned char counter	= 0b00101000;
  
	printf("\nYou may:\n");
	printf("  (1) Encrypt a message \n");
	printf("  (2) Decrypt a message \n");

	printf("\n  what is your selection: ");
  
	while (choice != 1 && choice != 2){				// if invalid input, prompt infinitely
		fgets(userStr, sizeof(userStr), stdin);
		sscanf(userStr, "%d", &choice);

		switch (choice) {

			case 1:		// Encrypt
				ind = 0;
				printf("\nEnter plaintext:\n");
				fgets(userStr, sizeof(userStr), stdin);
				
				while(userStr[ind]!='\n') {	// find the correct size of user's input	
					ind++;						
				}
			
				for (int i = 0; i < ind; i++) {
					counter = updateCounter(counter, key);
					counter++;
					key++;
					printf("%d ", encryptOrDecrypt(userStr[i], key, counter)); // print current encrypted source byte
				}
				printf("\n");
				break;
				
			case 2:		// Decrypt
				printf("\nEnter ciphertext:\n");
				scanf("%d", &forDecrypt);			
				
				while (forDecrypt != -1){			// terminate at -1
					counter = updateCounter(counter, key);
					counter++;
					key++;
					printf("%c", encryptOrDecrypt(forDecrypt, key, counter)); // print current decrypted source byte
					scanf("%d", &forDecrypt);		// update the next number in the ciphertext
				}
				printf("\n");								
				break;
      		
			default: 
				printf("\nInvalid selection, please try again: ");
				break;		
		}  
	}
	return 0;
}

/*
  Function:  updateCounter
  Purpose:   update the current counter
       in:   counter to be update
       in:   fixed key
   return:   value of updated counter in unsigned char
*/
unsigned char updateCounter(unsigned char counter, unsigned char key){
	unsigned char tempCounter, pos1, pos2;
	int index;
	
	for (int i = 7; i >= 0; i--){
		tempCounter = counter;
		pos1 = getBit(tempCounter, i);	// assign pos1
		
		if (getBit(key,i) == 0) {
			index = i + 2;
			if (index > 7)
				index = index - 8;		// wrap back to LSB
		}
		else {
			index = i + 1;
			if (index > 7)
				index = index - 8;
		}
		pos2 = getBit(tempCounter, index);	// assign pos2
		
		tempCounter = turnBitIntoThis(tempCounter, i, xOr(pos1,pos2));	
		counter = tempCounter;
	}
	return counter;	
}

/*
  Function:  encryptOrDecrypt
  Purpose:   encrypt or decrypt the current unsigned char element
       in:   element to be encrypted/decrypted
       in:   fixed key
       in:	 current counter
   return:   value of encrypted/decrypted element in integer
*/
int encryptOrDecrypt (unsigned char currentElement, unsigned char key, unsigned char counter){
	int tempValue = currentElement;

	// 3 >
	if ((counter % 9) < 3) {
		for (int i = 0; i <= 7; i = i + 2)
			tempValue = turnBitIntoThis(tempValue, i, xOr(getBit(currentElement, i), getBit(counter, i)));	
	}
	// 3 <= ... <= 5
	else if ((counter % 9) >= 3 && (counter % 9) <= 5) {
		for (int i = 1; i <= 7; i = i + 2)
			tempValue = turnBitIntoThis(tempValue, i, xOr(getBit(currentElement, i), getBit(counter, i))); 		
	}
	// 6 <
	else {		
		for (int i = 0; i <= 7; i++)
			tempValue = turnBitIntoThis(tempValue, i, xOr(getBit(currentElement, i), getBit(counter, i)));				
	}	
	return tempValue;
}

/*
  Function:  xOr
  Purpose:   x-or the 2 given positions
       in:   position 1
       in:   position 2
   return:   result of p1 x-or p2 in unsigned char
*/
unsigned char xOr (unsigned char p1, unsigned char p2){
	return p1^p2;
}

/*
  Function:  turnBitIntoThis
  Purpose:   turn the n-th position bit of given element into 1 or 0
       in:   given element
       in:   n-th position bit needs to be turned
       in:	 turn value of 1 or 0
   return:   the element with its nth-position bit turned into 1 or 0
*/
unsigned char turnBitIntoThis (unsigned char c, int n, unsigned char oneOrZero){
	if (oneOrZero == 1)
		c = setBit(c, n);
	else 
		c = clearBit(c, n);
	return c;
}

/*
  Function:  getBit
  Purpose:   retrieve value of bit at specified position
       in:   character from which a bit will be returned
       in:   position of bit to be returned
   return:   value of bit n in character c (0 or 1)
*/
unsigned char getBit(unsigned char c, int n)   
{ 
	return ((c & (1 << n)) >> n);
}

/*
  Function:  setBit
  Purpose:   set specified bit to 1
       in:   character in which a bit will be set to 1
       in:   position of bit to be set to 1
   return:   new value of character c with bit n set to 1
*/
unsigned char setBit(unsigned char c, int n)   
{ 
	return (c | (1 << n));
}

/*
  Function:  clearBit
  Purpose:   set specified bit to 0
       in:   character in which a bit will be set to 0
       in:   position of bit to be set to 0
   return:   new value of character c with bit n set to 0
*/
unsigned char clearBit(unsigned char c, int n) 
{ 
	return (c & (~(1 << n)));
}
