# Week 2

## Vraagstukken

### Wat is het verschil tussen een objectgeoriënteerde en een proceduregerichte taal?

### Wat is een crosscompiler?

Een crosscompiler is een programma dat op een systeem code compileert voor een ander systeem in plaats van de eigen processor.

### Wat is het verschil tussen interpreteren en compileren?

Interpreteren: Elke lijn van een programma word een voor een omgezet in instructies.
CompilerenL: Het hele programma word in 1 keer omgezet in instructies.

## 7 Segment Display

## Led-bar tellen

Probleem: Je kan niet binair tellen. Oplossing: Je maakt een apparaat dat binair voor jou telt.

Shematische en fysieke foto's van het project:

Schematisch | Foto
-|-
![schematisch](../assets/led-bar-tellen/led_bar_tellen_bb.png)|![fysiek](../assets/led-bar-tellen/led_bar_tellen_photo.png)

## Hoeveel heb je op?

Dit project is meer geschikt voor de uitgaansmensen. Stel je voor je bent lekker uit met vrienden. Je hebt gezegd dat je maar een of twee biertjes gaat drinken, maar dat is uiteindelijk niet gelukt. Je bent nu alweer vergeten hoeveel je nou precies op had. Daar is dit project voor! Er zijn 2 knoppen: eentje voor als je een drankje wil toevoegen aan een teller, en eentje voor als je er een af wilt halen (Bijvoorbeeld als je een glas "water" hebt laten vallen). Het aantal gedrongen glazen wordt weergegeven d.m.v een ledbar.

### Hoe werkt de het?

Het programma werkt door in een while-loop steeds voor nieuwe input te checken. De staat van beide knopjes worden uitgelezen met `gpio_get_level()`. Daarna worden de huidige (`getLevelLower` en `getLevelHigher`) en de vorige (`lowerCallback` en `higherCallback`) waarde van de knoppen vergeleken in een if-statement. Op basis daarvan word de teller verhoogd of verlaagd met 1. Als de teller lager dan 0 is dan word hij weer op 0 gezet. Daarna worden de ledjes aan- of uitgezet door de waarde van de teller.

### Schematisch en fysiek

![shematisch](../assets/hoeveel-heb-je-op/hoeveel-heb-je-op_bb.png)
![fysiek](../assets/hoeveel-heb-je-op/hoeveel-heb-je-op_fysiek.jpg)
