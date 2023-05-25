#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <random>
#include <ctime>

class Probabilidades {
    private:
        double pAcertar;
        double pDuplo;
        double pBonus;
        double pDesvio;
        double pContraAtaque;
        double pBloqueio;
    public:
        Probabilidades() {
            this->pAcertar = 0.6;
            this->pDuplo = 0.1;
            this->pBonus = 0.1;
            this->pDesvio = 0.1;
            this->pContraAtaque = 0.1;
            this->pBloqueio = 0.1;
        }
        Probabilidades(double pAcertar, double pDuplo, double pBonus, double pDesvio, double pContraAtaque, double pBloqueio) 
            : pAcertar(pAcertar), pDuplo(pDuplo), pBonus(pBonus), pDesvio(pDesvio), pContraAtaque(pContraAtaque), pBloqueio(pBloqueio) {};
        
        double getPAcertar() const { return this->pAcertar; }
        double getPDuplo() const { return this->pDuplo; }
        double getPBonus() const { return this->pBonus; }
        double getPDesvio() const { return this->pDesvio; }
        double getPContraAtaque() const { return this->pContraAtaque; }
        double getPBloqueio() const { return this->pBloqueio; }
        
        void beConfident() {
            this->pAcertar += (1 - this->pAcertar)/2;
            this->pDesvio += (1 - this->pDesvio)/4;
            this->pContraAtaque += (1 - this->pContraAtaque)/6;
        }
        void beAfraid() {
            this->pAcertar -= this->pAcertar/5;
            this->pDesvio -= this->pDesvio/8;
            this->pContraAtaque = 0;
        }
};

class Soldado {
    private:
        std::string nome;
        double saude;
        double poderAtaque;
        double bonusAtaque;
        Probabilidades p;
        bool dead;
    public:
        // Construtor
        Soldado(std::string nome, double saude, double poderAtaque, double bonusAtaque) 
            : nome(nome), saude(saude), poderAtaque(poderAtaque), bonusAtaque(bonusAtaque) {
                this->dead = false;
            };
        
        // Getters
        std::string getNome() const { return this->nome; }
        double getPoderAtaque() const { return this->poderAtaque; }

        // Setters
        void setProbabilities(Probabilidades& p) { this->p = p; }

        // Outros métodos
        double generatePower(double power) {
            std::mt19937 gen(std::time(0));
            std::normal_distribution<double> dist(power, power * 0.5);
            return dist(gen);
        }
        virtual void defender(double ataqueSofrido, Soldado& attacker) {
            if(1 + (rand() % 100) > this->p.getPDesvio()) {
                this->saude -= ataqueSofrido * (1 - 0.5 * ((rand() % 100) < this->p.getPBloqueio()));
            }
            if(this->saude <= 0) {
                this->dead = true;
            } else {
                if(1 + (rand() % 100) < this->p.getPContraAtaque()) {
                    this->atacar(attacker);
                }
            }
        }
        virtual void atacar(Soldado& other) {
            if(1 + (rand() % 100) < this->p.getPAcertar()) {
                other.defender(this->poderAtaque * (1 + this->bonusAtaque * (1 + (rand() % 100) <= this->p.getPBonus())), *this);
                if(1 + (rand() % 100) <= this->p.getPDuplo()) {
                    other.defender(this->poderAtaque * (1 + this->bonusAtaque * (1 + (rand() % 100) <= this->p.getPBonus())), *this);
                }
            }
        } 
        bool isDead() { return this->dead; }
};

class Elfo : public Soldado {
    public:
        Elfo(std::string nome, double saude, double poderAtaque)
            : Soldado::Soldado(nome, saude, poderAtaque, 0.1) {
                Probabilidades pElfo(0.6, 0.1, 0.2, 0.05, 0, 0.4);
                this->setProbabilities(pElfo);
            };
};

class Anao : public Soldado {
    public:
        Anao(std::string nome, double saude, double poderAtaque)
            : Soldado::Soldado(nome, saude, poderAtaque, 0.1) {
                Probabilidades pAnao(0.4, 0.1, 0.2, 0.4, 0.05, 0);
                this->setProbabilities(pAnao);
            };
};

class Mago : public Soldado {
    public:
        Mago(std::string nome, double saude, double poderAtaque)
            : Soldado::Soldado(nome, saude, poderAtaque, 0.5) {
                Probabilidades pMago(0.9, 0.4, 0.4, 0.2, 0.2, 0.5);
                this->setProbabilities(pMago);
            };
};


class Humano : public Soldado {
    public:
        Humano(std::string nome, double saude, double poderAtaque)
            : Soldado::Soldado(nome, saude, poderAtaque, 0.4) {
                Probabilidades pHumano(0.8, 0.2, 0.2, 0.1, 0.1, 0.4);
                this->setProbabilities(pHumano);
            };
};


class Legolas : public Soldado {
    public:
        Legolas(std::string nome, double saude, double poderAtaque)
            : Soldado::Soldado(nome, saude, poderAtaque, 0.6) {
                Probabilidades pLegolas(0.95, 0.2, 0.4, 0.2, 0.1, 0.5);
                this->setProbabilities(pLegolas);
            };
};

class Orc : public Soldado {
    public:
        Orc(std::string nome, double saude, double poderAtaque)
            : Soldado::Soldado(nome, saude, poderAtaque, 0.05) {
                Probabilidades pOrc(0.8, 0.1, 0.2, 0.05, 0, 0.2);
                this->setProbabilities(pOrc);
            };
};

class Sauron : public Soldado {
    public:
        Sauron(std::string nome, double saude, double poderAtaque)
            : Soldado::Soldado(nome, saude, poderAtaque, 0.6) {
                Probabilidades pSauron(1.0, 0.5, 0.5, 0.2, 0.2, 0.6);
                this->setProbabilities(pSauron);
            };
};

class Gollum : public Soldado {
    public:
        Gollum(std::string nome, double saude, double poderAtaque)
            : Soldado::Soldado(nome, saude, poderAtaque, 0.2) {
                Probabilidades pGollum(0.8, 0.2, 0.1, 0.4, 0.2, 0.1);
                this->setProbabilities(pGollum);
            };
};

class Troll : public Soldado {
    public:
        Troll(std::string nome, double saude, double poderAtaque)
            : Soldado::Soldado(nome, saude, poderAtaque, 0.1) {
                Probabilidades pTroll(0.6, 0.05, 0.1, 0.1, 0.1, 0);
                this->setProbabilities(pTroll);
            };
};

int main() {
    srand(time(0));
}