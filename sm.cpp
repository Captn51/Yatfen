#include "sm.h"

#include <set>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

namespace ScoreManager {

struct ScoreInfo
{
    int myScore;
    string myName;
    string myDate;

    ScoreInfo(int score, const string& name, const string& date):
        myScore{score},
        myName{name},
        myDate{date}
    {}

    bool operator<(const ScoreInfo& scoreInfos) const
    {
        // Vu le format de la date, aucune chance pour que deux ScoreInfo soient egaux
        if(myScore < scoreInfos.myScore)
        {
            return true;
        }
        else if(myScore == scoreInfos.myScore)
        {
            if(myName.compare(scoreInfos.myName) < 0)
            {
                return true;
            }
            else if(myName.compare(scoreInfos.myName) == 0)
            {
                if(myDate.compare(scoreInfos.myDate) < 0)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
};

bool saveScore(const string& filename, int score, const string& name, const string& date)
{
    ScoreInfo scoreInfo{score, name, date};
    set<ScoreInfo> mySet;

    ifstream fileIn{filename};
    if(!fileIn.is_open())
        return false;

    // Recuperation des anciens scores
    string line;
    int tmpScore{0};
    string tmp;
    string tmpName;
    string tmpDate;
    while(fileIn)
    {
        getline(fileIn, line);
        if(line.empty())
            continue;

        istringstream iss{line};
        getline(iss, tmp, '|');
        tmpScore = atoi(tmp.c_str());
        getline(iss, tmpName, '|');
        iss >> tmpDate;

        mySet.insert(ScoreInfo{tmpScore, tmpName, tmpDate});
    }
    fileIn.close();

    // On ajoute le dernier score
    mySet.insert(scoreInfo);

    ofstream fileOut{filename, ios::trunc};
    if(!fileOut.is_open())
        return false;

    // On reecrit dans le fichier seulement les 20 meilleurs
    int i{0};
    auto it = mySet.rbegin();
    for(; i < 20 && it != mySet.rend(); it++, i++)
    {
        fileOut << it->myScore << "|" << it->myName << "|" << it->myDate << endl;
    }

    return true;
}

string loadScores(const string& filename)
{
    ifstream file{filename};
    if(!file.is_open())
        return string{};

    ostringstream oss;
    string tmp;
    while(file)
    {
        getline(file, tmp);
        oss << tmp << endl;
    }

    return oss.str();
}

}

