#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <regex>
#include <windows.h>
#include <iomanip>
using namespace std;

//TODO: 人名标签 写笔记
struct IsmInfo
{
    string title;
    string url;
};
int main(int argc, char *argv[])
{
    map<string, IsmInfo> mIsm;
    ifstream fin;
    string line, ism, title, url;
    istringstream sin(istringstream::in);
    //read
    fin.open("D:/cmd/IsmBili", ifstream::in);
    while (getline(fin, line))
    {
        sin.str(line);
        sin >> ism >> title >> url;
        mIsm[ism] = {title, url};
        sin.clear();
        title.clear();
        url.clear();
    }

    if (argc == 1)
    {
        cout << "error: no args";
    }
    else
    {
        if (strcmp(argv[1], "-o") == 0) //open
        {
            if (argc < 3)
            {
                cout << "error: no input -ism(s)." << endl;
                return 1;
            }
            for (int i = 2; i != argc; ++i)
            {
                ism = string(argv[i]);
                auto it = mIsm.find(ism);
                if (it == mIsm.end())
                {
                    cout << "error: didn't found " << ism << endl;
                }
                else
                {
                    cout << "Opening " << it->second.title << endl;
                    string addr;
                    if (it->second.url[0] == 'B') //如果是bv号
                        (addr += "https://www.bilibili.com/video/") += it->second.url;
                    else //1-2-1中部分在硬盘里
                        addr = it->second.url;
                    ShellExecute(NULL, "open", addr.c_str(), NULL, NULL, SW_SHOWNORMAL);
                }
            }
        }
        else if (strcmp(argv[1], "-a") == 0) //add
        {
            ism.clear();
            cout << "Ism:";
            cin >> ism;
            cout << "title:";
            cin >> title;
            cout << "url:";
            cin >> url;
            if (mIsm.find(ism) != mIsm.end())
            {
                getchar();
                cout << "Already exist. Would you like to override? y/n:";
                char a = getchar();
                if (a != 'Y' && a != 'y')
                {
                    cout << "error: add failure" << endl;
                    return 0;
                }
            }
            ofstream fout("D:/cmd/IsmBili", ofstream::ate | ofstream::app);
            fout << ism << ' ' << title << ' ' << url << endl;
        }
        else if (strcmp(argv[1], "-l") == 0) //list
        {
            for (auto p : mIsm)
                cout << left << setw(12) << p.first << setw(60) << p.second.title << setw(24) << p.second.url << endl;
            cout << mIsm.size() << " entries in total.";
        }
        else if (strcmp(argv[1], "-f") == 0) //find
        {
            regex reg(argv[2]);
            for (auto p : mIsm)
                if (regex_match(p.first, reg))
                    cout << left << setw(12) << p.first << setw(60) << p.second.title << setw(24) << p.second.url << endl;
        }
        else if (strcmp(argv[1], "-m") == 0) //modify
        {
            if (argc == 2)
            {
                cout << "error: please specify an entry!" << endl;
                return 1;
            }
            auto it = mIsm.find(argv[2]);
            if (it == mIsm.end())
            {
                cout << "error: doesn't exist!";
                return 1;
            }
            title = it->second.title, url = it->second.url;
            cout << "title:";
            cin >> title;
            cout << "url:";
            cin >> url;
            it->second = {title, url};
            if(cin.bad())
            {
                cout << "Bad Input";
                return 1;
            }
            ofstream fout("D:/cmd/IsmBili");
            for (auto entry : mIsm)
            {
                fout << entry.first << ' ' << entry.second.title << ' ' << entry.second.url << endl;
            }
        }
        else if (strcmp(argv[1], "-h") == 0) //help
        {
            cout << "  -a \t\tadd\n"
                 << "  -m [ism]...\tmodify\n"
                 << "  -o [ism]...\topen\n"
                 << "  -f [regex]\tfind\n"
                 << "  -l \t\tlist\n"
                 << "  -h \t\thelp";
        }
        else //default
        {
            cout << "error: wrong args\n"
                 << "  -h help";
        }
    }
    return 0;
}