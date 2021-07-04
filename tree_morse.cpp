//using g++ 6.3.0 ; aka MinGW.org GCC-6.3.0-1
/**
 * logic tree of Morse code
 *
 * @author  andythebreaker
 * @version 0.0.1, 07/04/2021
 */
#include <algorithm>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <regex>
#include <sstream>
#include <queue>
#include <utility>

#define ALL_MOS_PAT_NUM 56

using namespace std;

class cell
{
public:
    // Default constructor
    cell() {}

    // Initialize a Box with custom dimensions
    cell(string name, int *morse)
        : m_name(name), m_morse(morse)
    {
    }

    string name(void) { return m_name; };
    int *morse(void) { return m_morse; }

private:
    // Will have value of 0 when default constructor is called.
    // If we didn't zero-init here, default constructor would
    // leave them uninitialized with garbage values.
    string m_name{"nan"};
    int *m_morse{0};
};

// 宣告類別
class tree_of_all
{
    // 宣告 public 成員
public:
    tree_of_all dot();
    tree_of_all dash();
    cell *endof;
};

string print_int_ary(int *int_ary)
{
    string output_str = "";
    int arrSize = sizeof(int_ary) / sizeof(int_ary[0]);
    for (size_t i = 0; i < arrSize; i++)
    {
        output_str += to_string(int_ary[i]) + " ,";
    }
    return output_str;
}

int main()
{
    cout << "Running..." << endl;

    //56
    int morse_ary_A[] = {1, 2, 0};
    int morse_ary_B[] = {2, 1, 1, 1, 0};
    int morse_ary_C[] = {2, 1, 2, 1, 0};
    int morse_ary_D[] = {2, 1, 1, 0};
    int morse_ary_E[] = {1, 0};
    int morse_ary_F[] = {1, 1, 2, 1, 0};
    int morse_ary_G[] = {2, 2, 1, 0};
    int morse_ary_H[] = {1, 1, 1, 1, 0};
    int morse_ary_I[] = {1, 1, 0};
    int morse_ary_J[] = {1, 2, 2, 2, 0};
    int morse_ary_K[] = {2, 1, 2, 0};
    int morse_ary_L[] = {1, 2, 1, 1, 0};
    int morse_ary_M[] = {2, 2, 0};
    int morse_ary_N[] = {2, 1, 0};
    int morse_ary_O[] = {2, 2, 2, 0};
    int morse_ary_P[] = {1, 2, 2, 1, 0};
    int morse_ary_Q[] = {2, 2, 1, 2, 0};
    int morse_ary_R[] = {1, 2, 1, 0};
    int morse_ary_S[] = {1, 1, 1, 0};
    int morse_ary_T[] = {2, 0};
    int morse_ary_U[] = {1, 1, 2, 0};
    int morse_ary_V[] = {1, 1, 1, 2, 0};
    int morse_ary_W[] = {1, 2, 2, 0};
    int morse_ary_X[] = {2, 1, 1, 2, 0};
    int morse_ary_Y[] = {2, 1, 2, 2, 0};
    int morse_ary_Z[] = {2, 2, 1, 1, 0};
    int morse_ary_0[] = {2, 2, 2, 2, 2, 0};
    int morse_ary_1[] = {1, 2, 2, 2, 2, 0};
    int morse_ary_2[] = {1, 1, 2, 2, 2, 0};
    int morse_ary_3[] = {1, 1, 1, 2, 2, 0};
    int morse_ary_4[] = {1, 1, 1, 1, 2, 0};
    int morse_ary_5[] = {1, 1, 1, 1, 1, 0};
    int morse_ary_6[] = {2, 1, 1, 1, 1, 0};
    int morse_ary_7[] = {2, 2, 1, 1, 1, 0};
    int morse_ary_8[] = {2, 2, 2, 1, 1, 0};
    int morse_ary_9[] = {2, 2, 2, 2, 1, 0};
    int morse_ary_DOT[] = {1, 2, 1, 2, 1, 2, 0};
    int morse_ary_LB[] = {2, 1, 2, 2, 1, 0};
    int morse_ary_RB[] = {2, 1, 2, 2, 1, 2, 0};
    int morse_ary_PLUS[] = {1, 2, 1, 2, 1, 0};
    int morse_ary_SP[] = {2, 1, 2, 1, 2, 1, 0};
    int morse_ary_UQ[] = {1, 1, 2, 1, 2, 0};
    int morse_ary_COMA[] = {2, 2, 1, 1, 2, 2, 0};
    int morse_ary_DASH[] = {2, 1, 1, 1, 1, 2, 0};
    int morse_ary_EQ[] = {2, 1, 1, 1, 2, 0};
    int morse_ary_UEXC[] = {2, 2, 1, 1, 1, 2, 0};
    int morse_ary_QM[] = {1, 1, 2, 2, 1, 1, 0};
    int morse_ary_AND[] = {1, 2, 1, 1, 1, 0};
    int morse_ary_DD[] = {1, 1, 2, 2, 1, 2, 0};
    int morse_ary_MONY[] = {1, 1, 1, 2, 1, 1, 2, 0};
    int morse_ary_EXC[] = {2, 1, 2, 1, 2, 2, 0};
    int morse_ary_SQUT[] = {1, 2, 2, 2, 2, 1, 0};
    int morse_ary_COLN[] = {2, 2, 2, 1, 1, 1, 0};
    int morse_ary_DQUT[] = {1, 2, 1, 1, 2, 1, 0};
    int morse_ary_AT[] = {1, 2, 2, 1, 2, 1, 0};
    int morse_ary_SLAH[] = {2, 1, 1, 2, 1, 0};

    cell morse_ary[ALL_MOS_PAT_NUM] = {
        {"ASCII88PATTERN_A", morse_ary_A},
        {"ASCII88PATTERN_B", morse_ary_B},
        {"ASCII88PATTERN_C", morse_ary_C},
        {"ASCII88PATTERN_D", morse_ary_D},
        {"ASCII88PATTERN_E", morse_ary_E},
        {"ASCII88PATTERN_F", morse_ary_F},
        {"ASCII88PATTERN_G", morse_ary_G},
        {"ASCII88PATTERN_H", morse_ary_H},
        {"ASCII88PATTERN_I", morse_ary_I},
        {"ASCII88PATTERN_J", morse_ary_J},
        {"ASCII88PATTERN_K", morse_ary_K},
        {"ASCII88PATTERN_L", morse_ary_L},
        {"ASCII88PATTERN_M", morse_ary_M},
        {"ASCII88PATTERN_N", morse_ary_N},
        {"ASCII88PATTERN_O", morse_ary_O},
        {"ASCII88PATTERN_P", morse_ary_P},
        {"ASCII88PATTERN_Q", morse_ary_Q},
        {"ASCII88PATTERN_R", morse_ary_R},
        {"ASCII88PATTERN_S", morse_ary_S},
        {"ASCII88PATTERN_T", morse_ary_T},
        {"ASCII88PATTERN_U", morse_ary_U},
        {"ASCII88PATTERN_V", morse_ary_V},
        {"ASCII88PATTERN_W", morse_ary_W},
        {"ASCII88PATTERN_X", morse_ary_X},
        {"ASCII88PATTERN_Y", morse_ary_Y},
        {"ASCII88PATTERN_Z", morse_ary_Z},
        {"ASCII88PATTERN_0", morse_ary_0},
        {"ASCII88PATTERN_1", morse_ary_1},
        {"ASCII88PATTERN_2", morse_ary_2},
        {"ASCII88PATTERN_3", morse_ary_3},
        {"ASCII88PATTERN_4", morse_ary_4},
        {"ASCII88PATTERN_5", morse_ary_5},
        {"ASCII88PATTERN_6", morse_ary_6},
        {"ASCII88PATTERN_7", morse_ary_7},
        {"ASCII88PATTERN_8", morse_ary_8},
        {"ASCII88PATTERN_9", morse_ary_9},
        {"ASCII88PATTERN_DOT", morse_ary_DOT},
        {"ASCII88PATTERN_LB", morse_ary_LB},
        {"ASCII88PATTERN_RB", morse_ary_RB},
        {"ASCII88PATTERN_PLUS", morse_ary_PLUS},
        {"ASCII88PATTERN_SP", morse_ary_SP},
        {"ASCII88PATTERN_UQ", morse_ary_UQ},
        {"ASCII88PATTERN_COMA", morse_ary_COMA},
        {"ASCII88PATTERN_DASH", morse_ary_DASH},
        {"ASCII88PATTERN_EQ", morse_ary_EQ},
        {"ASCII88PATTERN_UEXC", morse_ary_UEXC},
        {"ASCII88PATTERN_QM", morse_ary_QM},
        {"ASCII88PATTERN_AND", morse_ary_AND},
        {"ASCII88PATTERN_DD", morse_ary_DD},
        {"ASCII88PATTERN_MONY", morse_ary_MONY},
        {"ASCII88PATTERN_EXC", morse_ary_EXC},
        {"ASCII88PATTERN_SQUT", morse_ary_SQUT},
        {"ASCII88PATTERN_COLN", morse_ary_COLN},
        {"ASCII88PATTERN_DQUT", morse_ary_DQUT},
        {"ASCII88PATTERN_AT", morse_ary_AT},
        {"ASCII88PATTERN_SLAH", morse_ary_SLAH},
    };
    for (size_t i = 0; i < ALL_MOS_PAT_NUM; i++)
    {
        cout << morse_ary[i].name() << endl;
        cout << print_int_ary(morse_ary[i].morse()) << endl;
    }
}