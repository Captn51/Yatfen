#include "yatzee.h"

#include <iostream>
#include <algorithm>

using namespace std;

const int Yatzee::BONUS_YATZEE{100};
const int Yatzee::BONUS_POINTS{37};
const int Yatzee::POINTS_MIN_FOR_BONUS_POINTS{63};
const QMap<Combinaison, int> Yatzee::COMBINAISON_TO_POINTS{ {BRELAN, 15},
                                                            {CARRE, 40},
                                                            {FULL, 25},
                                                            {PTITE_SUITE, 30},
                                                            {GRANDE_SUITE, 40},
                                                            {YATZEE, 50} };

Yatzee::Yatzee()
{
    for(int i = 0; i < 5; i++)
        myDices.push_back(0);  // Initialisation de la premiere serie de des (a zero)
    myRemainingRolls = 3;
}

const QList<int>& Yatzee::dices() const {return myDices;}
const QList<int>& Yatzee::reserve() const {return myReserve;}
int Yatzee::remainingRolls() const {return myRemainingRolls;}

void Yatzee::print() const
{
    cout << myDices.size() << " des :";
    for(int dice : myDices)
        cout << " " << dice;
    cout << endl;
    cout << myReserve.size() << " reserve :";
    for(int dice : myReserve)
        cout << " " << dice;
    cout << endl;
    cout << myRemainingRolls << " lances restants.";
    cout << endl;
}

void Yatzee::reset()
{
    myReserve.clear();
    myDices.clear();
    for(int i = 0; i < 5; i++)
        myDices.push_back(0);
    myRemainingRolls = 3;
}

bool Yatzee::roll()
{
    // Si on peut lancer...
    if(myRemainingRolls > 0)
    {
        // On donne une valeur aleatoire pr les des presents ds myDices
        for(int& dice : myDices)
            dice = qrand() % 6 + 1;

        myRemainingRolls--;

        return true;
    }
    else
    {
        return false;
    }
}

bool Yatzee::moveToReserve(int diceIdx)
{
    // Si diceIdx est bien un n° de de valide (diceIdx est ds [1, 6])
    if(diceIdx >= 1 && diceIdx <= myDices.size())
    {
        myReserve.push_back(myDices[diceIdx-1]);   // On ajoute le de n° diceIdx a la fin de myReserve
        myDices.removeAt(diceIdx-1);               // On le sort de myDices
        return true;
    }
    else
    {
        return false;
    }
}

bool Yatzee::moveToDices(int diceIdx)
{
    // Si diceIdx est bien un n° de reserve valide (diceIdx est ds [1, 6])
    if(diceIdx >= 1 && diceIdx <= myReserve.size())
    {
        myDices.push_back(myReserve[diceIdx-1]);   // On ajoute le de n° diceIdx a la fin de myDices
        myReserve.removeAt(diceIdx-1);             // On le sort de myReserve
        return true;
    }
    else
    {
        return false;
    }
}

Combinaisons Yatzee::findCombinaisons() const
{
    Combinaisons v;

    QList<int> tmp{myDices + myReserve}; // On copie ts les des (myDices et myReserve)
    tmp.removeAll(0);                    // On vire les 0
    qSort(tmp.begin(), tmp.end());       // On trie

    // On regarde les combinaisons possibles (il y a maitenant 5 elements TRIES, eventuellement
    // egaux pr certains, ds tmp)
    // Attention !! Il faut verifier que tmp est non vide sinon full, brelan, carre et yatzee sont trouves...
    if(!tmp.empty() && (tmp[0]==tmp[2] || tmp[1]==tmp[3] || tmp[2]==tmp[4]))
    {
        v.push_back(BRELAN);
    }
    if(!tmp.empty() && (tmp[0]==tmp[3] || tmp[1]==tmp[4]))
    {
        v.push_back(CARRE);
    }
    if(!tmp.empty() && (tmp[0] == tmp[4]))
    {
        v.push_back(YATZEE);
    }
    if(!tmp.empty() && ((tmp[0]==tmp[2] && tmp[3]==tmp[4]) || (tmp[0]==tmp[1] && tmp[2]==tmp[4])))
    {
        v.push_back(FULL);
    }

    // On vire les doublons pour voir les suites (passage oblige par la STL)
    list<int> stdtmp = tmp.toStdList();
    stdtmp.unique();
    tmp = QList<int>::fromStdList(stdtmp);

    // Inutile de regarder les suites si le nb de des ds tmp est trop petit
    if(tmp.size() == 4 &&    // On ne peut avoir qu'une petite suite...
       tmp[0]+3 == tmp[3])   // ...et on est certain que les indices st valides
    {
        v.push_back(PTITE_SUITE);
    }

    // On peut avoir les deux suites
    if(tmp.size() == 5)
    {
        if((tmp[0]+1==tmp[1] && tmp[1]+1==tmp[2] && tmp[2]+1==tmp[3]) ||
           (tmp[1]+1==tmp[2] && tmp[2]+1==tmp[3] && tmp[3]+1==tmp[4]))
        {
            v.push_back(PTITE_SUITE);
        }

        if(tmp[0]+4 == tmp[4])
            v.push_back(GRANDE_SUITE);
    }

    return v;
}

int Yatzee::chance() const
{
    int res = 0;
    for(int dice : myReserve)
        res += dice;
    for(int dice : myDices)
        res += dice;
    return res;
}

int Yatzee::points(int num) const
{
    if(num >= 1 && num <= 6)
    {
        int res = 0;

        for(int dice : myReserve)
        {
            if(dice == num)
            {
                res += dice;
            }
        }

        for(int dice : myDices)
        {
            if(dice == num)
            {
                res += dice;
            }
        }

        return res;
    }
    else
    {
        return -1;
    }
}

