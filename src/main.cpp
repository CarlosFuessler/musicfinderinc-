// Main, welche alle Funktionen des Programems Ausführt
// Ich hoffe du hast viel spaß mit meinem Programm : -)

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
#include "func.h"

// Main Funktion
int main()
{

  startscreen();

  // User Vektor der die Daten speichert

  std::vector<user> users;

  // Holt die user Daten von der Textdatei

  getuserdata(users);

  // Auswahl für den Loginmods

  int loginmode;

  // Position des Benutzers

  int position = 0;

  // Bestaetigung das der Login abgeschlossen ist um in das Hauptmenu zugelangen

  bool loginworks = false;

  // Login unter loginworks als Bedingung

  do
  {

    // Optische Begrüßung

    willkommen();

    // Darstellung der Auswahl
    willkommenauswahl();

    std::cin >> loginmode;
    // Ignoriert irrelevante Eingabefehler
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    clearscreen();

    // Switch für die Auswahl vom Loginmodus
    switch (loginmode)
    {
    case 1:
      // Anmelde Prozess der wenn richtig den Loginmode auf true setzt und zum Menü weiterleitet
      login(users, position, loginworks);
      break;
    case 2:
      // Lässt einen Benutzer erstellen
      createuser(users);
      getuserdata(users);
      break;
    case 3:
      // Löscht das gesamte Benutzerverzeichnis
      clearuserdata();
      getuserdata(users);
      break;
    case 4:
      // Erstellt 200 User
      generateusers();
      getuserdata(users);
      break;
    default:
      // Wird ausgeführt, wenn keiner der Cases eingegebn wird
      if (loginmode == 0)
      {
        clearscreen();
        std::cout << "Musicfinder wird Beendet" << std::endl;
        break;
      }

      break;
    }

  } while (loginmode != 0 && !loginworks);

  clearscreen();
  // Wenn der Loginfunktioniert kommen wir ins Menü
  if (loginworks)
  {

    int mode;

    // Auswahl fürs Menü
    do
    {
      clearscreen();

      // Logo
      menu();

      // Menuauswahl / Darstellung
      menuauswahl(users, position);

      std ::cin >> mode;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      clearscreen();

      switch (mode)
      {

      case 1:
        // Leitet zum Quiz weiter
        quizes(users, position);
        setuserdata(users);
        getuserdata(users);
        break;
      case 2:
        // Zeigt Nutzer mit selben Genere und bietet die Möglichkeit einen Bestenfreund hinzuzufügen
        matchinguser(users, position);
        clearscreen();
        break;
      case 3:
        // Sortiert dei Benutzer nach Ascii Wert
        getuserdata(users);
        sortusers(users);
        fortfahren();
        clearscreen();
        break;
      case 4:
        // Funktionen zum Musik abspielen
        hoerprobe(users, position);
        clearscreen();
        break;
      case 5:
        // Zeigt Infos über den Benutzer und lässt Daten verändern
        user_verzeichnis(users, position);
        clearscreen();
        break;

      default:
        if (mode == 0)
        {
          clearscreen();
          std::cout << "Musicfinder wird Beendet" << std::endl;
          break;
        }
        break;
      }
    } while (mode != 0);
  }

  return 0;
}
