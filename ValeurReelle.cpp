#include "ValeurReelle.h"

#include <string>

ValeurReelle::ValeurReelle(float valeur) : m_valeur(valeur) {}

bool ValeurReelle::isVrai() const {
    return m_valeur != 0;
}

int ValeurReelle::getEntier() const {
    return ((int) m_valeur);
}

float ValeurReelle::getReel() const {
    return m_valeur;
}

std::string ValeurReelle::getChaine() const {
    return std::to_string(m_valeur);
}

Valeur* const ValeurReelle::operator+(const Valeur* v) const {
    return new ValeurReelle(m_valeur + v->getReel());
}

Valeur* const ValeurReelle::operator-(const Valeur* v) const {
    return new ValeurReelle(m_valeur - v->getReel());
}

Valeur* const ValeurReelle::operator*(const Valeur* v) const {
    return new ValeurReelle(m_valeur * v->getReel());
}

Valeur* const ValeurReelle::operator/(const Valeur* v) const {
    return new ValeurReelle(m_valeur / v->getReel());
}

bool ValeurReelle::operator>(const Valeur* v) const {
    return m_valeur > v->getReel();
}

bool ValeurReelle::operator>=(const Valeur* v) const {
    return m_valeur >= v->getReel();
}

bool ValeurReelle::operator<(const Valeur* v) const {
    return m_valeur < v->getReel();
}

bool ValeurReelle::operator<=(const Valeur* v) const {
    return m_valeur <= v->getReel();
}

bool ValeurReelle::operator==(const Valeur* v) const {
    return m_valeur == v->getReel();
}

bool ValeurReelle::operator!=(const Valeur* v) const {
    return m_valeur != v->getReel();
}