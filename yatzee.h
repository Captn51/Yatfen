#ifndef YATZEE_H
#define YATZEE_H

#include <QList>
#include <QMap>

/**
 * Cette enumeration contient les differentes combinaisons possibles
 * au yatzee.
 */
enum Combinaison
{
    BRELAN,
    CARRE,
    FULL,
    PTITE_SUITE,
    GRANDE_SUITE,
    YATZEE
};

/**
 * Declaration bien pratique...
 */
using Combinaisons = QList<Combinaison>;

/**
 * Cette classe implemente la couche "metier" du programme ; la GUI est
 * dans yatfen.h/cpp.
 */
class Yatzee
{
private:
    QList<int> myDices;
    QList<int> myReserve;
    int myRemainingRolls;   /// Nombre de lances max par joueur.

public:
    static const int bonusYatzee;
    static const int bonusPoints;
    static const int pointsMinForBonusPoints;
    static const QMap<Combinaison, int> combinaison2points;

public:
    Yatzee();       // Il faut faire un qsrand() en debut de main !!
    Yatzee(const Yatzee& y) = delete;
    Yatzee(Yatzee&& y) = delete;
    Yatzee& operator=(Yatzee y) = delete;

    const QList<int>& dices() const;
    const QList<int>& reserve() const;
    int remainingRolls() const;
    void print() const;
    void reset();                            /// Remet les des, la reserve et les lances comme a la creation du yatzee.
    bool moveToReserve(int diceIdx);
    bool moveToDices(int diceIdx);
    bool roll();                             /// Relance les valeurs de myDices.
    Combinaisons findCombinaisons() const;   /// Regarde quelles combinaisons sont presentes.
    int chance() const;                      /// Calcule la chance.
    int points(int num) const;               /// Calcule le nombre de points pour le de de valeur num (-1 si num pas bon).
};

#endif

