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
        return "Неможливо створити тварину - не вказана назва";
    }
};


class MaxCapacityException : public ZooException {
public:
    string showMessage() const override {
        return "Перевищено максимальну кількість тварин у клітці";
    }
};


class PredatorException : public ZooException {
public:
    string showMessage() const override {
        return "Не можна додавати хижака в клітку з не хижаками";
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
        cout << "Риба: " << name << ", Хижак: " << (predator ? "Так" : "Ні")
            << ", Глибоководна: " << (deepWater ? "Так" : "Ні") << endl;
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
        cout << "Птах: " << name << ", Хижак: " << (predator ? "Так" : "Ні")
            << ", Швидкість польоту: " << flightSpeed << " км/год" << endl;
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
        cout << "Звір: " << name << ", Хижак: " << (predator ? "Так" : "Ні")
            << ", Середовище проживання: " << habitat << endl;
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
        cout << "Тварину не знайдено" << endl;
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
