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
    //cell[56]=
    int newintary[] = {1, 2};
    cell tmp1{"abc", newintary};
    cout << tmp1.name() << endl;
    cout << print_int_ary(tmp1.morse()) << endl;

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
}