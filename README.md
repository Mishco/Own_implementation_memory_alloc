# Own memory implementation in C

## Block representation in memory
I used one header at the beginning of each block to represent memory blocks
memory that contained the block size and whether the block is busy or free for
user needs. Headers are represented by the data type "int", that is, four
Byte (byte). Block usage information is provided by the highest bit (last bit), so
 (1 - full or even odd, 0 - free, ie even). This means that if the user requests
The size of the memory and if the size is odd we will write the size into the header as suddenly
would ask the user for even size in the header we would write size + 1.

### Memory `init ()`
This function receives as an argument the size of the entire memory and the pointer that points to it
the beginning. In my case, I edited this first "block" of memory to even, that is, the header
has a written value, even if the block contains a different value. So it works like it does
malloc function described below.
 
### Memory `malloc ()`
I assign the memory itself in this function. The user will ask me for the size that
he needs and which I should reserve in his memory. First, I will increase its demand and size
header that this block needs. And compare this overall size to fit into memory.
If you found enough space then use First Fit, change the header if it is
the odd last bit will not change otherwise it will be overwritten.

```
* BlockOfMemory = * BlockOfMemory | 1;
Alternative notation
* BlockOfMemory = * BlockOfMemory | 0x1;
```

After a successful memory allocation, I will remember the pointer that points to the beginning
assigned memory block and return it to the user.
I will fill the rest of the memory with an empty block. So ultimately at
performing the malloc function creates two blocks of memory, one block as the one that has me
the user has asked the plus plus an usually larger block of memory label as free.
If memory allocation is unsuccessful, pointer returns null.
 
`Free ()` frees up memory space. First we check the indicator I am
got from the user is correct. This means whether it is from the beginning to the end of the memory. if
not so the function quits and returns 1.
If it belongs to the interval, it gradually goes through the memory (linear) until it finds the right one
indicator. The free memory block is compared to the next memory block (which is
distance exactly about the size we are going to release) if this block were also released (its
the last bit would contain 0) would connect it. This procedure creates fragmentation
memory, thus linking free memory blocks.
In this way, blocking memory blocks only modifies the first block header to which it is added
the next block value with its header, and the last bit is set to free (0)

The last function `Memory check` is given as point-allocated allocations, which were not
so far released by Memory_free ().
First, verify that the pointer that is on the input is valid and that it is from the current memory range.
Then it goes through the memory (it skips over the headers, where it reads how much it has)
make the next jump) and check when the pointer from the input is equal to the pointer in memory. if
finds such a pointer in memory, checks its last bit as it should still be active,
thus, the allocation should be set to 1. If there was or would have been a disagreement before
Returns Failure.
If the whole cycle goes through and does not find a suitable pointer, it also returns failure.
```
for (start_Char = start_Char + HEAD_SIZE; start_Char <size_of_memory;)
 if (next_p_char == start_Char)
 return 1;
```

----

## Slovak version

## Vlastná implementácia pamäte v C

# Reprezentácia blokov v pamäti 
Na reprezentáciu blokov pamäte som využíval jednu hlavičku na začiatku každého bloku 
pamäte, ktorá obsahovala veľkosť daného bloku a tiež či je blok obsadený alebo voľný pre 
potreby užívateľa. Hlavičky sú reprezentované údajovým typom „int“ ,to znamená štyrmi 
bajtami(byte). Informáciu o využívaní bloku poskytuje najvyšší bit (posledný bit), takže 
 (1 – plný, alebo aj nepárny, 0 – voľný, teda párny). To znamená, že ak používateľ požiada 
o size veľkosť pamäte a ak ide o nepárnu veľkosť tak zapíšeme size aj do hlavičky, ako náhle 
by požiadal užívateľ o párnu veľkosť do hlavičky by sme zapísali size+1. 

Memory `init()`
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

```c
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
```c
for ( start_Char = start_Char + HEAD_SIZE ; start_Char < size_of_memory; )
 if ( next_p_char == start_Char ) 
 return 1; 
```
