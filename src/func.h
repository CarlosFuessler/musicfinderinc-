
// Haeder für die Funktionen
// Verlinkung des Vectors

#include "user.h"

// Deklaration der funktionen

void createuser(std::vector<user> &users);
void login(std::vector<user> &users, int &, bool &);
void setuserdata(std::vector<user> &users);
void getuserdata(std::vector<user> &users);
void clearuserdata();
void clearscreen();
void quizes(std::vector<user> &users, int position);
void generateusers();
void matchinguser(std::vector<user> &users, int position);
void sortusers(std::vector<user> &users);
void hoerprobe(std::vector<user> &users, int position);
void hoerprobeuser(std::vector<user> &users, int position);
void erkundehoerproben();
void fortfahren();
void benutzerloeschen(std::vector<user> &users, int position);
void startscreen();
void willkommen();
void menu();
void willkommenauswahl();
void menuauswahl(std::vector<user> &users, int position);
void freundehinzufügen(std::vector<user> &users, int position);
int search_users(std::vector<user> &searchingusers, int n, int links, int rechts, std::string gesuchteruser);
void delay();
void user_verzeichnis(std::vector<user> &users, int position);
void passwort_aendern(std::vector<user> &users, int position);