# Piep
Eine Akkuspannungsüberwachung...	mit Arduino   
Spannungsüberwachung mit automatischer Zellenzahlerkennung für 2-4s LiPo.		
Die Zellenzahl wird beim Einschalten ermittelt und ein Teiler zur Berechnung der Zellenspannung festgelegt. 	
Auf diese Zellenspannung beziehen sich dann die Schaltschwellen.	
Zwei Warnschwellen: "argwenig" und "ganzargwenig"	
Zugehörige Pausenzeiten: "interval1" und "interval2"	
Variante mit Abschaltung:	
Zuschalten der Last über "minimum", abschalten unter "zuwenig".	
Mit der Abschaltung wird auch der Warnpiep abgeschaltet, das Programm läuft weiter, es wird weiterhin Strom verbraucht! 
