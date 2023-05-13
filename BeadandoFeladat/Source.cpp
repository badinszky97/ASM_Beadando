#include<iostream>
#include<ctime>
using namespace std;

int main()
{
	int N; // bekérendő háromszögek száma

	const char* beker = "Bekerendo haromszogek szama: "; //printf
	const char* elsobefogo		= "Adja meg a(z) %i. haromszog elso befogojat:\t"; //printf
	const char* masodikbefogo	= "Adja meg a(z) %i. haromszog masodik befogojat:\t"; //printf
	const char* legkisebbAtfogo = "A legkisebb atfogo: %f\n";
	const char* legnagyobbAtfogo = "A legnagyobb atfogo: %f\n";
	const char* formati			= "%d"; //scanf-hez
	const char* formatf			= "%f"; //lebegőpontos kiíráshoz
	const char* format_scanfDouble = "%lf";
	const char* hun				= "HUN"; //setlocale
	const char* _pause = "pause"; //systemhez
	const char* _kiiras_asmbol = "ASM Az %d.elem: %d\t%d\t%d\n";


	//ESP mentése
	int _saveesp;
	_asm
	{
		mov _saveesp, ESP;
	}


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

	//ellenőrzés:
	/*_asm
	{
		push N;
		push formati;
		call dword ptr printf;
	}*/
	//cout << endl << N << " elemu tombok letrehozasa...\n";

	double* A = new double[N];		// befogó #1
	double* B = new double[N];		// befogó #2
	double* C = new double[N];		// átfogó
	int haromszogszam = 1;			// N+1 -> kiírás ne 0-tól menjen
	int tombindex;					//asm tömbök indexelése (nem akarunk szorozni)
	const char* ujsor			= "\n";
	const char* tombszoveg		= "A(z) %d elemu tömb tartalma: \n";
	int mentsukmegazespt;

	// tömb feltöltése, adatok bekérése
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

		//elso befogo bekerese, szoveg kiirasa
		push haromszogszam;
		push elsobefogo;
		call printf;
		//elso befogo bekerese, scanf fuggveny
		mov eax, dword ptr[A];	//A címe eax-ben
		add eax, tombindex;		//A[i]
		push eax;				//verem tetejére tesszük
		push format_scanfDouble;//formázó karakter
		call dword ptr scanf;	//bekérés

		//masodik befogo bekerese, szoveg kiirasa
		push haromszogszam;
		push masodikbefogo;
		call printf;
		//masodik befogo bekerese, scanf fuggveny
		mov eax, dword ptr[B];	//B címe eax-ben
		add eax, tombindex;		//B[i]
		push eax;				//verem tetejére tesszük
		push format_scanfDouble;//formázó karakter
		call dword ptr scanf;	//bekérés

		mov esp, edi;			//esp vissza
		pop ecx;
		add tombindex, 8;
		add haromszogszam, 1;

		loop ciklus_befogo_beker;
		//üres sor kiírása:
		push ujsor;
		call printf;

	}

	_asm {
		// C - oldal szamitasa
		mov ecx, N;		// cilus számláló
		mov tombindex, 0;
	atfogo_szamitasa:
		push ecx; //ecx menteni kell
		mov edi, esp; //esp mentése

		mov eax, dword ptr[A];	// A cime másolása eax-ba
		add eax, tombindex;		// A[i]
		fldz;					// 0.0 a verem tetejere
		fadd qword ptr[eax];	// 0+A[i] a verem tetejen
		fmul qword ptr[eax];	// A*A a verem tetejen

		mov eax, dword ptr[B];	// B cime másolása eax-ba
		add eax, tombindex;		// B[i]
		fldz;					// 0.0 a verem tetejere
		fadd qword ptr[eax];	// 0+B[i] a verem tetejen
		fmul qword ptr[eax];	// B*B a verem tetejen

		faddp st(1), st(0);		// A*A+B*B a verem tetején
		fsqrt;					// verem tetején gyököt vonunk

		mov eax, dword ptr[C]; // C cime másolása eax-ba
		add eax, tombindex;		// C[i]
		fstp qword ptr[eax];	// verem tetejéről kimentünk C[i]-be

		mov esp, edi; //esp vissza
		pop ecx;
		add tombindex, 8;

		loop atfogo_szamitasa;
	}

	// ellenorzes
	cout << "IDX\tA\tB\tC\n";
	for (int i = 0; i < haromszogszam - 1; i++)
	{
		cout << i << ".elem\t" << A[i] << "\t" << B[i] << "\t" << C[i] << endl;
	}

	int minimum, maximum;
	double kuka;
	// min max kereses
	_asm {
		mov minimum, 0;
		mov ecx, N;
		mov tombindex, 0;

	min_kereses:

		mov eax, dword ptr[C];	// C cime másolása eax-ba
		add eax, minimum;		// eltolás minimummal-el
		fld qword ptr[eax];		// minimum ertek a verem tetejen


		mov eax, dword ptr[C];	// C cime másolása eax-ba
		add eax, tombindex;		// eltolás i-el
		fld qword ptr[eax];		// vizsgalando ertek a verem tetejen

		fcomi st(0), st(1);
		jz tovabb1;
		jnc tovabb1;
		mov minimum, ecx;
		fstp kuka;
		fstp kuka;
		jmp vege_mindennek;
	tovabb1:
		fstp kuka;
		fstp kuka;
	vege_mindennek:
		add tombindex, 8;
		loop min_kereses;
		
	// minimum-ban van a legkisebb atfogo indexe
		mov ax, minimum;
		mov cx, 8;
		mul cx;
		mov eax, ax;
		add eax, dword ptr[C];
		
		fstp qword ptr[eax];		// minimum ertek a verem tetejen
		push legkisebbAtfogo;
		call printf;
		

	}
	cout << "\n minimum: " << minimum << endl;


	delete[] A;
	delete[] B;
	delete[] C;

	//system("pause");
	_asm
	{
		push _pause;
		call dword ptr system;
	}
	//ESP visszaállítás:
	_asm
	{
		mov ESP, _saveesp;
	}
	
	return 0;
}