#ifndef SM_H
#define SM_H

#include <string>

namespace ScoreManager {

/**
 * Enregistre un score dans un fichier de scores tout en veillant a ce qu'il
 * n'y ait que les 20 meilleurs scores enregistres dans le fichier.
 *
 * \param [in] filename Nom du fichier dans lequel sont enregistres les scores
 * \param [in] score Score du joueur
 * \param [in] name Nom du joueur
 * \param [in] date Date a laquelle le score a ete fait
 * \return Si ca s'est bien passe ou non
 *
 * \warning date est suppose etre de la forme AAAA.MM.JJ.hh.mm.ss
 */
bool saveScore(const std::string& filename, int score, const std::string& name, const std::string& date);

/**
 * Renvoie tout le contenu du fichier de scores dans une string
 */
std::string loadScores(const std::string& filename);

}

#endif

