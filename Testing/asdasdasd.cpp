#include <bits/stdc++.h>
using namespace std;

vector<int> solve(int N, vector<string> command)
{
    vector<int> a;
    stack<char> st;
    map<char, stack<string>> r;
    for (auto i : command)
    {
        string c = i;
        if ((c[0] == 'a'))
        {
            r[c[7]].push(c.substr(9, c.length() - 9));
            st.push(c[7]);
        }
        else if (c == "{")
            st.push('{');
        else if (c == "}")
        {
            while (!st.empty() && st.top() != '{')
            {
                r[st.top()].pop();
                st.pop();
            }
            st.pop();
        }
        else
        {
            if (!r[c[6]].empty() && r[c[6]] != 0)
                cout << r[c[6]].top() << endl;
            else
                cout << "undefined" << endl;
        }
    }
    return a;
}

int main()
{

    ios::sync_with_stdio(0);
    cin.tie(0);
    int N;
    cin >> N;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    vector<string> command(N);
    for (int i_command = 0; i_command < N; i_command++)
    {
        getline(cin, command[i_command]);
    }

    vector<int> out_;
    out_ = solve(N, command);
    cout << out_[0];
    for (int i_out_ = 1; i_out_ < out_.size(); i_out_++)
    {
        cout << "\n"
             << out_[i_out_];
    }
}
