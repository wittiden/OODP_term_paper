#pragma once
#include<iostream>
#include<thread>
#include<chrono>
#include <windows.h>

void loadingImitation();
void inactivity_timer(int seconds);

//		std::cout << "Выходим из программы";
//		loadingImitation();
//		exit(0);

//    HANDLE consoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
//    std::thread timer(inactivity_timer, 100);
//    timer.detach();