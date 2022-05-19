#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "rooster.h"
int grid_vulling;
struct rooster_data
{
    int breedte;
    int hoogte;
    char **grid;
    toestand spel_toestand;
};
/**
 * Deze functie toont het spelveld met ncurses, als er niks mis gaat dan kan
 * de speler beginnen met spelen.
 * @param spel een pointer naar het rooster
 * @param chtype is het object op binnen het spelveld
 */
void toon_rooster(rooster *spel)
{
    if (spel->hoogte != 0 && spel->breedte != 0)
    {
        // ! begin met een nieuw ncurses scherm
        clear();
        chtype object;
        // ! elke object aan ncurses scherm toevoegen met functie "addch"
        for (int i = 0; i < spel->hoogte; i++)
        {
            for (int j = 0; j < spel->breedte; j++)
            {
                object = spel->grid[i][j];
                addch(object);
            }
            object = '\n';
            addch(object);
        }
        printw("\nw = Up\ns = Down\na = Left\nd = Right\n");
        // ! Als het aanmaken van spelveld gelukt is, dan begint het spel.
        spel->spel_toestand = AAN_HET_SPELEN;
    }
    else
    {
        // ! Als er iets mis gegaan , bijvoorbeeld grid is niet consistent of
        // ! geheugen is niet gealloceerd
        printf("Er is iets mis gegaan\n");
        // ! Eidig ncurses scherm, maak geheugen vrij.
        endwin();
        exit(0);
        free(spel);
    }
    // ! zorg dat ncurses scherm aantoont.
    refresh();
}

/**
 * Deze functie verandert de positie van de speler in het spelveld als het
 * mogelijk is, als de speler 'X' of '$' tegenkomt beeindigd het spel.
 *
 * @param spel een pointer naar het rooster
 * @param dx de x richting waarin de speler probeert te bewegen
 * @param dy de y richting waarin de speler probeert te bewegen.
 */
void beweeg(rooster *spel, int dx, int dy)
{
    char speler = '*';
    char barrier = '#';
    char verliezen = 'X';
    char winnen = '$';
    char verberg_spel = '!';
    char bomb = '@';
    int x_speler, y_speler, x_doel, y_doel, x_bomb, y_bomb;
    // ! coordinaat van het speler doorgeven aan x_speler en y_speler
    rooster_zoek(spel, speler, &x_speler, &y_speler);
    rooster_zoek(spel, bomb, &x_bomb, &y_bomb);
    // ! de richting die de speler naartoe wilt
    x_doel = x_speler + dx;
    y_doel = y_speler + dy;
    // ! je kan niet door barrier heen
    if (rooster_kijk(spel, x_doel, y_doel) != barrier &&
        rooster_kijk(spel, x_doel, y_doel) != bomb)
    {
        // ! als de speler 'X' tegenkomt verliest de speler
        if (rooster_kijk(spel, x_doel, y_doel) == verliezen)
        {
            // ! Game over.
            rooster_zet_toestand(spel, VERLOREN);
        }
        // ! als de speler '$' tegenkomt heeft de speler gewonnen
        else if (rooster_kijk(spel, x_doel, y_doel) == winnen)
        {
            // ! Game over.
            rooster_zet_toestand(spel, GEWONNEN);
        }
        // ! als de speler '!' tegenkomt, begint het verberg spel.
        else if (rooster_kijk(spel, x_doel, y_doel) == verberg_spel)
        {
            rooster_zet_toestand(spel, VERBERG_SPEL);
        }
        rooster_plaats(spel, x_speler, y_speler, ' ');
        rooster_plaats(spel, x_doel, y_doel, speler);
    }
    // ! '@' kan geduwd worden , tegen '#' en '$' kan die niet geduwd worden.
    else if (rooster_kijk(spel, x_doel, y_doel) == bomb &&
             rooster_kijk(spel, x_doel + dx, y_doel + dy) != barrier &&
             rooster_kijk(spel, x_doel + dx, y_doel + dy) != winnen)
    {
        rooster_plaats(spel, x_speler, y_speler, ' ');
        rooster_plaats(spel, x_bomb + dx, y_bomb + dy, bomb);
        rooster_plaats(spel, x_doel, y_doel, speler);
    }
}

/**
 * Deze functie verandert de positie van de speler in het spelveld als het
 * mogelijk is en laat '#' achter waar de speler geweest was, als de speler
 * het grid vol gevuld heeft , dan heeft die gewonnen.
 *
 * @param spel een pointer naar het rooster
 * @param dx de x richting waarin de speler probeert te bewegen
 * @param dy de y richting waarin de speler probeert te bewegen.
 * @param grid_vulling het aantal gevulde barrier door de speler
 */
void beweeg_verberg_spel(rooster *spel, int dx, int dy)
{
    char speler = '*';
    char barrier = '#';
    int x_speler, y_speler, x_doel, y_doel;
    // ! coordinaat van het speler doorgeven aan x_speler en y_speler
    rooster_zoek(spel, speler, &x_speler, &y_speler);
    // ! de richting die de speler naartoe wilt
    x_doel = x_speler + dx;
    y_doel = y_speler + dy;
    // ! je kan niet door barrier heen
    if (rooster_kijk(spel, x_doel, y_doel) != barrier)
    {
        // ! bij elke beweging vult de speler een barrier
        grid_vulling++;
        rooster_plaats(spel, x_speler, y_speler, barrier);
        rooster_plaats(spel, x_doel, y_doel, speler);
    }
    // ! bij vol gevuld grid heeft de speler het verberg spel gewonnen
    if (grid_vulling == 31)
    {
        rooster_zet_toestand(spel, GEWONNEN);
    }
}

/**
 * Speelt het spel met een gegeven rooster tot de toestand niet langer
 *  AAN_HET_SPELEN is.
 *
 * @param toets de input van de speler
 */
void speel(rooster *spel)
{
    int toets;
    while (spel->spel_toestand == AAN_HET_SPELEN)
    {
        // ! het spelveld tonen
        toon_rooster(spel);
        toets = getch();
        // ! Bewegen met w,s,a,d. Zie regel 42
        switch (toets)
        {
        case 'w':
        case 'W':
            beweeg(spel, 0, -1);
            break;
        case 's':
        case 'S':
            beweeg(spel, 0, 1);
            break;
        case 'a':
        case 'A':
            beweeg(spel, -1, 0);
            break;
        case 'd':
        case 'D':
            beweeg(spel, 1, 0);
            break;
        }
    }
}

/**
 * Speelt het verberg spel met een gegeven rooster tot de toestand niet langer
 *  AAN_HET_SPELEN is.
 *
 * @param toets de input van de speler
 */
void speel_verberg_spel(rooster *spel)
{
    int toets;
    while (spel->spel_toestand == AAN_HET_SPELEN)
    {
        // ! het spelveld tonen
        toon_rooster(spel);
        printw("r = restart\n");
        toets = getch();
        // ! Bewegen met w,s,a,d. Zie regel 42
        switch (toets)
        {
        case 'w':
        case 'W':
            beweeg_verberg_spel(spel, 0, -1);
            break;
        case 's':
        case 'S':
            beweeg_verberg_spel(spel, 0, 1);
            break;
        case 'a':
        case 'A':
            beweeg_verberg_spel(spel, -1, 0);
            break;
        case 'd':
        case 'D':
            beweeg_verberg_spel(spel, 1, 0);
            break;
        case 'r':
        case 'R':
            // ! Je kan het spel opnieuw starten door op r te druken
            rooster_zet_toestand(spel, OPNIEUW);
            break;
        }
    }
}
int main()
{
begin:
    grid_vulling = 0;
    // ! Initialiseer ncurses
    initscr();
    // ! Spel onderbreken met Ctrl + c
    cbreak();
    // ! Druk niet de letters af die de speler intypt
    noecho();
    const char *doolhof[] = {
        "#######################",
        "#*       #   X        #",
        "#       !#  X$X     ###",
        "##########   X        #",
        "#        #     ########",
        "#    #####            #",
        "#  @   # ### #####    #",
        "#      #     #        #",
        "#      ##### #        #",
        "#            #        #",
        "#        #####        #",
        "#                     #",
        "#                     #",
        "#######################",
        NULL};
    const char *verberg_veld[] = {
        "########",
        "#   # ##",
        "#      #",
        "#      #",
        "#   #  #",
        "#      #",
        "#*    ##",
        "########",
        NULL};
    // ! rooster maken voor spel en het verberg spel
    rooster *spel = rooster_maak(doolhof);
    rooster *verberg_spel = rooster_maak(verberg_veld);
    // ! rooster van het spel tonen en spelen
    toon_rooster(spel);
    speel(spel);
    // ! rooster van het verberg spel tonen en spelen.
    toon_rooster(verberg_spel);
    speel_verberg_spel(verberg_spel);
    // ! bij het drukken op r wordt het spel opnieuw gestart
    if (verberg_spel->spel_toestand == OPNIEUW)
    {
        goto begin;
    }
    // ! bij het winnen van verberg spel wordt barrier verwijdert zodat speler
    // ! verder kan
    rooster_plaats(spel, 1, 3, ' ');
    toon_rooster(spel);
    speel(spel);
    // ! Als het spel beeindigd is, close ncruses scherm
    endwin();
    if (spel->spel_toestand == GEWONNEN)
    {
        printf("Gewonnen!\n");
    }
    else
    {
        printf("Verloren!\n");
    }
    rooster_klaar(spel);
    rooster_klaar(verberg_spel);
    return 0;
}
