#	Kode stil for Team Pro :cowboy_hat_face:

Koden vi skriver i dette prosjektet skal kunne leses og forstås av alle.
Det er derfor viktig at vi legger et sterkt fokus på å skrive oversiktilig,
forståelig og likt.
Derfor setter vi noen klare regler på
hvordan koden skal skrives med tanke på stil:

Se eksempelet:
[style_example.cpp](https://github.com/schimen/team_pro_zumo/blob/master/guidance%20documents/style_example.cpp)

**Generelt**:
* skriv alt på engelsk.
* skriv funksjoner for så og si alt.
	Er det en operasjon som bør forklares? Er det kode som tar mye plass?
	Kode blir mye ryddigere med mye funksjoner
* prøv å bruke lokale variabler så langt det lar seg gjøre

**Layout**:
* ingen linjer skal ha mer enn 100 tegn. (inkludert mellomrom)
* ikke vær redd for å bruke plass. (her mener jeg plass vertikalt)
	Det er god plass så gjør koden luftig og lettlest
* legg koden et hakk ut (tab) etter en conditional, loop eller funksjon
* legg til en tom linje hver gang koden går et hakk (tab) tilbake
	(se eksempel)
* bruk mellomrom. sett mellomrom mellom f eks "=", "+" og lignende
* bruk whitespace. det er din venn. alle andre blir også glad.

**Kommentarer skal**:
* skrives om du føler du trenger å forklare koden
	(hvis du har en liten følelse av at dette bør forklares, forklar det)
* skrives i starten av hvert funksjon.
	disse kommentarene skal forklare hva funksjonen gjør,
	hvilke variabler den trenger og annen relevant informasjon.
* skrives i starten av hvert dokument
	disse kommentarene skal være store og forklare generelt hva koden er

(viktig: God kode trenger ikke mange kommentarer, den forklarer seg selv)

**Navn skal**:
* skrives i snakeCase. (snakeCase: stor bokstav skiller ord)
	(dette er for at stilen skal gå i et med arduino og zumo bibliotek)
* gi mening, leseren skal forstå hva som ligger i navnet
* være over to bokstaver
	(hvis det ikke er vanlgie navn som "i", "x", eller "y")
* være i caps om de er konstanter som ikke skal endre seg.
	(typisk konstanter brukt i utregninger. Undersore brukes for å skille ord)
* ha en lengde som reflekterer hvilket scope de er i
	f. eks en global variabel bør ha et lenger navn en en lokal variabel
* **starte med**:
	* liten bokstav (navn på klasse skal starte med stor bokstav)
	* "is" hvis det er en funksjon som returnerer en bool
	* "get" hvis det er en funksjon som henter og returnerer en verdi
	* "set" hvis det er en funksjon som setter en verdi
