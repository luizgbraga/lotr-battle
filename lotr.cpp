#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>

class Probabilidades {
    private:
        double pAcertar;
        double pDuplo;
        double pBonus;
        double pDesvio;
        double pContraAtaque;
        double pBloqueio;
        bool afraid;
        bool confident;
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
            : pAcertar(pAcertar), pDuplo(pDuplo), pBonus(pBonus), pDesvio(pDesvio), pContraAtaque(pContraAtaque), pBloqueio(pBloqueio) {
                this->afraid = false;
                this->confident = false;
            };
        
        double getPAcertar() const { return this->pAcertar; }
        double getPDuplo() const { return this->pDuplo; }
        double getPBonus() const { return this->pBonus; }
        double getPDesvio() const { return this->pDesvio; }
        double getPContraAtaque() const { return this->pContraAtaque; }
        double getPBloqueio() const { return this->pBloqueio; }
        bool getConfident() const { return this->confident; }
        bool getAfraid() const { return this->afraid; }
        
        void beConfident() {
            this->pAcertar += (1 - this->pAcertar)/2;
            this->pDesvio += (1 - this->pDesvio)/4;
            this->pContraAtaque += (1 - this->pContraAtaque)/6;
            this->confident = true;
            this->afraid = false;
        }
        void beAfraid() {
            this->pAcertar -= this->pAcertar/5;
            this->pDesvio -= this->pDesvio/8;
            this->pContraAtaque = 0;
            this->afraid = true;
            this->confident = false;
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
        int waitList;
    public:
        // Construtor
        Soldado(std::string nome, double saude, double poderAtaque, double bonusAtaque) 
            : nome(nome), saude(saude), poderAtaque(poderAtaque), bonusAtaque(bonusAtaque) {
                this->dead = false;
                this->waitList = 0;
            };
        
        // Getters
        std::string getNome() const { return this->nome; }
        double getPoderAtaque() const { return this->poderAtaque; }
        int getWaitList() const { return this->waitList; }

        // Setters
        void setProbabilities(Probabilidades& p) { this->p = p; }
        void resetWaitList() { this->waitList = 0; }

        // Outros métodos
        double generatePower(double power) {
            std::mt19937 gen(std::time(0));
            std::normal_distribution<double> dist(power, power * 0.5);
            return dist(gen);
        }
        virtual void defender(double ataqueSofrido, Soldado& attacker) {
            if((double)(rand() % 100)/100 > this->p.getPDesvio()) {
                double damage = ataqueSofrido * (1 - 0.5 * ((double)(rand() % 100)/100 < this->p.getPBloqueio()));
                this->saude -= damage;
                std::cout << "Ouch! -" << damage << std::endl;
                if(damage > this->saude/2) {
                    this->p.beAfraid();
                }
            } else {
                std::cout << "Desvio!" << std::endl;
            }
            if(this->saude <= 0) {
                this->dead = true;
            } else {
                if((double)(rand() % 100)/100 < this->p.getPContraAtaque()) {
                    this->atacar(attacker);
                }
            }
        }
        virtual void atacar(Soldado& other) {
            std::cout << this->getNome();
            if(this->p.getConfident()) {
                std::cout << "++";
            } else if(this->p.getAfraid()) {
                std::cout << "--";
            }
            std::cout << " ataca " << other.getNome();
            if(other.p.getConfident()) {
                std::cout << "++";
            } else if(other.p.getAfraid()) {
                std::cout << "--";
            }
            std::cout << std::endl;
            if((double)(rand() % 100)/100 < this->p.getPAcertar()) {
                other.defender(this->poderAtaque * (this->bonusAtaque * ((double)(rand() % 100)/100 <= this->p.getPBonus())), *this);
                if((double)(rand() % 100)/100 <= this->p.getPDuplo()) {
                    other.defender(this->poderAtaque * (this->bonusAtaque * ((double)(rand() % 100)/100 <= this->p.getPBonus())), *this);
                    this->p.beConfident();
                }
            }
        } 
        bool isDead() { return this->dead; }
        void incrementWaitList() { this->waitList++; }
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

bool compareSoldados(const Soldado& s1, const Soldado& s2) {
    return s1.getWaitList() > s2.getWaitList();
}

class Battle {
    private:
        std::vector<Soldado> eBem;
        std::vector<Soldado> eMal;
    public:
        Battle() {
            this->eBem = {};
            this->eMal = {};
        };
        Battle& addBem(Soldado &sBem) {
            eBem.push_back(sBem);
            return *this;
        }
        Battle &addMal(Soldado &sMal) {
            eMal.push_back(sMal);
            return *this;
        }
        bool ended() const { 
            return (eBem.size() * eMal.size() == 0); 
        }
        void shuffle() { 
            std::random_shuffle(this->eBem.begin(), this->eBem.end());
            std::random_shuffle(this->eMal.begin(), this->eMal.end());   
        }
        void sort() {
            std::sort(this->eBem.begin(), this->eBem.end(), compareSoldados);
            std::sort(this->eMal.begin(), this->eMal.end(), compareSoldados);
        }
        void round(int r) {
            this->shuffle();
            this->sort();
            int i = 0;
            std::cout << std::endl << "********** Round " << r << " ***********" << std::endl << std::endl;
            while(i < eBem.size() && i < eMal.size()) {
                eBem[i].atacar(eMal[i]);
                if(!eMal[i].isDead() && !eBem[i].isDead()) {
                    eMal[i].atacar(eBem[i]);
                }
                if(eBem[i].isDead()) {
                    eBem.erase(eBem.begin() + i);
                } else {
                    eBem[i].resetWaitList();
                }
                if(eMal[i].isDead()) {
                    eMal.erase(eMal.begin() + i);
                } else {
                    eMal[i].resetWaitList();
                }
                i++;
            }
            if(i < eBem.size()) {
                eBem[i].incrementWaitList();
                i++;
            }
            if(i < eMal.size()) {
                eMal[i].incrementWaitList();
                i++;
            }
        }
        void battle() {
            int r = 0;
            while(!this->ended()) {
                this->round(r++);
                char next = 'A';
                while(next != 'C') {
                    std::cout << "Aperte C para continuar: ";
                    std::cin >> next;
                }
            }
            if(eBem.size()) {
                std::cout << "O Bem venceu!" << std::endl;
            } 
            if(eMal.size()) {
                std::cout << "O Mal venceu!" << std::endl;
            }
        }
};

int main() {
    srand(time(0));

    Battle b;

    Elfo e1("Elfo 1", 30, 10);
    Elfo e2("Elfo 2", 32, 12);
    // Elfo e3("Elfo 3", 30, 3);
    // Elfo e4("Elfo 4", 28, 2);
    // Elfo e5("Elfo 5", 36, 2);
    b.addBem(e1).addBem(e2);
    //.addBem(e3).addBem(e4).addBem(e5);

    Anao a1("Anao 1", 20, 8);
    Anao a2("Anao 2", 22, 10);
    Anao a3("Anao 3", 18, 10);
    // Anao a4("Anao 4", 28, 1);
    // Anao a5("Anao 5", 24, 2);
    b.addBem(a1).addBem(a2).addBem(a3);
    //.addBem(a4).addBem(a5);

    Humano h1("Humano 1", 50, 25);
    Humano h2("Humano 2", 42, 30);
    // Humano h3("Humano 3", 50, 8);
    // Humano h4("Humano 4", 48, 10);
    // Humano h5("Humano 5", 56, 8);
    b.addBem(h1).addBem(h2);
    //addBem(h3).addBem(h4).addBem(h5);

    Mago m1("Mago 1", 100, 60);
    b.addBem(m1);

    Legolas l1("Legolas 1", 80, 58);
    b.addBem(l1);

    Troll t1("Troll 1", 25, 10);
    Troll t2("Troll 2", 22, 8);
    Troll t3("Troll 3", 18, 6);
    Troll t4("Troll 4", 28, 10);
    Troll t5("Troll 5", 24, 12);
    // Troll t6("Troll 6", 20, 4);
    // Troll t7("Troll 7", 22, 1);
    // Troll t8("Troll 8", 18, 2);
    // Troll t9("Troll 9", 28, 1);
    // Troll t10("Troll 10", 24, 2);
    b.addMal(t1).addMal(t2).addMal(t3).addMal(t4).addMal(t5);
    //addMal(t6).addMal(t7).addMal(t8).addMal(t9).addMal(t10);

    Orc o1("Orc 1", 15, 10);
    Orc o2("Orc 2", 12, 4);
    // Orc o3("Orc 3", 18, 2);
    // Orc o4("Orc 4", 20, 6);
    // Orc o5("Orc 5", 14, 2);
    // Orc o6("Orc 6", 16, 4);
    // Orc o7("Orc 7", 22, 8);
    // Orc o8("Orc 8", 18, 2);
    b.addMal(o1).addMal(o2);
    //addMal(o3).addMal(o4).addMal(o5).addMal(o6).addMal(o7).addMal(o8);

    Sauron s1("Sauron 1", 200, 80);
    b.addMal(s1);

    Gollum g1("Gollum 1", 50, 90);
    b.addMal(g1);

    b.battle();

    return 0;
}