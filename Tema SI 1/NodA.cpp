#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include <thread>
#include <math.h>
#include <stdio.h> 
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <windows.h>

using namespace std;

class NodA
{
	string initializationVector;
	string encriptedtext;
	string decriptedtext;
	string plaintext;
	string K3;
	string K1encripted;
	string K1;
	string K2encripted;
	string K2;
	int desiredEncSystem = 2;
	string MessageOK = "keys have been properly received. Sending the message now";
	
	void decriptK1()
	{
		for (int i = 0; i < 16; i++)
		{
			int sum;
			sum = K1encripted[i] - K3[i];

			if (sum < 35)sum = sum + 125 - 35;

			K1 += (char)sum;
		}
	}
	void decriptK2()
	{
		for (int i = 0; i < 16; i++)
		{
			int sum;
			sum = K2encripted[i] - K3[i];

			if (sum < 35)sum = sum + 125 - 35;

			K2 += (char)sum;
		}
	}
	string applyTwoStrings(string s1, string s2)
	{
		string toReturn;
		for (int i = 0; i < 16; i++)
		{
			if (s1 == s2)toReturn += '1';
			else toReturn += '0';
		}
		return toReturn;
	}
	
	void letterIntoAsciiBaseTwo(char letter, char* baseTwo)
	{
		char buffer[9];
		int number = letter;
		_itoa_s(number, buffer, 2);
		for (int i = 0; i < 8; i++)baseTwo[i] = '0';
		int baseTwoLength = 7;
		for (int i = strlen(buffer) - 1; i >= 0; i--)baseTwo[baseTwoLength--] = buffer[i];
		baseTwo[8] = NULL;
	}
	int baseTwoIntoBaseTen(char* baseTwo)
	{
		int nrInBase10=0;
		for (int i = 0; i < strlen(baseTwo); i++)
		{
			if(baseTwo[i]=='1')nrInBase10 += pow(2, 7 - i);
		}
		return nrInBase10;
	}

	string decryptECB(string ECB)
	{
		string plaintext;
		int i = 0;
		while (i <= ECB.length())
		{
			string block;
			for (int j = 0; j < K1.length(); j++)
			{
				if (i <= ECB.length())
				{
					block += ECB[i];
					i++;
				}
				else
				{
					break;
				}
			}
			for (int j = 0; j < block.length(); j++)
			{
				int sum;
				sum = block[j] - K1[j];
				if (sum < 35 && sum != 0 && sum != 32)sum = sum + 125 - 35;
				if ((char)sum == '-')break;
				plaintext += (char)sum;
			}
			//cout << endl << block;
		}
		plaintext.erase(plaintext.length() - 1);
		return plaintext;
	}
	string encryptECB(string plain)
	{
		string encryptedText;
		int i = 0;
		while (i <= plain.length())
		{
			string block;
			for (int j = 0; j < K1.length(); j++)
			{
				if (i <= plain.length())
				{
					block += plain[i];
					i++;
				}
				else
				{
					block += '-';
				}
			}
			string encBlock;
			for (int j = 0; j < block.length(); j++)
			{
				int sum;
				sum = K1[j] + block[j];
				if (sum > 125)sum = sum - 125 + 35;
				encBlock += (char)sum;

				//cout << sum << endl;
			}
			encryptedText += encBlock;
			cout << encBlock << endl;
			//cout << endl << block;
		}
		
		return encryptedText;
	}

	string encryptCFBOneBlock(string plainBlock, string encryptedBlock)
	{
		string keyEnc;
		for (int j = 0; j < 8; j++)
		{
			int sum;
			sum = encryptedBlock[j] + K2[j];
			if (sum > 125)sum = sum - 125 + 35;
			keyEnc += (char)sum;
		}
		
		string cipherBlock;
		for (int j = 0; j < 8; j++)
		{
			int sum;
			sum = plainBlock[j] ^ keyEnc[j];
			/*if (sum < 0)sum = -sum;
			if (sum < 35)sum = sum + 125 - 35;
			if (sum > 125)sum = sum - 125 + 35;*/
			cipherBlock += (char)sum;
			
		}
		//cout << "cipher --- " << keyEnc << endl;
		return cipherBlock;
	}
	
	string encryptCFB(string plaintxt)
	{
		string cipher;
		int i = 0;

		string encryptedBlock;

		for (int j = 0; j < K2.length(); j++)
		{
			int sum;
			sum = initializationVector[j] + K2[j];
			if (sum > 125)sum = sum - 125 + 35;
			encryptedBlock += (char)sum;
			
		}

		string block;
		for (int j = 0; j < 8; j++)
		{

			//char c= (encryptedBlock[j] ^ plain[j]);
			int sum;
			sum = encryptedBlock[j] ^ plaintxt[j];
		//	if (sum < 0)sum = 128+sum;
		//	if (sum < 35 && sum!=32)sum = sum + 125 - 35;
			//if (sum > 125)sum = sum - 125 + 35;
			block += (char)sum;
		}
		cipher += block;
		i = 8;
		while (i < plaintxt.length())
		{
			string plainBlock;
			for (int j = 0; j < 8; j++)
				if (i < plaintxt.length())plainBlock += plaintxt[i++];
				else plainBlock += '-';
			block = encryptCFBOneBlock( plainBlock, block);
			cipher += block;
		}
		//cout << endl << "---------" << endl << cipher << endl << "-----------" << endl;
		return cipher;
	}

	string decryptCFB(string encryptedText)
	{
		//cout << endl << "---------" << endl << encryptedText << endl << "-----------qqqqq" << endl;
		string plaintxt;

		string encryptedBlock;
		string encBlock1;
		for (int j = 0; j < K2.length(); j++)
		{
			int sum;
			sum = initializationVector[j] + K2[j];
			if (sum > 125)sum = sum - 125 + 35;
			encryptedBlock += (char)sum;
		}
		string plaintextBlock;
		for (int j = 0; j < 8; j++)
		{

			int sum;
			encBlock1 += encryptedText[j];
			sum = encryptedBlock[j] ^ encryptedText[j];
		//	if (sum < 0)sum = 128+sum;
			//if (sum > 125)sum = sum - 125 + 35;
		//	if (sum <35 && sum != 0 && sum != 32)sum = sum -35+125;
			plaintextBlock += (char)sum;
		}
		plaintxt += plaintextBlock;
		int i;
		i = 8;
		cout << endl << endl;
		while (i < encryptedText.length())
		{
			string encBlock2;
			for (int j = 0; j < 8; j++)
			{
				encBlock2 += encryptedText[i++];
			}
			//cout << encBlock1 << "    -    -    - " << encBlock2 << endl;
			plaintextBlock = decryptCFBOneBlock(encBlock1, encBlock2);
			encBlock1 = encBlock2;
			plaintxt += plaintextBlock;
		}



		return plaintxt;
	}
	string decryptCFBOneBlock(string cipher1, string cipher2)
	{
		//cout << cipher1;
		string keyDec;
		for (int j = 0; j < 8; j++)
		{
			int sum;
			sum = cipher1[j] + K2[j];
			if (sum > 125)sum = sum - 125 + 35;
			keyDec += (char)sum;
		}
		//cout << "cipher1 --- " << cipher1 << endl;
		string deCipheredBlock;
		for (int j = 0; j < 8; j++)
		{
			int sum;
			if ((cipher2[j] ^ keyDec[j]) == '-')break;
			sum = cipher2[j] ^ keyDec[j];
			/*if (sum < 0)sum = -sum;
			if (sum < 35)sum = sum + 125 - 35;
			if (sum > 125)sum = sum - 125 + 35;*/
			deCipheredBlock += (char)sum;

		}
		return deCipheredBlock;
	}

	void pasteIntoFile(string s)
	{
		f << s;
	}

public:
	
	ifstream g;
	ofstream f;
	NodA(string k3)
	{
		K3 = k3;
		/*for (int i = 0; i < 400; i++) {
			char c;
			c = i;
			int b;
			b = c;
			cout << i << " : " << c <<" : "<<b<< endl;
		}*/
		
		g.open("A resources\\to send.txt");
		f.open("A resources\\received.txt");
		getline(g, plaintext);
		//cout << plaintext;
		//thread SystemRun();
	}
	void getInitializationVector(string x)
	{
		initializationVector = x;
	}
	int getDesiredEnc()
	{
		srand(time(NULL));
		desiredEncSystem = rand() % 2 + 1;
		Sleep(200);
		return desiredEncSystem;
	}
	void setK1enc(string enc)
	{
		K1encripted = enc;
		decriptK1();
	}
	void setK2enc(string enc)
	{
		K2encripted = enc;
		decriptK2();
	}
	
	string sendMessageOK()
	{
		return encryptECB(MessageOK);
	}
	string sendMessageOKCFB()
	{
		return encryptCFB(MessageOK);
	}
	string sendMessageECB()
	{
		return encryptECB(plaintext);
	}
	string sendMessageCFB()
	{
		return encryptCFB(plaintext);
	}
	string sendMessageReceived()
	{
		return encryptECB("A: Message REceived<<<------");
	}

	void receiveMessageECB(string encryptedMessage)
	{
		encriptedtext = encryptedMessage;
		decriptedtext = decryptECB(encriptedtext);
		pasteIntoFile(decriptedtext);
	}
	void receiveMessageCFB(string encryptedMessage)
	{
		encriptedtext = encryptedMessage;
		decriptedtext = decryptCFB(encriptedtext);
		pasteIntoFile(decriptedtext);
	}

};

