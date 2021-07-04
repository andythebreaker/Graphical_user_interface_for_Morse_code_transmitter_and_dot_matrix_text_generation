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
#define MORSE_ARRAY_TO_VECTOR(MORSE_ARRAY_INPUT, MORSE_ARRAY_NNAME) vector<int> MORSE_ARRAY_NNAME(MORSE_ARRAY_INPUT, MORSE_ARRAY_INPUT + sizeof(MORSE_ARRAY_INPUT) / sizeof(MORSE_ARRAY_INPUT[0]))

using namespace std;

class cell
{
public:
    // Default constructor
    cell() {}

    // Initialize a Box with custom dimensions
    cell(string name, vector<int> morse)
        : m_name(name), m_morse(morse)
    {
    }

    string name(void) { return m_name; };
    vector<int> morse(void) { return m_morse; }

private:
    // Will have value of 0 when default constructor is called.
    // If we didn't zero-init here, default constructor would
    // leave them uninitialized with garbage values.
    string m_name{NULL};
    vector<int> m_morse{NULL};
};

// 宣告類別
class tree_node
{
    // 宣告 public 成員
public:
    tree_node()
    {
        numb = count;
        count++;
    }

    static int count;
    int numb;
    tree_node *dot;
    tree_node *dash;
    vector<cell> endof;
};
int tree_node::count = 0;

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

string int_vec_2_str(vector<int> int_vec)
{
    string str_out = "";
    vector<int>::iterator it;
    for (it = int_vec.begin(); it != int_vec.end(); it++)
    {
        str_out += to_string(*it) + ", ";
    }
    return str_out;
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

    MORSE_ARRAY_TO_VECTOR(morse_ary_A, NNAME_morse_ary_A);
    MORSE_ARRAY_TO_VECTOR(morse_ary_B, NNAME_morse_ary_B);
    MORSE_ARRAY_TO_VECTOR(morse_ary_C, NNAME_morse_ary_C);
    MORSE_ARRAY_TO_VECTOR(morse_ary_D, NNAME_morse_ary_D);
    MORSE_ARRAY_TO_VECTOR(morse_ary_E, NNAME_morse_ary_E);
    MORSE_ARRAY_TO_VECTOR(morse_ary_F, NNAME_morse_ary_F);
    MORSE_ARRAY_TO_VECTOR(morse_ary_G, NNAME_morse_ary_G);
    MORSE_ARRAY_TO_VECTOR(morse_ary_H, NNAME_morse_ary_H);
    MORSE_ARRAY_TO_VECTOR(morse_ary_I, NNAME_morse_ary_I);
    MORSE_ARRAY_TO_VECTOR(morse_ary_J, NNAME_morse_ary_J);
    MORSE_ARRAY_TO_VECTOR(morse_ary_K, NNAME_morse_ary_K);
    MORSE_ARRAY_TO_VECTOR(morse_ary_L, NNAME_morse_ary_L);
    MORSE_ARRAY_TO_VECTOR(morse_ary_M, NNAME_morse_ary_M);
    MORSE_ARRAY_TO_VECTOR(morse_ary_N, NNAME_morse_ary_N);
    MORSE_ARRAY_TO_VECTOR(morse_ary_O, NNAME_morse_ary_O);
    MORSE_ARRAY_TO_VECTOR(morse_ary_P, NNAME_morse_ary_P);
    MORSE_ARRAY_TO_VECTOR(morse_ary_Q, NNAME_morse_ary_Q);
    MORSE_ARRAY_TO_VECTOR(morse_ary_R, NNAME_morse_ary_R);
    MORSE_ARRAY_TO_VECTOR(morse_ary_S, NNAME_morse_ary_S);
    MORSE_ARRAY_TO_VECTOR(morse_ary_T, NNAME_morse_ary_T);
    MORSE_ARRAY_TO_VECTOR(morse_ary_U, NNAME_morse_ary_U);
    MORSE_ARRAY_TO_VECTOR(morse_ary_V, NNAME_morse_ary_V);
    MORSE_ARRAY_TO_VECTOR(morse_ary_W, NNAME_morse_ary_W);
    MORSE_ARRAY_TO_VECTOR(morse_ary_X, NNAME_morse_ary_X);
    MORSE_ARRAY_TO_VECTOR(morse_ary_Y, NNAME_morse_ary_Y);
    MORSE_ARRAY_TO_VECTOR(morse_ary_Z, NNAME_morse_ary_Z);
    MORSE_ARRAY_TO_VECTOR(morse_ary_0, NNAME_morse_ary_0);
    MORSE_ARRAY_TO_VECTOR(morse_ary_1, NNAME_morse_ary_1);
    MORSE_ARRAY_TO_VECTOR(morse_ary_2, NNAME_morse_ary_2);
    MORSE_ARRAY_TO_VECTOR(morse_ary_3, NNAME_morse_ary_3);
    MORSE_ARRAY_TO_VECTOR(morse_ary_4, NNAME_morse_ary_4);
    MORSE_ARRAY_TO_VECTOR(morse_ary_5, NNAME_morse_ary_5);
    MORSE_ARRAY_TO_VECTOR(morse_ary_6, NNAME_morse_ary_6);
    MORSE_ARRAY_TO_VECTOR(morse_ary_7, NNAME_morse_ary_7);
    MORSE_ARRAY_TO_VECTOR(morse_ary_8, NNAME_morse_ary_8);
    MORSE_ARRAY_TO_VECTOR(morse_ary_9, NNAME_morse_ary_9);
    MORSE_ARRAY_TO_VECTOR(morse_ary_DOT, NNAME_morse_ary_DOT);
    MORSE_ARRAY_TO_VECTOR(morse_ary_LB, NNAME_morse_ary_LB);
    MORSE_ARRAY_TO_VECTOR(morse_ary_RB, NNAME_morse_ary_RB);
    MORSE_ARRAY_TO_VECTOR(morse_ary_PLUS, NNAME_morse_ary_PLUS);
    MORSE_ARRAY_TO_VECTOR(morse_ary_SP, NNAME_morse_ary_SP);
    MORSE_ARRAY_TO_VECTOR(morse_ary_UQ, NNAME_morse_ary_UQ);
    MORSE_ARRAY_TO_VECTOR(morse_ary_COMA, NNAME_morse_ary_COMA);
    MORSE_ARRAY_TO_VECTOR(morse_ary_DASH, NNAME_morse_ary_DASH);
    MORSE_ARRAY_TO_VECTOR(morse_ary_EQ, NNAME_morse_ary_EQ);
    MORSE_ARRAY_TO_VECTOR(morse_ary_UEXC, NNAME_morse_ary_UEXC);
    MORSE_ARRAY_TO_VECTOR(morse_ary_QM, NNAME_morse_ary_QM);
    MORSE_ARRAY_TO_VECTOR(morse_ary_AND, NNAME_morse_ary_AND);
    MORSE_ARRAY_TO_VECTOR(morse_ary_DD, NNAME_morse_ary_DD);
    MORSE_ARRAY_TO_VECTOR(morse_ary_MONY, NNAME_morse_ary_MONY);
    MORSE_ARRAY_TO_VECTOR(morse_ary_EXC, NNAME_morse_ary_EXC);
    MORSE_ARRAY_TO_VECTOR(morse_ary_SQUT, NNAME_morse_ary_SQUT);
    MORSE_ARRAY_TO_VECTOR(morse_ary_COLN, NNAME_morse_ary_COLN);
    MORSE_ARRAY_TO_VECTOR(morse_ary_DQUT, NNAME_morse_ary_DQUT);
    MORSE_ARRAY_TO_VECTOR(morse_ary_AT, NNAME_morse_ary_AT);
    MORSE_ARRAY_TO_VECTOR(morse_ary_SLAH, NNAME_morse_ary_SLAH);

    cell morse_ary[ALL_MOS_PAT_NUM] = {
        {"ASCII88PATTERN_A", NNAME_morse_ary_A},
        {"ASCII88PATTERN_B", NNAME_morse_ary_B},
        {"ASCII88PATTERN_C", NNAME_morse_ary_C},
        {"ASCII88PATTERN_D", NNAME_morse_ary_D},
        {"ASCII88PATTERN_E", NNAME_morse_ary_E},
        {"ASCII88PATTERN_F", NNAME_morse_ary_F},
        {"ASCII88PATTERN_G", NNAME_morse_ary_G},
        {"ASCII88PATTERN_H", NNAME_morse_ary_H},
        {"ASCII88PATTERN_I", NNAME_morse_ary_I},
        {"ASCII88PATTERN_J", NNAME_morse_ary_J},
        {"ASCII88PATTERN_K", NNAME_morse_ary_K},
        {"ASCII88PATTERN_L", NNAME_morse_ary_L},
        {"ASCII88PATTERN_M", NNAME_morse_ary_M},
        {"ASCII88PATTERN_N", NNAME_morse_ary_N},
        {"ASCII88PATTERN_O", NNAME_morse_ary_O},
        {"ASCII88PATTERN_P", NNAME_morse_ary_P},
        {"ASCII88PATTERN_Q", NNAME_morse_ary_Q},
        {"ASCII88PATTERN_R", NNAME_morse_ary_R},
        {"ASCII88PATTERN_S", NNAME_morse_ary_S},
        {"ASCII88PATTERN_T", NNAME_morse_ary_T},
        {"ASCII88PATTERN_U", NNAME_morse_ary_U},
        {"ASCII88PATTERN_V", NNAME_morse_ary_V},
        {"ASCII88PATTERN_W", NNAME_morse_ary_W},
        {"ASCII88PATTERN_X", NNAME_morse_ary_X},
        {"ASCII88PATTERN_Y", NNAME_morse_ary_Y},
        {"ASCII88PATTERN_Z", NNAME_morse_ary_Z},
        {"ASCII88PATTERN_0", NNAME_morse_ary_0},
        {"ASCII88PATTERN_1", NNAME_morse_ary_1},
        {"ASCII88PATTERN_2", NNAME_morse_ary_2},
        {"ASCII88PATTERN_3", NNAME_morse_ary_3},
        {"ASCII88PATTERN_4", NNAME_morse_ary_4},
        {"ASCII88PATTERN_5", NNAME_morse_ary_5},
        {"ASCII88PATTERN_6", NNAME_morse_ary_6},
        {"ASCII88PATTERN_7", NNAME_morse_ary_7},
        {"ASCII88PATTERN_8", NNAME_morse_ary_8},
        {"ASCII88PATTERN_9", NNAME_morse_ary_9},
        {"ASCII88PATTERN_DOT", NNAME_morse_ary_DOT},
        {"ASCII88PATTERN_LB", NNAME_morse_ary_LB},
        {"ASCII88PATTERN_RB", NNAME_morse_ary_RB},
        {"ASCII88PATTERN_PLUS", NNAME_morse_ary_PLUS},
        {"ASCII88PATTERN_SP", NNAME_morse_ary_SP},
        {"ASCII88PATTERN_UQ", NNAME_morse_ary_UQ},
        {"ASCII88PATTERN_COMA", NNAME_morse_ary_COMA},
        {"ASCII88PATTERN_DASH", NNAME_morse_ary_DASH},
        {"ASCII88PATTERN_EQ", NNAME_morse_ary_EQ},
        {"ASCII88PATTERN_UEXC", NNAME_morse_ary_UEXC},
        {"ASCII88PATTERN_QM", NNAME_morse_ary_QM},
        {"ASCII88PATTERN_AND", NNAME_morse_ary_AND},
        {"ASCII88PATTERN_DD", NNAME_morse_ary_DD},
        {"ASCII88PATTERN_MONY", NNAME_morse_ary_MONY},
        {"ASCII88PATTERN_EXC", NNAME_morse_ary_EXC},
        {"ASCII88PATTERN_SQUT", NNAME_morse_ary_SQUT},
        {"ASCII88PATTERN_COLN", NNAME_morse_ary_COLN},
        {"ASCII88PATTERN_DQUT", NNAME_morse_ary_DQUT},
        {"ASCII88PATTERN_AT", NNAME_morse_ary_AT},
        {"ASCII88PATTERN_SLAH", NNAME_morse_ary_SLAH},
    };

    int max_step_numb = 0;

    for (size_t i = 0; i < ALL_MOS_PAT_NUM; i++)
    {
        cout << morse_ary[i].name() << endl;
        cout << int_vec_2_str(morse_ary[i].morse()) << endl;
        max_step_numb = (morse_ary[i].morse().size() > max_step_numb) ? morse_ary[i].morse().size() : max_step_numb;
    }
    cout << "\t\tmax_step_numb:" << to_string(max_step_numb) << endl;

    double length_of_all_tree_node = 0;
    double pause_point = 0;
    for (size_t i = 0; i < max_step_numb; i++)
    {
        length_of_all_tree_node += pow((double)2, (double)i);
        if (i + 2 == max_step_numb)
        {
            pause_point = length_of_all_tree_node;
        }
    }

    cout << "\t\tlength_of_all_tree_node:" << to_string(length_of_all_tree_node) << endl;
    cout << "\t\tpause_point:" << to_string(pause_point) << endl;

    vector<tree_node> tree_all;

    for (double i = 0; i < length_of_all_tree_node; i++)
    {
        tree_all.push_back(tree_node());
    }
    for (double i = 0; i < pause_point; i++)
    {
        tree_all[int(i)].dot = &tree_all[(int(i) + 1) * 2 - 1];
        tree_all[int(i)].dash = &tree_all[(int(i) + 1) * 2];
    }
    for (double i = pause_point; i < length_of_all_tree_node; i++)
    {
        tree_all[int(i)].dot = NULL;
        tree_all[int(i)].dash = NULL;
    }

    for (size_t i = 0; i < ALL_MOS_PAT_NUM; i++)
    {
        tree_node *tmp_tn = &tree_all[0];
        cout << "pattern:" + to_string(i);
        for (size_t ii = 0; ii < max_step_numb; ii++)
        {
            cout << ", " + to_string(morse_ary[i].morse()[ii]);
            if (morse_ary[i].morse()[ii] == 1)
            {
                cout << "[dot](" << to_string((*tmp_tn).numb);
                tmp_tn = tmp_tn->dot;
                cout << "->" << to_string((*tmp_tn).numb) << ")";
            }
            else if (morse_ary[i].morse()[ii] == 2)
            {
                cout << "[dash](" << to_string((*tmp_tn).numb);
                tmp_tn = tmp_tn->dash;
                cout << "->" << to_string((*tmp_tn).numb) << ")";
            }
            else
            {
                cout << "[endof](" << to_string((*tmp_tn).numb);
                (*tmp_tn).endof.push_back(morse_ary[i]);
                cout << "->" << to_string((*tmp_tn).numb) << ")";

                break;
            }
        }
        cout << endl;
    }
    string main_output = "";

    for (double i = 0; i < length_of_all_tree_node; i++)
    {
        main_output += "case " + to_string(tree_all[int(i)].numb) + ":\nif(morse_pattern_status==1){\n" + ((tree_all[int(i)].dot) ? ("morse_pattern_status=" + to_string((*(tree_all[int(i)].dot)).numb) + ";") : "target_morse_pattern_error_event") +
                       "\n}else if(morse_pattern_status==1){\n" + ((tree_all[int(i)].dash) ? ("morse_pattern_status=" + to_string((*(tree_all[int(i)].dash)).numb) + ";") : "target_morse_pattern_error_event") +
                       "\n}else{\n" +
                       ((tree_all[int(i)].endof.size() > 0) ? ("SCREEN_SHOW_FRAM(" + (*(tree_all[int(i)].endof.begin())).name() + ")") : "target_morse_pattern_error_event") + "\n}break;";
    }
    cout << "================================================================" << endl;
    cout << main_output << endl;
}