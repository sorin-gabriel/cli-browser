Detalii legate de implementare:
- implementarea structurilor de date este una generica
- lista de taburi este una circulara, pentru a putea indexa taburile si cu
numere negative (index 0: primul tab, index 1: al doilea tab, index -1:
ultimul tab, index -2: penultimul tab)
- coada descarcarilor este mentinuta ordonata prin:
a) inserarea descarcarilor noi in coada la pozitia potrivita, in functie de
prioritate (resursele cu mai putini bytes de descarcat au prioritate mai mare)
b) rearanjarea cozii descarcarilor dupa comanda wait
- sunt folosite mai multe macro-uri pentru acces rapid la campurile browserului,
taburilor, paginilor si structurilor de date
- functiile primesc drept parametru pointer catre o structura de tip browser,
deci daca am dori deschiderea mai multor browsere, numarul schimbarilor necesare
este minim: declararea unui browser nou in main() si adaugarea unei comenzi
pentru a schimba intre browserele deschise
