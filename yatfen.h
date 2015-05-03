#ifndef YATFEN_H
#define YATFEN_H

#include <QtGui>
#include <QtWidgets>
#include "yatzee.h"

/**
 * Cette structure rassemble les caracteristiques du joueur
 */
struct Player
{
    QString name;
    int points;          // Nb de points du joueur
    int bonusPoints;     // Nb de points obtenus sans combinaison ni chance
    int remainingRounds; // Nb de coups encore a jouer
    bool bonusYatzee;    // S'il a eu un yatzee
};

/**
 * Classe definissant la GUI pour le yatzee :)
 */
class Yatfen : public QMainWindow
{
    Q_OBJECT

private:
    Yatzee myYatzee;
    Player myPlayer;
    QString myScoreFilename;
    QString myScoreDirname;

    // Cf dessin de Margaux pour comprendre... Et ma correction... (mais tout est perdu...)
    QAction* myActionQuitter;                            // Pour quitter
    QAction* myActionVoirScores;
    QAction* myActionEffacerScores;
    QWidget* myCentralWidget;
    QVBoxLayout* myLayout;
        QLabel* myLabelTotal;
        QHBoxLayout* myLayout1;
            QGridLayout* myLayout11;
                QGroupBox* myGBDes;
                    QVBoxLayout* myLayout111;            // Pr les des
                        QPushButton* myPBDes[5];         // Les des
                QGroupBox* myGBReserve;
                    QVBoxLayout* myLayout112;            // Pr la reserve
                        QPushButton* myPBReserve[5];     // La reserve
                QPushButton* myPBLancer;                 // Pr lancer
            QVBoxLayout* myLayout12;                     // Pr les boutons et les bonus
                QGroupBox* myGBChiffres;
                    QGridLayout* myLayout121;
                        QPushButton* myPBChiffres[6];    // Pr les boutons pr calculer les points
                        QLabel* myLabelBonusPoints;      // + 37 pts si 63 pts au niveau des points "points"
                QGroupBox* myGBCombinaison;
                    QGridLayout* myLayout122;
                        QPushButton* myPBCombinaison[6]; // Pr les combinaisons (indexes par les valeur de Combinaison)
                        QPushButton* myPBChance;
                        QLabel* myLabelBonusYatzee;      // + 50 pts si yatzee

private:
    void update();                   // Actualise les textes des des, de la reserve et du bouton lancer
    void reset();                    // RAZ du yatzee et MAJ des textes des des, de la reserve et du bouton lancer
    void updateScores();             // Actualise le texte indiquant le score
    void endOfGame();                // Gere la fin du jeu : message de fin et demande de si on veut rejouer
    QString int2points(int i) const; // Fait la liaison entre entier et nombre de points sur un de

public:
    Yatfen(QWidget* parent = nullptr);

public slots:
    void roll();           // Lance les des (utilise Yatzee::roll() puis update())
    void moveToReserve(int diceIdx);
    void moveToDices(int diceIdx);
    void computePoints(int number);
    void computeChance();
    void computeCombinaisonPoints(Combinaison combinaison);
    void seeScores();      // Voir les scores dans une boite de dialogue
    void deleteScores();   // Effacer le fichier des scores
};

#endif

