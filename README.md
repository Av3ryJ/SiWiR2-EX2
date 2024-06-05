# Informationsverbreitung in einem Strahlenwellenleiter

### Aufruf:
./waveguide δ ϵ (reflvl) mit  
δ = Parameter in der Gleichung des Gradientenprofils  
ϵ = Abbruchkriterium für den Löser  
reflvl = Optionaler Eingabeparamter um das übergebene Gitter reflvl-mal zu verfeinern, wenn nichts übergeben wird, ist das reflvl defaultmäßig 0, was keine Verfeinerung bedeutet

#### WICHTIG: Die Eingabedatei muss in dem Verzeichnis liegen, in der das Programm ausgeführt wird. Der Pfad zur Datei kann in Zeile 73 von waveguide.cpp angepasst werden. 


### Datenstrukturen für Eckpunkte (vertices) und Dreiecke (Faces)
Um das Gitter zu verwalten werden die übergebenen Eckpunkte aus unit_circle.txt mit ihrem Index und den dazugehörigen x- und y-Werten in der Struktur eines Vertex gespeichert. Zudem bietet jeder Vertex die Funktion an, den Mittelpunkt zwischen sich und einem anderen Eckpunkt zu berechnen.
Die übergebenen Dreiecke werden in der Datenstruktur Face gespeichert, wo für jedes Dreieck alle 3 Eckpunkte gespeichert werden. Zudem kann man für jedes Dreieck die zugehörige lokale Steifigkeits- und Massenmatrix berechnen.

### Ausgabedateien
Zum einen werden die globale Steifigkeits- und Massenmatrix in die Dateien *A.txt* und *M.txt* geschrieben.  
Zum anderen entsteht die Datei *eigenmode.txt*, in der die mit Hilfe der "inverse power iteration" berechnete Strahlenverbreitung angegeben wird.  
Zuletzt entsteht die Datei *lambda.txt*. Hier wird der kleinste berechnete Eigenwert nach jeder Iteration festgehalten.  
Außerdem befinden sich in der Datei *ksq.txt* die an jedem Eckpunkt (vertex) für k(x, y)^2 berechneten Werte.  
Alle Dateien werden in dem Verzeichnis angelegt, in dem das Programm ausgeführt wird.