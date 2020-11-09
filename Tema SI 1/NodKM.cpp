#pragma once
#include<iostream>
#include<fstream>
#include "NodA.cpp"
#include "NodB.cpp"
#include <stdlib.h>
#include<thread>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;

class NodKM
{

	ifstream g;
	string K1 = "x!A%D*G-KaPdSgVk";
	string K2 = "JaNdRgUjXn2r5u8x";
	string K3 = "SgVkYp3s6v9y$B&E";
	string initializationVector;
	string K1encrypted;
	string K2encrypted;
	int Aenc = -1, Benc = -1;

	NodA *A;
	NodB *B;
	string generateInitializationVector()
	{
		srand(time(NULL));
		for (int i = 0; i < 16; i++) { 
			int nr;
			nr = rand() % 125 + 35;
			initializationVector += (char)nr;
		}
		return initializationVector;
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
			block = encryptCFBOneBlock(plainBlock, block);
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
			//if ((char)sum == '-')break;
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
	string encryptK1()
	{
		char c1, c2;
		string K1enc;
		for (int i = 0; i < 16; i++)
		{
			c1 = K1[i];
			c2 = K3[i];
			int sum = c1 + c2;
			if (sum > 125)sum = sum - 125 + 35;
			K1enc += (char)sum;
			//cout << endl << K1enc;
		}
		return K1enc;
	}

	string encryptK2()
	{
		char c1, c2;
		string K2enc;
		for (int i = 0; i < 16; i++)
		{
			c1 = K2[i];
			c2 = K3[i];
			int sum = c1 + c2;
			if (sum > 125)sum = sum - 125 + 35;
			K2enc += (char)sum;
			//cout << endl << K1enc;
		}
		return K2enc;
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
				sum = block[j]-K1[j];
				if (sum < 35 && sum!=0 && sum!=32)sum = sum + 125 - 35;
				plaintext += (char)sum;
			}
			//cout << endl << block;
		}
		return plaintext;
	}

public:
	NodKM()
	{
		generateInitializationVector();
		K1encrypted = encryptK1();
		K2encrypted = encryptK2();
		//cout << endl << "K2 after encryption = "<< K2encrypted;
		A = new NodA(K3);
		B = new NodB(K3);

		systemRun();
	}

	void runECB()
	{
		cout << "received ECB" << endl;
		A->setK1enc(K1encrypted);
		B->setK1enc(K1encrypted);
		//cout << A->encryptECB()<<endl<<endl;
		cout << decryptECB(A->sendMessageOK()) << endl;
		cout << decryptECB(B->sendMessageOK()) << endl << endl;
		B->receiveMessageECB(A->sendMessageECB());
		A->receiveMessageECB(B->sendMessageECB());
	}
	void runCFB()
	{
		cout << "received CFB" << endl;
		A->setK2enc(K2encrypted);
		B->setK2enc(K2encrypted);
		A->getInitializationVector(initializationVector);
		B->getInitializationVector(initializationVector);
		cout << decryptCFB(A->sendMessageOKCFB()) << endl;
		cout << decryptCFB(B->sendMessageOKCFB());

		B->receiveMessageCFB(A->sendMessageCFB());
		cout << endl;
		//				cout << decryptCFB(B->sendMessageReceived()) << endl;
		A->receiveMessageCFB(B->sendMessageCFB());
		//cout << decryptCFB(A->sendMessageReceived()) << endl;
	}

	void systemRun()
	{
		cout << endl << "listening for the nodes" << endl << endl;
		
		
		
			Aenc=A->getDesiredEnc();
			Benc=B->getDesiredEnc();
			cout << Aenc << " --- " << Benc<<endl;
			if (Aenc == 1 && Benc == 1)
			{
				runECB();
				return;
			}
			if (Aenc == 2 && Benc == 2)
			{
				runCFB();
				return;
			}
			srand(time(NULL));
		    int randomVar = rand() % 100 + 1;
			if (randomVar > 50)runECB();
			else runCFB();
		
	}

	bool getAenc(int x)
	{
		if (x != 1 && x != 2)return false;
		Aenc = x;
		return true;
	}
	bool getBenc(int x)
	{
		if (x != 1 && x != 2)return false;
		Benc = x;
		return true;
	}

};

