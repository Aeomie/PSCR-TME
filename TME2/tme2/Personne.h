#pragma once
#include <iostream>
using namespace std;
class Personne
{
public:
    string nom;
    string prenom;
    int age;
    char sexe; // or std::string for more options

    Personne(string nom, string prenom, int age, char sex) : nom(nom), prenom(prenom), age(age), sexe(sex) {};
};