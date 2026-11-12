//process1

#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

int main()
{
    char message[1000];
    char result[1000];

    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    cout << "Enter sentences:" << endl;
    cout << "(Enter END on a new line to finish)" << endl;

    string input = "";
    string line;

    while(true)
    {
        getline(cin, line);

        if(line == "END")
            break;

        input += line;
        input += '\n';
    }

    int fd1 = open("fifo1", O_WRONLY);

    write(fd1, input.c_str(), input.length() + 1);

    close(fd1);

    int fd2 = open("fifo2", O_RDONLY);

    int n = read(fd2, result, sizeof(result) - 1);

    result[n] = '\0';

    close(fd2);

    cout << "\nResult received from Process 2:\n";
    cout << result << endl;

    return 0;
}

