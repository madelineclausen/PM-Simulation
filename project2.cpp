#include "project2.hpp"
int PM[524288]; // 1024 frames, 512 words each

int D[1024][512];
std::list<int> free_frames;

void read_block(int b, int m)
{
    for (int i=0; i<512; i++)
    {
        PM[m+i] = D[std::abs(b)][i];
    }
}

void setup_PM()
{
    std::ifstream input_file("init-dp.txt"); // change name for no-dp
    if (input_file.is_open())
    {
        std::string input1; 
        std::string input2;
        std::getline(input_file, input1);
        std::getline(input_file, input2);
        const int length1 = input1.length();
        const int length2 = input2.length();
        char* char_input1 = new char[length1 + 1];
        char* char_input2 = new char[length2 + 1];
        strcpy(char_input1, input1.c_str());
        strcpy(char_input2, input2.c_str());
        init_ST(char_input1);
        init_PT(char_input2);
        input_file.close();
    }
}

void init_ST(char* char_input)
{    
    char* command1 = strtok(char_input, " \n\t");
    char* command2 = strtok(NULL, " \n\t");
    char* command3 = strtok(NULL, " \n\t");
    while(command1 != NULL)
    {  
        int si = atoi(command1);
        int zi = atoi(command2);
        int fi = atoi(command3);
        PM[2*si] = zi;
        PM[(2*si)+1] = fi;
        if (fi < 0)
        {
            for (int i=0; i<512; i++)
            {
                D[std::abs(fi)][i] = 0;
            }
        }
        free_frames.remove(std::abs(fi));
        command1 = strtok(NULL, " \n\t");
        command2 = strtok(NULL, " \n\t");
        command3 = strtok(NULL, " \n\t");
    }
}

void init_PT(char* char_input)
{
    char* command1 = strtok(char_input, " \n\t");
    char* command2 = strtok(NULL, " \n\t");
    char* command3 = strtok(NULL, " \n\t");
    while(command1 != NULL)
    {  
        int si = atoi(command1);
        int pi = atoi(command2);
        int fi = atoi(command3);
        PM[(std::abs(PM[(2*si)+1])*512)+pi] = fi; 
        if (fi < 0)
        {
            for (int i=0; i<512; i++)
            {
                D[std::abs(pi)][i] = 0;
            }
        } 
        free_frames.remove(std::abs(fi));
        command1 = strtok(NULL, " ");
        command2 = strtok(NULL, " ");
        command3 = strtok(NULL, " ");
    }

}

int get_s(int VA)
{
    return (VA >> 18);
}

int get_p(int VA)
{
    int p = VA >> 9;
    return p & (0b111111111);
}

int get_w(int VA)
{
    return VA & (0b111111111);
}

int get_pw(int VA)
{
    return VA & (0b111111111111111111);
}

void PT_not_res(int s)
{ 
    int f1 = free_frames.front();
    free_frames.pop_front();
    read_block(std::abs(PM[(2*s) + 1]), f1*512);
    PM[(2*s) + 1] = f1;
}

void page_not_res(int s, int p)
{
    int f1 = free_frames.front();
    free_frames.pop_front();
    read_block(std::abs(PM[std::abs(PM[(2*s) + 1]*512) + p]), f1*512);
    PM[std::abs(PM[(2*s) + 1])*512 + p] = f1;
}

bool check_valid(int VA, int s, int p)
{
    if (get_pw(VA) >= std::abs(PM[2*s]))
    {
        return false;
    }
    else 
    {
        for (int i; i<free_frames.size(); i++)
        {
            std::cout << free_frames[i] << " ";
        }
        if (PM[(2*s) + 1] < 0) 
        {
            PT_not_res(s);
            page_not_res(s, p);
        }
        if (PM[std::abs(PM[(2*s) + 1])*512 + p] < 0)
        {
            page_not_res(s, p);
        }
        return true;
    }
}

int get_PA(int VA)
{
    int s = get_s(VA);
    int p = get_p(VA);
    int w = get_w(VA);
    if (check_valid(VA, s, p))
    {
        return (PM[std::abs(PM[((2*s)+1)]*512)+p]*512) + w; 
    }
    else
    {
        return -1;
    }
}

void read_input()
{
    std::ofstream output_file;
    output_file.open("output-dp.txt"); // change name for no-dp
    std::ifstream input_file2("input-dp.txt"); // change name for no-dp
    std::string input;
    getline(input_file2, input);
    const int length = input.length();
    char* char_input = new char[length + 1];
    strcpy(char_input, input.c_str());
    char* command = strtok(char_input, " \n\t");
    while (command != NULL)
    {
        int VA = atoi(command);
        output_file << get_PA(VA);
        output_file << " ";
        command = strtok(NULL, " \n\t");
    }
    input_file2.close();
    output_file.close();
}

int main()
{
    for (int i=2; i<1024; i++)
    {
        free_frames.push_back(i);
    }
    setup_PM();
    read_input();
}