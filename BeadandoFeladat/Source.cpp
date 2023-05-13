// *******************************************
// Név: Badinszky Dániel Bence
// NEPTUN: HLISZU
// Feladat:
// Írjon programot amely bekéri a felhasználótól N darab háromszög 
// befogóinak a méretét, kiszámolja az átfogó méretét 
// (Pithagoras tételével) és kiírja a képernyőre. 
// Továbbá határozza meg a legkisebb és legnagyobb 
// átmérőt, és a bekérési index szerint írassa ki a képernyőre ezeket!
// ******************************************


#include<iostream>
#include<ctime>
using namespace std;

int main()
{
	int N; // bekérendő háromszögek száma
	const char* beker = "Bekerendo haromszogek szama: "; //printf
	const char* elsobefogo		= "Adja meg a(z) %i. haromszog elso befogojat:\t"; //printf
	const char* masodikbefogo	= "Adja meg a(z) %i. haromszog masodik befogojat:\t"; //printf
	const char* ujsor		= "\n";
	const char* tombszoveg	= "A(z) %d elemu tömb tartalma: \n";
	const char* tombfejlec	= "A\tB\tC\n";
	const char* haromszog	= "%05.3f\t%05.3f\t%05.3f\n";
	const char* formati		= "%d"; //scanf-hez
	const char* formatf		= "%05.3f"; //lebegőpontos kiíráshoz
	const char* format_scanfDouble = "%lf";
	const char* hun		= "HUN";	//setlocale
	const char* _pause = "pause";	//systemhez
	const char* _kiiras_asmbol = "ASM Az %d.elem: %d\t%d\t%05.3f\n";
	const char* legkisebbAtfogo = "A legkisebb  atfogo indexe: %d, erteke: %05.3f\n";
	const char* legnagyobbAtfogo = "A legnagyobb atfogo indexe: %d, erteke: %05.3f\n";
	int haromszogszam = 1;			// N+1 -> kiírás ne 0-tól menjen
	int tombindex;					//asm tömbök indexelése (nem akarunk szorozni)
	int mentsukmegazespt;			// ESP tároló

	// ************(0)*******************
	// **********************************
	// ESP mentése
	// **********************************
	int _saveesp;
	_asm
	{
		mov _saveesp, ESP;
	}
	// ************(1)*******************
	// **********************************
	// Háromszögszám bekérése
	// **********************************
	_asm
	{
		//setlocale
		push hun;
		push LC_ALL;
		call dword ptr setlocale;
		//N bekérése
		push beker;		// statikus szöveg kiírása
		call printf;	
		//bekérés
		lea eax, N;		// N címe eax-ben
		push eax;		// verem tetejére tesszük
		push formati;	// formázó karakter
		call scanf;		// bekérés
	}


#pragma region ellenorzes
	/*_asm
	{
		push N;
		push formati;
		call dword ptr printf;
	}*/
	//cout << endl << N << " elemu tombok letrehozasa...\n";
#pragma endregion

	double* A = new double[N];		// befogó #1
	double* B = new double[N];		// befogó #2
	double* C = new double[N];		// átfogó

	// ************(2)*******************
	// **********************************
	// tömb feltöltése, adatok bekérése
	// **********************************
	_asm {
		//fejléc kiírása
		push N;
		push tombszoveg;		// "A(z) %d elemu tömb tartalma: \n"
		call printf;

		//tömbelemek kiírása:
		mov ecx, N;
		mov tombindex, 0;
	ciklus_befogo_beker:
		push ecx; //ecx menteni kell
		mov edi, esp; //esp mentése
		// Első befogó bekérése, szöveg kiírása
		push haromszogszam;		// (i+1)
		push elsobefogo;		// "Adja meg a(z) %i. haromszog elso befogojat:\t"
		call printf;
		// Első befogó bekérése, scanf függvény
		mov eax, dword ptr[A];	// A címe eax-ben
		add eax, tombindex;		// A[i]
		push eax;				// verem tetejére tesszük
		push format_scanfDouble;// formázó karakter
		call dword ptr scanf;	// bekérés
		// Második befogó bekérése, szöveg kiírása
		push haromszogszam;		// (i+1)
		push masodikbefogo;		// "Adja meg a(z) %i. haromszog masodik befogojat:\t"
		call printf;
		// Második befogó bekérése, scanf függvény
		mov eax, dword ptr[B];	// B címe eax-ben
		add eax, tombindex;		// B[i]
		push eax;				// verem tetejére tesszük
		push format_scanfDouble;// formázó karakter
		call dword ptr scanf;	// bekérés

		mov esp, edi;			// esp vissza
		pop ecx;
		add tombindex, 8;		// 8bájttal toljuk a pointert
		add haromszogszam, 1;	// (i+1) tárolása printf-hez

		loop ciklus_befogo_beker;
		// Üres sor kiírása:
		push ujsor;
		call printf;

	}

	// ************(3)*******************
	// **********************************
	// átfogó kiszámítása minden háromszögre
	// **********************************
	_asm {
		// C - oldal szamitasa
		mov ecx, N;				// cilus számláló
		mov tombindex, 0;
	atfogo_szamitasa:
		push ecx;				//ecx menteni kell
		mov edi, esp;			//esp mentése
		// A*A számolása
		mov eax, dword ptr[A];	// A cim másolása eax-ba
		add eax, tombindex;		// A[i]
		fldz;					// 0.0 a verem tetejere
		fadd qword ptr[eax];	// 0+A[i] a verem tetejen
		fmul qword ptr[eax];	// A*A a verem tetejen
		// B*B számolása
		mov eax, dword ptr[B];	// B cim másolása eax-ba
		add eax, tombindex;		// B[i]
		fldz;					// 0.0 a verem tetejere
		fadd qword ptr[eax];	// 0+B[i] a verem tetejen
		fmul qword ptr[eax];	// B*B a verem tetejen
		// sqrt(A*A+B*B)
		faddp st(1), st(0);		// A*A+B*B a verem tetején
		fsqrt;					// verem tetején gyököt vonunk
		// eredmény mentése
		mov eax, dword ptr[C];	// C cim másolása eax-ba
		add eax, tombindex;		// C[i]
		fstp qword ptr[eax];	// verem tetejéről kimentünk C[i]-be

		mov esp, edi;			//esp vissza
		pop ecx;
		add tombindex, 8;		// 8bájttal toljuk a pointert

		loop atfogo_szamitasa;
	}

#pragma region ellenprzes
	// ellenorzes
	/*cout << "IDX\tA\tB\tC\n";
	for (int i = 0; i < haromszogszam - 1; i++)
	{
		cout << i << ".elem\t" << A[i] << "\t" << B[i] << "\t" << C[i] << endl;
	}*/
	// ************(4)*******************
	// **********************************
	// Tömb kiírása
	// **********************************
	_asm {
		// Fejléc kiírása
		push tombfejlec;		// "A\tB\tC\n"
		call printf;			// printf()

		// Tömbelemek kiírása:
		mov ecx, N;
		mov tombindex, 0;
	tomb_kiir:
		push ecx;				//ecx menteni kell
		mov edi, esp;			//esp mentése

		mov eax, tombindex;		// index mentése eax-ba
		add eax, dword ptr[C];	// C cimének hozzáadása az offsethez
		fld qword ptr[eax];		// C érték a verem tetejére
		sub esp, 8;				// ESP csökkentése
		fstp qword ptr[esp];	// verem tetejéről memóriába másolás

		mov eax, tombindex;		// index mentése eax-ba
		add eax, dword ptr[B];	// B cimének hozzáadása az offsethez
		fld qword ptr[eax];		// B érték a verem tetejére
		sub esp, 8;				// ESP csökkentése
		fstp qword ptr[esp];	// verem tetejéről memóriába másolás

		mov eax, tombindex;		// index mentése eax-ba
		add eax, dword ptr[A];	// A cimének hozzáadása az offsethez
		fld qword ptr[eax];		// A érték a verem tetejére
		sub esp, 8;				// ESP csökkentése
		fstp qword ptr[esp];	// verem tetejéről memóriába másolás

		push haromszog;			// printf szöveg
		call printf;			// printf()

		mov esp, edi;			// esp vissza
		pop ecx;
		add tombindex, 8;		// 8bájttal toljuk a pointert
		add haromszogszam, 1;	// (i+1) tárolása printf-hez

		loop tomb_kiir;
		// Üres sor kiírása:
		push ujsor;
		call printf;

	}


#pragma endregion

	int i;				// ebben tároljuk a ciklusszámlálót, ecx érintetlen marad
	int minimum_index;	// itt tároljuk a minimum elem indexét a tömbön belül
	int maximum_index;	// itt tároljuk a maximum elem indexét a tömbön belül
	double kuka;		// ebbe a változóba emeljük ki a nemkellő elemeket a veremből
	int nyolc = 8;		// konstans szorzó

	// ************(5)*******************
	// **********************************
	// Minimum keresés
	// **********************************
	_asm {
		mov minimum_index, 0;	// kezdőérték
		mov ecx, N;				// ciklusváltozó feltöltése
		mov tombindex, 0;		// kezdőérték
		mov i, 0;				// kezdőérték
	min_kereses:
		// minimum értéket a verem tetejére:
		mov eax, minimum_index;	// minimum_index-et eax-ba mozgatjuk
		mul nyolc;				// szorozzuk 8-al, hogy jó helyre tudjon mutatni
		add eax, dword ptr[C];	// a tömb kezdőcímének hozzáadása az offsethez
		fld qword ptr[eax];		// maximum érték a verem tetején
		// vizsgálandó értéket a verem tetejére:
		mov eax, dword ptr[C];	// C cime másolása eax-ba
		add eax, tombindex;		// eltolás tombindex-el
		fld qword ptr[eax];		// vizsgalando ertek a verem tetejen
		// összehasonlítás:
		fcomi st(0), st(1);		// <>?
		jz tovabb1;				// verem ürítése
		jnc tovabb1;			// verem ürítése
		mov ebx, i;				// i-t mentjük ebx-be
		mov minimum_index, ebx; // ebx-et mentjük minimum_index-be
		fstp kuka;				// felesleges elemek törlése
		fstp kuka;				// felesleges elemek törlése
		jmp vege_mindennek;		// continue;
	tovabb1:					// verem ürítése
		fstp kuka;				// felesleges elemek törlése
		fstp kuka;				// felesleges elemek törlése
	vege_mindennek:				// continue;
		add i, 1;				// i=i+1
		add tombindex, 8;		// 8bájttal toljuk a pointert
		loop min_kereses;
		
	// Ebben a pontban a "minimum_index"-ben van a legkisebb atfogo indexe
	}

	// ************(6)*******************
	// **********************************
	// Maximum keresés
	// **********************************
	_asm {
		mov maximum_index, 0;		// kezdőérték
		mov ecx, N;					// ciklusváltozó feltöltése
		mov tombindex, 0;			// kezdőérték
		mov i, 0;					// kezdőérték
	max_kereses:
		// minimum értéket a verem tetejére:
		mov eax, maximum_index;
		mul nyolc;
		add eax, dword ptr[C];		// C cime másolása eax-ba
		fld qword ptr[eax];			// maximum ertek a verem tetejen
		// vizsgálandó értéket a verem tetejére:
		mov eax, dword ptr[C];		// C cime másolása eax-ba
		add eax, tombindex;			// eltolás i-el
		fld qword ptr[eax];			// vizsgalando ertek a verem tetejen
		// összehasonlítás:
		fcomi st(0), st(1);			// <>?
		jz tovabb2;					// verem ürítése
		jc tovabb2;					// verem ürítése
		mov ebx, i;					// i-t mentjük ebx-be
		mov maximum_index, ebx;		// ebx-et mentjük maximum_index-be
		fstp kuka;					// felesleges elemek törlése
		fstp kuka;					// felesleges elemek törlése
		jmp vege_mindennek_maximum;	// continue;
	tovabb2:						// verem ürítése
		fstp kuka;					// felesleges elemek törlése
		fstp kuka;					// felesleges elemek törlése
	vege_mindennek_maximum:			// continue;
		add i, 1;					// i=i+1
		add tombindex, 8;			// 8bájttal toljuk a pointert
		loop max_kereses;
	// Ebben a pontban a "maximum_index"-ben van a legkisebb atfogo indexe
	}


#pragma region Ellenorzes
	// cout << "\nMinimum elem indexe: " << minimum_index <<  " erteke: " <<  C[minimum_index] << endl;
	// cout << "\nMaximum elem indexe: " << maximum_index <<  " erteke: " <<  C[maximum_index] <<  endl;
#pragma endregion


	// ************(7)*******************
	// **********************************
	// Minimum és maximum kiírása
	// **********************************
	_asm
	{
		// minimum
		mov eax, minimum_index;	// index mentése eax-ba
		mul nyolc;				// szotozzuk 8-al, hogy indexből, offset legyen
		add eax, dword ptr[C];	// C cimének hozzáadása az offsethez
		fld qword ptr[eax];		// minimum érték a verem tetejére
		sub esp, 8;				// ESP csökkentése
		fstp qword ptr[esp];	// verem tetejéről memóriába másolás
		push minimum_index;		// printf paraméter
		push legkisebbAtfogo;	// printf szöveg
		call printf;			// printf()
		// maximum
		mov eax, maximum_index;	// index mentése eax-ba
		mul nyolc;				// szotozzuk 8-al, hogy indexből, offset legyen
		add eax, dword ptr[C];	// C cimének hozzáadása az offsethez
		fld qword ptr[eax];		// maximum érték a verem tetejére
		sub esp, 8;				// ESP csökkentése
		fstp qword ptr[esp];	// verem tetejéről memóriába másolás
		push maximum_index;		// printf paraméter
		push legnagyobbAtfogo;	// printf szöveg
		call printf;			// printf()
	}

	delete[] A;					// memória felszabadítása
	delete[] B;					// memória felszabadítása
	delete[] C;					// memória felszabadítása

	// ************(8)*******************
	// **********************************
	// system("pause")
	// **********************************
	_asm
	{
		push _pause;			// paraméter átadás
		call dword ptr system;	// system()
	}

	// ************(9)*******************
	// **********************************
	// ESP visszaállítása
	// **********************************
	_asm
	{
		mov ESP, _saveesp;
	}
	
	return 0;
}