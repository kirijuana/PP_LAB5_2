#include "stdafx.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <vector>
using namespace std;

int front_1 = 0; //������ ��� ������ �� ������
int front_2 = 0; //������ ��� ������ �� ������

sem_t empty_1; //�������, ������������ ��������� ����� ����
sem_t full_1; //�������, ������������ ��������� ����� �����
sem_t empty_2; //�������, ������������ ��������� ����� ����
sem_t full_2; //�������, ������������ ��������� ����� �����

pthread_mutex_t mutexD; //������� ��� �������� ������
pthread_mutex_t mutexF; //������� ��� �������� ������
int k = 100; // ���������� �/� ��������
map<int, vector<int>> cannon_1;
map<int, vector<int>> cannon_2;

int z_1 = 0; // ���-�� �����
int z_2 = 0;

// 0 - �������� ����� // 1 - ��������� �������� // 2 - �������� �������� // 3 - ���� �� ������� // 4 ������������ �� ������ ������

void cannons_generation()
{
	srand(time(NULL));
	z_1 = rand() % 10 + 1; // ���������� �����
	for (int i = 0; i < z_1; i++)
	{
		cannon_1[i].push_back(10); // �������� ������ ����� 10
		for (int j = 1; j < 5; j++)
		{
			if (j == 2)
				cannon_1[i].push_back(rand() % 4); // �������� �� 0 �� 3
			else
				cannon_1[i].push_back(rand() % 10 + 1);
		}
	}


	z_2 = rand() % 10 + 1; // ���������� �����
	for (int i = 0; i < z_2; i++)
	{
		cannon_2[i].push_back(10); // �������� ������ ����� 10
		for (int j = 1; j < 5; j++)
		{
			if (j == 2)
				cannon_2[i].push_back(rand() % 4); // �������� �� 0 �� 3
			else
				cannon_2[i].push_back(rand() % 10 + 1);
		}
	}
}

bool check_damage_2 = true;
bool check_damage = true;
void *Cannons_1(void *param)
{
	pthread_mutex_lock(&mutexD); //������ �������� ������
	sem_wait(&empty_1);
	if (!check_damage)
		front_1 = (front_1 + 1);
	if (front_1 >= z_1 || front_1 + 1 == z_1)
	{
		printf("����� ����� � ������� ������ �� ��������, ���� ���������� ����� �� ������� �� ���������\n");
		system("pause");
	}
	check_damage = false;
	while (!check_damage)
	{
		if (cannon_1[front_1][0] > 0) // ���� ����� ����, �� ��������
		{
			for (int j = 0; j < z_2; j++)
			{
				if ((cannon_1[front_1][1] + k >= cannon_2[j][4] + k) && (cannon_2[j][0] > 0))
				{

					cannon_2[j][0] = cannon_2[j][0] - cannon_1[front_1][3] * cannon_1[front_1][2];
					check_damage = true;
					if (cannon_2[j][0] <= 0)
					{
						printf("����� #2: ����� � %d ��������.\n", j + 1);
						break;
					}
				}

			}
			if (!check_damage)
				break;
		}
		else
		{
			break;
		}
	}

	sem_post(&full_1);
	pthread_mutex_unlock(&mutexD);

	return NULL;
}

void *Cannons_2(void *param)
{
	pthread_mutex_lock(&mutexF); //������ �������� ������
	sem_wait(&empty_2);
	if (!check_damage_2)
		front_2 = (front_2 + 1);
	if (front_2 >= z_2 || front_2 + 1 == z_2)
	{
		printf("����� ����� � ������� ������ �� ��������, ���� ���������� ����� �� ������� �� ���������\n");
		system("pause");
	}
	check_damage_2 = false;
	while (!check_damage_2)
	{
		if (cannon_2[front_2][0] > 0) // ���� ����� ����, �� ��������
		{
			for (int j = 0; j < z_1; j++)
			{
				if ((cannon_2[front_2][1] + k >= cannon_1[j][4] + k) && (cannon_1[j][0] > 0))
				{

					cannon_1[j][0] = cannon_1[j][0] - cannon_2[front_2][3] * cannon_2[front_2][2];
					check_damage_2 = true;
					if (cannon_1[j][0] <= 0)
					{
						printf("����� #1: ����� � %d ��������.\n", j + 1);
					}
				}


			}
			if (!check_damage_2)
				break;
		}
		else
		{
			break;
		}
	}

	sem_post(&full_2);
	pthread_mutex_unlock(&mutexF);

	return NULL;
}

int main()
{
	int i;
	//������������� ��������� � ���������
	pthread_mutex_init(&mutexD, NULL);
	pthread_mutex_init(&mutexF, NULL);
	sem_init(&empty_1, 0, 100); //���������� ��������� ����� 
	sem_init(&full_1, 0, 0); //���������� ������� ����� ����� 0
	sem_init(&empty_2, 0, 100); //���������� ��������� ����� ����� 100
	sem_init(&full_2, 0, 0); //���������� ������� ����� ����� 0
	cannons_generation();

	printf("����� ������� ������ (%d):\n", z_1);
	for (int i = 0; i < z_1; i++)
	{
		printf("����� # %d: %d %d %d %d %d \n", i + 1, cannon_1[i][0], cannon_1[i][1], cannon_1[i][2], cannon_1[i][3], cannon_1[i][4]);
	}

	printf("����� ������� ������ (%d):\n", z_2);
	for (int i = 0; i < z_2; i++)
	{
		printf("����� # %d: %d %d %d %d %d \n", i + 1, cannon_2[i][0], cannon_2[i][1], cannon_2[i][2], cannon_2[i][3], cannon_2[i][4]);
	}


	//������ ����� ������� ������
	pthread_t *threadP = new pthread_t[z_1 * 2];
	for (i = 0; i<z_1 * 2; i++)
		pthread_create(&threadP[i], NULL, Cannons_1, NULL);
	//������ ����� ������� ������
	pthread_t *threadC = new pthread_t[z_2 * 2];
	for (i = 0; i<z_2 * 2; i++)
		pthread_create(&threadC[i], NULL, Cannons_2, NULL);



	//for (i = 0; i < 2; i++)
	//	pthread_join(threadP[i], NULL);
	////������ ����� ������� ������

	//for (i = 0; i < 5; i++)
	//	pthread_join(threadC[i], NULL);

//Cannons_2(NULL);

	//int check = 0;
	//for (int i = 0; i < z_1; i++)
	//{
	//	if (cannon_1[i][0] >= 0)
	//	{
	//		check++;
	//		break;
	//	}
	//}

	//if (check > 0)
	//{
	//	printf("� ������� ������ ������� ����� �����");
	//}

	//check = 0;
	//for (int i = 0; i < z_2; i++)
	//{
	//	if (cannon_2[i][0] >= 0)
	//	{
	//		check++;
	//		break;
	//	}
	//}

	//if (check > 0)
	//{
	//	printf("� ������� ������ ������� ����� �����");
	//}

	system("pause");
}

