-> Docker info:
sudo docker stats

-> Hauptspeicherverbrauch limitieren:
sudo docker run --memory="0x" praktikum:5
"0" = Größe des Limits
"x" = Einheit -> "g(igabyte), k(ilobyte), m(egabyte), b(yte)
Bsp.: sudo docker run --memory="16m" praktikum:5

-> Anteil des Containers an der CPU limitieren
sudo docker run --cpus="x.y" praktikum:5
*default CPU Anteil: 2 -> 200%
"x.y" = Größe des Anteils
Bsp.: sudo docker run --cpus="0.1" praktikum:5
0.1 = 10% der CPU dar nur verwendet.
!Hard-Limit! -> Wenn die CPU von keinen anderen wichtigen Prozessen verwendet wird, darf der Container trotzdem den vollen Kapazität von der CPU nicht nutzen.

-> Gewicht des Containers ändern
sudo docker run -it --cpu-shares="x" praktikum:5
"x" = Größe des Gewichts des Containers. Je größer das Gewicht des Containers, desto mehr darf der Container auf die CPU zugreifen. 
!Soft-Limit! -> Wenn die CPU von keinen anderen wichtigen Prozessen verwendet wird, darf der Container den vollen Kapazität von der CPU nutzen.
default Wert von x = 1024
Bsp.: sudo docker run -it --cpu-shares="700" praktikum:5

Befehle:
1a. sudo docker run -it --cpus="0.05" praktikum:5
1b. sudo docker run -it --cpu-shares="102" praktikum:5
2.  sudo docker run -it --memory="6m" praktikum:5