#include <iostream>
#include <fstream>
#include <queue>
#include <deque>
#include <cstring>
#include <string>
#include <cstdlib>
#include <list>
void read_block(int b, int m);
void setup_PM();
void init_ST(char* char_input);
void init_PT(char* char_input);
void PT_not_res(int s);
void page_not_res(int s, int p);
bool check_valid(int VA, int s, int p);
int get_s(int VA);
int get_p(int VA);
int get_w(int VA);
int get_pw(int VA);
int get_PA(int VA);
void read_input();

