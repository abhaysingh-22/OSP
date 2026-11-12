
//process2

#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <sstream>

using namespace std;

int main()
{
    char message[1000];

    int fd1 = open("fifo1", O_RDONLY);

    int n = read(fd1, message, sizeof(message) - 1);

    message[n] = '\0';

    close(fd1);

    int characters = strlen(message);
    int words = 0;
    int lines = 0;

    // Count lines
    for(int i = 0; message[i] != '\0'; i++)
    {
        if(message[i] == '\n')
            lines++;
    }

    // Count words
    stringstream ss(message);
    string word;

    while(ss >> word)
    {
        words++;
    }

    // Write result to file
    ofstream file("result.txt");

    file << "Number of characters: " << characters << endl;
    file << "Number of words: " << words << endl;
    file << "Number of lines: " << lines << endl;

    file.close();

    // Read result file
    ifstream input("result.txt");

    string result;
    string line;

    while(getline(input, line))
    {
        result += line;
        result += '\n';
    }

    input.close();

    int fd2 = open("fifo2", O_WRONLY);

    write(fd2, result.c_str(), result.length() + 1);

    close(fd2);

    return 0;
}