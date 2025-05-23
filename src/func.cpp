
// Funktionen der Datenbank

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <thread>
#include "user.h"
#include "func.h"

const std::string path = "bin/userdata.csv";
const std::string quizpath = "bin/quiz.txt";

void createuser(std::vector<user> &users)
{

    user newuser;
    int i = 0;
    std::cout << R"(
     _______________________________
    |                               |
    | Gebe einen Benutzername ein:  |
    |_______________________________|
                                     
)" << std::endl;
    std::cin >> newuser.username;
    std::transform(newuser.username.begin(), newuser.username.end(), newuser.username.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    for (const auto &vorhandener_user : users)
    {
        if (newuser.username == vorhandener_user.username)
        {
            std::cout << "Der Benutzername ist leider vergeben" << std::endl;
            fortfahren();
            return;
        }
    }
    clearscreen();
    std::cout << R"(
     ___________________________
    |                           |
    | Gebe einen Passwort ein:  |
    |___________________________|
                                       
)" << std::endl;
    std::cin >> newuser.password;
    std::cout << std::endl;
    std::cout << "Benutzer wurde erstellt" << std::endl;

    newuser.createid();

    users.push_back(newuser);

    setuserdata(users);

    clearscreen();
}

// Funktion für Login
void login(std::vector<user> &users, int &p, bool &loginworks)
{
    if (users.empty())
    {
        std::cout << "Keine Benutzer vorhanden. Bitte erstelle einen neuen "
                     "Benutzer."
                  << std::endl;
        return;
    }
    user userlogin;
    std::cout << R"(
    _______________________________
   |                               |
   | Gebe einen Benutzername ein:  |
   |_______________________________|
                                       
)" << std::endl;
    std::cin >> userlogin.username;
    clearscreen();
    std::cout << R"(
    ___________________________
   |                           |
   | Gebe einen Passwort ein:  |
   |___________________________|
                                       
)" << std::endl;
    std::cin >> userlogin.password;

    clearscreen();

    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].username == userlogin.username &&
            users[i].password == userlogin.password)
        {
            std::cout << "Anmeldung erfolgreich!" << std::endl;

            loginworks = true;
            p = i;

            return;
        }
    }
    std::cout << "Ungültiger Benutzername oder Passwort." << std::endl;
}

// Funktion Speichert neue Benutzerdaten in der Datei
void setuserdata(std::vector<user> &users)
{

    std::ofstream addtofile(path, std::ios::binary);
    if (addtofile.is_open())
    {

        for (const auto &user : users)
        {
            addtofile << user.id << ";" << user.username << ";" << user.password << ";" << user.genere << ";" << user.freunde << std::endl;
        }

        addtofile.close();

        clearscreen();
        // std::cout << "Benutzerdaten wurden gespeichert!" << std::endl;
    }
    else
    {
        std::cerr << "Es ist ein Fehler beim Speicern der Daten entstanden" << std::endl;
    }
}

// Funktion läd die Daten neu aus der Datei ein
void getuserdata(std::vector<user> &users)
{
    std::ifstream fromfile(path);
    if (fromfile.is_open())
    {
        users.clear();
        user uploaduserdata;
        std::string zeile;
        while (std::getline(fromfile, zeile))
        {
            std::stringstream ss(zeile);
            std::getline(ss, uploaduserdata.id, ';');
            std::getline(ss, uploaduserdata.username, ';');
            std::getline(ss, uploaduserdata.password, ';');
            std::getline(ss, uploaduserdata.genere, ';');
            std::getline(ss, uploaduserdata.freunde, ';');
            users.push_back(uploaduserdata);
        }

        fromfile.close();
        // std::cout << "Daten wurden geladen" << std::endl;
    }
    else
    {
        std::cout << "Daten konnten nicht gefunden werden!" << std::endl;
        exit(0);
    }
}

// Funktion löscht die Benutzerdaten
void clearuserdata()
{
    std::ofstream clearfile(path);
    if (clearfile.is_open())
    {
        std::ofstream ofs(path, std::ios::out | std::ios::trunc);
        ofs.close();
        clearfile.close();
        std::cout << "Das User  verzeichnis wurde gelöscht!" << std::endl;
    }
}

// Cleart den Bildschirm
void clearscreen()
{
#if defined(WIN32)
    system("cls");
#else
    system("clear");
#endif
    // std::cout << "\033[2J\033[1;1H";
}

// ruft das quiz auf
void quizes(std::vector<user> &users, int position)
{
    clearscreen();
    std::cout << R"(
     ______      __   __    _    ______
    /  ___ \    |  | |  |  | |  |   ___|
    | /   \ |   |  | |  |  | |   \ \
    | |   | |   |  | |  |  | |    \ \
    | \___/ \   |  |_|  |  | |   ___\ \
     \_____/\ \ \_______/  |_|  |______|


 )" << std::endl;

    int bestaetigung;

    do
    {
        clearscreen();

        std::cout << R"(
    ____________________________________
   |                                    |
   |   Quiz (1)  |  Zurück zum Menü (0) |
   |____________________________________|
  )" << std::endl;

        std::cin >> bestaetigung;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (bestaetigung)
        {
        case 1:

            users[position].quiz();

            break;
        default:
            if (bestaetigung == 0)
            {

                clearscreen();
                std::cout << std::endl
                          << "      Danke für ihre Teilnahme" << std::endl
                          << std::endl;
                std::cout << "      ihr Genere ist : " << users[position].genere << std::endl;

                fortfahren();
            }
            break;
        }

    }

    while (bestaetigung != 0);

    return;
}

// Generiert Benutuer mit zufälligen Daten
void generateusers()
{

    int users = 200;
    std::ofstream addtofile(path);
    if (addtofile.is_open())
    {
        std::vector<std::string> genres = {"Popmusik", "Rockmusik", "HipHop",
                                           "Jazz", "Techno", "Klassik",
                                           "RnB", "House", "Rap"};
        std::vector<std::string> usernames = {
            "user1", "testuser", "musiclover", "coder", "guest", "alpha",
            "beta", "gamma", "delta", "epsilon", "one", "two",
            "three", "four", "five", "red", "green", "blue",
            "yellow", "purple", "sun", "moon", "star", "planet",
            "comet", "ocean", "river", "mountain", "valley", "forest"};
        std::vector<std::string> passwords = {
            "pass123", "secure", "mysecret", "code42", "hello", "alpha1",
            "beta2", "gamma3", "delta4", "epsilon5", "one1", "two2",
            "three3", "four4", "five5", "red1", "green2", "blue3",
            "yellow4", "purple5", "sun1", "moon2", "star3", "planet4",
            "comet5", "ocean1", "river2", "mountain3", "valley4", "forest5"};

        for (int i = 0; i < users; ++i)
        {
            user newuser;
            newuser.username = usernames[i % usernames.size()] + std::to_string(i + 1);
            newuser.password = passwords[i % passwords.size()];
            newuser.genere = genres[i % genres.size()];
            newuser.createid();

            addtofile << newuser.id << ";" << newuser.username << ";"
                      << newuser.password << ";" << newuser.genere << std::endl;
        }
        addtofile.close();
        std::cout << "Datei erfolgreich mit " << users
                  << " vorgefertigten Benutzern gefüllt." << std::endl;
    }
    else
    {
        std::cerr << "Fehler beim Öffnen der Datei" << std::endl;
    }
}

// Sucht nach nutzern mit dem selben Genere
void matchinguser(std::vector<user> &users, int position)
{
    std::cout << std::endl;
    std::vector<user> matchingusers;

    for (const auto &user : users)
    {
        if (user.genere == users[position].genere &&
            user.username != users[position].username)
        {
            matchingusers.push_back(user);
        }
    }
    if (matchingusers.empty())
    {
        std::cout << "Es gibt keine Bnutzer die das selbe Genere wie du mögen!" << std::endl
                  << std::endl;
        delay();
        return;
    }
    else
    {
        std::cout << R"(
      _______________________________________________
     |                                               |
     | Diese Benutzer haben das selbe Genere wie du! |
     |_______________________________________________|
  
     
    )" << std::endl;
        for (int i = 0; i < matchingusers.size(); i++)
        {
            std::cout << "    " << i + 1 << ". " << matchingusers[i].username << std::endl
                      << std::endl;
        }
        std::cout << std::endl
                  << "            Insgesamt gibt es " << matchingusers.size() << " die " << users[position].genere << " mögen!" << std::endl;
    }

    int input;

    do
    {
        std::cout << R"(


         _________________________________________________________________________
        |                                                                         |
        |   Wollen sie einen Freund hinzufügen? (1) | Nein und zurück zum Menü(0) |
        |_________________________________________________________________________|
        
        )" << std::endl;

        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (input == 1)
        {
            freundehinzufügen(users, position);
        }
        if (input != 1 && input != 0)
        {
            clearscreen();
        }

    } while (input != 1 && input != 0);
}

// sortiert benutzer nach benutzernamen und ascii wert
void sortusers(std::vector<user> &users)
{
    std::cout << R"(
    __________________________________________
   |                                          |
   |  Alle Benutzer Sortiert nach Ascii- Wert |
   |__________________________________________|

   
)" << std::endl;

    std::cout << "Es gibt " << users.size() << " Benutzer..." << std::endl
              << std::endl;

    std::vector<user> sortedUsers = users;

    if (sortedUsers.empty())
    {
        std::cout << "Keine Benutzer zum Sortieren vorhanden." << std::endl;
        return;
    }

    for (int i = 1; i < sortedUsers.size(); i++)
    {
        user buf = sortedUsers[i];
        int j = i - 1;

        while (j >= 0 && sortedUsers[j].username > buf.username)
        {
            sortedUsers[j + 1] = sortedUsers[j];
            j--;
        }
        sortedUsers[j + 1] = buf;
    }

    for (int i = 0; i < sortedUsers.size(); i++)
    {
        std::cout << (i + 1) << ".   " << sortedUsers[i].username;

        if (!sortedUsers[i].genere.empty())
        {
            std::cout << " (" << sortedUsers[i].genere << ")";
        }

        std::cout << std::endl;
    }
}

// Auswahl für den Hoerprobenmodus
void hoerprobe(std::vector<user> &users, int position)

{
    bool bestaetigung = true;
    sf::SoundBuffer buffer;

    int auswahl = 1;

    while (auswahl != 0)
    {
        std::cout << R"(
                                        _________________________________________________________________________________________________________________
                                        |                                                                                                                 |
                                        |  Willst du die passende Hörprobe zu deinem Genere hören? (1) | Etdecke neue Hörproben (2) | Zurück zum Menü (0) |
                                        |_________________________________________________________________________________________________________________|
    )" << std::endl;
        std::cin >> auswahl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (auswahl == 1 && auswahl != 0)
        {
            clearscreen();
            hoerprobeuser(users, position);
        }
        if (auswahl == 2 && auswahl != 0)
        {
            clearscreen();
            erkundehoerproben();
        }
    }

    clearscreen();

    // Gibt eine Hoerprobe passend zum Genre des Users aus
}

// Hoerprope matching zum User
void hoerprobeuser(std::vector<user> &users, int position)
{

    clearscreen();

    sf::SoundBuffer buffer;

    if (users[position].genere == "Rockmusik")
    {
        if (!buffer.loadFromFile("bin/musicassets/rock.wav"))
        {
            exit(0);
        }
    }
    if (users[position].genere == "Popmusik")
    {
        if (!buffer.loadFromFile("bin/musicassets/pop.wav"))
        {
            exit(0);
        }
    }
    if (users[position].genere == "HipHop")
    {
        if (!buffer.loadFromFile("bin/musicassets/hiphop.wav"))
        {
            exit(0);
        }
    }
    if (users[position].genere == "Rap")
    {
        if (!buffer.loadFromFile("bin/musicassets/rap.wav"))
        {
            exit(0);
        }
    }
    if (users[position].genere == "Techno")
    {
        if (!buffer.loadFromFile("bin/musicassets/techno.wav"))
        {
            exit(0);
        }
    }
    if (users[position].genere == "House")
    {
        if (!buffer.loadFromFile("bin/musicassets/house.mp3"))
        {
            exit(0);
        }
    }
    if (users[position].genere == "RnB")
    {
        if (!buffer.loadFromFile("bin/musicassets/rnb.wav"))
        {
            exit(0);
        }
    }
    if (users[position].genere == "Jazz")
    {
        if (!buffer.loadFromFile("bin/musicassets/jazz.mp3"))
        {
            exit(0);
        }
    }
    if (users[position].genere == "Klassik")
    {
        if (!buffer.loadFromFile("bin/musicassets/klassik.wav"))
        {
            exit(0);
        }
    }

    sf::Sound sound(buffer);

    sound.play();

    fortfahren();

    sound.stop();
}

// Auswahl für alle verfügbaren Hoerproben
void erkundehoerproben()
{
    int eingabe;
    bool check = true;
    do
    {

        clearscreen();
        std::cout << R"(
                                                                 _______________________________________________________________
                                                                |                                                               |
                                                                | Wähle ein Musikgenre um die dazu passende Hörprobe zu satrten |
                                                                |_______________________________________________________________|

                                                                                       _________________
                                                                                      |                 |
                                                                                      |  Rockmusik (1)  |
                                                                                      |_________________|
                                                                                      |                 |
                                                                                      |   Popmusik (2)  |
                                                                                      |_________________|
                                                                                      |                 |
                                                                                      |    HipHop (3)   |
                                                                                      |_________________|
                                                                                      |                 |
                                                                                      |     Rap (4)     |
                                                                                      |_________________|
                                                                                      |                 |
                                                                                      |     Jazz (5)    |
                                                                                      |_________________|
                                                                                      |                 |
                                                                                      |     RnB (6)     |
                                                                                      |_________________|
                                                                                      |                 |
                                                                                      |    Klassik (7)  |
                                                                                      |_________________|
                                                                                      |                 |
                                                                                      |    Techno (8)   |
                                                                                      |_________________|
                                                                                      |                 |
                                                                                      |    House (9)    |
                                                                                      |_________________|

  )" << std::endl;

        std::cin >> eingabe;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        sf::SoundBuffer buffer;

        clearscreen();

        switch (eingabe)
        {
        case 1:
            if (!buffer.loadFromFile("bin/musicassets/rock.wav"))
            {
                exit(0);
            }
            break;

        case 2:
            if (!buffer.loadFromFile("bin/musicassets/pop.wav"))
            {
                exit(0);
            }
            break;

        case 3:
            if (!buffer.loadFromFile("bin/musicassets/hiphop.wav"))
            {
                exit(0);
            }
            break;

        case 4:
            if (!buffer.loadFromFile("bin/musicassets/rap.wav"))
            {
                exit(0);
            }
            break;

        case 5:
            if (!buffer.loadFromFile("bin/musicassets/jazz.mp3"))
            {
                exit(0);
            }
            break;

        case 6:
            if (!buffer.loadFromFile("bin/musicassets/rnb.wav"))
            {
                exit(0);
            }
            break;

        case 7:
            if (!buffer.loadFromFile("bin/musicassets/klassik.wav"))
            {
                exit(0);
            }
            break;

        case 8:
            if (!buffer.loadFromFile("bin/musicassets/techno.wav"))
            {
                exit(0);
            }
            break;

        case 9:
            if (!buffer.loadFromFile("bin/musicassets/house.mp3"))
            {
                exit(0);
            }
            break;
        }
        sf::Sound sound(buffer);

        sound.play();

        fortfahren();

        sound.stop();

        clearscreen();

        std::cout << R"(
                                                           _____________________________________________________
                                                          |                                                     |
                                                          |  Willst du weiter Hörproben testen? (1)| Zurück (0) |
                                                          |_____________________________________________________|
    )" << std::endl;
        std::cin >> check;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        clearscreen();

    } while (check);
}

// Fortfahren zwischen Screen der auf das Tippen einer Taste wartet
void fortfahren()
{
    std::cout << R"(
         _______________________________
        |                               |
        |  Drücke Enter zum Fortfahren! |
        |_______________________________|
                                            
  )" << std::endl;
    std::cin.get();
    clearscreen();
}

// fargt nach einem Benutzert der in dein Verzeihnis, als Freund hinzugefügt wird
void freundehinzufügen(std::vector<user> &users, int position)
{

    int links = 0;
    int rechts = users.size();
    int n = users.size();

    std::string gesuchteruser = "";
    std::vector<user> sortedusers = users;
    bool gefunden = false;

    for (int i = 1; i < sortedusers.size(); i++)
    {
        user buf = sortedusers[i];
        int j = i - 1;

        while (j >= 0 && sortedusers[j].username > buf.username)
        {
            sortedusers[j + 1] = sortedusers[j];
            j--;
        }
        sortedusers[j + 1] = buf;
    }
    clearscreen();
    do
    {
        std::cout << "Gebe den Benutzuername deines Freundes ein! " << std::endl
                  << std::endl;
        std::cin >> gesuchteruser;

        int gesuchteposition = search_users(sortedusers, n, links, rechts, gesuchteruser);

        if (gesuchteruser == sortedusers[gesuchteposition].username && gesuchteruser != users[position].username)
        {
            gefunden = true;
            users[position].freunde = sortedusers[gesuchteposition].username;
        }
        if (!gefunden)
        {
            clearscreen();
            std::cout << "Der Benutzer existiert nicht oder du hast versucht sich selbst hinzufügen! Versuche es erneut :-)" << std::endl;
            delay();
        }
    }

    while (!gefunden);

    clearscreen();

    setuserdata(users);

    std::cout << std::endl
              << "Der Benutzer " << gesuchteruser << " ist nun dein Besterfreund! " << std::endl;
    delay();
}

// sucht binaer-rekursiv nach dem gesuchtenbenutzer
int search_users(std::vector<user> &searchingusers, int n, int links, int rechts, std::string gesuchteruser)
{
    if (links > rechts)
    {
        return -1;
    }

    int mitte = (links + rechts) / 2;

    if (searchingusers[mitte].username == gesuchteruser)
    {
        return mitte;
    }

    if (searchingusers[mitte].username > gesuchteruser)
    {
        return search_users(searchingusers, n, links, mitte - 1, gesuchteruser);
    }
    if (searchingusers[mitte].username < gesuchteruser)
    {
        return search_users(searchingusers, n, mitte + 1, rechts, gesuchteruser);
    }

    return 0;
}

// gibt den angemeldeten user seine eigenen daten zu löschen
void benutzerloeschen(std::vector<user> &users, int position)
{
    users.erase(users.begin() + position);
    setuserdata(users);
    std::cout << "Du wurdest ausgeloggt! :-(" << std::endl;
    exit(0);
}

// erzeugt ein 3 sec delay
void delay()
{
    std::this_thread::sleep_for(std::chrono::nanoseconds(10));
    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(3));
    clearscreen();
}

// zeigt
void user_verzeichnis(std::vector<user> &users, int position)
{
    std::string auswahl;
    do
    {
        clearscreen();
        std::cout << "\033[95m";
        std::cout << R"(
   __________________________________
  |                                  |
  |     Deine Daten!                  
  |                                  |
  |     Benutzername: )"
                  << users[position].username << R"(                  
  |                                  |
  |     ID: )"
                  << users[position].id << R"(                  
  |                                  |
  |     Genere: )"
                  << users[position].genere << R"(                  
  |                                  |
  |     Besterfreund: )"
                  << users[position].freunde << R"(                  
  |__________________________________|

  
  )" << std::endl;
        std::cout << "\033[0m";

        std::cout << R"(
   ___________________________________________________________________________________________ 
  |                                                                                           |  
  | Willst du dein Passwort ändern? [J] | Zurück zum Menü [N] | benutzer loeschen (loeschen)  |
  |___________________________________________________________________________________________|

)" << std::endl;
        std::cin >> auswahl;
        std::transform(auswahl.begin(), auswahl.end(), auswahl.begin(), [](unsigned char c)
                       { return std::tolower(c); });

        if (auswahl == "j")
        {
            passwort_aendern(users, position);
        }

        if (auswahl == "n")
        {
            return;
        }
        if (auswahl == "loeschen")
        {
            benutzerloeschen(users, position);
        }

    } while (auswahl != "n");
}

// Ändert das Passwort des Users
void passwort_aendern(std::vector<user> &users, int position)
{

    std::string buf = "";
    std::string neuespasswort = "";
    int i = 3;

    clearscreen();

    for (; i > 0; i--)
    {

        std::cout << R"(
    ____________________
   |                    |
   |  Passwort ändern!  |
   |____________________|
                         

)" << std::endl;

        std::cout << users[position].username << " gebe dein altes Passwort ein :" << std::endl
                  << std::endl;
        if (i > 1)
        {
            std::cout << "Du hast: " << i << " Versuche! " << std::endl;
        }
        else
        {
            std::cout << "Du hast: " << i << " Versuch! " << std::endl;
        }
        std::cin >> buf;

        if (buf == users[position].password)
        {
            clearscreen();
            std::cout << "Gebe dein neues Passswort ein:" << std::endl;
            std::cin >> neuespasswort;
            clearscreen();
            users[position].password = neuespasswort;
            setuserdata(users);
            getuserdata(users);
            std::cout << "Dein Passwort wurde ernuert :-)" << std::endl;
            delay();
            clearscreen();
            return;
        }
        clearscreen();
    }

    std::cout << "Du hast zu oft dein Passwort falsch eingegeben du wirst nun in drei Sekunden ausgeloggt!" << std::endl;
    delay();
    exit(0);
}

// Optische Elemente
void startscreen()
{
    clearscreen();
    std::cout << "\033[34m";
    std::cout << std::endl;
    std::cout << R"(
         _____             _____         __        __   ____________   ___  __________   _________   __         _____       __   ______      ________    _______ 
        /      \\         /     \       |  |      |  | |  __________| |  | | _________| |  _______| |  |       /  _   \    |  | | ____  \   |  ______|  |    _  \
       /   __   \\       /  ___  \      |  |      |  | |  |_________  |  | | |          |  |______  |  |      /  / \\  \   |  | | |    \ \  |  |        |   | |  |
      /  /   \\  \\     /  /  \\  \     |  |      |  | |__________  | |  | | |          |  _______| |  |     /  /   \\  \  |  | | |    | |  |  |_____   |   |_|  |
     /  /     \\  \\   /  /    \\  \    |  |      |  |            | | |  | | |          |  |        |  |    /  /     \\  \ |  | | |    | |  |  ______|  |  __    /
    /  /       \\  \\_/  /      \\  \   |  |      |  |            | | |  | | |          |  |        |  |   /  /       \\  \|  | | |    | |  |  |        |  |  \  \
   /  /         \\     /         \\  \  |  |______|  |  __________| | |  | | |________  |  |        |  |  /  /         \\     | | |___/  /  |  |_____   |  |   \  \
  /__/           \\___/           \\__\ |____________| |____________| |__| |__________| |__|        |__| /__/           \\___/  |_______/   |________|  |__|    \__\
  )" << std::endl;
    std::cout << "\033[0m";

    fortfahren();

    clearscreen();
}
void willkommen()
{
    std::cout << "\033[33m";
    std::cout << R"(
                         _    __      __      _   _   ______       __       __          __         __       _/////   __     _                       
   \\      /\\      //  | |  |  |    |  |    | | / / |  __  |     /  \     /  \        /   \      /  \     | |      |   \  | |
    \\    // \\    //   | |  |  |    |  |    | |/ /  | |  | |    / /\ \   / /\ \      / / \ \    / /\ \    |  ////  | |\ \ | |
     \\  //   \\  //    | |  |  |__  |  |__  | |\ \  | |__| |   / /  \ \_/ /  \ \    / /   \ \__/ /  \ \   | |      | | \ \| |
      \\//      \\/     |_|  |____|  |____|  |_| \_\ |______|  /_/    \___/    \_\  /_/     \____/    \_\  |__////  |_|  \___|
  
)" << std::endl;
    std::cout << "\033[0m";
    std::cout << std::endl;
}
void menu()
{
    std::cout << "\033[35m";
    std::cout << R"(
                                                                                            _          __       _   -      -
                                                                       /\\        /\\      | |///////  |\\     | | | |    | |
                                                                      / /\\      / /\\     | |         | \\    | | | |    | |
                                                                     / /  \\    / /  \\    | |///////  | | \\  | | | |    | |
                                                                    / /    \\  / /    \\   | |         | |  \\ | | | |____| |
                                                                   / /       \\/       \\  |_|///////  |_|   \\|_| |________|


)"
              << std::endl;
    std::cout << "\033[0m";
    std::cout << std::endl;
}
void willkommenauswahl()
{
    std::cout << R"(
                                             Bitte wähle einen Modus!
               _____________________________________________________________________________________________
               |                                                                                           |
               |    Login (1)      |       Benutzer Erstellen (2)       |        Lösche Userdaten (3)      |
               |___________________________________________________________________________________________|     
             )"
              << std::endl;
}
void menuauswahl(std::vector<user> &users, int position)
{
    std::cout << "                                                                                        Wähle einen Modus " << users[position].username << " !" << std::endl;
    std::cout << R"(        
  
                 ____________________________________________________________________________________________________________________________________________________________________________
                 |                                                                                                                                                                           |
                 |    Quiz (1)      |       Freunde finden (2)       |        Alle Benutzer (3)     |    Hoerproebe (4)      |    Benutzerdaten (5)     |       Musicfinder beenden (0)      |  
                 |___________________________________________________________________________________________________________________________________________________________________________|     
               
  
    )" << std::endl;
}