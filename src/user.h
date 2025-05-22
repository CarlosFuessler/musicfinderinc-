
// Haeder datei für den Vecto

#include <string>
#include <vector>
#include <limits>
#include <iostream>
#include <fstream>

#pragma once
class user
{
public:
    // Atribute
    std::string id;
    std::string username;
    std::string password;
    std::string genere;
    std::string freunde;
    // Immer wenn der user neu deklariert wird werden seine werte auf lee gesetzt
    user()
    {
        id = "";
        username = "";
        password = "";
        genere = "";
        freunde = "";
    }
    // Erstellt eine ID

    void createid()
    {

        id = "";
        for (char c : username)
        {
            id += std::to_string(int(c));
        }
    }
    // Ruft das Quiz auf
    void quiz()
    {
        std::vector<std::string> genre_names = {
            "Popmusik", "Rockmusik", "HipHop", "Rap", "Techno",
            "House", "RnB", "Jazz", "Klassik"};

        std::vector<int> generes(9, 0);
        std::string quizpath = "bin/quiz.txt";
        std::ifstream fromfile(quizpath);

        if (!fromfile.is_open())
        {
            std::cerr << "Fehler: Quiz-Datei konnte nicht geöffnet werden." << std::endl;
            return;
        }

        system("clear");

        int question_number = 1;
        std::string line;

        while (question_number <= 5 && std::getline(fromfile, line))
        {
            system("clear");
            std::string question_lines[5];
            question_lines[0] = line;
            std::cout << question_lines[0] << std::endl
                      << std::endl;

            for (int j = 1; j < 5; ++j)
            {
                if (std::getline(fromfile, question_lines[j]))
                {
                    std::cout << j << ". " << question_lines[j] << std::endl
                              << std::endl;
                }
                else
                {
                    std::cerr << "Fehler: Unerwartetes Dateiende beim Lesen der Antworten für Frage " << question_number << std::endl;
                    fromfile.close();
                    return;
                }
            }

            int auswahl = 0;
            bool validierung = false;

            while (!validierung)
            {
                std::cin >> auswahl;
                if (auswahl >= 1 && auswahl <= 4)
                {
                    validierung = true;
                }
                else
                {
                    std::cout << "Ungültige Eingabe. Bitte gib eine Zahl zwischen 1 und 4 ein." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }

            switch (question_number - 1)
            {
            case 1:
                switch (auswahl)
                {
                case 1:
                    generes[1]++;
                    generes[4]++;
                    generes[2]++;
                    break;
                case 2:
                    generes[0]++;
                    generes[5]++;
                    generes[6]++;
                    break;
                case 3:
                    generes[3]++;
                    generes[7]++;
                    generes[6]++;
                    break;
                case 4:
                    generes[8]++;
                    generes[7]++;
                    break;
                }
                break;
            case 2:
                switch (auswahl)
                {
                case 1:
                    generes[1]++;
                    generes[4]++;
                    generes[2]++;
                    break;
                case 2:
                    generes[0]++;
                    generes[5]++;
                    generes[6]++;
                    break;
                case 3:
                    generes[3]++;
                    generes[7]++;
                    generes[8]++;
                    break;
                case 4:
                    generes[8]++;
                    generes[7]++;
                    break;
                }
                break;
            case 3:
                switch (auswahl)
                {
                case 1:
                    generes[1]++;
                    generes[4]++;
                    generes[2]++;
                    break;
                case 2:
                    generes[0]++;
                    generes[7]++;
                    generes[6]++;
                    break;
                case 3:
                    generes[3]++;
                    generes[7]++;
                    generes[8]++;
                    break;
                case 4:
                    generes[8]++;
                    generes[5]++;
                    break;
                }
                break;
            case 4:
                switch (auswahl)
                {
                case 1:
                    generes[1]++;
                    generes[4]++;
                    generes[2]++;
                    break;
                case 2:
                    generes[0]++;
                    generes[5]++;
                    generes[6]++;
                    break;
                case 3:
                    generes[7]++;
                    generes[6]++;
                    break;
                case 4:
                    generes[0]++;
                    generes[7]++;
                    break;
                }
                break;
            case 5:
                switch (auswahl)
                {
                case 1:
                    generes[1]++;
                    generes[4]++;
                    generes[2]++;
                    break;
                case 2:
                    generes[2]++;
                    generes[7]++;
                    generes[6]++;
                    break;
                case 3:
                    generes[3]++;
                    generes[7]++;
                    break;
                case 4:
                    generes[6]++;
                    generes[5]++;
                    break;
                }
            }
            question_number++;
        }
        fromfile.close();

        int max = 0;
        for (int i = 1; i < 9; i++)
        {
            if (generes[i] > generes[max])
            {
                max = i;
            }
        }
        genere = genre_names[max];
    }
};