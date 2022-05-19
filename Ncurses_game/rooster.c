
/** 
 * Rooster.c:
 * Hierin worden functies geimplementeerd die handig zijn om het daadwerklijk
 * spel te maken, met eerste functies kun je een spelveld maken met daarin alle
 * objecten (als het goed gaat). Andere functies kun je gebruik om datas van het
 * spel op te vragen of veranderen.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rooster.h"

/**
 * Struct die datas van het spel bevat.
 * @param breedte is de breedte van het spel veld (aantal kolomen).
 * @param hoogte is de lengte van het spel (aantal rijen).
 * @param grid is een 2D array met daarin alle objecten van het spelveld.
 * @param toestand is de toestand van het spel, zie "rooster.h" regel 18 t/m 24.
 */
struct rooster_data
{
    int breedte;
    int hoogte;
    char **grid;
    toestand spel_toestand;
};

/**
 * Deze functie maakt een pointer die naar het struct verwijst, de struct bevat
 * een 2d array spelveld met alle objecten , de breedte en de lengte van het
 * spelveld en de toestand van het spel.
 *
 * @param beschrijving is een array van strings, met na de laatste string een
 * NULL pointer. Elke string representeert een rij van het doolhof.
 * Eenvoorbeeld van zo'n beschrijving staat in "game.c".regel 111 t/m 125.
 * @param kolom is de breedte van het spelveld
 * @param rij is de lengte van het spelveld (aantal rijen).
 * @return Een pointer die naar rooster verwijst.
 */
rooster *rooster_maak(const char *beschrijving[])
{
    // ! Een pointer naar rooster die op de head is gealloceerd met daarin
    // ! een 2d array(char) en 3 integers
    rooster *data = malloc(sizeof(char **) + 3 * sizeof(int));
    data->hoogte = 0, data->breedte = 0;
    int kolom = 0, rij = 0;
    int eerste_kolom = strlen(beschrijving[0]);

    // ! Als de lengte van eerste rij 0 is
    if (eerste_kolom == 0)
    {
        return data;
    }

    while (beschrijving[rij] != NULL)
    {
        // ! Kijk of alle rijen consistent zijn
        kolom = strlen(beschrijving[rij]);
        if (kolom == eerste_kolom)
        {
            // ! Aantal rijen
            rij++;
        }
        else
        {
            return data;
        }
    }

    // ! Geheugen op de heap alloceren voor een 2D array (char)
    char **grid = malloc((rij) * sizeof(char *));
    for (int i = 0; i < rij; i++)
    {
        grid[i] = malloc((kolom + 1) * sizeof(char));
        if (grid[i] == NULL || grid == NULL)
        {
            return data;
        }
    }

    // ! Elke object kopieren
    for (int i = 0; i < rij; i++)
    {
        for (int j = 0; j <= kolom; j++)
        {
            grid[i][j] = beschrijving[i][j];
        }
    }
    data->breedte = kolom;
    data->hoogte = rij;
    data->spel_toestand = BEGIN;
    data->grid = grid;
    return data;
}

/**
 * Vraag de huidige toestand van het spel op.
 *
 * @param rp is een pointer naar het rooster.
 * @return de toestand (AAN_HET_SPELEN, GEWONNEN of VERLOREN).
 */
toestand rooster_vraag_toestand(rooster *rp)
{
    toestand spel_toestand = rp->spel_toestand;
    return spel_toestand;
}

/**
 * Deze functie verandert de huidige toestand van het spel.
 *
 * @param rp is een pointer naar het rooster.
 * @param t is de nieuwe toestand (AAN_HET_SPELEN, GEWONNEN of VERLOREN).
 */
void rooster_zet_toestand(rooster *rp, toestand t)
{
    rp->spel_toestand = t;
}

/**
 * Deze functie geeft alle resources vrij die zijn gealloceerd in funcite
 * " *rooster_maak ".
 *
 * @param rp: een pointer naar het rooster.
 */
void rooster_klaar(rooster *rp)
{
    // ! Free elke rij
    for (int i = 0; i < rp->hoogte; i++)
    {
        free(rp->grid[i]);
    }
    // ! Free het veld
    free(rp->grid);
    // ! Free het rooster
    free(rp);
}

/**
 * Deze functie geeft de breedte van het rooster, dat wil zeggen, het aantal
 * kolommen.
 *
 * @param rp: een pointer naar het rooster.
 * @return de breedte
 */
int rooster_breedte(const rooster *rp)
{
    int breedte = rp->breedte;
    return breedte;
}

/**
 * Deze functie geeft de hoogte van het rooster, dat wil zeggen, het aantal
 * rijen.
 *
 * @param rp: een pointer naar het rooster.
 * @return de hoogte
 */
int rooster_hoogte(const rooster *rp)
{
    int hoogte = rp->hoogte;
    return hoogte;
}

/**
 * Deze functie kijkt of de gegeven positie binnen het rooster valt.
 *
 * @param rp: een pointer naar het rooster.
 * @param x de x positie
 * @param y de y positie
 * @return 1 als de positie binnen het rooster valt, anders 0.
 */
int rooster_bevat(const rooster *rp, int x, int y)
{
    if (x >= 0 && x < rp->breedte && y >= 0 && y < rp->hoogte)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * Deze functie kijkt welk object er staat op een bepaalde positie in het
 * rooster.
 *
 * @param rp: een pointer naar het rooster.
 * @param x de x positie
 * @param y de y positie
 * @return  het object op die positie, of '\0' als de positie buiten het
 * grid valt.
 */
char rooster_kijk(const rooster *rp, int x, int y)
{
    // ! kijken of de positie binnen het spelveld valt, zo nee return '\0'
    if (rooster_bevat(rp, x, y) == 0)
    {
        return '\0';
    }
    // ! return object als er binnen het spelveld valt
    else
    {
        return rp->grid[y][x];
    }
}

/**
 * Deze functie schrijf een bepaald object op een bepaalde positie in het
 * rooster.
 *
 * @param rp: een pointer naar het rooster.
 * @param x de x positie
 * @param y de y positie
 * @param c het object
 *
 * ? side effect : als (x,y) binnen het rooster ligt, wordt het object op
 * ? de opgegeven positie geplaatst, anders verandert er niets.
 *
 * @return 1 als het object is geplaatst, of 0 als het buiten de grenzen lag.
 */
int rooster_plaats(rooster *rp, int x, int y, char c)
{
    // ! Als de positie binnen het spelveld valt, plaats object return 1
    if (rooster_bevat(rp, x, y) == 1)
    {
        rp->grid[y][x] = c;
        return 1;
    }
    // ! Als de positie buiten het spelveld valt return 0
    else
    {
        return 0;
    }
}

/**
 * Deze functie zoek een bepaald object in het rooster, en geef de coordinaten
 * van het object terug via de gegeven pointers. Als er meerdere objecten van
 * het gezochte soort in het rooster voorkomen, is niet gedefinieerd van welke
 * de positie wordt gevonden.
 *
 * @param rp: een pointer naar het rooster.
 * @param c het object dat moet worden gezocht
 *
 *
 * ? side effect : pointers naar de geheugenlocaties waar de gevonden positie
 * ? moet worden geschreven
 *
 * @return via de pointers x en y, als het object niet niet wordt gevonden
 * worden *x en *y op -1 gezet.
 */
void rooster_zoek(const rooster *rp, char c, int *x, int *y)
{
    *x = -1;
    *y = -1;
    // ! Elke rij checken of het gevraagd object er in het spelveld zit.
    for (int i = 0; i < rp->hoogte; i++)
    {
        for (int j = 0; j < rp->breedte; j++)
        {
            //! geeft de coordinaat van het object terug
            if (rp->grid[i][j] == c)
            {
                *x = j;
                *y = i;
            }
        }
    }
}
