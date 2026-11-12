#include <iostream>
using namespace std;

int main()
{
    int n, m;

    cout << "Enter number of processes: ";
    cin >> n;

    cout << "Enter number of resources: ";
    cin >> m;

    int allocation[10][10];
    int maxNeed[10][10];
    int need[10][10];
    int available[10];
    int work[10];
    int finish[10] = {0};
    int safeSequence[10];

    // Input Allocation Matrix
    cout << "\nEnter Allocation Matrix:\n";

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            cin >> allocation[i][j];
        }
    }

    // Input Maximum Matrix
    cout << "\nEnter Maximum Matrix:\n";

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            cin >> maxNeed[i][j];
        }
    }

    // Calculate Need Matrix
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            need[i][j] = maxNeed[i][j] - allocation[i][j];
        }
    }

    // Input Available Resources
    cout << "\nEnter Available Resources:\n";

    for(int j = 0; j < m; j++)
    {
        cin >> available[j];
        work[j] = available[j];
    }

    int count = 0;

    // Find Safe Sequence
    while(count < n)
    {
        bool found = false;

        for(int i = 0; i < n; i++)
        {
            if(finish[i] == 0)
            {
                bool possible = true;

                for(int j = 0; j < m; j++)
                {
                    if(need[i][j] > work[j])
                    {
                        possible = false;
                        break;
                    }
                }

                if(possible)
                {
                    for(int j = 0; j < m; j++)
                    {
                        work[j] = work[j] + allocation[i][j];
                    }

                    safeSequence[count] = i;
                    finish[i] = 1;
                    count++;
                    found = true;
                }
            }
        }

        if(!found)
        {
            break;
        }
    }

    // Display Need Matrix
    cout << "\nNeed Matrix:\n";

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            cout << need[i][j] << " ";
        }
        cout << endl;
    }

    // Check Safe State
    if(count == n)
    {
        cout << "\nSystem is in SAFE state.\n";

        cout << "Safe Sequence: ";

        for(int i = 0; i < n; i++)
        {
            cout << "P" << safeSequence[i];

            if(i != n - 1)
                cout << " -> ";
        }

        cout << endl;
    }
    else
    {
        cout << "\nSystem is NOT in a safe state.\n";
    }

    return 0;
}


// Enter number of processes: 5
// Enter number of resources: 3

// Enter Allocation Matrix:
// 0 1 0
// 2 0 0
// 3 0 2
// 2 1 1
// 0 0 2

// Enter Maximum Matrix:
// 7 5 3
// 3 2 2
// 9 0 2
// 2 2 2
// 4 3 3

// Enter Available Resources:
// 3 3 2