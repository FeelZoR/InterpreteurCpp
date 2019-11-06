#ifndef VALEURENTIERE_H
#define VALEURENTIERE_H

#include "Valeur.h"
#include <string>

class ValeurEntiere : public Valeur {
public:
    ValeurEntiere(int valeur);
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
    int m_valeur;
};

#endif /* VALEURENTIERE_H */

