#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <windows.h>
using namespace std;

//TODO: search name/tag note
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
    while(getline(fin,line))
    {
        sin.str(line);
        sin >> ism >> title >> url;
        mIsm[ism] = {title,url};
        sin.clear();
        url.clear();
    }
    
    if(argc==1)
    {
        cout<<"error: no args\n";
    }
    else
    {
        if(strcmp(argv[1],"-o")==0) //open
        {
            if(argc<3)
            {
                cout<<"no input -ism(s)."<<endl;
                return 1;
            }
            for(int i = 2; i != argc; ++i)
            {
                ism = string(argv[i]);
                auto it = mIsm.find(ism);
                if(it==mIsm.end())
                {
                    cout<<"Didn't found "<<ism<<endl;
                }
                else
                {
                    cout<<"Opening "<<it->second.title<<endl;
                    ShellExecute(NULL, "open", it->second.url.c_str(), NULL, NULL, SW_SHOWNORMAL);
                }
            }
        }
        else if(strcmp(argv[1],"-a")==0) //add
        {
            cout<<"Ism:";
            cin>>ism;
            cout<<"title:";
            cin>>title;
            cout<<"url:";
            cin>>url;
            if(mIsm.find(ism)!=mIsm.end())
            {
                cout<<"Already exist. Would you like to override? y/n";
                char a = getchar();
                if(a!='Y'&&a!='y')
                {
                    cout<<"Add failure"<<endl;
                    return 0;
                }
            }
            ofstream fout("D:/cmd/IsmBili",ofstream::ate|ofstream::app);
            fout<<ism<<' '<<title<<' '<<url<<endl;
        }
        else if(strcmp(argv[1],"-l")==0) //list
        {
            for(auto p : mIsm)
                cout<<p.first<<' '<<p.second.title<<' '<<p.second.url<<endl;
            cout<<mIsm.size()<<" entries in total."<<endl;
        }
        else if(strcmp(argv[1],"-h")==0) //help
        {
            cout<<"  -o [ism]...\topen\n"<<"  -a \t\tadd\n"<<"  -l \t\tlist\n"<<"  -h \t\thelp\n";
        }
        else //default
        {
            cout<<"error: wrong args\n"
                <<"  -h help"<<endl;
        }
    }
    
    return 0;
}