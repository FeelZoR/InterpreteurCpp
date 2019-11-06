#include "ValeurEntiere.h"
#include "Valeur.h"

#include <string>

ValeurEntiere::ValeurEntiere(int valeur) : m_valeur(valeur) {}

bool ValeurEntiere::isVrai() const {
    return m_valeur != 0;
}

int ValeurEntiere::getEntier() const {
    return m_valeur;
}

float ValeurEntiere::getReel() const {
    return ((float) m_valeur);
}

std::string ValeurEntiere::getChaine() const {
    return std::to_string(m_valeur);
}

Valeur* const ValeurEntiere::operator+(const Valeur* v) const {
    return new ValeurEntiere(m_valeur + v->getEntier());
}

Valeur* const ValeurEntiere::operator-(const Valeur* v) const {
    return new ValeurEntiere(m_valeur - v->getEntier());
}

Valeur* const ValeurEntiere::operator*(const Valeur* v) const {
    return new ValeurEntiere(m_valeur * v->getEntier());
}

Valeur* const ValeurEntiere::operator/(const Valeur* v) const {
    return new ValeurEntiere(m_valeur / v->getEntier());
}

bool ValeurEntiere::operator>(const Valeur* v) const {
    return m_valeur > v->getEntier();
}

bool ValeurEntiere::operator>=(const Valeur* v) const {
    return m_valeur >= v->getEntier();
}

bool ValeurEntiere::operator<(const Valeur* v) const {
    return m_valeur < v->getEntier();
}

bool ValeurEntiere::operator<=(const Valeur* v) const {
    return m_valeur <= v->getEntier();
}

bool ValeurEntiere::operator==(const Valeur* v) const {
    return m_valeur == v->getEntier();
}

bool ValeurEntiere::operator!=(const Valeur* v) const {
    return m_valeur != v->getEntier();
}