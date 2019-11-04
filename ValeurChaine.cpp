#include "ValeurChaine.h"
#include "ValeurReelle.h"

#include <string>

ValeurChaine::ValeurChaine(std::string valeur) : m_valeur(valeur) {}

bool ValeurChaine::isVrai() const {
    return m_valeur.size() > 0;
}

int ValeurChaine::getEntier() const {
    return std::stoi(m_valeur);
}

float ValeurChaine::getReel() const {
    return std::stof(m_valeur);
}

std::string ValeurChaine::getChaine() const {
    return m_valeur;
}

Valeur* const ValeurChaine::operator+(const Valeur* v) const {
    return new ValeurChaine(m_valeur + v->getChaine());
}

Valeur* const ValeurChaine::operator-(const Valeur* v) const {
    return new ValeurReelle(std::stof(m_valeur) - v->getReel());
}

Valeur* const ValeurChaine::operator*(const Valeur* v) const {
    std::string val = "";
    for (int i = 0; i < v->getEntier(); i++) {
        val += m_valeur;
    }
    return new ValeurChaine(val);
}

Valeur* const ValeurChaine::operator/(const Valeur* v) const {
    return new ValeurReelle(std::stof(m_valeur) / v->getReel());
}

bool ValeurChaine::operator>(const Valeur* v) const {
    return m_valeur > v->getChaine();
}

bool ValeurChaine::operator>=(const Valeur* v) const {
    return m_valeur >= v->getChaine();
}

bool ValeurChaine::operator<(const Valeur* v) const {
    return m_valeur < v->getChaine();
}

bool ValeurChaine::operator<=(const Valeur* v) const {
    return m_valeur <= v->getChaine();
}

bool ValeurChaine::operator==(const Valeur* v) const {
    return m_valeur == v->getChaine();
}

bool ValeurChaine::operator!=(const Valeur* v) const {
    return m_valeur != v->getChaine();
}