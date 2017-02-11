# Vlastná implementácia pamäte v C

# Reprezentácia bloko v pamäti 
Na reprezentáciu blokov pamäte som využíval jednu hlavičku na začiatku každého bloku 
pamäte, ktorá obsahovala veľkosť daného bloku a tiež či je blok obsadený alebo voľný pre 
potreby užívateľa. Hlavičky sú reprezentované údajovým typom „int“ ,to znamená štyrmi 
bajtami(byte). Informáciu o využívaní bloku poskytuje najvyšší bit (posledný bit), takže 
 (1 – plný, alebo aj nepárny, 0 – voľný, teda párny). To znamená, že ak používateľ požiada 
o size veľkosť pamäte a ak ide o nepárnu veľkosť tak zapíšeme size aj do hlavičky, ako náhle 
by požiadal užívateľ o párnu veľkosť do hlavičky by sme zapísali size+1. 

Memory `init() `
Táto funkcia dostáva ako argumenty veľkosť celej pamäte a ukazovateľ, ktorý ukazuje na jej 
začiatok. V mojom prípade som tento prvý „blok“ pamäte upravoval na párny, teda hlavička 
má zapísanú parnú hodnotu, aj keď blok obsahuje inú hodnotu. Pracuje teda podobne ako 
funkcia malloc opísaná nižšie. 

Memory `malloc() `
V tejto funkcii priradzujem samotnú pamäť. Používateľ ma požiada o veľkosť, ktorú 
potrebuje a ktorú mu mám vyhradiť v pamäti. Najskôr zvýšim jeho požiadavku aj o veľkosť 
hlavičky, ktorú tento blok potrebuje. A túto celkovú veľkosť porovnám či sa vojde do pamäti. 
Ak našiel dostatočne miesto teda použijem First Fit, zmením hlavičku, v prípade že je 
nepárna posledný bit ostane nezmení v opačnom prípade sa prepíše. 

```
*BlockOfMemory = *BlockOfMemory | 1; 
Alternatívny zápis 
*BlockOfMemory = *BlockOfMemory | 0x1; 
```
Po úspešnom pridelení pamäte si zapamätám ukazovateľ, ktorý ukazuje na začiatok 
prideleného bloku pamäte a ten vraciam užívateľovi. 
Zvyšok pamäte ktorý ostal, vyplním prázdnym blokom. Takže v konečnom dôsledku sa pri 
vykonávaní funkcie malloc vytvárajú dva bloky pamäte, jeden blok ako ten o ktorý ma 
požiadal používateľ plus ďalší zvyčajne väčší blok pamäte označení ako voľný. 
V prípade neúspešného alokovania pamäte vracia pointer null. 
 
Funkcia `free()` uvoľňuje miesto v pamäti. Najskôr kontrolujeme či ukazovateľ, ktorý som 
dostal od užívateľa je správny. To znamená či je z intervalu od začiatku do konca pamäte. Ak 
nie tak funkcia sa ukončí a vracia 1. 
Ak patrí do intervalu, postupne prechádza pamäťou (lineárne) kým nenájde ten správny 
ukazovateľ. Uvoľnený blok pamäte sa porovnáva s nasledujúcim blokom pamäte( ktorý je 
vzdialený presne o veľkosť, ktorú ideme uvoľniť) ak by aj tento blok bol uvoľnený ( jeho 
posledný bit by obsahoval 0) pripojil by ho. Takýmto postupom sa vytvára fragmentácia 
pamäte, teda spájanie voľných blokov pamäte. 
Pri takomto spájaní blokov pamäte sa iba upraví iba hlavička prvého bloku do ktorej sa pridá 
hodnota nasledujúceho bloku aj s jeho hlavičkou, a posledný bit sa nastaví na voľný( 0 ) 

Posledná funkcia `Memory check`, dostáva ako argumenty už alokované pointre, ktoré neboli 
zatiaľ uvoľnené funkciou Memory_free(). 
Najskôr sa overí či pointer, ktorý je na vstupe je platný a či je z rozsahu aktuálnej pamäte. 
Potom postupne prechádza pamäťou ( preskakuje po hlavičkách , kde si prečíta o koľko má 
vykonať nasledujúci skok ) a kontroluje kedy sa pointer zo vstupu rovná pointru v pamäti. Ak 
takýto pointer v pamäti nájde, skontroluje jeho posledný bit, keďže by mal byť ešte aktívny, 
teda alokovaný mal by byť nastavený na 1. Ak by nebol alebo by nastala nezhoda ešte skôr 
vracia neúspech. 
V prípade že prejde celý cyklus a nenájde vyhovujúci pointer taktiež vracia neúspech. 
```
for ( start_Char = start_Char + HEAD_SIZE ; start_Char < size_of_memory; )
 if ( next_p_char == start_Char ) 
 return 1; 
```
