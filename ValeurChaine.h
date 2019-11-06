#ifndef VALEURCHAINE_H
#define VALEURCHAINE_H

#include "Valeur.h"
#include <string>

class ValeurChaine : public Valeur {
public:
    ValeurChaine(std::string valeur);
    int getEntier() const override;
    float getReel() const override;
    std::string getChaine() const override;
    bool isVrai() const override;
    
    Valeur* const operator+(const Valeur* v) const override;
    Valeur* const operator-(const Valeur* v) const override;
    Valeur* const operator*(const Valeur* v) const override;
    Valeur* const operator/(const Valeur* v) const override;
    bool operator> (const Valeur* v) const override;
    bool operator>=(const Valeur* v) const override;
    bool operator< (const Valeur* v) const override;
    bool operator<=(const Valeur* v) const override;
    bool operator==(const Valeur* v) const override;
    bool operator!=(const Valeur* v) const override;
private:
    std::string m_valeur;
};

#endif /* VALEURCHAINE_H */

