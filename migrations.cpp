#include<iostream>
#include<vector>

class modifier {
    private:
        // Modifier type describes which value the modifier will affect
        enum modifier_type_t {
            TYPE_POP=0, TYPE_QOL=1, TYPE_GR=2
        } modifier_type;

        // Delta is the fractional change in that amount
        float delta;
    public:
        modifier(int type, float amt);
        int getCategory() {return modifier_type;}
        float getIntensity() {return delta;}
};

class delta_pop {
    private:
        int amt;
    public:
        delta_pop(int amt) {this->amt = amt;}
        void add_change(int chng) {this->amt += chng;}
        int get_amt() {return amt;}
};

class province {
    private:
        // Unique ID of province
        int id;
        // Population
        int pop;
        // Quality of life
        float qol;
        // Growth rate
        float gr;
        // Pointers to adjacent province
        std::vector<province *> adjacents;
    public:
        province(int id, int pop, float qol, float gr);
        void apply_modifier(modifier mod);
        delta_pop generate_delta();
        void add_adjacent(province * p) { adjacents.push_back(p); }
        void add_delta(delta_pop d) { this->pop += d.get_amt(); }

        void print_details() {
            std::cout << "ID: " << id << std::endl;
            std::cout << "Population: " << pop << std::endl;
            std::cout << "Quality of Living: " << qol << std::endl;
            std::cout << "Growth rate: " << gr << std::endl;
        }
};

modifier::modifier(int type, float amt) {
    this->modifier_type = (modifier_type_t)type;
    this->delta = amt;
}

province::province(int id, int pop, float qol, float gr) {
    this->id = id;
    this->pop = pop;
    this->gr = gr;
    this->qol = qol;
}

void province::apply_modifier(modifier mod) {
    switch(mod.getCategory()) {
        case 0:
            this->pop *= (1 + mod.getIntensity());
            break;
        case 1:
            this->qol *= (1 + mod.getIntensity());
            break;
        case 2:
            this->gr *= (1 + mod.getIntensity());
            break;
        default:
            std::cout << "Error unresolved modifier type";
            break;
    }
}

delta_pop province::generate_delta() {
    delta_pop delta = delta_pop(0);
    for (province * p : this->adjacents) {
        float diff = this->qol - p->qol;
        diff = diff / 100.0;
        delta.add_change((int)(this->pop * (diff + gr)));
    }
    return delta;
}

int main() {
    std::cout.precision(6);
    province p1 = province(0, 1000, 3, 0.02);
    province p2 = province(1, 1501, 2, 0.03);
    p1.add_adjacent(&p2);
    p2.add_adjacent(&p1);
    modifier plague = modifier(2, -2.1);
    modifier plague_gon = modifier(2, -2.5);
    int t = 0;
    while(t < 100) {
        std::cout << "TURN " << t << std::endl;
        delta_pop d1 = p1.generate_delta();
        delta_pop d2 = p2.generate_delta();
        p1.add_delta(d1);
        p2.add_delta(d2);
        if (t == 25) {
            p1.apply_modifier(plague);
        }
        if (t == 50) {
            p1.apply_modifier(plague_gon);
            p2.apply_modifier(plague);
        }
        if (t == 75) {
            p2.apply_modifier(plague_gon);
        }
        std::cout << "Province 1: " << std::endl;
        p1.print_details();
        std::cout << "------" << std::endl;
        std::cout << "Province 2: " << std::endl;
        std::cout << "------" << std::endl;
        p2.print_details();
        std::cout << ">>>-<<<" << std::endl;
        t++;
    }
}