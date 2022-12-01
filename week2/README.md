# Week 2

## Vraagstukken

### Wat is het verschil tussen een objectgeoriënteerde en een proceduregerichte taal?

### Wat is een crosscompiler?

Een crosscompiler is een programma dat op een systeem code compileert voor een ander systeem in plaats van de eigen processor.

### Wat is het verschil tussen interpreteren en compileren?

Interpreteren: Elke lijn van een programma word een voor een omgezet in instructies.
Compileren: Het hele programma word in 1 keer omgezet in instructies.

## 7 Segment Display

## Led-bar tellen

Probleem: Je kan niet binair tellen. Oplossing: Je maakt een apparaat dat binair voor jou telt. Dit project zorgt ervoor dat binaire waardes om de seconde worden weergegeven op een led-bar.

### Hoe werkt het?

[Het programma](./led-bar-tellen/src/main.c) werkt door middel van 2 functies: `checkDigits()` en `setLedValues()`.

`checkDigits()` checkt 1 enkele bit van een getal ren geeft 1 terug als de bit op 1 staat en 0 als de bit op 0 staat. Dit wordt berekend door 2 operaties: bit shifting (`a << b`) en and-operaties (`a & b`). In deze functie wordt een binair getal gebitschift met een bepaalde hoeveelheid. Dat getal word dan vergeleken met het getal waarvan een digit gecheckt word. Als de uitkomst daarvan niet gelijk is aan nul betekend dat dat de digit op 1 staat, en dus geeft de functie 1 terug.

`setLedValues()` zet elke LED uit een array met GPIO pins aan of uit. Met de for-loop wordt voor elke LED met `gpio_set_level()` aan of uit. Hierin wordt dus de functie `checkDigits()` voor elke digit van het nummer gebruikt op te bepalen of de led aan of uit moet. Deze funcie word dan weer gebruikt in een for-loop om elk getal langs te gaan.

checkDigits Fuctie:

```c
int checkDigits (int numberToCheck, int index) {
    if (((1 << index) & numberToCheck) !=0 ) {
        return 1;
    } else {
        return 0;
    }    
}
```

setLedValues Functie:

```c
void setLedValues (int numberToConvert, int numOfDigits, int ledPinsArr[]) {
    for (size_t i = 0; i < numOfDigits; i++)
    {
        if (checkDigits(numberToConvert, i)) {
            printf("1");
            gpio_set_level(ledPinsArr[i], HIGH);
        } else {
            printf("0");
            gpio_set_level(ledPinsArr[i], LOW);
        }
    }
}
```

Shematische en fysieke foto's van het project:

![schematisch](../assets/led-bar-tellen/led_bar_tellen_bb.png)
![fysiek](../assets/led-bar-tellen/led_bar_tellen_photo.png)

## Hoeveel heb je op?

Dit project is meer geschikt voor de uitgaansmensen. Stel je voor je bent lekker uit met vrienden. Je hebt gezegd dat je maar een of twee biertjes gaat drinken, maar dat is uiteindelijk niet gelukt. Je bent nu alweer vergeten hoeveel je nou precies op had. Daar is dit project voor! Er zijn 2 knoppen: eentje voor als je een drankje wil toevoegen aan een teller, en eentje voor als je er een af wilt halen (Bijvoorbeeld als je een glas "water" hebt laten vallen). Het aantal gedronkgen glazen wordt weergegeven d.m.v een ledbar.

### Hoe werkt de het?

[Het programma](./led-bar-tellen/src/main.c) werkt door in een while-loop steeds voor nieuwe input te checken. De staat van beide knopjes worden uitgelezen met `gpio_get_level()`. Daarna worden de huidige (`getLevelLower` en `getLevelHigher`) en de vorige (`lowerCallback` en `higherCallback`) waarde van de knoppen vergeleken in een if-statement. Op basis daarvan word de teller verhoogd of verlaagd met 1. Als de teller lager dan 0 is dan word hij weer op 0 gezet. Daarna worden de ledjes aan- of uitgezet door de waarde van de teller.

### Schematisch en fysiek

![shematisch](../assets/hoeveel-heb-je-op/hoeveel-heb-je-op_bb.png)
![fysiek](../assets/hoeveel-heb-je-op/hoeveel-heb-je-op_fysiek.jpg)
