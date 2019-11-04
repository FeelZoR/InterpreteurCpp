#ifndef VALEUR_H
#define VALEUR_H

#include <string>
#include <ostream>

class Valeur {
public:
    virtual int getEntier() const = 0;
    virtual float getReel() const = 0;
    virtual std::string getChaine() const = 0;
    virtual bool isVrai() const = 0;
    
    virtual Valeur* const operator+(const Valeur* v) const = 0;
    virtual Valeur* const operator-(const Valeur* v) const = 0;
    virtual Valeur* const operator*(const Valeur* v) const = 0;
    virtual Valeur* const operator/(const Valeur* v) const = 0;
    virtual bool operator> (const Valeur* v) const = 0;
    virtual bool operator>=(const Valeur* v) const = 0;
    virtual bool operator< (const Valeur* v) const = 0;
    virtual bool operator<=(const Valeur* v) const = 0;
    virtual bool operator==(const Valeur* v) const = 0;
    virtual bool operator!=(const Valeur* v) const = 0;
    
    bool operator&&(Valeur* v) {
        return isVrai() && v->isVrai();
    }
    
    bool operator||(Valeur* v) {
        return isVrai() || v->isVrai();
    }
    
    bool operator!() {
        return !isVrai();
    }
};

inline std::ostream& operator<<(std::ostream& out, Valeur* v) {
    out << v->getChaine();
    return out;
}

#endif /* VALEUR_H */

