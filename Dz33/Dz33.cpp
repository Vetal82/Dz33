#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include<locale>


using namespace std;


class ZooException : public exception {
    
public:
    virtual string showMessage() const = 0;
};


class NameException : public ZooException {
public:
    string showMessage() const override {
        return "��������� �������� ������� - �� ������� �����";
    }
};


class MaxCapacityException : public ZooException {
public:
    string showMessage() const override {
        return "���������� ����������� ������� ������ � �����";
    }
};


class PredatorException : public ZooException {
public:
    string showMessage() const override {
        return "�� ����� �������� ������ � ����� � �� ��������";
    }
};


class Animal {
protected:
    string name;
    bool predator;

public:
    Animal(string name, bool predator) {
        if (name.empty()) {
            throw NameException();
        }
        this->name = name;
        this->predator = predator;
    }

    string getName() const { return name; }
    void setName(string name) {
        if (name.empty()) {
            throw NameException();
        }
        this->name = name;
    }

    bool isPredator() const { return predator; }
    void setPredator(bool predator) { this->predator = predator; }

    virtual void showDescription() const = 0;
};


class Fish : public Animal {
private:
    bool deepWater;

public:
    Fish(string name, bool predator, bool deepWater)
        : Animal(name, predator), deepWater(deepWater) {}

    bool isDeepWater() const { return deepWater; }
    void setDeepWater(bool deepWater) { this->deepWater = deepWater; }

    void showDescription() const override {
        cout << "����: " << name << ", �����: " << (predator ? "���" : "ͳ")
            << ", ������������: " << (deepWater ? "���" : "ͳ") << endl;
    }
};


class Bird : public Animal {
private:
    int flightSpeed;

public:
    Bird(string name, bool predator, int flightSpeed)
        : Animal(name, predator), flightSpeed(flightSpeed) {}

    int getFlightSpeed() const { return flightSpeed; }
    void setFlightSpeed(int flightSpeed) { this->flightSpeed = flightSpeed; }

    void showDescription() const override {
        cout << "����: " << name << ", �����: " << (predator ? "���" : "ͳ")
            << ", �������� �������: " << flightSpeed << " ��/���" << endl;
    }
};


class Mammal : public Animal {
private:
    string habitat;

public:
    Mammal(string name, bool predator, string habitat)
        : Animal(name, predator), habitat(habitat) {}

    string getHabitat() const { return habitat; }
    void setHabitat(string habitat) { this->habitat = habitat; }

    void showDescription() const override {
        cout << "���: " << name << ", �����: " << (predator ? "���" : "ͳ")
            << ", ���������� ����������: " << habitat << endl;
    }
};


class Cage {
private:
    int number;
    int maxAnimals;
    int currentAnimals;
    vector<Animal*> animals;

public:
    Cage(int number, int maxAnimals)
        : number(number), maxAnimals(maxAnimals), currentAnimals(0) {}

    int getNumber() const { return number; }
    void setNumber(int number) { this->number = number; }

    int getMaxAnimals() const { return maxAnimals; }
    void setMaxAnimals(int maxAnimals) { this->maxAnimals = maxAnimals; }

    int getCurrentAnimals() const { return currentAnimals; }

    void addAnimal(Animal* animal) {
        if (currentAnimals >= maxAnimals) {
            throw MaxCapacityException();
        }
        for (Animal* a : animals) {
            if (a->isPredator() != animal->isPredator()) {
                throw PredatorException();
            }
        }
        animals.push_back(animal);
        currentAnimals++;
    }

    void removeAnimal(Animal* animal) {
        auto it = find(animals.begin(), animals.end(), animal);
        if (it != animals.end()) {
            animals.erase(it);
            currentAnimals--;
        }
    }

    void findAnimal(string name) const {
        for (Animal* animal : animals) {
            if (animal->getName() == name) {
                animal->showDescription();
                return;
            }
        }
        cout << "������� �� ��������" << endl;
    }

    void showAnimals() const {
        for (Animal* animal : animals) {
            animal->showDescription();
        }
    }
};

int main() {
    setlocale(LC_CTYPE, "Ukr");
    try {
        Cage cage1(1, 2);

        Fish* fish1 = new Fish("Goldfish", false, false);
        Bird* bird1 = new Bird("Eagle", true, 120);
        Mammal* mammal1 = new Mammal("Lion", true, "Savannah");

        cage1.addAnimal(fish1);
        cage1.addAnimal(bird1); 

        cage1.showAnimals();

    }
    catch (const ZooException& e) {
        cout << e.showMessage() << endl;
    }

    try {
        Mammal* mammal2 = new Mammal("", true, "Jungle"); 
    }
    catch (const NameException& e) {
        cout << e.showMessage() << endl;
    }

    return 0;
}
