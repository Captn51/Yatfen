#include "yatfen.h"
#include "sm.h"

Yatfen::Yatfen(QWidget* parent):
    QMainWindow{parent},
    myScoreFilename{"DossierYatfen/scores.yat"},
    myScoreDirname{"DossierYatfen"}
{
    /***********************************
    * Intro + initialisation du joueur *
    ***********************************/

    QMessageBox::information(this, "Yatfen : Accueil", "Bienvenue dans le jeu du YATFEN !!");

    bool ok = false;
    do
    {
        myPlayer.name = QInputDialog::getText(this, "Yatfen : Joueur", "Qui es-tu ?", QLineEdit::Normal, QString(), &ok);
    } while(!ok);
    if(myPlayer.name.isEmpty())
        myPlayer.name = "Anne Onime"; // Craquage...
    myPlayer.points = 0;
    myPlayer.bonusPoints = 0;
    myPlayer.remainingRounds = 13;
    myPlayer.bonusYatzee = false;

    /**********************************************
    * Mise en place de la structure de la fenetre *
    **********************************************/

    // Mise en place des menus
    QMenu* menu = menuBar()->addMenu("Menu");
    myActionQuitter = menu->addAction("Quitter");
    myActionQuitter->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    myActionVoirScores = menu->addAction("Voir les scores");
    myActionVoirScores->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    myActionEffacerScores = menu->addAction("Effacer les scores");

    // Mise en place du groupbox des des
    for(int i = 0; i < 5; i++)
        myPBDes[i] = new QPushButton("   \n   \n   ");
    myLayout111 = new QVBoxLayout;
    myGBDes = new QGroupBox("DES");
    myGBDes->setAlignment(Qt::AlignHCenter);
    for(int i = 0; i < 5; i++)
        myLayout111->addWidget(myPBDes[i]);
    myGBDes->setLayout(myLayout111);

    // Mise en place du groupbox de la reserve
    for(int i = 0; i < 5; i++)
        myPBReserve[i] = new QPushButton("   \n   \n   ");
    myLayout112 = new QVBoxLayout;
    myGBReserve = new QGroupBox("RESERVE");
    myGBReserve->setAlignment(Qt::AlignHCenter);
    for(int i = 0; i < 5; i++)
        myLayout112->addWidget(myPBReserve[i]);
    myGBReserve->setLayout(myLayout112);

    // Mise en place du layout des + reserve + lancer
    myPBLancer = new QPushButton;
    myPBLancer->setText("Lancer ("+QString::number(myYatzee.remainingRolls())+")");
    myLayout11 = new QGridLayout;
    myLayout11->addWidget(myGBDes, 0, 0, 6, 1);
    myLayout11->addWidget(myGBReserve, 0, 1, 6, 1);
    myLayout11->addWidget(myPBLancer, 7, 0, 1, 2);

    // Mise en place du groupbox "chiffres" + bonus
    for(int i = 0; i < 6; i++)
        myPBChiffres[i] = new QPushButton(int2points(i+1));
    myLabelBonusPoints = new QLabel;
    myLabelBonusPoints->setAlignment(Qt::AlignHCenter);
    myLayout121 = new QGridLayout;
    myLayout121->addWidget(myPBChiffres[0], 0, 0);
    myLayout121->addWidget(myPBChiffres[1], 0, 1);
    myLayout121->addWidget(myPBChiffres[2], 0, 2);
    myLayout121->addWidget(myPBChiffres[3], 1, 0);
    myLayout121->addWidget(myPBChiffres[4], 1, 1);
    myLayout121->addWidget(myPBChiffres[5], 1, 2);
    myLayout121->addWidget(myLabelBonusPoints, 2, 0, 1, 3);
    myGBChiffres = new QGroupBox("CHIFFRES");
    myGBChiffres->setAlignment(Qt::AlignHCenter);
    myGBChiffres->setLayout(myLayout121);

    // Mise en place du groupbox "combinaisons" + bonus
    myPBCombinaison[BRELAN] = new QPushButton("3X");
    myPBCombinaison[CARRE] = new QPushButton("4X");
    myPBCombinaison[FULL] = new QPushButton("FULL");
    myPBCombinaison[PTITE_SUITE] = new QPushButton("PTITE SUITE");
    myPBCombinaison[GRANDE_SUITE] = new QPushButton("GRANDE SUITE");
    myPBCombinaison[YATZEE] = new QPushButton("YATZEE");
    myPBChance = new QPushButton("CHANCE");
    myLabelBonusYatzee = new QLabel;
    myLabelBonusYatzee->setAlignment(Qt::AlignHCenter);
    myLayout122 = new QGridLayout;
    myLayout122->addWidget(myPBCombinaison[BRELAN], 0, 0);
    myLayout122->addWidget(myPBCombinaison[CARRE], 0, 1);
    myLayout122->addWidget(myPBCombinaison[FULL], 0, 2);
    myLayout122->addWidget(myPBCombinaison[PTITE_SUITE], 1, 0);
    myLayout122->addWidget(myPBCombinaison[GRANDE_SUITE], 1, 1);
    myLayout122->addWidget(myPBChance, 1, 2);
    myLayout122->addWidget(myPBCombinaison[YATZEE], 2, 0, 1, 3);
    myLayout122->addWidget(myLabelBonusYatzee, 3, 0, 1, 3);
    myGBCombinaison = new QGroupBox("COMBINAISONS");
    myGBCombinaison->setAlignment(Qt::AlignHCenter);
    myGBCombinaison->setLayout(myLayout122);

    // Mise en place du layout contenant tous les boutons a jouer
    myLayout12 = new QVBoxLayout;
    myLayout12->addWidget(myGBChiffres);
    myLayout12->addWidget(myGBCombinaison);

    // Mise en place du layout comprenant tous les boutons
    myLayout1 = new QHBoxLayout;
    myLayout1->addLayout(myLayout11);
    myLayout1->addLayout(myLayout12);

    // Mise en place du layout entier : boutons + affichage des points
    myLabelTotal = new QLabel(myPlayer.name + " a 0 points");
    myLayout = new QVBoxLayout;
    myLayout->addWidget(myLabelTotal);
    myLayout->addLayout(myLayout1);

    // Mise en place des layouts dans la fenetre
    myCentralWidget = new QWidget(this);
    myCentralWidget->setLayout(myLayout);
    setCentralWidget(myCentralWidget);

    /******************************
    * Connexion des signaux/slots *
    ******************************/

    connect(myActionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(myActionVoirScores, &QAction::triggered, this, &Yatfen::seeScores);
    connect(myActionEffacerScores, &QAction::triggered, this, &Yatfen::deleteScores);
    connect(myPBLancer, &QPushButton::clicked, this, &Yatfen::roll);
    connect(myPBChance, &QPushButton::clicked, this, &Yatfen::computeChance);

    for(int i = 0; i < 5; i++)
        connect(myPBDes[i], &QPushButton::clicked, [this, i](){moveToReserve(i+1);});
    for(int i = 0; i < 5; i++)
        connect(myPBReserve[i], &QPushButton::clicked, [this, i](){moveToDices(i+1);});

    for(int i = 0; i < 6; i++)
        connect(myPBChiffres[i], &QPushButton::clicked, [this, i](){computePoints(i+1);});
    connect(myPBCombinaison[BRELAN], &QPushButton::clicked, [this](){computeCombinaisonPoints(BRELAN);});
    connect(myPBCombinaison[CARRE], &QPushButton::clicked, [this](){computeCombinaisonPoints(CARRE);});
    connect(myPBCombinaison[FULL], &QPushButton::clicked, [this](){computeCombinaisonPoints(FULL);});
    connect(myPBCombinaison[PTITE_SUITE], &QPushButton::clicked, [this](){computeCombinaisonPoints(PTITE_SUITE);});
    connect(myPBCombinaison[GRANDE_SUITE], &QPushButton::clicked, [this](){computeCombinaisonPoints(GRANDE_SUITE);});
    connect(myPBCombinaison[YATZEE], &QPushButton::clicked, [this](){computeCombinaisonPoints(YATZEE);});
}

/*******************
* Methodes privees *
*******************/

void Yatfen::update()
{
    // On vide ts les boutons
    for(QPushButton* pb : myPBDes)
        pb->setText("   \n   \n   ");
    for(QPushButton* pb : myPBReserve)
        pb->setText("   \n   \n   ");

    // On les remplit avec la valeur du de qui va bien
    for(int i = 0; i < myYatzee.dices().size(); i++)
        myPBDes[i]->setText(int2points(myYatzee.dices()[i]));
    for(int i = 0; i < myYatzee.reserve().size(); i++)
        myPBReserve[i]->setText(int2points(myYatzee.reserve()[i]));

    // On actualise aussi le bouton lancer
    myPBLancer->setText("Lancer ("+QString::number(myYatzee.remainingRolls())+")");
}

void Yatfen::reset()
{
    myYatzee.reset();

    // Actualisation de l'UI
    for(int i = 0; i < 5; i++)
    {
        myPBDes[i]->setText("   \n   \n   ");
        myPBReserve[i]->setText("   \n   \n   ");
    }
    myPBLancer->setText("Lancer ("+QString::number(myYatzee.remainingRolls())+")");
}

void Yatfen::updateScores()
{
    myLabelTotal->setText(myPlayer.name +
                          " a " +
                          QString::number(myPlayer.points) +
                          " points (dont " +
                          QString::number(myPlayer.bonusPoints) +
                          " points avec les chiffres)");
}

void Yatfen::endOfGame()
{
    // Desactiver la fenetre
    setEnabled(false);

    // Fenetre d'info pour enregistrer
    QString score = myPlayer.name +
                    ", tu as fini !!\nTon score est de " +
                    QString::number(myPlayer.points) +
                    " points.\n\nVeux-tu enregistrer ton score ? :)";
    if(QMessageBox::information(this,
                                "Yatfen : Fin du jeu",
                                score,
                                QMessageBox::No|QMessageBox::Yes) == QMessageBox::Yes)
    {
        QFile scoreFile{myScoreFilename};
        if(!scoreFile.exists())
        {
            // Les boucles sont la pour permettre des creations manuelles si necessaire

            bool retry = false;

            // Creation du repertoire s'il n'existe pas
            do
            {
                QDir dirYatfen(QDir::current());
                if(!dirYatfen.exists(myScoreDirname))
                {
                    bool dirIsCreated = dirYatfen.mkdir(myScoreDirname);
                    if(!dirIsCreated)
                    {
                        QString msg{"Impossible de créer le dossier des scores...\nTu peux le créer manuellement et retenter d'enregistrer ton score. :)"};
                        QMessageBox::critical(this, "Yatfen : Enregistrement du score", msg);
                        retry = true;
                    }
                    else
                        retry = false;
                }
            } while(retry);

            // Creation di fichier s'il n'existe pas
            do
            {
                bool fileIsCreated = scoreFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
                if(!fileIsCreated)
                {
                    QString msg{"Impossible de créer le fichier des scores...\nTu peux le créer manuellement et retenter d'enregistrer ton score. :)"};
                    QMessageBox::critical(this, "Yatfen : Enregistrement du score", msg);
                    retry = true;
                }
                else
                {
                    scoreFile.close();
                    retry = false;
                }
            } while(retry);
        }

        // A ce stade, le fichier existe bien !!
        // Maintenant, enregistrement des infos

        QDateTime dateTime = QDateTime::currentDateTime();

        if(myPlayer.name.toLower() == "evelyne" ||
           myPlayer.name.toLower() == "mule" ||
           myPlayer.name.toLower() == "eve")
        {
            switch(dateTime.time().hour())
            {
            case 9:
                QMessageBox::critical(this, "Yatfen : Non mais dis donc !!",
                                      "C'est une heure pour jouer ?");
                break;

            case 10:
                QMessageBox::critical(this, "Yatfen : Non mais dis donc !!",
                                      "T'as fais le lit avant de jouer ?");
                break;

            case 11:
            case 12:
                QMessageBox::critical(this, "Yatfen : Non mais dis donc !!",
                                      "T'as vu l'heure ? Va faire cuire les patates !!");
                break;

            case 13:
            case 14:
                QMessageBox::critical(this, "Yatfen : Non mais dis donc !!",
                                      "La vaisselle est faite j'imagine...");
                break;

            case 15:
                QMessageBox::critical(this, "Yatfen : Non mais dis donc !!",
                                      "Les corrections sont faites ?");
                break;

            case 21:
                QMessageBox::critical(this, "Yatfen : Non mais dis donc !!",
                                      "Tiens, il n'y a pas NCIS ce soir ?");
                break;
            }
        }

        ScoreManager::saveScore(myScoreFilename.toStdString(),
                                myPlayer.points,
                                myPlayer.name.toStdString(),
                                dateTime.toString("yyyy.MM.dd.hh.mm.ss").toStdString());

        QMessageBox::information(this, "Yatfen : Enregistrement du score", "Ton score est bien pris en compte. ;)");
    }

    // Fenetre d'info pour quitter
    if(QMessageBox::information(this,
                                "Yatfen : Fin du jeu",
                                "Veux-tu rejouer ? :)",
                                QMessageBox::No|QMessageBox::Yes) == QMessageBox::Yes)
    {
        // Reactivation de la fenetre
        setEnabled(true);

        // Reinitialisation au niveau yatzee et GUI
        reset();
        for(QPushButton* pb : myPBChiffres)
            pb->setEnabled(true);
        myPBCombinaison[BRELAN]->setEnabled(true);
        myPBCombinaison[CARRE]->setEnabled(true);
        myPBCombinaison[FULL]->setEnabled(true);
        myPBCombinaison[PTITE_SUITE]->setEnabled(true);
        myPBCombinaison[GRANDE_SUITE]->setEnabled(true);
        myPBCombinaison[YATZEE]->setEnabled(true);
        myPBChance->setEnabled(true);
        myLabelBonusPoints->setText("");
        myLabelBonusYatzee->setText("");
        myLabelTotal->setText(myPlayer.name + " a 0 points");

        // Reinitialisation au niveau joueur
        myPlayer.points = 0;
        myPlayer.bonusPoints = 0;
        myPlayer.remainingRounds = 13;
        myPlayer.bonusYatzee = false;
    }
    else
        qApp->quit();
}

QString Yatfen::int2points(int i) const
{
    QString res;
    switch(i)
    {
    case 1:
        res = "   \n o \n   ";
        break;

    case 2:
        res = "o  \n   \n  o";
        break;

    case 3:
        res = "o  \n o \n  o";
        break;

    case 4:
        res = "o o\n   \no o";
        break;

    case 5:
        res = "o o\n o \no o";
        break;

    case 6:
        res = "o o\no o\no o";
        break;

    default:
        res = "   \n   \n   ";
        break;
    }
    return res;
}

/********
* Slots *
********/

void Yatfen::roll()
{
    myYatzee.roll();
    update();
}

void Yatfen::moveToReserve(int diceIdx)
{
    myYatzee.moveToReserve(diceIdx);
    update();
}

void Yatfen::moveToDices(int diceIdx)
{
    myYatzee.moveToDices(diceIdx);
    update();
}

void Yatfen::computePoints(int number)
{
    // Calcul des points
    myPlayer.points += myYatzee.points(number);
    myPlayer.bonusPoints += myYatzee.points(number);

    // Bonus points
    if(myPlayer.bonusPoints >= Yatzee::POINTS_MIN_FOR_BONUS_POINTS && myLabelBonusPoints->text().isEmpty())
    {
        myLabelBonusPoints->setText("Bonus de points !!");
        myPlayer.points += Yatzee::BONUS_POINTS;
    }

    // Bonus yatzee (s'il y a deja eu un yatzee et que l'on a a nouveau cing des identiques)
    if(myPlayer.bonusYatzee && myYatzee.findCombinaisons().contains(YATZEE))
        myPlayer.points += Yatzee::BONUS_YATZEE;

    // Affichage + raz yatzee
    updateScores();
    reset();

    // Desactivation du bouton appelant
    myPBChiffres[number-1]->setEnabled(false);

    // Fin du jeu ?
    myPlayer.remainingRounds--;
    if(myPlayer.remainingRounds == 0)
        endOfGame();
}

void Yatfen::computeCombinaisonPoints(Combinaison combinaison)
{
    // On calcule les points
    if(myYatzee.findCombinaisons().contains(combinaison))
        myPlayer.points += Yatzee::COMBINAISON_TO_POINTS[combinaison];

    // Cas du yatzee
    if(myYatzee.findCombinaisons().contains(YATZEE))
    {
        myPlayer.bonusYatzee = true;
        myLabelBonusYatzee->setText("Bonus YATZEE !!");
    }

    // On change le texte, raz et desactive le bouton
    updateScores();
    reset();
    myPBCombinaison[combinaison]->setEnabled(false);

    // Fin du jeu ?
    myPlayer.remainingRounds--;
    if(myPlayer.remainingRounds == 0)
        endOfGame();
}

void Yatfen::computeChance()
{
    myPlayer.points += myYatzee.chance();
    updateScores();
    reset();
    myPBChance->setEnabled(false);

    // Fin du jeu ?
    myPlayer.remainingRounds--;
    if(myPlayer.remainingRounds == 0)
        endOfGame();
}

void Yatfen::seeScores()
{
    QFile fichierScores{myScoreFilename};
    if(!fichierScores.exists())
    {
        QMessageBox::information(this, "Yatfen : Scores", "Il n'y a pas de score enregistré...");
        return;
    }

    // On recupere les scores
    QString scores = QString::fromStdString(ScoreManager::loadScores(myScoreFilename.toStdString()));
    QStringList lignesScore = scores.split("\n", QString::SkipEmptyParts);

    /**************************************************
    * Creation et remplissage de la boite de dialogue *
    **************************************************/

    QGridLayout* l = new QGridLayout;
    QLabel* tmpLabel = new QLabel("SCORE");
    tmpLabel->setAlignment(Qt::AlignCenter);
    l->addWidget(tmpLabel, 0, 0);
    tmpLabel = new QLabel("JOUEUR");
    tmpLabel->setAlignment(Qt::AlignCenter);
    l->addWidget(tmpLabel, 0, 1);
    tmpLabel = new QLabel("DATE");
    tmpLabel->setAlignment(Qt::AlignCenter);
    l->addWidget(tmpLabel, 0, 2);

    for(int i = 0; i < lignesScore.size(); i++)
    {
        QStringList tmp = lignesScore[i].split('|');

        QStringList tmpDate = tmp[2].split('.');
        QString cuteDate = QString("%1.%2.%3 %4h%5")
                           .arg(tmpDate[0])
                           .arg(tmpDate[1])
                           .arg(tmpDate[2])
                           .arg(tmpDate[3])
                           .arg(tmpDate[4]);

        QLabel* label1 = new QLabel(tmp[0]);
        label1->setAlignment(Qt::AlignCenter);
        QLabel* label2 = new QLabel(tmp[1]);
        label2->setAlignment(Qt::AlignCenter);
        QLabel* label3 = new QLabel(cuteDate);
        label3->setAlignment(Qt::AlignCenter);

        l->addWidget(label1, i+1, 0);
        l->addWidget(label2, i+1, 1);
        l->addWidget(label3, i+1, 2);
    }

    QDialog* dialogueWindow = new QDialog(this);
    dialogueWindow->setLayout(l);
    dialogueWindow->show();
    dialogueWindow->setWindowTitle(QString("Yatfen : Scores"));
    dialogueWindow->exec();
}

void Yatfen::deleteScores()
{
    QFile fichierScores{myScoreFilename};
    if(fichierScores.exists())
    {
        fichierScores.remove();
        QMessageBox::information(this, "Yatfen : Scores", "C'est bon, il n'y a plus de scores.");
    }
    else
        QMessageBox::information(this, "Yatfen : Scores", "Il n'y avait aucun score...");
}

