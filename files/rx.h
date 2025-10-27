/* Copyright 2024 Neil Kirby, all rights reserved. */
/* Do not publish this code without written permission */
#include <stdbool.h>
/* Neil Kirby */

void rx_beep();
void rx_clear();
void rx_flash();
int rx_getch();
int rx_initialize();
void rx_refresh();
bool rx_rock (int color, double X, double Y, double radius);
bool rx_ship(int color, int score, double X, double Y, double angle);
bool rx_shot(int color, double X, double Y);
void rx_sim_time(int milliseconds);
void rx_status(const char *statstr);
void rx_teardown();
