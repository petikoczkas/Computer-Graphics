# Computer-Graphics
Ezek a munkák a Számitógépes Grafika c. nevű tárgy házi megoldásaim.

# Első feladat
Készítsen programot, amely egy véletlen gráfot esztétikusan megjelenít és lehetőséget ad a felhasználónak annak tetszőleges részének kinagyítására, mialatt a maradék rész még mindig látszik. A gráf 50 csomópontból áll, telítettsége 5%-os (a lehetséges élek 5% valódi él). Az esztétikus elrendezés érdekében a csomópontok helyét egyrészt heurisztikával, másrészt a hiperbolikus sík szabályainak megfelelő erő-vezérelt gráfrajzoló algoritmussal kell meghatározni a SPACE lenyomásának hatására.

A fókuszálás érdekében a gráfot a hiperbolikus síkon kell elrendezni és a Beltrami-Klein módszerrel a képernyőre vetíteni. A fókuszálás úgy történik, hogy a gráfot a hiperbolikus síkon eltoljuk úgy, hogy az érdekes rész a hiperboloid aljára kerüljön. Az eltolás képi vetülete az egér jobb gombjának lenyomása és lenyomott állapotbeli egérmozgatás pillanatnyi helyének a különbsége.

Az egyes csomópontok a hiperbolikus sík körei, amelyek a csomópontot azonosító textúrával bírnak.

# Második feladat
Készítsen sugárkövető programot, amely egy √3 m sugarú gömbbe írható dodekaéder szobát jelenít meg. A szobában egy 𝑓(𝑥,𝑦,𝑧)=exp⁡(𝑎𝑥^2+𝑏𝑦^2−𝑐𝑧)−1 implicit egyenlettel definiált, a szoba közepén levő 0.3 m sugarú gömbre vágott, optikailag sima arany objektum van és egy pontszerű fényforrás. A szoba falai a saroktól 0.1 méterig diffúz-spekuláris típusúak, azokon belül egy másik, hasonló, de a fal középpontja körül 72 fokkal elforgatott és a fal síkjára tükrözött szobára nyíló portálok. A fényforrás a portálon nem világít át, minden szobának saját fényforrása van. A megjelenítés során elég max 5-ször átlépni a portálokat. A virtuális kamera a szoba közepére néz és a körül forog. Az arany törésmutatója és kioltási tényezője: n/k: 0.17/3.1, 0.35/2.7, 1.5/1.9 A többi paraméter egyénileg megválasztható, úgy, hogy a kép szép legyen. Az 𝑎,𝑏,𝑐 pozitív, nem egész számok.
