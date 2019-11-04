#ifndef VALEURREELLE_H
#define VALEURREELLE_H

#include "Valeur.h"
#include <string>

class ValeurReelle : public Valeur {
public:
    ValeurReelle(float valeur);
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
    float m_valeur;
};

#endif /* VALEURREELLE_H */

